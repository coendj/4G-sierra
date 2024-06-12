#ifndef SRAM_SETUP_H
#define SRAM_SETUP_H

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include "node.h"
#if DT_HAS_COMPAT_STATUS_OKAY(st_stm32_backup_sram)
#define BACKUP_DEV_COMPAT st_stm32_backup_sram
#endif

int sram_store(const struct custom_uuid *uuid);
int sram_load(struct custom_uuid *uuid);

#endif // SRAM_SETUP_H