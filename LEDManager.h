#ifndef _LEDMANAGER_H_
#define _LEDMANAGER_H_

#include <Arduino.h>

class LED
{
    private:
        unsigned long interval;
        unsigned long nextTriggerTime;
        unsigned long blinkStop;
        boolean mortalBlink;
        boolean blinking;
        boolean state;
        int pin;
    public:
        LED(int pin);
        void on(void);
        void off(void);
        void toggle(void);
        void blink(long interval,long stopAfter);
        void blink(long interval);
        void cancelBlink(void);
        boolean isOn(void);
        void refresh(void);
};



#endif
