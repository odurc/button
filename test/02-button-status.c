#include <stdio.h>
#include <curses.h>
#include "button.h"

int gpio_get(int port, int pin)
{
    (void) port;
    char key = getch();

    if (key == pin)
        return 0;

    return 1;
}

int main(void)
{
    initscr();
    noecho();
    timeout(50);

    button_t *button = button_create((const int []){0, 'f'}, 1);
    button_hold_time(button, 50);

    printw("Press f\n");

    while (1)
    {
        if (button_status(button, BUTTON_PRESSED))
        {
            printw("--> button pressed\n");
        }
        else if (button_status(button, BUTTON_RELEASED))
        {
            printw("--> button released\n");
        }
        else if (button_status(button, BUTTON_HELD))
        {
            printw("--> button held\n");
        }

        button_tick();
    }

    endwin();

    return 0;
}
