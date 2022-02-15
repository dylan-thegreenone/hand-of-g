#include <Adafruit_MPU6050.h>
#include <bluefruit.h>
#include "BluefruitConfig.h"

const int MOVE_SCALE = 50;
const int SENSITIVITY_POT = A5;
const int INDEX_BUTTON = 11;
const int MIDDLE_BUTTON = 7;
const int RING_BUTTON = 15;
const int PINKY_BUTTON = 16;

bool indexPrevState = false;
bool middlePrevState = false;
bool ringPrevState = false;
bool pinkyPrevState = false;

unsigned long prevTime = 0;

int moveSensitivity = 0;

BLEDis bledis;
BLEHidAdafruit blehid;
Adafruit_MPU6050 mpu;


void startAdv(void)
{  
    // Advertising packet
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();
    Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_MOUSE);
    
    // Include BLE HID service
    Bluefruit.Advertising.addService(blehid);
  
    Bluefruit.Advertising.addName();
    Bluefruit.Advertising.restartOnDisconnect(AUTO_RESTART_ADVERTISING);
    Bluefruit.Advertising.setInterval(ADVERTISING_INTERVAL_MIN, ADVERTISING_INTERVAL_MAX);    // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(FAST_MODE_TIMEOUT);      // number of seconds in fast mode
    Bluefruit.Advertising.start(ADVERTISING_TIMEOUT);
}

void setup() 
{
    // setup input buttons/devices
    pinMode(SENSITIVITY_POT, INPUT);
    pinMode(INDEX_BUTTON, INPUT);
    pinMode(MIDDLE_BUTTON, INPUT);
    pinMode(RING_BUTTON, INPUT);
    pinMode(PINKY_BUTTON, INPUT);
    
    if (SERIAL_DEBUG) 
    {
        Serial.begin(SERIAL_BAUD);
        while ( !Serial ) delay(10);   // for nrf52840 with native usb
        
        Serial.println("Enter following characters");
        Serial.println("- 'WASD'  to move mouse (up, left, down, right)");
        Serial.println("- 'LRMBF' to press mouse button(s) (left, right, middle, backward, forward)");
        Serial.println("- 'X'     to release mouse button(s)");
    }
    if (PASSCODE_ENABLED)
    {
        Bluefruit.Security.setPIN(PASSCODE);
    }
    
    Bluefruit.setName(DEVICE_NAME);
    Bluefruit.begin(1,0);
    
//    Bluefruit.Periph.clearBonds();
    // HID Device can have a min connection interval of 9*1.25 = 11.25 ms
    Bluefruit.Periph.setConnInterval(CONN_INTERVAL_MIN, CONN_INTERVAL_MAX); // min = 9*1.25=11.25 ms, max = 16*1.25=20ms
    Bluefruit.setTxPower(4);   
    
    
    // Configure and Start Device Information Service
    bledis.setManufacturer(DEVICE_MANUFACTURER);
    bledis.setModel(DEVICE_MODEL);

//    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

    if (bledis.begin()) 
    {
        if (SERIAL_DEBUG)
        {
            Serial.println("Could not start BLE device information service");
        }
        while (1) delay(100);   //Necessary component, just accept futility if it fails
    }
    if (blehid.begin()) 
    {
        if (SERIAL_DEBUG)
        {
            Serial.println("Could not start BLE HID service");
        }
        while (1) delay(100);   //Necessary component, just accept futility if it fails
    }
    if (!mpu.begin()) 
    {
        if (SERIAL_DEBUG)
        {
            Serial.println("Could not find MPU6050 component");
        }
        // while (1) delay(100);   //Necessary component, just accept futility if it fails
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

    if (SERIAL_DEBUG)
    {
        Serial.println("All components started");
    }
   
    startAdv();
    if (SERIAL_DEBUG)
    {
        Serial.println("BLE Advertising started");
    }
    prevTime = millis();
}

void loop() 
{    
    moveSensitivity = map(analogRead(SENSITIVITY_POT), 1023, 0, 0, MOVE_SCALE);
    sensors_event_t a, g, t;
    mpu.getEvent(&a, &g, &t);
    
    if (Serial.available())
    {
//        Serial.println(Bluefruit.connected());
        char ch = (char) Serial.read();
        switch(ch)
        {
        // WASD to move the mouse
        case 'W':
            blehid.mouseMove(0, -moveSensitivity);
            break;
  
        case 'A':
            blehid.mouseMove(-moveSensitivity, 0);
            break;
  
        case 'S':
            blehid.mouseMove(0, moveSensitivity);
            break;
  
        case 'D':
            blehid.mouseMove(moveSensitivity, 0);
            break;
  
        // LRMBF for mouse button(s)
        case 'L':
            blehid.mouseButtonPress(MOUSE_BUTTON_LEFT);
            break;
  
        case 'R':
            blehid.mouseButtonPress(MOUSE_BUTTON_RIGHT);
            break;
  
        case 'M':
            blehid.mouseButtonPress(MOUSE_BUTTON_MIDDLE);
            break;
  
        case 'B':
            blehid.mouseButtonPress(MOUSE_BUTTON_BACKWARD);
            break;
  
        case 'F':
            blehid.mouseButtonPress(MOUSE_BUTTON_FORWARD);
            break;
  
        case 'X':
            blehid.mouseButtonRelease();
            break;
  
       default: 
            break;
       }
    }
    prevTime = millis();
}
