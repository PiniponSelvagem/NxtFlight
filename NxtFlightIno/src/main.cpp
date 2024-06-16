#include <Arduino.h>

#include "pinicore.h"

//#include "avr8-stub.h"
//#include "app_api.h"

Led led = Led();
Led led2 = Led(false, 22);

BTN_INFO btns[] = { {24}, {26} };
Button button = Button(btns, sizeof(btns));

void setup() {
    //debug_init();
    //Serial.begin(115200);
    led.init();
    led2.init();
    button.init();
}

void loop() {
    button.pullButtonsEvents();
    if (button.isSinglePressed(0))
        led.toggle();
    if (button.isSinglePressed(1))
        led2.toggle();
    delay(100);
}