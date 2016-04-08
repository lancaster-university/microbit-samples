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

#ifndef MICROBIT_SAMPLES_H
#define MICROBIT_SAMPLES_H


//
// Uncomment ONE of the following #defines to select which sample to build.
// Afterwards, save this file and build the project. The resulting HEX
// file will contain your chosen sample.
//


//
// Introductory examples using the uBit object.
//

//#define     MICROBIT_SAMPLE_HELLO_WORLD
//#define     MICROBIT_SAMPLE_ACCELEROMETER_DEMO
//#define     MICROBIT_SAMPLE_BUTTON_EVENTS
//#define     MICROBIT_SAMPLE_SIMPLE_ANIMATION
#define     MICROBIT_SAMPLE_GREYSCALE
//#define     MICROBIT_SAMPLE_LOGIC_GATES
//#define     MICROBIT_SAMPLE_SNAKE

//
// Examples using MicroBitRadio.
//
// n.b. you MUST disable the BLE stack to run these samples.
// Do this by setting "#define MICROBIT_BLE_ENABLED 0" in your MicroBitConfig.h file.
//
// For yotta based environments this file is located at: 
//   "yotta_modules/microbit-dal/inc/core/MicroBitConfig.h"
//
// For project compiling on mbed.org, it is located at: 
//   "microbit/microbit-dal/inc/core/MicroBitConfig.h"
//

//#define     MICROBIT_SAMPLE_SIMPLE_RADIO_TX
//#define     MICROBIT_SAMPLE_SIMPLE_RADIO_RX


#endif
