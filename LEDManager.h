#ifndef _LEDMANAGER_H_
#define _LEDMANAGER_H_

#include <Arduino.h>

class LED
{
    private:
        unsigned long onDur; // duration in milliseconds to be on for
        unsigned long offDur; // duration in milliseconds to be off for
        unsigned long nextTriggerTime; // next time to flip LED state
        unsigned long blinkStop; // duration to stop blink after
        bool mortalBlink; // if LED should stop blinking after a certain duration
        bool blinking; // if the LED is currently blinking
        bool state; // if the LED is on 
        int pin; // GPIO pin of LED
    public:
        /*
        * LED management constructor
        * @param pin GPIO pin number to output to
        */
        LED(int pin);

        /*
        * set LED on, cancels blinking
        * @return
        */
        void on(void);

        /*
        * set LED off, cancels blinking
        * @return
        */
        void off(void);

        /*
        * invert state of LED
        * does NOT cancel blinking
        * @return
        */
        void toggle(void);

        /*
        * blink for certain amount of time on, a certain amount of time off, and stop after a certain amount of time
        * @param onMillis on duration in milliseconds
        * @param offMillis off duration in milliseconds
        * @param stopAfter duration to stop after in milliseconds
        * @return
        */
        void blink(long onMillis, long offMillis, long stopAfter);

        /*
        * blink for certain amount of time on, a certain amount of time off
        * does not stop blinking until cancelled
        * @param onMillis on duration in milliseconds
        * @param offMillis off duration in milliseconds
        * @return
        */
        void blink(long onMillis, long offMillis);

        /*
        * blink for certain amount of time
        *  on and off period are the same
        * does not stop blinking until cancelled
        * @param interval duration of blink state in milliseconds
        * @return
        */
        void blink(long interval);

        /*
        * Still doesn't work, fuck this, basically the same as the last blink
        */
        void flash(long durationOn);

        /*
        * manually cancel blinking
        * @return
        */
        void cancelBlink(void);

        /*
        * check whether the LED is currently on or not
        * @return LED on state
        */ 
        boolean isOn(void);

        /*
        * refresh function, required for time based LED states
        * recommeneded running constantly in main loop
        * @return
        */ 
        void refresh(void);
};



#endif
