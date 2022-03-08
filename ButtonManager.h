#ifndef _BUTTONMANAGER_H_
#define _BUTTONMANAGER_H_

#include <Arduino.h>

class Button {
    private:
        int pin;
        boolean prevState;
        boolean pullup;
        boolean suspended, onPressSet, onReleaseSet, onChangeSet, onHoldSet;
        boolean holdTriggered;
        unsigned long holdStart;
        unsigned long holdDuration;
        void (*onPress)(void);
        void (*onRelease)(void);
        void (*onChange)(void);
        void (*onHold)(void);
    public:
        Button(int buttonPin, boolean pullup);
        Button(int buttonPin);
        void setOnPress(void (*callback)(void));
        void setOnRelease(void (*callback)(void));
        void setOnChange(void (*callback)(void));
        void setOnHold(void (*callback)(void), unsigned long holdMillis);
        void unSetOnPress(void);
        void unSetOnRelease(void);
        void unSetOnChange(void);
        void unSetOnHold(void);
        void check(void);
        void suspend(void);
        void resume(void);
        boolean pressed(void);
        int getPin(void);
};

#endif
