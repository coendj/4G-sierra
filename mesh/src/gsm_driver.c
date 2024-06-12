#include "gsm_driver.h"
#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(gsm, LOG_LEVEL_DBG);

#define RF1_PWR DT_ALIAS(rf1pwr)
static const struct gpio_dt_spec modem_pwr = GPIO_DT_SPEC_GET(RF1_PWR, gpios);

#define RF1_RST DT_ALIAS(rf1rst)
static const struct gpio_dt_spec modem_rst = GPIO_DT_SPEC_GET(RF1_RST, gpios);

#define UART1_DEV DT_ALIAS(uart1)
const struct device *uart_dev = DEVICE_DT_GET(UART1_DEV);

/* queue to store up to 10 messages (aligned to 4-byte boundary) */
K_MSGQ_DEFINE(uart_msgq, UART_MAX_MSG_LEN, UART_MAX_MSG_QUEUE_SIZE, 4);

int switched_mode= 0;
K_TIMER_DEFINE(my_sync_timer, NULL, NULL);

bool gsm_driver_init()
{
	gpio_pin_configure_dt(&modem_pwr, GPIO_OUTPUT_ACTIVE);
	gpio_pin_set_dt(&modem_pwr, 1);

    // k_event_init(&at_res_e);

    if (!device_is_ready(uart_dev))
    {
        LOG_ERR("uart_dev device not ready");
        return 0;
    }

    /* configure interrupt and callback to receive data */
    int ret = uart_irq_callback_user_data_set(uart_dev, serial_cb, NULL);

    if (ret < 0)
    {
        if (ret == -ENOTSUP)
        {
            printk("Interrupt-driven UART API support not enabled\n");
        }
        else if (ret == -ENOSYS)
        {
            printk("UART device does not support interrupt-driven API\n");
        }
        else
        {
            printk("Error setting UART callback: %d\n", ret);
        }
        return 0;
    }

    printk("succesfully init irq\n");
    uart_irq_rx_enable(uart_dev);

    return 0;
}

void gsm_modem_init()
{
    // printk("%i", sizeof(gsm_init_commands));

    for (int i = 0; i < sizeof(gsm_init_commands) / 4; i++)
    {
        send_at_command(gsm_init_commands[i]);
         k_msleep(2000);
    }
}

void send_at_command(const char *buf)
{
    int msg_len = strlen(buf);
    if (*buf == '+') {
    k_timer_start(&my_sync_timer, K_MSEC(1000), K_NO_WAIT);
    while(k_timer_status_sync(&my_sync_timer));
    for (int i = 0; i < msg_len; i++)
    {
        // while(k_timer_status_sync(&my_sync_timer));
        uart_poll_out(uart_dev, buf[i]);
        // k_timer_start(&my_sync_timer, K_MSEC(700), K_WAIT);
    }
    k_timer_start(&my_sync_timer, K_MSEC(1000), K_NO_WAIT);
    while(k_timer_status_sync(&my_sync_timer));
    }
    else
    {
    for (int i = 0; i < msg_len; i++)
    {
        uart_poll_out(uart_dev, buf[i]);
    }
    }
    uart_poll_out(uart_dev, '\n');
    //  while (k_msgq_get(&uart_msgq, &tx_buf, K_MSEC(2000)) == 0) {
    //     printk("%s\n", tx_buf);
    // }

    k_msleep(2000);
}

void serial_cb(const struct device *dev, void *user_data) {
	uint8_t c;

	if (!uart_irq_update(uart_dev)) {
		return;
	}

	if (!uart_irq_rx_ready(uart_dev)) {
		return;
	}

	/* read until FIFO empty */
   
	while (uart_fifo_read(uart_dev, &c, 1) == 1) {
		if ((c == '\n' || c == '\r' || c == '\0') && rx_buf_pos > 0) {
			/* terminate string */
			rx_buf[rx_buf_pos] = '\0';

			/* if queue is full, message is silently dropped */
			k_msgq_put(&uart_msgq, &rx_buf, K_NO_WAIT);

			/* reset the buffer (it was copied to the msgq) */
			rx_buf_pos = 0;
		} else if (rx_buf_pos < (sizeof(rx_buf) - 1) && c != '\n' && c != '\r' && c != '\0') {
			rx_buf[rx_buf_pos++] = c;
		}
	}
}

void read_msg_q() {
    while (k_msgq_get(&uart_msgq, &tx_buf, K_FOREVER) == 0)
    {
        printk("%s\n", tx_buf);
    }
}

int get_switched_mode()
{
    return switched_mode;
}

int get_signal_strength() {
    send_at_command("+++"); // change to command mode
    printk("getting signal strength\n");
    send_at_command("AT+CSQ");
    int signal_strength = 0;
    while (k_msgq_get(&uart_msgq, &tx_buf, K_FOREVER) == 0) {
        if (strstr(tx_buf, "+CSQ") != NULL) {
            char *token = strtok(tx_buf, ": ");
            token = strtok(NULL, ","); // This should now point to the part after ": "
            if (token != NULL) {
                signal_strength = atoi(token);
                printk("Parsed signal strength: %d\n", signal_strength);
            }
        }
    }
    send_at_command("At+ktcpstart=1"); // change to data mode (+++ and ATO doesn't work in our workspace)
    send_at_command("reconnected"); // send message to server for verification
    // Let the queue be empty
    return signal_strength; // Return 0 or an error code if signal strength is not found
}
