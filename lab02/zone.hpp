#pragma once

#include "lamp.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace smarthome
{

class Zone
{
private:
    static constexpr auto        TAG         = "zone";
    static constexpr std::size_t MAX_DEVICES = 4;

    std::string         m_name{"noname"};
    bool                m_isActive{true};
    std::vector<Lamp*>  m_lamps; 

public:
    explicit Zone(std::string_view name);

    Zone()                       = delete;
    Zone(const Zone&)            = delete;
    Zone(Zone&&)                 = default; 
    Zone& operator=(const Zone&) = delete;
    Zone& operator=(Zone&&)      = default;
    ~Zone();

    [[nodiscard]] std::string_view GetName() const
    {
        return m_name;
    }

    [[nodiscard]] bool IsActive() const
    {
        return m_isActive;
    }

    [[nodiscard]] std::size_t GetDeviceCount() const
    {
        return m_lamps.size();
    }

    void SetActive(bool active);
    bool AddLamp(Lamp* lamp);
    bool RemoveLamp(Lamp::IdType id);
    Lamp* FindLamp(Lamp::IdType id) const;

    bool ExecuteTurnOnAll();

    void PrintContents() const;
};

} // namespace smarthome
