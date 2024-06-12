#include "gsm_driver.h"
#include "mocks.h"  


bool gsm_driver_init(){
    return 0;
}

void gsm_modem_init(){
    return;
}

void send_at_command(const char *buf){
    return;
}

void serial_cb(const struct device *dev, void *user_data){
    return;
}

void read_msg_q(){
    return;
}

int get_signal_strength(){
    //get a random number between 0 and 31

    return mock_signal_strength;
}
