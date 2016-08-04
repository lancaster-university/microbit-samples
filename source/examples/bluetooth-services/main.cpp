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

// we use events abd the 'connected' variable to keep track of the status of the Bluetooth connection
void onConnected(MicroBitEvent)
{
    uBit.display.print("C");
}

void onDisconnected(MicroBitEvent)
{
    uBit.display.print("D");
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    // Configuration Tips
    //
    // config.json contains various Bluetooth related properties some of which are explained here:
    //
    // "dfu_service": 1,               // 1 will cause the DFU service to be instantiated
    // "event_service": 1,             // 1 causes the event service to be instantiated
    // "device_info_service": 1
    // "enabled": 1,                   // 1 means the Bluetooth stack will be included as standard. 0 means it will not.
    // "pairing_mode": 1,              // 1 means it's possible to go into pairing mode which will include bringing up the Bluetooth stack whilst in that mode.
    // "open": 0,                      // 1 means there's no Bluetooth security i.e. no need to pair the micro:bit with other devices that want to communicate with it.
    // "tx_power": 7,                  // Transmission power of the Bluetooth radio. A value of 0 - 7 with 0 the lowest power and 7 the highest power.
    // "gatt_table_size": "0x700"      // Amount of memory (in hex bytes) set aside for the Bluetooth GATT table
    // "nested_heap_proportion": 0.75, // Reducing this can sometimes help make enough memory available for all the Bluetooth services you want. Only experiment with this as a last resort.

    // MicrobitConfig.h in yotta_modules\microbit-dal\inc\core contains MICROBIT_BLE_SECURITY_LEVEL which can be set to SECURITY_MODE_ENCRYPTION_WITH_MITM for passkey authentication when
    // pairing or SECURITY_MODE_ENCRYPTION_NO_MITM to use Just Works pairing.

    // A cunning code to indicate during start-up the particular Bluetooth configuration in the build
    //
    // SERVICE CODES
    // A: Accelerometer Service
    // B: Button Service
    // D: Device Information Service
    // E: Event Service
    // F: DFU Service
    // I: IO Pin Service
    // L: LED Service
    // M: Magnetometer Service
    // T: Temperature Service
    // U: UART Service
    //
    // PAIRING CONFIG
    // Note that switching pairing on or off is achieved by setting "open" in config.json to 1 or 0 respectively

    // P: PASSKEY
    // J: Just Works
    // N: No Pairing Required
    //
    // TX Power Level
    // 0-7 taken from tx_power in config.json


    // Services/Pairing Config/Power Level
    uBit.display.scroll("BLE ABDILMT/P/0");

    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);

    new MicroBitAccelerometerService(*uBit.ble, uBit.accelerometer);
    new MicroBitButtonService(*uBit.ble);
    new MicroBitIOPinService(*uBit.ble, uBit.io);
    new MicroBitLEDService(*uBit.ble, uBit.display);
    new MicroBitMagnetometerService(*uBit.ble, uBit.compass);
    new MicroBitTemperatureService(*uBit.ble, uBit.thermometer);

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
