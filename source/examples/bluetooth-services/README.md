# Bluetooth Services

The Bluetooth services project aims to provide a sample program that creates all of the services in micro:bit profile.

However, with the current Bluetooth stack the micro:bit will throw an `020` [out of memory error](https://microbit.org/guide/hardware/error-codes/) if all services are created simultaneously.

To avoid this both the DFU and Event Service are disabled in the project's `config.json`. If you wish to test these out, the easiest way to free enough memory is to disable the Magnetometer Service.

---

Enable DFU and Event Service in `config.json`
```
{
    "microbit-dal": {
        "bluetooth": {
            "enabled": 1,
            "pairing_mode": 1,
            "private_addressing": 0,
            "open": 0,
            "whitelist": 1,
            "advertising_timeout": 0,
            "tx_power": 0,
            "dfu_service": 1,
            "event_service": 1,
            "device_info_service": 1,
            "security_level": "SECURITY_MODE_ENCRYPTION_NO_MITM"
        },
        "gatt_table_size": "0x700"
    }
}
```

Disable the Magnetometer Service by commenting it out:
```
    new MicroBitAccelerometerService(*uBit.ble, uBit.accelerometer);
    new MicroBitButtonService(*uBit.ble);
    new MicroBitIOPinService(*uBit.ble, uBit.io);
    new MicroBitLEDService(*uBit.ble, uBit.display);
    // new MicroBitMagnetometerService(*uBit.ble, uBit.compass);
    new MicroBitTemperatureService(*uBit.ble, uBit.thermometer);
```
