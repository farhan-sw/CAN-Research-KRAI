/* mbed Microcontroller Library
 * Copyright (c) 2021 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "MBED.h"
#include "../CanBusKRAI/canKRAI.hpp"

// Printing through USB Cable============================================
// bisar bisa pake printf
FileHandle *mbed::mbed_override_console(int fd)
{
    static BufferedSerial serial_port(USBTX, USBRX, 115200);
    return &serial_port;
}
//======================================================================
// buat lampu bisa blink"
#ifdef LED1
DigitalOut led(LED1);
#else
bool led;
#endif

//======================================================================
// CAN BUS Setup=========================================================
CAN can(PA_11, PA_12,200000);
canKRAI micon;
// CAN Message Setup

uint32_t send_timer = 0;
uint32_t read_timer = 0;
uint32_t try_timer = 0;
uint32_t reset = 0;
//=======================================================================

int main()
{
    int speed1 = 1;
    int speed2 = 2;
    int speed3 = 3;
    int speed4 = 4;
    int angle1 = 1;
    int angle2 = 2;
    int angle3 = 3;
    int angle4 = 4;
    send_timer = us_ticker_read();
    reset = us_ticker_read();
    while (us_ticker_read() - reset < 100000)
    {
        if (us_ticker_read() - send_timer > 10000)
        {
            micon.sendspeedall(speed1, speed2, speed3, speed4);
            micon.sendangleall(angle1, angle2, angle3, angle4);
            send_timer = us_ticker_read();
        }
    }
    read_timer = us_ticker_read();
    while (1)
    {
        if (us_ticker_read() - send_timer > 50000)
        {
            speed1 -=1;
            speed2 -=1;
            speed3 -=1;
            speed4 -=1;
            micon.sendspeedall(speed1,speed2,speed3,speed4);
            if (micon.sendspeedall(speed1, speed2, speed3, speed4))
            {
                led = !led;
            };
            micon.sendangleall(angle1, angle2, angle3, angle4);
            printf("M %d %d %d %d %d %d %d %d\n", speed1, angle1, speed2, angle2, speed3, angle3, speed4, angle4);
            send_timer = us_ticker_read();
            // led =1;
        }
        // if (us_ticker_read() - read_timer > 5000)
        // {
        //     micon.masterreadmsg(&speed1,&angle1,&speed2,&angle2,&speed3,&angle3,&speed4,&angle4);
        // }
    }
    return 0;
}