#include "house.hpp"

#include <iostream>

namespace smarthome
{

House::House(std::string_view name)
    : m_name{name}
{
    std::cout << "[Дом] Создан: \"" << m_name << "\"\n";
}

House::~House()
{
    std::cout << "[Дом] Уничтожен: \"" << m_name << "\" (содержал " << m_zones.size() << " зон)\n";
    // Composition: zones are destroyed automatically with the vector
}

void House::AddZone(std::string_view zoneName)
{
    m_zones.reserve(m_zones.size() + 1);
    m_zones.emplace_back(zoneName);
    std::cout << "[Дом] Добавлена зона \"" << zoneName << "\". Всего зон: " << m_zones.size()
              << "\n";
}

Zone* House::GetZone(std::size_t index)
{
    if (index >= m_zones.size())
    {
        std::cout << "[Дом] Неверный индекс зоны: " << index << "\n";
        return nullptr;
    }
    return &m_zones[index];
}

Zone* House::FindZone(std::string_view name)
{
    for (auto& zone : m_zones)
    {
        if (zone.GetName() == name)
        {
            return &zone;
        }
    }
    return nullptr;
}

std::size_t House::CountAllDevices() const
{
    std::size_t total = 0;
    for (const auto& zone : m_zones)
    {
        total += zone.GetDeviceCount();
    }
    return total;
}

void House::PrintInfo() const
{
    std::cout << "=== Дом \"" << m_name << "\" ===\n";
    std::cout << "Зон: " << m_zones.size() << ", всего устройств: " << CountAllDevices() << "\n";
    for (const auto& zone : m_zones)
    {
        zone.PrintContents();
    }
    std::cout << "========================\n";
}

} // namespace smarthome
