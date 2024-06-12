#include "lora_driver.h"
LOG_MODULE_REGISTER(lora, LOG_LEVEL_DBG);

#define DEFAULT_RADIO_NODE DT_ALIAS(lora0)
BUILD_ASSERT(DT_NODE_HAS_STATUS(DEFAULT_RADIO_NODE, okay),
			 "No default LoRa radio specified in DT");

const struct device *const lora_dev = DEVICE_DT_GET(DEFAULT_RADIO_NODE);
struct lora_modem_config config;

// Queue
K_QUEUE_DEFINE(lora_q);

struct k_msgq lora_msgq;
K_MSGQ_DEFINE(lora_msgq, sizeof(struct lora_msgq_data_type), LORA_MAX_MSGQ_LEN, 1);

bool config_lora()
{
	// Configure Lora device
	int ret = lora_config(lora_dev, &config);
	if (ret < 0)
	{
		LOG_ERR("device configuration failed%d", ret);
		return false;
	}
	return true;
}

bool set_tx_rx(bool value)
{
	config.tx = value;
	if (config_lora())
	{
		return true;
	}
	else
	{
		LOG_ERR("cannot set TxRx to %s", value ? "true" : "false");
		return false;
	}
}

bool lora_driver_init()
{
	config.frequency = 865100000;
	config.bandwidth = BW_250_KHZ;
	config.datarate = SF_8;
	config.preamble_len = 8;
	config.coding_rate = CR_4_5;
	config.iq_inverted = false;
	config.public_network = false;
	config.tx_power = 4;
	config.tx = ANTENNA_TX;

	// Setup LoRa
	if (!device_is_ready(lora_dev))
	{
		LOG_ERR("deivce is not ready");
		return false;
	}

	config_lora();

	k_queue_init(&lora_q);

	return true;
}

void lora_driver_send(uint8_t *data, uint32_t data_len)
{
	set_tx_rx(ANTENNA_TX);
	int ret = lora_send(lora_dev, data, data_len);
	if (ret < 0)
	{
		LOG_ERR("send failed%d", ret);
	}
}

bool lora_driver_receive(k_timeout_t timeout, uint8_t *data,
						 int16_t *rssi, int8_t *snr)
{
	set_tx_rx(ANTENNA_RX);
	uint8_t r_data[LORA_MAX_MSG_LEN] = {0};
	int len = lora_recv(lora_dev, r_data, LORA_MAX_MSG_LEN, timeout, rssi,
						snr);

	if (len < 0)
	{
		LOG_ERR("receive failed%d", len);
		return false;
	}
	else
	{
		LOG_DBG("Received data: %s (RSSI:%ddBm, SNR:%ddB)", r_data, *rssi, *snr);
		return true;
	}
	memcpy(data, r_data, LORA_MAX_MSG_LEN);
}

bool lora_driver_start_callback()
{
	set_tx_rx(ANTENNA_RX);
	int rec = lora_recv_async(lora_dev, lora_callback_handler);
	if (rec < 0)
	{
		LOG_ERR("lora callback start error:%d", rec);
		return false;
	}
	return true;
}

bool lora_driver_stop_callback()
{
	int rec = lora_recv_async(lora_dev, NULL);
	if (rec < 0)
	{
		LOG_ERR("lora callback stop%d", rec);
		return false;
	}
	return true;
}

void lora_callback_handler(const struct device *dev, uint8_t *data,
						   uint16_t size, int16_t rssi, int8_t snr)
{
	LOG_DBG("Received data: %s (RSSI:%ddBm, SNR:%ddB)", data, rssi, snr);

	// k_queue_alloc_append(&lora_q, data);
	// LOG_DBG("k_queue_is_empty2: %d", k_queue_is_empty(&lora_q));

	struct lora_msgq_data_type lora_d;
	memcpy(lora_d.r_data, data, LORA_MAX_MSG_LEN);

	while (k_msgq_put(&lora_msgq, &lora_d, K_NO_WAIT) != 0)
	{
		/* message queue is full: purge old data & try again */
		k_msgq_purge(&lora_msgq);
	}
}