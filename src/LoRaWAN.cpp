#include "LoRaWAN.h"
#include <zephyr/logging/log.h>
#include <zephyr/lorawan/lorawan.h>
#include <zephyr/random/random.h>
// #include <zephyr/sys/util.h>

LOG_MODULE_REGISTER(lorawan_class, LOG_LEVEL_DBG);

namespace DevBit {

// Initialize static instance pointer
LoRaWAN* LoRaWAN::instance_ = nullptr;

LoRaWAN::LoRaWAN(const struct device* dev)
    : lora_dev_(dev)
{
    // Set static instance for callbacks
    instance_ = this;
    
    // Register callbacks using static wrapper functions
    static struct lorawan_downlink_cb downlink_cb = {
        .port = LW_RECV_PORT_ANY,
        .cb = LoRaWAN::static_downlink_callback
    };
    lorawan_register_downlink_callback(&downlink_cb);
    lorawan_register_dr_changed_callback(LoRaWAN::static_dr_changed_callback);
}

LoRaWAN::~LoRaWAN()
{
    if (instance_ == this) {
        instance_ = nullptr;
    }
}

void LoRaWAN::set_adr(bool enabled)
{
    adr_enabled_ = enabled;
}

void LoRaWAN::set_datarate(enum lorawan_datarate dr)
{
    datarate_ = dr;
    datarate_configured_ = true;
}

int LoRaWAN::connect(const LoRaWANKeys& keys)
{
    int ret;
    
    if (!device_is_ready(lora_dev_)) {
        LOG_ERR("LoRa device not ready");
        return -1;
    }

    // Reset any previous LoRaWAN state to ensure fresh DevNonce
    ret = lorawan_set_conf_msg_tries(3);
    if (ret < 0) {
        LOG_WRN("Failed to set conf msg tries: %d", ret);
    }

    ret = lorawan_start();
    if (ret < 0) {
        LOG_ERR("Failed to start LoRaWAN stack: %d", ret);
        return ret;
    }

    lorawan_enable_adr(adr_enabled_);
    LOG_INF("ADR %s", adr_enabled_ ? "enabled" : "disabled");

    if (datarate_configured_) {
        ret = lorawan_set_datarate(datarate_);
        if (ret < 0) {
            LOG_ERR("Failed to set datarate DR_%d: %d", datarate_, ret);
            return ret;
        }

        LOG_INF("Default datarate set to DR_%d", datarate_);
    }

    // Generate random DevNonce using hardware entropy
    uint16_t dev_nonce = (uint16_t)sys_rand32_get();
    LOG_INF("Generated DevNonce: 0x%04X", dev_nonce);

    struct lorawan_join_config join_cfg;

    // Configure OTAA join parameters for LoRaWAN 1.0.3
    join_cfg.mode = LORAWAN_ACT_OTAA;
    join_cfg.dev_eui = const_cast<uint8_t*>(keys.devEui);
    join_cfg.otaa.join_eui = const_cast<uint8_t*>(keys.appEui);
    join_cfg.otaa.app_key = const_cast<uint8_t*>(keys.appKey);
    join_cfg.otaa.nwk_key = const_cast<uint8_t*>(keys.appKey); // Must be set (same as app_key for v1.0.x)
    join_cfg.otaa.dev_nonce = dev_nonce; // Use our random DevNonce

    LOG_INF("Joining network using OTAA...");
    ret = lorawan_join(&join_cfg);
    if (ret < 0) {
        LOG_ERR("Failed to join network: %d", ret);
        return ret;
    }

    LOG_INF("Join request sent, waiting for acceptance...");

    return 0;
}

void LoRaWAN::send(const uint8_t* data, size_t len, uint8_t port, bool confirmed)
{
    int ret;


    ret = lorawan_send(port, const_cast<uint8_t*>(data), len, LORAWAN_MSG_UNCONFIRMED);


    if (ret < 0) {
        LOG_ERR("Failed to send data: %d", ret);
    } else {
        LOG_INF("Data sent successfully, %d bytes", ret);
    }
}


void LoRaWAN::downlink_handler(uint8_t port, uint8_t data_pending,
                             int16_t rssi, int8_t snr,
                             uint8_t len, const uint8_t* data)
{
    LOG_INF("Downlink received on port %d, RSSI: %d, SNR: %d, Length: %d",
            port, rssi, snr, len);
    if (data) {
        LOG_HEXDUMP_INF(data, len, "Received data:");
    }
}


void LoRaWAN::data_rate_changed_handler(enum lorawan_datarate dr)
{
    LOG_INF("Data rate changed to DR_%d", dr);
}

// Static callback wrappers
void LoRaWAN::static_downlink_callback(uint8_t port, uint8_t data_pending,
                                      int16_t rssi, int8_t snr,
                                      uint8_t len, const uint8_t* data)
{
    if (instance_) {
        instance_->downlink_handler(port, data_pending, rssi, snr, len, data);
    }
}

void LoRaWAN::static_dr_changed_callback(enum lorawan_datarate dr)
{
    if (instance_) {
        instance_->data_rate_changed_handler(dr);
    }
}

} // namespace DevBit