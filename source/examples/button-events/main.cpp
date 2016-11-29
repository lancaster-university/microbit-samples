/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "MicroBit.h"

MicroBit uBit;

//
// Print details of all events received to the serial port.
// Default settings are 115200 baud, 8N1 over the USB interface.
//
void onButton(MicroBitEvent e)
{
    if (e.source == MICROBIT_ID_BUTTON_A)
        uBit.serial.printf("BUTTON A: ");

    if (e.source == MICROBIT_ID_BUTTON_B)
        uBit.serial.printf("BUTTON B: ");

    if (e.source == MICROBIT_ID_BUTTON_AB)
        uBit.serial.printf("BUTTON A+B: ");

    if (e.source == MICROBIT_ID_IO_P0)
        uBit.serial.printf("TOUCH P0: ");

    if (e.source == MICROBIT_ID_IO_P1)
        uBit.serial.printf("TOUCH P1: ");

    if (e.source == MICROBIT_ID_IO_P2)
        uBit.serial.printf("TOUCH P2: ");

    if (e.value == MICROBIT_BUTTON_EVT_DOWN)
        uBit.serial.printf("DOWN");

    if (e.value == MICROBIT_BUTTON_EVT_UP)
        uBit.serial.printf("UP");

    if (e.value == MICROBIT_BUTTON_EVT_CLICK)
        uBit.serial.printf("CLICK");

    if (e.value == MICROBIT_BUTTON_EVT_LONG_CLICK)
        uBit.serial.printf("LONG_CLICK");

    if (e.value == MICROBIT_BUTTON_EVT_HOLD)
        uBit.serial.printf("HOLD");

    if (e.value == MICROBIT_BUTTON_EVT_DOUBLE_CLICK)
        uBit.serial.printf("DOUBLE_CLICK");

    uBit.serial.printf("\r\n");
}


int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    // Register to receive events when any buttons are clicked, including the A+B virtual button (both buttons at once).
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY, onButton);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_EVT_ANY, onButton);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_EVT_ANY, onButton);

    // Also register for touch events on P0, P1 and P2.
    uBit.messageBus.listen(MICROBIT_ID_IO_P0, MICROBIT_EVT_ANY, onButton);
    uBit.messageBus.listen(MICROBIT_ID_IO_P1, MICROBIT_EVT_ANY, onButton);
    uBit.messageBus.listen(MICROBIT_ID_IO_P2, MICROBIT_EVT_ANY, onButton);

    // Put the P0, P1 and P2 pins into touch sense mode.
    uBit.io.P0.isTouched();
    uBit.io.P1.isTouched();
    uBit.io.P2.isTouched();

    // We're done, so just enter a power efficient sleep while we wait for an event.
    while (1)
        uBit.sleep(10000);
}

