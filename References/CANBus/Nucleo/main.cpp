/* mbed Microcontroller Library
 * Copyright (c) 2023 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"


// Printing through USB Cable============================================
// bisar bisa pake printf
FileHandle *mbed::mbed_override_console(int fd)
{
    static BufferedSerial serial_port(USBTX, USBRX, 115200);
    return &serial_port;
}

// buat lampu bisa blink"
#ifdef LED1
DigitalOut led(PC_13);
#else
bool led;
#endif
//======================================================================

// ------------------------- CAN DEF -----------------------------------
Ticker ticker;
DigitalOut led1(LED1);

CAN can1(PA_11, PA_12);
CAN can2(PB_5, PB_6);

char counter = 0;
uint32_t send_timer = 0;
uint32_t read_timer = 0;

void send()
{
    // printf("send()\n");
    if (can1.write(CANMessage(1337, &counter, 1))) {
        printf("wloop()\n");
        counter++;
        printf("Message sent: %d\n", counter);
    }
    
}
// -------------------------------------------------------------------


int main() {
    printf("main()\n");
    CANMessage msg;

    read_timer = us_ticker_read();
    send_timer = us_ticker_read();

    while (1) {
        if (us_ticker_read() - read_timer > 50000)
        {   
            if (can2.read(msg)) 
            {
                printf("Message received: %d\n", msg.data[0]);
                led1 = !led1;
            }
            read_timer = us_ticker_read();
        }

        if (us_ticker_read() - send_timer > 50000)
        {   
            
            send();
            send_timer = us_ticker_read();
        }
    }
}