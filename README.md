button
======

Library to control push buttons, implemented in C. The library is designed to use only static
memory allocation and it can be used with any microcontroller.

Features
---

* CPU agnostic implementation
* use only static memory allocation
* configurable via macros
* support press, release and hold events
* support callback function for events

How to install
---

Simply copy the content of src directory to your work directory.


Configuration
---

The configuration of the library is done by setting 'define' macros in the header file,
under the configuration section.

There are two mandatory configurations before the use, the GPIO include file and the
setting of the *BUTTON_GPIO_GET* macro. In the include files section replace the line
`#include "gpio.h"` with the include of your GPIO library. Then, in the configuration
section, adjust the macro *BUTTON_GPIO_GET* to the "get gpio" function of your library.

To use the library with an Arduino the configuration would be:

    #include <Arduino.h>  // replace the "gpio.h"
    // ...
    #define BUTTON_GPIO_GET(port,pin)   digitalRead(pin)

The other settings are preset and can be adjusted as needed. The macro *BUTTON_MAX_INSTANCES*
is used to define the total amount of buttons being controlled by the library.
The macro *BUTTON_PRESSED_VALUE* defines if the logic value read when the button is pressed
and the *BUTTON_TICK_PERIOD* macro is used to set the interrupt service routine (ISR) period.

How to use
---

Once the configuration is done the last required step, before start to use the library, is to
place the function `button_tick` inside one timer ISR. Remember that the period of this timer must
match with the value in *BUTTON_TICK_PERIOD* macro.

Remark: this library does not configure the GPIO direction, you have to do it before use the library
functions.

To see more details how to use the library, please check the online
[API documentation](http://ricardocrudo.github.io/button).

License
---

MIT
