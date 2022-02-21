# Hand of G

Motion control and BLE mouse simulation for gyroscope based mouse glove project AKA Hand of G (yro/reenberg)

Entirely self contained, only requires a computer to connect via Bluetooth (also works on Android for some reason)

# Hardware
Adafruit Feather nrf512832

MPU6050 combined Gyroscope Accelerometer Temperature sensor

3.7 1s Lipo (Feather has built in charge circuit)


# Resources
Uses arduino compiler with DFU bootloader

Adafruit nrf52 API 
https://learn.adafruit.com/bluefruit-nrf52-feather-learning-guide/bluefruit-nrf52-api

Adafruit MPU6050 library
https://adafruit.github.io/Adafruit_MPU6050/html/class_adafruit___m_p_u6050.html

HID over GATT profile reference for BLE 
https://www.ti.com/lit/an/swra715/swra715.pdf?ts=1644949603328
