/* mbed Microcontroller Library
 * Copyright (c) 2021 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "MBED.h"
#include "../CanBusKRAI/canBMKRAI.hpp"

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
canBMKRAI micon;
// CAN Message Setup

uint32_t send_timer = 0;
uint32_t read_timer = 0;
uint32_t try_timer = 0;
uint32_t reset = 0;
//=======================================================================

int main()
{
    float data = 1.0;
    int id = 2;

    send_timer = us_ticker_read();
    reset = us_ticker_read();
    read_timer = us_ticker_read();
    while (1)
    {
        if (us_ticker_read() - send_timer > 50000)
        {
            micon.sendmsg64(data, 1, 1, 20);
            if (micon.sendmsg64(data, 1, 1, 20))
            {
                led = !led;
            };

            data -= 0.1;

            printf("Send Data : %.2f ID : %d \n", data, id);
            send_timer = us_ticker_read();
        }
    }
    return 0;
}