#include "UUID.h"
#include <random>

void UUID::generate()
{
    std::random_device rnd;
    std::mt19937 generator(rnd());
    std::uniform_int_distribution<int> dist(0, 255);
    for (uint8_t& byte : bytes)
    {
        byte = static_cast<uint8_t>(dist(generator));
    }

    bytes[6] = (bytes[6] & 0x0f) | 0x40;
    bytes[8] = (bytes[8] & 0x3f) | 0x80;
}

bool UUID::operator==(const UUID& other)
{
    return bytes == other.bytes;
}

UUID UUID::null()
{
    UUID id;
    id.bytes.fill(0);
    return id;
}

std::string UUID::to_string() const
{
    std::string return_string;
    for (const uint8_t& byte : bytes)
    {
        return_string += std::to_string(byte) + "-";
    }
    return_string.pop_back();
    return return_string;
}
