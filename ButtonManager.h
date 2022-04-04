#ifndef _BUTTONMANAGER_H_
#define _BUTTONMANAGER_H_

#include <Arduino.h>

class Button {
    private:
        int pin; // GPIO pin button is connected to
        boolean prevState; // state of button on last refresh 
        boolean pullup; // if the button is in pullup mode, needed to invert signal
        boolean suspended; // if the button manager has been suspended
        boolean onPressSet, onReleaseSet, onChangeSet, onHoldSet;  // whether each of the callbacks has been set, required to prevent null ptr being call
        boolean holdTriggered; // whether the button has been held for long enough to trigger onHold callback
        unsigned long holdStart; // when the button started being held down
        unsigned long holdDuration; // how long the button has to be pressed for to count as held down
        void (*onPress)(void); // callback when button is pressed down
        void (*onRelease)(void); // callback when button is released
        void (*onChange)(void); // callback when button state changes (pressed or released)
        void (*onHold)(void); // callback when button has been held for a certain duration (cancels onRelease callback until buttons been reset)
    public:
        /*
        * Full button constructor
        * @param buttonPin GPIO pin to monitor as a button
        * @param pullup set to true if button needs to use internal pullup resistor
        */
        Button(int buttonPin, boolean pullup);

        /*
        * Common button constructor, delegates to full constructor
        * Default is a non-pullup input
        * @param buttonPin GPIO pin to monitor as a button
        */
        Button(int buttonPin);

        /*
        * set callback to be called on button press
        * @param callback to set
        * @return
        */
        void setOnPress(void (*callback)(void));

        /*
        * set callback to be called on button release 
        * @param callback to set
        * @return
        */
        void setOnRelease(void (*callback)(void));

        /*
        * set callback to be called on button state change
        * @param callback to set
        * @return
        */
        void setOnChange(void (*callback)(void));
        
        /*
        * set callback to be called on button hold
        * @param callback to set
        * @param holdMillis millisecond duration of press required to trigger callback
        * @return
        */
        void setOnHold(void (*callback)(void), unsigned long holdMillis);

        /*
        * unset callback to be called on button press
        * @return
        */
        void unSetOnPress(void);

        /*
        * unset callback to be called on button release
        * @return
        */
        void unSetOnRelease(void);
        
        /*
        * unset callback to be called on button state change 
        * @return
        */
        void unSetOnChange(void);

        /*
        * unset callback to be called on button hold
        * @return
        */
        void unSetOnHold(void);

        /*
        * refresh function, updates state of button manager
        * for fast state updates, recommended run constantly main loop
        * @param
        */
        void check(void);

        /*
        * suspend button monitoring
        * refreshing state will not trigger callbacks
        * changing button state does change state
        * @return
        */ 
        void suspend(void);

        /*
        * resume button monitoring
        * @return
        */ 
        void resume(void);
        
        /*
        * get most recent button state set during refresh
        * @return button state
        */ 
        boolean pressed(void);

        /*
        * get pin of button
        * @return pin number
        */
        int getPin(void);
};

#endif
