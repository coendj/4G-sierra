#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <zephyr/console/console.h>
#include "lora_driver.h"
#include "heartbeat_handler.h"
#include "sram_setup.h"

#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
// #include "config.h"

// #if defined(CONFIG_USERSPACE)
// #include <zephyr/app_memory/app_memdomain.h>
// K_APPMEM_PARTITION_DEFINE(app_partition);
// struct k_mem_domain app_domain;
// #define APP_BMEM K_APP_BMEM(app_partition)
// #define APP_DMEM K_APP_DMEM(app_partition)
// #else
// #define APP_BMEM
// #define APP_DMEM
// #endif

// /* Buffers for MQTT client. */
// static APP_BMEM uint8_t rx_buffer[APP_MQTT_BUFFER_SIZE];
// static APP_BMEM uint8_t tx_buffer[APP_MQTT_BUFFER_SIZE];

// /* MQTT client context */
// static struct mqtt_client client_ctx;

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

#define LOAD_SWITCH1 DT_ALIAS(loadswitch1)
static const struct gpio_dt_spec load_switch_1 = GPIO_DT_SPEC_GET(LOAD_SWITCH1, gpios);

#define LOAD_SWITCH0 DT_ALIAS(loadswitch0)
static const struct gpio_dt_spec load_switch_0 = GPIO_DT_SPEC_GET(LOAD_SWITCH0, gpios);

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

#define MY_STACK_SIZE 500
#define MY_PRIORITY 5
struct node n;
char uuid_str[UUID_STR_LEN];
bool heart_beat_start = false;
int main(void)
{
	gpio_pin_configure_dt(&load_switch_0, GPIO_OUTPUT_ACTIVE);
	gpio_pin_set_dt(&load_switch_0, 1);
	int ret = sram_load(&n.id);
	if (ret != 0)
	{
		printk("Failed to load UUID from SRAM, generating new UUID.\n");
		generate_id(&n.id);
		uuid_to_str(&n.id, uuid_str, UUID_STR_LEN);
		n.id.str = uuid_str;
		sram_store(&n.id);
	}else{
		uuid_to_str(&n.id, uuid_str, UUID_STR_LEN);
		n.id.str = uuid_str;
		
	}
	n.status = true;
	printk("UUID: %s\n", n.id.str);
	console_getline_init();
	k_msleep(5000);
	gsm_driver_init();
	k_msleep(10000);
	gsm_modem_init();
	k_msleep(5000);
	printk("start\n");
	// start_heartbeat(&n);
	while (1)
	{	
		// switch_data_mode();
		// start_heartbeat(&n);

		// if(get_switched_mode() == 1)
		// {
		// 	get_signal_strength(&n);
		// }
		// else
		// {
		// 	switch_data_mode();
		// }



		// if(heart_beat_start == false)
		// {
		// char *s = console_getline();
		// send_at_command(s);
		// if(s == "+++")
		// {
		// 	printk("start heartbeat");
		// 	heart_beat_start = true;
		// }
	
		// }
		// else
		// {
		// start_heartbeat(&n);
		char *s = console_getline();
		send_at_command(s);
		// switch_data_mode();
		// }

		// printk("%s", s);
	}
	return 0;
}
