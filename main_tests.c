#include <stdio.h>

#include "lowlevel/clock.h"
#include "lowlevel/actuator.h"
//#include "uart.h"

void test_actuator();
void test_com();

int main() {
    
    //setup
    clock_setup();
    actuators_setup();
    
    while(1){
        test_actuator();
    }
}

void test_actuator(){
    actuator_set(ARM, 50);
    actuator_set(FLAG, -60);
}

void test_com(){
    //uart_send_string("super nul");
    //printf("coucou \n");
}
