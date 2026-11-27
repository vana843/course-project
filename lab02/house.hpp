#pragma once

#include "zone.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace smarthome
{

class House
{
private:
    static constexpr auto TAG = "house";

    std::string        m_name{"noname"};
    std::vector<Zone>  m_zones;

public:
    explicit House(std::string_view name);

    House()                        = delete;
    House(const House&)            = delete;
    House(House&&)                 = default;
    House& operator=(const House&) = delete;
    House& operator=(House&&)      = default;
    ~House();

    [[nodiscard]] std::string_view GetName() const
    {
        return m_name;
    }

    [[nodiscard]] std::size_t GetZoneCount() const
    {
        return m_zones.size();
    }

    void AddZone(std::string_view zoneName);
    Zone* GetZone(std::size_t index);
    Zone* FindZone(std::string_view name);

    [[nodiscard]] std::size_t CountAllDevices() const;

    void PrintInfo() const;
};

} // namespace smarthome
