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


/*
 * This is a simple example that uses the micro:bit radio to show if another
 * micro:bit running the same code is nearby.
 *
 * Each micro:bit is periodically broadcasting a group name, and listening
 * to see if another micro:bit in that group is also in-range. 
 *
 * When another micro:bit in the same group is detected, the display of the
 * detecting micro:bit will show a filled-in heart shape
 *
 * Otherwise, the display will show an empty heart shape
 *
 */

#include "MicroBit.h"

#if MICROBIT_BLE_ENABLED 
 #ifdef YOTTA_CFG
  #error "This example needs BLE to be disabled. Use the yotta config.json in the proximit-heart directory to do this"
 #else
  #error "This example needs BLE to be disabled in the microbit config file in the microbit-dal: MicroBitConfig.h"
 #endif
#endif

MicroBit    uBit;

// Have we seen a friend recently?
uint8_t friend_seen = 0;

// Are we currently sending out messages?
uint8_t broadcast = 1;

/* We have a group name, and if any micro:bit is in range and in the group
 * then the others will see it.
 */
const char group_name[] = "tiger";

const uint8_t empty_heart_arr[] {
                          0, 1, 0, 1, 0,
                          1, 0, 1, 0, 1,
                          1, 0, 0, 0, 1,
                          0, 1, 0, 1, 0,
                          0, 0, 1, 0, 0, };

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
                          0, 1, 1, 0, 0,
                          0, 0, 0, 0, 0, };

MicroBitImage empty_heart(5,5,empty_heart_arr);
MicroBitImage full_heart(5,5,full_heart_arr);
MicroBitImage small_heart(5,5,small_heart_arr);

/* We send messages when people press buttons 'A' and 'B'.
 * At the moment, all micro:bits listening for messages
 * will see these and can respond to them
 * Challenge: make only certain micro:bits respond to these
 */
void onButtonA(MicroBitEvent)
{
    uBit.radio.datagram.send("1");    
}

void onButtonB(MicroBitEvent)
{
    uBit.radio.datagram.send("2");    
}

/* We toggle broadcasting if both buttons are pressed together */
void onButtonAB(MicroBitEvent)
{
    broadcast = !broadcast;
    uBit.display.print("!");
}


void onData(MicroBitEvent)
{
    ManagedString s = uBit.radio.datagram.recv();
    int rssi = uBit.radio.getRSSI();

    if (s == "1")
        uBit.display.print(full_heart);

    if (s == "2")
        uBit.display.print(small_heart);
    
    /* For detecting the presence of our friend, we require them to be sending
     * the same group name as we are in 
     */ 
    if (s == group_name && rssi < 70) {
        // We can make this larger to allow more missed packets
        friend_seen = 3;
    }
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    // Setup some button handlers to allow direct heartbeat control with buttons
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY, onButtonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_EVT_ANY, onButtonB);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_EVT_ANY, onButtonAB);

    //Setup a hander to run when data is received
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onData);

    uBit.radio.enable();

    while(1) {
        if (friend_seen) {
	    uBit.display.print(full_heart); 
            friend_seen -= 1;
        } else {
            uBit.display.print(empty_heart);
        }
        
        if (broadcast)
            uBit.radio.datagram.send(group_name);

        uBit.sleep(1000);
    }
}
