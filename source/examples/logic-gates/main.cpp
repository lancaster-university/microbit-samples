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

#define LOGIC_MODE_NOT            1
#define LOGIC_MODE_AND            2
#define LOGIC_MODE_OR             3
#define LOGIC_MODE_OUTPUT         4

#define LOGIC_MODE_MIN            1
#define LOGIC_MODE_MAX            4

#define TOOL_SELECT_DELAY   1000

MicroBit uBit;

MicroBitImage NOT("\
        0 0 1 0 0\n\
        0 0 1 1 0\n\
        1 1 1 1 1\n\
        0 0 1 1 0\n\
        0 0 1 0 0\n"); 

MicroBitImage AND("\
        0 0 1 1 0\n\
        1 1 1 1 1\n\
        0 0 1 1 1\n\
        1 1 1 1 1\n\
        0 0 1 1 0\n");

MicroBitImage OR("\
        0 0 0 1 0\n\
        1 1 1 1 1\n\
        0 0 0 1 1\n\
        1 1 1 1 1\n\
        0 0 0 1 0\n"); 

MicroBitImage OUTPUT_ON("\
        0 1 1 1 0\n\
        1 1 1 1 1\n\
        1 1 1 1 1\n\
        1 1 1 1 1\n\
        0 1 1 1 0\n"); 

MicroBitImage OUTPUT_OFF("\
        0 1 1 1 0\n\
        1 0 0 0 1\n\
        1 0 0 0 1\n\
        1 0 0 0 1\n\
        0 1 1 1 0\n"); 

int mode = LOGIC_MODE_NOT;

void onShake(MicroBitEvent)
{
    // The micro:bit has been shaken, so move on to the next logic gate.
    mode++;

    // Wrap back to the start if necessary.
    if (mode > LOGIC_MODE_MAX)
        mode = LOGIC_MODE_MIN;

    // Update the display to 
    switch (mode)
    {
        case LOGIC_MODE_NOT:
            uBit.display.print(NOT);
            break;

        case LOGIC_MODE_AND:
            uBit.display.print(AND);
            break;

        case LOGIC_MODE_OR:
            uBit.display.print(OR);
            break;

        case LOGIC_MODE_OUTPUT:
            uBit.display.print(OUTPUT_OFF);
            break;
    }
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    // Register to receive events when the micro:bit is shaken.
    uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_SHAKE, onShake);

    // 
    // Create a simple logic gate simulator, using the P0, P1 and P2 pins.
    // The micro:bit can then be configured as an NOT / AND / OR gate, by shaking the device.
    //
    int output = 0;

    // Our icons are drawn left to right, so rotate the display so the outputs point at the pins on the edge connector. :-)
    uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_270);

    while (1)
    {        
        // Check inputs and update outputs accordingly.
        switch (mode)
        {
            int o1;
            int o2;

            case LOGIC_MODE_NOT:
                output = uBit.buttonA.isPressed() ? 0 : !uBit.io.P0.getDigitalValue();
                uBit.display.print(NOT);
                break;
                
            case LOGIC_MODE_AND:
                o1 = uBit.buttonA.isPressed() || uBit.io.P0.getDigitalValue();
                o2 = uBit.buttonB.isPressed() || uBit.io.P1.getDigitalValue();
                output = o1 && o2;
                break;

            case LOGIC_MODE_OR:
                output = uBit.buttonA.isPressed() || uBit.io.P0.getDigitalValue() || uBit.buttonB.isPressed() || uBit.io.P1.getDigitalValue();
                break;

            case LOGIC_MODE_OUTPUT:
                if (uBit.io.P0.getDigitalValue())
                    uBit.display.print(OUTPUT_ON);                
                else
                    uBit.display.print(OUTPUT_OFF);
                
                output = 0;
                break;
        }
        
        // Update output pin value
        uBit.io.P2.setDigitalValue(output);

        // Perform a power efficient sleep for a little while. No need to run too quickly!
        uBit.sleep(1000);
    }
}    

