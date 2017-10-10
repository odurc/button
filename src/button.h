/*
 * Button Library
 * https://github.com/ricardocrudo/button
 *
 * Copyright (c) 2017 Ricardo Crudo <ricardo.crudo@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef BUTTON_H
#define BUTTON_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
****************************************************************************************************
*       INCLUDE FILES
****************************************************************************************************
*/

#include <stdint.h>

// adjust the header according your library
#include "gpio.h"


/*
****************************************************************************************************
*       MACROS
****************************************************************************************************
*/

#define BUTTON_VERSION      "0.0.0"


/*
****************************************************************************************************
*       CONFIGURATION
****************************************************************************************************
*/

// configure the function to read a GPIO
#define BUTTON_GPIO_GET(port,pin)   gpio_get(port,pin)

// maximum of button instances
#define BUTTON_MAX_INSTANCES        2

// configure the logic value read when the button is pressed (must be 0 or 1)
#define BUTTON_PRESSED_VALUE        0

// tick period in us
#define BUTTON_TICK_PERIOD          1000


/*
****************************************************************************************************
*       DATA TYPES
****************************************************************************************************
*/

/**
 * @struct button_t
 * An opaque structure representing a button object
 */
typedef struct button_t button_t;

/**
 * @struct button_event_t
 * Button events
 */
typedef enum button_event_t {
    BUTTON_PRESSED  = 0x01,
    BUTTON_RELEASED = 0x02,
    BUTTON_HELD     = 0x04,
} button_event_t;


/*
****************************************************************************************************
*       FUNCTION PROTOTYPES
****************************************************************************************************
*/

/**
 * @defgroup button_funcs Button Functions
 * Set of functions to read a button
 * @{
 */

/**
 * Create button object
 *
 * @param[in] pin an integer array of the port and pin of the button
 * @param[in] debounce_ms define the time in ms to debounce the button
 *
 * @return pointer to button object or NULL if no more button is available
 */
button_t* button_create(const int *pin, uint8_t debounce_ms);

/**
 * Read button state
 *
 * This function returns the button state after the debounce.
 *
 * @param[in] button button object pointer
 *
 * @return non zero value if button is active or zero otherwise
 */
int button_read(button_t *button);

/**
 * Check button status
 *
 * This function tests if an event has happened by passing the button_event_t
 * flags to the event parameter. Note that once the event status is read it
 * will be cleared automatically.
 *
 * @param[in] button button object pointer
 * @param[in] event button object pointer
 *
 * @return non zero value if button is active or zero otherwise
 */
int button_status(button_t *button, button_event_t event);

/**
 * Set hold time of button
 *
 * When the button is keep pressed by time_ms the flag BUTTON_HELD
 * is set. To disable the hold button feature set time_ms to zero.
 *
 * @param[in] button button object pointer
 * @param[in] time_ms the time in milliseconds
 */
void button_hold_time(button_t *button, uint16_t time_ms);

/**
 * Destroy button_t object
 *
 * @param[in] button button object pointer
 */
void button_destroy(button_t *button);

/**
 * The tick function
 *
 * This function is used to define the clock of the button core. It must be called
 * from an interrupt service routine (ISR). The period of the interruption must be set
 * using the BUTTON_TICK_PERIOD macro.
 */
void button_tick(void);

/**
 * @}
 */


/*
****************************************************************************************************
*       CONFIGURATION ERRORS
****************************************************************************************************
*/

#ifndef BUTTON_GPIO_GET
#error "BUTTON_GPIO_GET macro must defined"
#endif

#if BUTTON_PRESSED_VALUE < 0 || BUTTON_PRESSED_VALUE > 1
#error "BUTTON_PRESSED_VALUE must be set to 0 or 1"
#endif

#if BUTTON_TICK_PERIOD < 1000 || BUTTON_TICK_PERIOD > 100000
#error "BUTTON_TICK_PERIOD macro value must be set between 1000 and 100000"
#endif

#ifdef __cplusplus
}
#endif

// BUTTON_H
#endif
