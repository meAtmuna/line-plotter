#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT 30
#define WIDTH 100

int make_screen_array(char *screen_buffer);
int draw_symbol(int ball_x, int ball_y, char *screen_buffer, char symbol);

#ifdef _WIN32
#include <windows.h>
#define SLEEP(t) Sleep(t * 1000)
#define CLEAR_SCREEN() system("cls")
#else
#include <unistd.h>
#define SLEEP(t) sleep(t)
#define CLEAR_SCREEN() system("clear")
#endif

int main() {
    printf("\x1b[8;%d;%dt", HEIGHT + 1, WIDTH + 2);
    printf("\x1b[?25l");
    CLEAR_SCREEN();

    char *screen_buffer = malloc((WIDTH + 1) * HEIGHT + 1);

    make_screen_array(screen_buffer);
    draw_symbol(10, 10, screen_buffer, '0');

    SLEEP(5);
    CLEAR_SCREEN();

    return 0;
}

int make_screen_array(char *screen_buffer) {
    screen_buffer[0] = '\0';

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            strcat(screen_buffer, " ");
        }
        if (y < HEIGHT - 1)
        {
            strcat(screen_buffer, "\n");
        }
    }
    
    return 0;
}

int draw_symbol(int ball_x, int ball_y, char *screen_buffer, char symbol) {
    
    if (ball_x < 0 || ball_x >= WIDTH || ball_y < 0 || ball_y >= HEIGHT) {
        return 1;
    }

    int location = ball_y * (WIDTH + 1) + ball_x;
    screen_buffer[location] = symbol;

    printf("\x1b[H");
    printf("%s", screen_buffer);
    fflush(stdout);

    return 0;
}