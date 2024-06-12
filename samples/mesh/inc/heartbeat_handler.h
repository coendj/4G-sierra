#ifndef HEARTBEAT_HANDLER_H
#define HEARTBEAT_HANDLER_H
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/data/json.h>
#include "gsm_driver.h"
#include "node.h"
#define HEARTBEAT_INTERVAL K_SECONDS(10) // Interval for sending heartbeat signal
#define LOW_SIGNAL_THRESHOLD 10 // Threshold for signal strength 10 = -93 dBm
#define NOT_KNOWN 99 // Value for unknown signal strength
/**
* @brief Handler for the heartbeat timer.
* 
* This function is called when the heartbeat timer expires. It sends a heartbeat
* signal to the server.
*
* @param timer Pointer to the timer structure.
*/
void heartbeat_handler(struct k_timer *timer);

/**
*   @brief Starts the heartbeat timer.
* 
*   This function starts the heartbeat timer with the interval defined by HEARTBEAT_INTERVAL.
*   The timer is started in one-shot mode.
*/
void start_heartbeat(struct node *node);

/**
*  @brief Sends a heartbeat signal to the server.
*   
*  This function sends a heartbeat signal to the server. If the signal strength is below
*  LOW_SIGNAL_THRESHOLD, the signal is sent over LoRa. Otherwise, the signal is sent over
*  MQTT.
*/
int send_heartbeat(struct node *node);

char* create_heartbeat_json(struct node *node);
#endif