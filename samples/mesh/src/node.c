#include "node.h"
#include <zephyr/random/random.h>

void generate_id(struct custom_uuid *uuid) {
    uint8_t uuid_bytes[16];

    // Example for timestamp and random generation
    uint64_t timestamp = k_uptime_get();  // Get current uptime in milliseconds
    uint32_t random_part1 = sys_rand32_get();
    uint32_t random_part2 = sys_rand32_get();

    // UUIDv6 structure (timestamp-based)
    // 48-bit timestamp high
    uuid_bytes[0] = (timestamp >> 40) & 0xFF;
    uuid_bytes[1] = (timestamp >> 32) & 0xFF;
    uuid_bytes[2] = (timestamp >> 24) & 0xFF;
    uuid_bytes[3] = (timestamp >> 16) & 0xFF;
    uuid_bytes[4] = (timestamp >> 8) & 0xFF;
    uuid_bytes[5] = timestamp & 0xFF;

    // 16-bit version and timestamp low
    uuid_bytes[6] = ((timestamp >> 12) & 0x0F) | 0x60;  // Version 6
    uuid_bytes[7] = (timestamp >> 4) & 0xFF;

    // 48-bit random part
    uuid_bytes[8] = (random_part1 >> 24) & 0xFF;
    uuid_bytes[9] = (random_part1 >> 16) & 0xFF;
    uuid_bytes[10] = (random_part1 >> 8) & 0xFF;
    uuid_bytes[11] = random_part1 & 0xFF;
    uuid_bytes[12] = (random_part2 >> 24) & 0xFF;
    uuid_bytes[13] = (random_part2 >> 16) & 0xFF;
    uuid_bytes[14] = (random_part2 >> 8) & 0xFF;
    uuid_bytes[15] = random_part2 & 0xFF;

    memcpy(uuid->uuid, uuid_bytes, sizeof(uuid_bytes));
   

}

void uuid_to_str(const struct custom_uuid *uuid, char *str, size_t len) {
    snprintf(str, len,
        "%02x%02x%02x%02x-"
        "%02x%02x-"
        "%02x%02x-"
        "%02x%02x-"
        "%02x%02x%02x%02x%02x%02x",
        uuid->uuid[0], uuid->uuid[1], uuid->uuid[2], uuid->uuid[3],
        uuid->uuid[4], uuid->uuid[5],
        uuid->uuid[6], uuid->uuid[7],
        uuid->uuid[8], uuid->uuid[9],
        uuid->uuid[10], uuid->uuid[11], uuid->uuid[12], uuid->uuid[13], uuid->uuid[14], uuid->uuid[15]);
}


bool is_valid_hex_char(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

bool is_valid_uuid_v6(const char *uuid) {
    // Check length
    if (strlen(uuid) != UUID_STR_LEN - 1) {
        return false;
    }

    // Check format
    for (int i = 0; i < UUID_STR_LEN - 1; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            if (uuid[i] != '-') {
                return false;
            }
        } else {
            if (!is_valid_hex_char(uuid[i])) {
                return false;
            }
        }
    }

    // Check UUID version (should be '6' for UUIDv6)
    if (uuid[14] != '6') {
        return false;
    }

    return true;
}