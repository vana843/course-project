#include "house.hpp"
#include "lamp.hpp"
#include "zone.hpp"

#include <iostream>

int main()
{
    using namespace smarthome;

    // Чтобы в консоли Windows корректно отображалась кириллица
    system("chcp 65001 > nul");

    std::cout << "========== ДЕМОНСТРАЦИЯ «УМНЫЙ ДОМ» ==========\n\n";

    // Независимые лампы (объекты агрегации) — живут вне зон

    std::cout << "--- Создание независимых ламп ---\n";
    Lamp livingLamp(101, "Основной свет гостиной", 70);
    Lamp kitchenLamp(102, "Потолочный свет кухни", 40);
    Lamp hallLamp(103, "Свет в прихожей", 30);
    Lamp balconyLamp(104, "Лампа на балконе", 60);

    // Статический объект
    Lamp staticLamp(201, "Аварийный свет (статический)", 100);

    std::cout << "\n--- Начальное состояние ламп ---\n";
    livingLamp.PrintState();
    kitchenLamp.PrintState();
    hallLamp.PrintState();
    balconyLamp.PrintState();
    staticLamp.PrintState();

 
    // Проверка правил лампы

    std::cout << "\n--- Проверка правил: яркость и включение/выключение ---\n";

    // Корректная последовательность
    livingLamp.TurnOn();
    livingLamp.SetBrightness(85);          // OK

    // Нарушения правил
    livingLamp.SetBrightness(150);         // яркость вне диапазона → отказ
    livingLamp.TurnOff();
    livingLamp.SetBrightness(20);          // лампа выключена → отказ

    livingLamp.TurnOn();                   // восстанавливается предыдущая яркость (85)

    // Динамические объекты

    std::cout << "\n--- Динамические лампы (new / delete) ---\n";
    Lamp* dynLamp = new Lamp(301, "Настольная лампа (динамическая)", 55);
    dynLamp->PrintState();

    // Массив динамических объектов
    const std::size_t dynCount = 2;
    Lamp** dynArray = new Lamp*[dynCount];
    dynArray[0] = new Lamp(302, "Динамическая лампа А", 25);
    dynArray[1] = new Lamp(303, "Динамическая лампа Б", 75);

    //    Композиция + Агрегация
    //    Дом владеет Зонами (композиция)
    //    Зона хранит невладеющие указатели на Лампы (агрегация)

    std::cout << "\n--- Создание дома и демонстрация композиции + агрегации ---\n";

    {
        // Вложенный блок → Дом (и его Зоны) будут уничтожены в конце блока
        House myHouse("Коттедж на холме");

        myHouse.AddZone("Гостиная");
        myHouse.AddZone("Кухня");
        myHouse.AddZone("Прихожая");

        Zone* living = myHouse.FindZone("Гостиная");
        Zone* kitchen = myHouse.FindZone("Кухня");
        Zone* hall = myHouse.FindZone("Прихожая");

        if (living && kitchen && hall)
        {
            // Агрегация: передаём уже существующие лампы
            living->AddLamp(&livingLamp);
            living->AddLamp(dynLamp);
            living->AddLamp(&staticLamp);

            kitchen->AddLamp(&kitchenLamp);
            hall->AddLamp(&hallLamp);
            hall->AddLamp(&balconyLamp);

            // Проверка правила: ёмкость зоны
            std::cout << "\n--- Проверка правила: ёмкость зоны ---\n";
            living->AddLamp(&kitchenLamp);   // 4-я лампа — OK (MAX=4)
            living->AddLamp(&hallLamp);      // 5-я — должна быть отклонена

            std::cout << "\n--- Состояние дома (доступ через объект-целое) ---\n";
            myHouse.PrintInfo();

            // Работа через указатель и ссылку
            std::cout << "\n--- Работа через указатель и ссылку ---\n";
            Lamp* found = living->FindLamp(101);
            if (found)
            {
                found->SetBrightness(40);
            }

            Lamp& refLamp = kitchenLamp;
            refLamp.TurnOn();
            refLamp.SetBrightness(90);

            // Команда автоматики, пока зона активна
            std::cout << "\n--- Автоматика при АКТИВНОЙ зоне ---\n";
            living->ExecuteTurnOnAll();

            // Отключаем зону и пробуем автоматику снова
            std::cout << "\n--- Отключение зоны и попытка автоматики ---\n";
            living->SetActive(false);
            living->ExecuteTurnOnAll();      // должна быть отклонена
        }

        std::cout << "\n--- Выход из вложенного блока: Дом и его Зоны будут уничтожены ---\n";
        std::cout << "(Композиция: зоны уничтожаются вместе с домом. "
                  << "Агрегация: лампы продолжают существовать.)\n";
    } // здесь срабатывает деструктор Дома, затем деструкторы Зон

    std::cout << "\n--- После уничтожения Дома ---\n";
    std::cout << "Лампы всё ещё живы (агрегация):\n";
    livingLamp.PrintState();
    kitchenLamp.PrintState();
    hallLamp.PrintState();
    balconyLamp.PrintState();
    dynLamp->PrintState();

    // Очистка динамических объектов

    std::cout << "\n--- Удаление динамических объектов ---\n";
    delete dynLamp;

    for (std::size_t i = 0; i < dynCount; ++i)
    {
        delete dynArray[i];
    }
    delete[] dynArray;

    std::cout << "\n========== КОНЕЦ ДЕМОНСТРАЦИИ ==========\n";
    std::cout << "(Статические и автоматические объекты будут уничтожены сейчас)\n";

    return 0;
}
