/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zephyr/drivers/uart.h>
#include <ctype.h>
#include "heartbeat_handler.h"
#include "mocks.h"
int mock_signal_strength = 0;


bool validate_uuid_format(const char *uuid_str) {
    // Check length
    if (strlen(uuid_str) != 36) {
        return false;
    }

    // Check positions of hyphens
    if (uuid_str[8] != '-' || uuid_str[13] != '-' || uuid_str[18] != '-' || uuid_str[23] != '-') {
        return false;
    }

    // Check version field (position 14 should be '6' for UUIDv6)
    if (uuid_str[14] != '6') {
        return false;
    }

    // Check that all other characters are hex digits
    for (int i = 0; i < 36; i++) {
        if ((i == 8) || (i == 13) || (i == 18) || (i == 23)) {
            continue;
        }
        if (!isxdigit(uuid_str[i])) {
            return false;
        }
    }

    return true;
}
/* Test suite definition using ZTEST macro */

ZTEST_SUITE(node_tests, NULL, NULL, NULL, NULL, NULL);
ZTEST_SUITE(heartbeat_tests, NULL, NULL, NULL, NULL, NULL);

ZTEST(node_tests, test_create_node_id) {
    struct node node;
    node.status = true;

    // Call the function to test
    generate_id(&node.id);

    // Validate the UUID bytes (check only the version field here)
    uint8_t version_field = (node.id.uuid[6] & 0xF0) >> 4;


    zassert_equal(version_field, 6, "UUID version field is incorrect");



    // Print the generated UUID for manual inspection
}

ZTEST(node_tests, test_uuid_to_str) {
    struct node node;
    node.status = true;
    generate_id(&node.id);

    char uuid_str[UUID_STR_LEN];
    uuid_to_str(&node.id, uuid_str, UUID_STR_LEN);
    node.id.str = uuid_str;
    printk("Generated UUIDv6: %s\n", node.id.str);

    // Validate the UUID string format
    bool valid = validate_uuid_format(uuid_str);
    zassert_true(valid, "UUID string format is incorrect");

}

ZTEST(heartbeat_tests, test_create_heartbeat_json) {
    struct node node;
    node.status = true;
    generate_id(&node.id);

    char uuid_str[UUID_STR_LEN];
    uuid_to_str(&node.id, uuid_str, UUID_STR_LEN);
    node.id.str = uuid_str;

    char* json_string = create_heartbeat_json(&node);
    printk("Generated JSON: %s\n", json_string);

    // Validate the JSON string format
    zassert_not_null(json_string, "JSON string is NULL");
    zassert_true(strlen(json_string) > 0, "JSON string is empty");

}

/* Test suite for gsm_driver_init() function */

// ZTEST_SUITE(send_heartbeat_test, NULL, NULL, NULL, NULL, NULL);





ZTEST(heartbeat_tests, test_signal_strength_below_threshold){
     struct node node;
    node.status = true;
    generate_id(&node.id);

    char uuid_str[UUID_STR_LEN];
    uuid_to_str(&node.id, uuid_str, UUID_STR_LEN);
    node.id.str = uuid_str;
    mock_signal_strength = LOW_SIGNAL_THRESHOLD - 1;
    int result = send_heartbeat(&node);
    zassert_equal(result, 0, "send_heartbeat() did not return 0");
}

ZTEST(heartbeat_tests, test_signal_strength_above_threshold){
     struct node node;
    node.status = true;
    generate_id(&node.id);

    char uuid_str[UUID_STR_LEN];
    uuid_to_str(&node.id, uuid_str, UUID_STR_LEN);
    node.id.str = uuid_str;
    mock_signal_strength = LOW_SIGNAL_THRESHOLD + 1;
    int result = send_heartbeat(&node);
    zassert_equal(result, 1, "send_heartbeat() did not return 1");
}

ZTEST(heartbeat_tests, test_signal_strength_not_known){
     struct node node;
    node.status = true;
    generate_id(&node.id);

    char uuid_str[UUID_STR_LEN];
    uuid_to_str(&node.id, uuid_str, UUID_STR_LEN);
    node.id.str = uuid_str;

    mock_signal_strength = NOT_KNOWN;
    int result = send_heartbeat(&node);
    zassert_equal(result, 0, "send_heartbeat() did not return 0");
}