#pragma once

#include <array>
#include <cstdint>
#include "LoRaWAN.h"

namespace DevBit {

// Compile-time hex string to byte array conversion
namespace detail {
    constexpr uint8_t hex_char_to_nibble(char c) {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        return 0;
    }

    template<size_t N>
    constexpr std::array<uint8_t, N> hex_string_to_bytes(const char* hex_str) {
        std::array<uint8_t, N> result{};
        for (size_t i = 0; i < N; ++i) {
            result[i] = (hex_char_to_nibble(hex_str[i * 2]) << 4) | 
                        hex_char_to_nibble(hex_str[i * 2 + 1]);
        }
        return result;
    }
}

// Load LoRaWAN credentials from Kconfig at compile time
inline LoRaWANKeys get_lorawan_keys() {
    constexpr auto dev_eui = detail::hex_string_to_bytes<8>(CONFIG_LORAWAN_DEV_EUI);
    constexpr auto join_eui = detail::hex_string_to_bytes<8>(CONFIG_LORAWAN_JOIN_EUI);
    constexpr auto app_key = detail::hex_string_to_bytes<16>(CONFIG_LORAWAN_APP_KEY);

    LoRaWANKeys keys;
    memcpy(keys.devEui, dev_eui.data(), 8);
    memcpy(keys.appEui, join_eui.data(), 8);
    memcpy(keys.appKey, app_key.data(), 16);
    
    return keys;
}

} // namespace DevBit
