#ifndef GSM_CONTROLLER_H
#define GSM_CONTROLLER_H

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <errno.h>

#define UART_MAX_MSG_LEN 1280
#define UART_MAX_MSG_QUEUE_SIZE 20

// static struct k_event at_res_e;

// static char at_eol[3] = {'\r', '\r', '\n'};
// static char response_eol[2] = {'\r', '\n'};

// static char response[3] = {0};

/* receive buffer used in UART ISR callback */
static char rx_buf[UART_MAX_MSG_LEN];
static uint8_t rx_buf_pos = 0;
static char tx_buf[UART_MAX_MSG_LEN] = {0};
extern struct k_msgq uart_msgq;  // Change this line

/**
 * @brief Initialize the GSM driver.
 * 
 * This function initializes the GSM driver by setting up UART device and
 * configuring the interrupt and callback for data reception.
 *
 * @return bool True if initialization is successful, false otherwise.
 */
bool gsm_driver_init();

/**
 * @brief Initializes the GSM modem.
 * 
 * This function sends initial configuration commands to the GSM modem
 * through UART, so that it can connect to internet.
 */
void gsm_modem_init();

/**
 * @brief Sends AT COMMAND to 4G module.
 * 
 * This function sends a given string to the  4G module.
 *
 * @param buf The string to be sent to 4G module.
 */
void send_at_command(const char *buf);

/**
 * @brief Sierra module interrupt callback function.
 * 
 * This function is called when Sierra module receives data. It reads the data
 * from Sierra module and processes it accordingly.
 *
 * @param dev Pointer to the UART device structure.
 * @param user_data User data passed to the callback function (unused).
 */
void serial_cb(const struct device *dev, void *user_data);

void read_msg_q();
int get_signal_strength();


bool check_signal_quality();
void switch_data_mode();
int get_switched_mode();

static const char *gsm_init_commands[] =
    {// {("AT+IFC=2,2"),
    //  ("ATE0"),
    //  ("AT+CMEE=1"),
    //  ("AT+WUSLMSK=00000004,0"),
    //  ("AT+WUSLMSK=00000000,1"),
    //  ("AT+KSREP=1"),
    //  ("AT+KSREP?"),
    //  ("AT!RXDEN?"),
    //  ("AT+CGMR"),
    //  ("AT+KGSN=1"),
    //  ("AT+CIMI"),
    //  ("AT+CCID?"),
    //  ("AT+COPS=0"),
    //  ("AT+KSRAT=9"),
    //  ("AT+CGREG=2"),
    //  ("AT+CEREG=2"),
    //  ("+KCNXCFG: 1,\"GPRS\",\"65.21.249.231\",\"\",\"\",\"IPV4\",\"0.0.0.0\",\"0.0.0.0\",\"0.0.0.0\",0"),
    //  ("AT+KCNXTIMER=1,5,3,30,30"),
    //  ("")
    ("At+creg=1"),
    ("At+cgreg=1"),
    ("At+cfun=1,1"),
    ("ATI"),
    ("At+kcnxcfg=1,\"GPRS\",\"internet.swir\""),
    ("At+ktcpcfg=1,0,\"217.104.100.191\",5100"),
    ("At+ktcpcnx=1"),
    ("At+ktcpstart=1")};

#endif // GSM_CONTROLLER_H
