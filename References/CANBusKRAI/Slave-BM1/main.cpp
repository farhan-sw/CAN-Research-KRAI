/* mbed Microcontroller Library
 * Copyright (c) 2021 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "../CanBusKRAI/canKRAI.hpp"

#define NO_MICON 4

// Printing through USB Cable============================================
// bisar bisa pake printf
#define PRINTF_TX PB_6
#define PRINTF_RX PB_7
FileHandle *mbed::mbed_override_console(int fd)
{
    static BufferedSerial serial_port(PRINTF_TX, PRINTF_RX, 115200);
    return &serial_port;
}
//======================================================================
// buat lampu bisa blink"
#ifdef LED1
DigitalOut led(PC_13);
#else
bool led;
#endif
//======================================================================
// CAN BUS Setup=========================================================
CAN can(PA_11, PA_12,CAN_BAUD_RATE);
canKRAI micon;

// CAN Message Setup
uint32_t send_timer = 0;
uint32_t read_timer = 0;
//=======================================================================

int main()
{
    int speed = 0;
    int angle = 0;
    read_timer = us_ticker_read();
    send_timer = us_ticker_read();
    while (1)
    {
        if (us_ticker_read() - read_timer > 5000)
        {
            micon.readmsg(&speed, &angle, NO_MICON);
            read_timer = us_ticker_read();
        }
        printf("S%d %d %d %d\n", NO_MICON, speed, angle, micon.sendmsg32(speed, angle, NO_MICON));
    }
    return 0;
}