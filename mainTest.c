#include <stdio.h>

#include "clock.h"
#include "actuator.h"
#include "uart.h"

void test_actuator();
void test_com();

int main() {
    
    //setup
    clock_setup();
    actuator_setup();
    uart_setup();

    
    test_actuator();
    while(1){
        //test_actuator();
        fprintf(stderr,"un message de test\n");
        delay_ms(1000);
    }
}

void test_actuator(){
    actuator_set(ARM, 80);
    actuator_set(FLAG, 10);
}

void test_com(){
    //uart_send_string("super nul");
    //printf("coucou \n");
}
