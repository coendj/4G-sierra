#ifndef NODE_H
#define NODE_H

#include <zephyr/sys/printk.h>

#include <string.h>
#include <stdio.h>
#define UUID_STR_LEN 37
struct custom_uuid {
    uint8_t uuid[16];
    char* str;
};
struct node {
    struct custom_uuid id;
    bool status;
};
void generate_id(struct custom_uuid *uuid);
void uuid_to_str(const struct custom_uuid *uuid, char *str, size_t len);
bool is_valid_hex_char(char c);
bool is_valid_uuid_v6(const char *uuid);

#endif
