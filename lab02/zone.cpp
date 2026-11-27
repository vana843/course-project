#include "zone.hpp"

#include <iostream>
#include <utility>

namespace smarthome
{

Zone::Zone(std::string_view name)
    : m_name{name}
{
    std::cout << "[Зона] Создана: \"" << m_name << "\"\n";
}

Zone::~Zone()
{
    if (!m_name.empty())
    {
        std::cout << "[Зона] Уничтожена: \"" << m_name << "\" (содержала " << m_lamps.size()
                  << " ссылок на лампы)\n";
    }
    // Aggregation: do NOT delete lamps
    m_lamps.clear();
}

void Zone::SetActive(bool active)
{
    m_isActive = active;
    std::cout << "[Зона] \"" << m_name << "\" теперь " << (m_isActive ? "АКТИВНА" : "ОТКЛЮЧЕНА")
              << "\n";
}

bool Zone::AddLamp(Lamp* lamp)
{
    if (lamp == nullptr)
    {
        std::cout << "[Зона] Нельзя добавить пустую лампу в \"" << m_name << "\".\n";
        return false;
    }

    if (m_lamps.size() >= MAX_DEVICES)
    {
        std::cout << "[Зона] ОШИБКА: зона \"" << m_name << "\" заполнена (максимум " << MAX_DEVICES
                  << " устройств). Нельзя добавить \"" << lamp->GetName() << "\".\n";
        return false;
    }

    for (const auto* l : m_lamps)
    {
        if (l->GetId() == lamp->GetId())
        {
            std::cout << "[Зона] Лампа \"" << lamp->GetName() << "\" уже находится в зоне \""
                      << m_name << "\".\n";
            return false;
        }
    }

    m_lamps.push_back(lamp);
    std::cout << "[Зона] Добавлена \"" << lamp->GetName() << "\" в \"" << m_name
              << "\". Устройств: " << m_lamps.size() << "\n";
    return true;
}

bool Zone::RemoveLamp(Lamp::IdType id)
{
    for (auto it = m_lamps.begin(); it != m_lamps.end(); ++it)
    {
        if ((*it)->GetId() == id)
        {
            std::cout << "[Зона] Удалена \"" << (*it)->GetName() << "\" из \"" << m_name << "\".\n";
            m_lamps.erase(it);
            return true;
        }
    }
    std::cout << "[Зона] Лампа id=" << id << " не найдена в \"" << m_name << "\".\n";
    return false;
}

Lamp* Zone::FindLamp(Lamp::IdType id) const
{
    for (auto* l : m_lamps)
    {
        if (l->GetId() == id)
        {
            return l;
        }
    }
    return nullptr;
}

bool Zone::ExecuteTurnOnAll()
{
    if (!m_isActive)
    {
        std::cout << "[Зона] ОШИБКА: зона \"" << m_name
                  << "\" ОТКЛЮЧЕНА. Команды автоматики игнорируются.\n";
        return false;
    }

    std::cout << "[Зона] Выполняется команда «включить всё» в \"" << m_name << "\"...\n";
    for (auto* lamp : m_lamps)
    {
        lamp->TurnOn();
    }
    return true;
}

void Zone::PrintContents() const
{
    std::cout << "Зона \"" << m_name << "\" [" << (m_isActive ? "АКТИВНА" : "ОТКЛЮЧЕНА")
              << ", устройств: " << m_lamps.size() << "]:\n";

    if (m_lamps.empty())
    {
        std::cout << "  (устройств нет)\n";
        return;
    }

    for (const auto* lamp : m_lamps)
    {
        lamp->PrintState();
    }
}

} // namespace smarthome
