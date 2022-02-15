#ifndef _BUTTONMANAGER_H_
#define _BUTTONMANAGER_H_

#include <Arduino.h>

class Button {
    private:
        int pin;
        boolean prevState;
        boolean pullup;
        boolean suspended;
        void (*onPress)(void);
        void (*onRelease)(void);
        void (*onChange)(void);
    public:
        Button(int buttonPin, boolean pullup);
        void setOnPress(void (*callback)(void));
        void setOnRelease(void (*callback)(void));
        void setOnChange(void (*callback)(void));
        void check(void);
        void suspend(void);
        void resume(void);
};

#endif
