#include "MicroBit.h"

MicroBit uBit;

char URL[] = "https://goo.gl/TlUTF7";
const int8_t CALIBRATED_POWERS[] = {-49, -37, -33, -28, -25, -20, -15, -10};

uint8_t advertising = 0;
uint8_t tx_power_level = 6;

void startAdvertising() {
    uBit.bleManager.advertiseEddystoneUrl(URL, CALIBRATED_POWERS[tx_power_level-1], false);
    uBit.bleManager.setTransmitPower(tx_power_level);
    uBit.display.scroll("ADV");
    advertising = 1;
}

void stopAdvertising() {
    uBit.bleManager.stopAdvertising();
    uBit.display.scroll("OFF");
    advertising = 0;
}

void onButtonA(MicroBitEvent)
{
    if (advertising == 1) {
        return;
    }
    startAdvertising();
}

void onButtonB(MicroBitEvent)
{
    if (advertising == 0) {
        return;
    }
    stopAdvertising();
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    
    startAdvertising();
    
    release_fiber();
}
