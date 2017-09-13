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

    button_t *button = button_create((const int []){0, 'f'}, 10);

    printw("Press f\n");

    while (1)
    {
        if (button_read(button))
        {
            printw("--> button pressed\n");
        }

        button_tick();
    }

    endwin();

    return 0;
}
