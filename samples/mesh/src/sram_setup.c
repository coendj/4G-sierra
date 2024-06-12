#include "sram_setup.h"

__stm32_backup_sram_section uint8_t backup_value[16];

int sram_store(const struct custom_uuid *uuid){
    const struct device *const dev = DEVICE_DT_GET_ONE(BACKUP_DEV_COMPAT);
    	if (!device_is_ready(dev)) {
		printk("ERROR: BackUp SRAM device is not ready\n");
		return -1;
	}

    //put the uuid in the backup sram
    memcpy(backup_value, uuid->uuid, sizeof(uuid->uuid));


    printk("Storing value in backup SRAM (%p): %d\n", &backup_value, backup_value);
    #if __DCACHE_PRESENT
    SCB_CleanDCache_by_Addr(&backup_value, sizeof(backup_value));
    #endif
    	printk("Next reported value should be: %s\n", backup_value);
	printk("Keep VBAT power source and reset the board now!\n");
    return 0;

}


int sram_load(struct custom_uuid *uuid) {
    const struct device *const dev = DEVICE_DT_GET_ONE(BACKUP_DEV_COMPAT);
    if (!device_is_ready(dev)) {
        printk("ERROR: BackUp SRAM device is not ready\n");
        return -1;
    }
    printk("Current value in backup SRAM (%p): %s\n", backup_value, backup_value);
    char uuid_str[UUID_STR_LEN];
    memcpy(uuid->uuid, backup_value, sizeof(backup_value));
    uuid_to_str(uuid, uuid_str, UUID_STR_LEN);
    uuid->str = uuid_str;
    printk("UUID: %s\n", uuid->str);
    if (!is_valid_uuid_v6(uuid->str)) {
        printk("Invalid UUID in backup SRAM.\n");
        return -1;
    }
    printk("Valid UUID in backup SRAM.\n");
    printk("loading value %s into uuid\n", backup_value);


    #if __DCACHE_PRESENT
    SCB_CleanDCache_by_Addr(backup_value, UUID_STR_LEN);
    #endif

    printk("Reported value should be: %s\n", uuid->str);
    printk("Keep VBAT power source and reset the board now!\n");
    return 0;
}