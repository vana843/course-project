#include "lamp.hpp"

#include <iostream>

namespace smarthome
{

Lamp::Lamp(IdType id, std::string_view name, int initialBrightness)
    : m_id{id}
    , m_name{name}
{
    if (initialBrightness < BRIGHTNESS_MIN || initialBrightness > BRIGHTNESS_MAX)
    {
        std::cout << "[Лампа] Предупреждение: начальная яркость " << initialBrightness
                  << " вне допустимого диапазона. Установлено значение 50.\n";
        m_brightness     = 50;
        m_lastBrightness = 50;
    }
    else
    {
        m_brightness     = initialBrightness;
        m_lastBrightness = initialBrightness;
    }

    std::cout << "[Лампа] Создана: \"" << m_name << "\" (id=" << m_id << ")\n";
}

Lamp::~Lamp()
{
    std::cout << "[Лампа] Уничтожена: \"" << m_name << "\" (id=" << m_id << ")\n";
}

bool Lamp::TurnOn()
{
    if (m_isOn)
    {
        std::cout << "[Лампа] \"" << m_name << "\" уже включена.\n";
        return false;
    }

    m_isOn       = true;
    m_brightness = m_lastBrightness;
    std::cout << "[Лампа] \"" << m_name << "\" включена (яркость=" << m_brightness << "%)\n";
    return true;
}

bool Lamp::TurnOff()
{
    if (!m_isOn)
    {
        std::cout << "[Лампа] \"" << m_name << "\" уже выключена.\n";
        return false;
    }

    m_lastBrightness = m_brightness;
    m_isOn           = false;
    std::cout << "[Лампа] \"" << m_name << "\" выключена (запомнена яркость=" << m_lastBrightness
              << "%)\n";
    return true;
}

bool Lamp::SetBrightness(int value)
{
    if (!m_isOn)
    {
        std::cout << "[Лампа] ОШИБКА: нельзя изменить яркость \"" << m_name
                  << "\" — лампа выключена. Сначала включите её.\n";
        return false;
    }

    if (value < BRIGHTNESS_MIN || value > BRIGHTNESS_MAX)
    {
        std::cout << "[Лампа] ОШИБКА: яркость " << value << " вне диапазона [" << BRIGHTNESS_MIN
                  << ".." << BRIGHTNESS_MAX << "] для \"" << m_name << "\". Отклонено.\n";
        return false;
    }

    m_brightness     = value;
    m_lastBrightness = value;
    std::cout << "[Лампа] \"" << m_name << "\" яркость установлена на " << m_brightness << "%\n";
    return true;
}

void Lamp::PrintState() const
{
    std::cout << "  Лампа[id=" << m_id << ", название=\"" << m_name << "\", "
              << (m_isOn ? "ВКЛ" : "ВЫКЛ") << ", яркость=" << m_brightness << "%]\n";
}

} // namespace smarthome
