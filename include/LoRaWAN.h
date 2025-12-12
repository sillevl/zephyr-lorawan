#pragma once

#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/lorawan/lorawan.h>

namespace DevBit {

struct LoRaWANKeys {
    uint8_t devEui[8];
    uint8_t appEui[8];
    uint8_t appKey[16];
};

class LoRaWAN {
public:
    LoRaWAN(const struct device* dev);
    ~LoRaWAN();

    int connect(const LoRaWANKeys& keys);
    void send(const uint8_t* data, size_t len, uint8_t port, bool confirmed);

private:
    const struct device* lora_dev_;
    
    // Instance methods for handling callbacks
    void downlink_handler(uint8_t port, uint8_t data_pending,
                         int16_t rssi, int8_t snr,
                         uint8_t len, const uint8_t* data);
    void data_rate_changed_handler(enum lorawan_datarate dr);

    // Static callback wrappers for C API
    static void static_downlink_callback(uint8_t port, uint8_t data_pending,
                                        int16_t rssi, int8_t snr,
                                        uint8_t len, const uint8_t* data);
    static void static_dr_changed_callback(enum lorawan_datarate dr);
    
    // Static instance pointer for callbacks
    static LoRaWAN* instance_;
};
} // namespace DevBit