#include "heartbeat_handler.h"
static struct node *global_node;
K_TIMER_DEFINE(heartbeat_timer, heartbeat_handler, NULL);

void heartbeat_handler(struct k_timer *timer)
{
    // printk("Heartbeat\n");
    send_heartbeat(global_node);
}

void start_heartbeat(struct node *node)
{
    global_node = node;
    k_timer_start(&heartbeat_timer, HEARTBEAT_INTERVAL, HEARTBEAT_INTERVAL);
}

int send_heartbeat(struct node *node)
{
    printk("Sending heartbeat\n");
    int signal_strength = get_signal_strength();
    char *json_string = create_heartbeat_json(node);
    printk("data: %s\n", json_string);
    if (signal_strength < LOW_SIGNAL_THRESHOLD || signal_strength == NOT_KNOWN)
    {
        printk("Sending over LoRa\n");
        return 0;
    }
    else
    {

        printk("Sending Data to the Server\n");
        return 1;
    }
}

char *create_heartbeat_json(struct node *node)
{
    // Define and initialize the heartbeat object

    static const struct json_obj_descr node_descr[] = {
        JSON_OBJ_DESCR_PRIM(struct node, status, JSON_TOK_TRUE),
        JSON_OBJ_DESCR_PRIM_NAMED(struct node, "id", id.str, JSON_TOK_STRING),
    };
    // Buffer to hold the JSON string
    static char json_string[128]; // Ensure the buffer is large enough
    int ret;

    // Encode the JSON object
    ret = json_obj_encode_buf(node_descr, ARRAY_SIZE(node_descr), node, json_string, sizeof(json_string));
    if (ret < 0)
    {
        printk("Error encoding JSON: %d\n", ret);
        return NULL;
    }

    return json_string;
}
