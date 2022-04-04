// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Main program for the Hand of G gyroscope and accelerometer enabled, motion based mouse glove
// loop is primarily for event updates, but still has serial test commands enabled
// Connects over BLE like any other bluetooth mouse
//
// @author Dylan Greenberg
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#include <Adafruit_MPU6050.h>
#include <bluefruit.h>
#include "BluefruitConfig.h"
#include "GestureManager.h"
#include "ButtonManager.h"
#include "LEDManager.h"

bool leftPressed, rightPressed, middlePressed, backPressed, frontPressed;

const int MOVE_SCALE = 50;
const int SENSITIVITY_POT = A5;
const int INDEX_BUTTON = 27;
const int MIDDLE_BUTTON = 30;
const int RING_BUTTON = 11;
const int PINKY_BUTTON = 7;
const int UTIL_BUTTON_1 = 15;
const int UTIL_BUTTON_2 = 16;
const int STATUS_LED_RED = A3;
// const int STATUS_LED_GREEN = A2;

unsigned long prevTime = 0;

int moveSensitivity = 0;
int ledStatus = 0;

//LED greenLED(STATUS_LED_GREEN);
LED redLED(STATUS_LED_RED);

BLEDis bledis;
BLEHidAdafruit blehid;
Adafruit_MPU6050 mpu;
GestureManager gMan (
  &blehid,
  &mpu,
  (int) INDEX_BUTTON,
  (int) RING_BUTTON,
  (int) MIDDLE_BUTTON,
  (int) PINKY_BUTTON
);
Button utilB1(UTIL_BUTTON_1);
Button utilB2(UTIL_BUTTON_2);


/*
* update LED pattern to reflect BLE connection status
* Stay on if connected
* Blinking with 0.5s intervals if advertising
* off if not advertising or connected
*/
void updateLEDStatus(void)
{
    if (Bluefruit.connected() > 0 && ledStatus != 1)
    {
        ledStatus = 1;
        redLED.on();
    }
    else if (Bluefruit.Advertising.isRunning() && ledStatus != 2)
    {
        redLED.blink(250, 250, ADVERTISING_TIMEOUT * 1000);
        ledStatus = 2;
    }
    else if (ledStatus != 3 && Bluefruit.connected() <= 0 && !Bluefruit.Advertising.isRunning())
    {
        redLED.off();
        ledStatus = 3;
    }
}
void disconnectAndStartAdv(void)
{  
    if (Bluefruit.connected() > 0)
    {
        Bluefruit.disconnect(0);
        if (SERIAL_DEBUG) Serial.println("Disconnected");
    }
    if (!Bluefruit.Advertising.isRunning())
    {
        Bluefruit.Advertising.start(ADVERTISING_TIMEOUT);
        redLED.blink(250, 250, ADVERTISING_TIMEOUT * 1000);
        if (SERIAL_DEBUG) Serial.println("Advertising started");
    }
//    updateLEDStatus();
}

// Must be run once before advertise is started
void setupAdv(void)
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
    Bluefruit.Advertising.setStopCallback(updateLEDStatus);
    
}

// if crucial component fails, prevent running further
// AKA feather freak out
void panic(const char* message)
{   
    if (SERIAL_DEBUG)
    {
        Serial.println(message);
    }
    // delay(250);
    redLED.blink(500);
    if (DEATH_ON_FAILURE) 
    {
        while (1) 
        {
            redLED.refresh();
            delay(10);
        }
    }
}

void setup() 
{
    // setup input buttons/devices
    pinMode(SENSITIVITY_POT, INPUT);

    if (SERIAL_DEBUG) 
    {
        Serial.begin(SERIAL_BAUD);
        while ( !Serial ) delay(10);   // for nrf52840 with native usb
        
        Serial.println("Enter following characters");
        Serial.println("- 'WASD'  to move mouse (up, left, down, right)");
        Serial.println("- 'LRMBF' to press mouse button(s) (left, right, middle, backward, forward)");
    }
    if (PASSCODE_ENABLED)
    {
        Bluefruit.Security.setPIN(PASSCODE);
    }
    
    Bluefruit.begin();
    Bluefruit.setName(DEVICE_NAME);
    // HID Device can have a min connection interval of 9*1.25 = 11.25 ms
    Bluefruit.Periph.setConnInterval(CONN_INTERVAL_MIN, CONN_INTERVAL_MAX); // min = 9*1.25=11.25 ms, max = 16*1.25=20ms
    Bluefruit.setTxPower(4);   
    
    setupAdv();
        
    // Configure and Start Device Information Service
    bledis.setManufacturer(DEVICE_MANUFACTURER);
    bledis.setModel(DEVICE_MODEL);
    

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    

    if (bledis.begin()) 
    {
        panic("Could not start BLE device information service");
    }
    if (blehid.begin()) 
    {
        panic("Could not start BLE HID service");
    }
    if (!mpu.begin()) 
    {
        panic("Could not find MPU6050 acceleromoter/gyroscope component");
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

    if (SERIAL_DEBUG)
    {
        Serial.println("All components started");
    }
     
    if (SERIAL_DEBUG && DEATH_ON_FAILURE)
    {
        Serial.println("BLE Advertising started");
    }

    utilB1.setOnRelease(disconnectAndStartAdv);
    utilB1.setOnHold([](){
        if (SERIAL_DEBUG) Serial.println("Bonds cleared");
        Bluefruit.Periph.clearBonds();
        disconnectAndStartAdv();
    }, 4000);
    
//    Serial.println("WW");

    gMan.startGestures();

    prevTime = millis();
}

void loop() 
{    
    moveSensitivity = map(analogRead(SENSITIVITY_POT), 1023, 0, 0, SENSITIVITY_MAX);
//    gMan.setSensitivity(moveSensitivity);
    
    sensors_event_t a, g, t;
    mpu.getEvent(&a, &g, &t);
//    sensors_event_t a, g, t;
//    this->mpu.getEvent(&a, &g, &t);
    double xAccel = a.acceleration.x;
    double yAccel = a.acceleration.y;
    double zAccel = a.acceleration.z;

    double xRot = g.gyro.x;
    double yRot = g.gyro.y;
    double zRot = g.gyro.z;

//    Serial.println(String(xAccel) + ", " + String(yAccel) + ", " + String(zAccel) + ", " + String(xRot) + ", " + String(yRot) + ", " + String(zRot));

    // double aX = a.acceleration.x;
    utilB1.check();
    utilB2.check();
    redLED.refresh();
    gMan.refresh();

    updateLEDStatus();

    
    if (Serial.available())
    {
        char ch = (char) Serial.read();
        int xDiff = 0;
        int yDiff = 0;
        switch(ch)
        {
        // WASD to move the mouse
        case 'W':
            yDiff = -moveSensitivity;
            break;
  
        case 'A':
            xDiff = -moveSensitivity;
            break;
  
        case 'S':
            yDiff = moveSensitivity;
            break;
  
        case 'D':
            xDiff = moveSensitivity;
            break;
  
        // LRMBF for mouse button(s)
        case 'L':
            leftPressed = !leftPressed;
            break;
  
        case 'R':
            rightPressed = !rightPressed;
            break;
  
        case 'M':
            middlePressed = !middlePressed;
            break;
  
        case 'B':
            backPressed = !backPressed;
            break;
  
        case 'F':
            frontPressed = !frontPressed;
            break;
  
        case 'X':
            rightPressed = false;
            leftPressed = false;
            middlePressed = false;
            backPressed = false;
            frontPressed = false;
            break;
  
       default: 
            break;
       }
       
        uint8_t buttonBin = leftPressed * MOUSE_BUTTON_LEFT + rightPressed * MOUSE_BUTTON_RIGHT + middlePressed * MOUSE_BUTTON_MIDDLE;
        blehid.mouseReport(buttonBin, xDiff, yDiff);
        Serial.println(String(xDiff) + "," + String(yDiff) + "," + String(buttonBin));
    }
    prevTime = millis();
}
