#include "MicroBit.h"

MicroBit uBit;

char UID_NAMESPACE[] = {0x0E,0x67,0x47,0x04,0x42,0xD0,0x14,0x06,0xD5,0x83}; // sha-1 hash of "com.bittysoftware" 
char UID_INSTANCE[]  = {0x00 , 0x00, 0x00, 0x00, 0x00, 0x01};
const int8_t CALIBRATED_POWERS[] = {-49, -37, -33, -28, -25, -20, -15, -10};

uint8_t advertising = 0;
uint8_t tx_power_level = 6;

void startAdvertising() {
    uBit.bleManager.advertiseEddystoneUid(UID_NAMESPACE, UID_INSTANCE, CALIBRATED_POWERS[tx_power_level-1], false);
    uBit.bleManager.setTransmitPower(6);
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
