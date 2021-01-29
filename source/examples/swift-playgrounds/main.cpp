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
#include "MicroBitUARTService.h"

#if !MICROBIT_BLE_ENABLED 
 #ifdef YOTTA_CFG
  #error "This example needs BLE to be enabled. Use the yotta config.json in the swift-playgrounds directory to do this"
 #else
  #error "This example needs BLE to be enabled in the microbit config file in the microbit-dal: MicroBitConfig.h"
 #endif
#endif

MicroBit    uBit;

const uint8_t full_heart_arr[] {
                          0, 1, 0, 1, 0,
                          1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1,
                          0, 1, 1, 1, 0,
                          0, 0, 1, 0, 0, }; 

const uint8_t small_heart_arr[] {
                          0, 0, 0, 0, 0,
                          0, 1, 0, 1, 0,
                          0, 1, 1, 1, 0,
                          0, 0, 1, 0, 0,
                          0, 0, 0, 0, 0, };

MicroBitImage full_heart(5,5,full_heart_arr);
MicroBitImage small_heart(5,5,small_heart_arr);

// we use events abd the 'connected' variable to keep track of the status of the Bluetooth connection
void onConnected(MicroBitEvent)
{
    uBit.display.print(full_heart);
}

void onDisconnected(MicroBitEvent)
{
    uBit.display.print(small_heart);
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    // Configuration Tips
    // Swift Playgrounds relies on the presence of the DFU service to intiate bonding, so
    // the DFU service must be enabled. The yotta config json file in the same directory
    // as this source file should be copied to the root of the samples repository.

    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);

    new MicroBitAccelerometerService(*uBit.ble, uBit.accelerometer);
    new MicroBitButtonService(*uBit.ble);
    new MicroBitIOPinService(*uBit.ble, uBit.io);
    new MicroBitLEDService(*uBit.ble, uBit.display);
    new MicroBitTemperatureService(*uBit.ble, uBit.thermometer);

    uBit.display.print(small_heart);
 
    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
