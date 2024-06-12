#ifndef LORA_CONTROLLER_H
#define LORA_CONTROLLER_H

#include <zephyr/device.h>
#include <errno.h>
#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/lora.h>

#define LORA_MAX_MSG_LEN 255
#define LORA_MAX_MSGQ_LEN 20
#define ANTENNA_RX false
#define ANTENNA_TX true

// Message Queue
struct lora_msgq_data_type
{
    uint8_t r_data[LORA_MAX_MSG_LEN];
};

/**
 * @brief Configure the LoRa device with global settings.
 * 
 * @return true if configuration succeeds, false otherwise.
 */
bool config_lora();

/**
 * @brief Configure the LoRa device with specific bandwidth, datarate, and
 * coding rate. It can be used to test LoRa with different parameters. 
 *
 * @param bw Bandwidth for the LoRa configuration.
 * @param dr Datarate for the LoRa configuration.
 * @param cd Coding rate for the LoRa configuration.
 * @return true if configuration succeeds, false otherwise.
 */
bool config_lora_test(int bw, int dr, int cd);

/**
 * @brief Set the transmission/reception mode of the LoRa device.
 *
 * @param value true to set to transmission mode, false for reception.
 * @return true if setting Tx/Rx succeeds, false otherwise.
 */
bool set_tx_rx(bool value);

/**
 * @brief Initialize the LoRa driver with default configuration settings.
 *
 * @return true if initialization succeeds, false otherwise.
 */
bool lora_driver_init();

/**
 * @brief Send data using the LoRa device.
 *
 * @param data Pointer to the data to be sent.
 * @param data_len Length of the data to be sent.
 */
void lora_driver_send(uint8_t *data, uint32_t data_len);

/**
 * @brief Receive data using the LoRa device.
 *
 * @param timeout Time to wait for the data.
 * @param data Pointer to buffer where received data will be stored.
 * @param rssi Pointer to variable where RSSI will be stored.
 * @param snr Pointer to variable where SNR will be stored.
 */
bool lora_driver_receive(k_timeout_t timeout, uint8_t *data,
                         int16_t *rssi, int8_t *snr);

/**
 * @brief Start asynchronous reception of LoRa messages by using a callbacks.
 *
 * @return true if starting the callback reception succeeds, false otherwise.
 */
bool lora_driver_start_callback();

/**
 * @brief Stop the asynchronous reception on the LoRa messages.
 *
 * @return true if stopping the callback reception succeeds, false otherwise.
 */
bool lora_driver_stop_callback();

/**
 * @brief Callback handler for asynchronous LoRa data reception.
 *
 * @param dev Pointer to the LoRa device structure.
 * @param data Pointer to the received data.
 * @param size Size of the received data.
 * @param rssi RSSI of the received signal.
 * @param snr SNR of the received signal.
 */
void lora_callback_handler(const struct device *dev, uint8_t *data,
                           uint16_t size, int16_t rssi, int8_t snr);

#endif // LORA_CONTROLLER_H
