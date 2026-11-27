#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace smarthome
{

class Lamp
{
public:
    using IdType = std::uint64_t;

    static constexpr int BRIGHTNESS_MIN = 0;
    static constexpr int BRIGHTNESS_MAX = 100;

private:
    static constexpr auto TAG = "lamp";

    IdType m_id{};
    std::string m_name{"noname"};
    bool m_isOn{false};
    int m_brightness{50};
    int m_lastBrightness{50};

public:
    Lamp(IdType id, std::string_view name, int initialBrightness = 50);

    Lamp()                          = delete;
    Lamp(const Lamp&)               = delete;
    Lamp(Lamp&&)                    = delete;
    Lamp& operator=(const Lamp&)    = delete;
    Lamp& operator=(Lamp&&)         = delete;
    ~Lamp();

    // Unique lamp ID
    [[nodiscard]] auto GetId() const
    {
        return m_id;
    }

    [[nodiscard]] std::string_view GetName() const
    {
        return m_name;
    }

    [[nodiscard]] bool IsOn() const
    {
        return m_isOn;
    }

    [[nodiscard]] int GetBrightness() const
    {
        return m_brightness;
    }

    bool TurnOn();
    bool TurnOff();
    bool SetBrightness(int value);

    void PrintState() const;
};

} // namespace smarthome
