#pragma once

class UUID
{
public:
    UUID() { generate(); }
    std::string to_string() const;
    bool operator==(const UUID& other);
    static UUID null();

private:
    std::array<uint8_t, 16> bytes{};
    void generate();
};