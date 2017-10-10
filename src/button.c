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

/*
****************************************************************************************************
*       INCLUDE FILES
****************************************************************************************************
*/

#include "button.h"


/*
****************************************************************************************************
*       INTERNAL MACROS
****************************************************************************************************
*/

#define RELEASED_VALUE      (1 - BUTTON_PRESSED_VALUE)
#define BUTTON_READ(b)      (BUTTON_GPIO_GET(b->pin[0], b->pin[1]) ^ RELEASED_VALUE)


/*
****************************************************************************************************
*       INTERNAL CONSTANTS
****************************************************************************************************
*/


/*
****************************************************************************************************
*       INTERNAL DATA TYPES
****************************************************************************************************
*/

struct button_t {
    const int *pin;
    uint8_t state, debounce;
    uint16_t count, hold_time;

    union {
        uint8_t flags;
        struct {
            uint8_t pressed_event:1;
            uint8_t released_event:1;
            uint8_t held_event:1;

            uint8_t pressed:1;
            uint8_t released:1;
            uint8_t held:1;
        };
    };
};


/*
****************************************************************************************************
*       INTERNAL GLOBAL VARIABLES
****************************************************************************************************
*/

static button_t g_buttons[BUTTON_MAX_INSTANCES];
static unsigned int g_buttons_available = BUTTON_MAX_INSTANCES;


/*
****************************************************************************************************
*       INTERNAL FUNCTIONS
****************************************************************************************************
*/

static inline button_t* button_take(void)
{
    static unsigned int button_counter;

    // first round request
    if (button_counter < BUTTON_MAX_INSTANCES)
    {
        g_buttons_available--;
        button_t *button = &g_buttons[button_counter++];
        return button;
    }

    // iterate all array searching for a free spot
    // a button is considered free when pin is null
    for (int i = 0; i < BUTTON_MAX_INSTANCES; i++)
    {
        button_t *button  = &g_buttons[i];

        if (button->pin == 0)
        {
            g_buttons_available--;
            return button;
        }
    }

    return 0;
}

static inline void button_give(button_t *button)
{
    if (button)
    {
        button->pin = 0;
        g_buttons_available++;
    }
}


/*
****************************************************************************************************
*       GLOBAL FUNCTIONS
****************************************************************************************************
*/

button_t* button_create(const int *pin, uint8_t debounce_ms)
{
    button_t *button = button_take();
    button->pin = pin;
    button->state = 0;
    button->count = 0;
    button->flags = 0;
    button->debounce = 1000 * debounce_ms / BUTTON_TICK_PERIOD;
    button->hold_time = 0;
    button->released = 1;

    return button;
}

void button_destroy(button_t *button)
{
    button_give(button);
}

inline int button_read(button_t *button)
{
    return button->state;
}

int button_status(button_t *button, button_event_t event)
{
    unsigned int flags = button->flags;

    event &= 0x0F;
    button->flags &= (~event);

    return (flags & event);
}

inline void button_hold_time(button_t *button, uint16_t time_ms)
{
    button->hold_time = time_ms;
}

void button_tick(void)
{
    for (int i = 0; i < BUTTON_MAX_INSTANCES; i++)
    {
        button_t *button = &g_buttons[i];

        // skip button if not in use
        if (button->pin == 0)
            continue;

        // read button
        int button_on = BUTTON_READ(button);

        // debounce using integrator algorithm
        // algorithm by Kenneth A. Kuhn
        if (button_on)
        {
            if (button->count < button->debounce)
                button->count++;
            else
                button->state = 1;
        }
        else
        {
            if (button->count > button->debounce)
                button->count = button->debounce;

            if (button->count > 0)
                button->count--;
            else
                button->state = 0;
        }

        // pressed event
        if (button->state & !button->pressed)
        {
            button->pressed = 1;
            button->pressed_event = 1;
            button->released = 0;
        }

        // released event
        if (!button->state & !button->released)
        {
            button->released = 1;
            button->released_event = 1;
            button->pressed = 0;
            button->held = 0;
        }

        // hold event
        if (button->state && button_on && button->hold_time > 0)
        {
            if (button->count < button->hold_time)
            {
                button->count++;
            }
            else if (!button->held)
            {
                button->held = 1;
                button->held_event = 1;
            }
        }
    }
}
