#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define HEIGHT 30
#define WIDTH 100

#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

int make_screen_array(char *screen_buffer);
int draw_symbol(int ball_x, int ball_y, char *screen_buffer, char symbol);
double ask_for_data(char *question);
int *calc_y_bounds();
int draw_graph();
int draw_max_min_scale(int *bounds, char *screen_buffer);
double evaluate(int x, int second_func);

int function_type;
double a, b, c;

double a2, b2, c2;
int second = 0;

double zoom = 1.0;

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
    printf("choose function (1 =  line, 2 = quadratic): ");
    scanf("%d", &function_type);
    getchar();
    
    if (function_type == 1) {
        b = ask_for_data("slope");
        c = ask_for_data("y_intercept");
    }
    else {
        a = ask_for_data("a");
        b = ask_for_data("b");
        c = ask_for_data("c");
    }

    printf("Do you want second function? (1=yes, 0=no): ");
    scanf("%d", &second);
    getchar();

    if (second) {
        printf("Second funtion:\n");

        if (function_type == 1) {
            b2 = ask_for_data("slope");
            c2 = ask_for_data("y_intercept");
        }
        else {
            a2 = ask_for_data("a");
            b2 = ask_for_data("b");
            c2 = ask_for_data("c");
        }
    }

    zoom = ask_for_data("zoom (1 = normal)");

    printf("\x1b[8;%d;%dt", HEIGHT + 1, WIDTH + 2);
    printf("\x1b[?25l");
    CLEAR_SCREEN();

    draw_graph();

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

    ball_y = -(ball_y - (HEIGHT - 1));

    int location = ball_y * (WIDTH + 1) + ball_x;
    screen_buffer[location] = symbol;

    return 0;
}

double ask_for_data(char *question) {
    while (1) {
        char input_buffer[50];

        printf("Enter %s: ", question);
        fgets(input_buffer, 50, stdin);

        char *temp_pointer = strchr(input_buffer, '\n');
        if (temp_pointer) *temp_pointer = '\0';

        double output = strtod(input_buffer, &temp_pointer);
        
        if(*temp_pointer == '\0') {
            return output;
        } else {
            printf("Please enter a number!\n");
        }
    }
}

double evaluate(int x, int second_func) {
    double real_x = x / zoom;

    if (!second_func)
    {
        if (function_type == 1) {
            return b * real_x + c;
        }
        else {
            return a * real_x * real_x + b * real_x + c;
        } 
    }
    else
    {
        if (function_type == 1) {
            return b2 * real_x + c2;
        }
        else {
            return a2 * real_x * real_x + b2 * real_x + c2;
        } 
    }
}

int *calc_y_bounds() {
    int *arr = malloc(2 * sizeof(int));
    
    arr[0] = INT_MAX;
    arr[1] = INT_MIN;

    for (int i = 0; i < WIDTH; i++)
    {
        double y1 = evaluate(i, 0);
        
        if (!isnan(y1) && !isinf(y1))
        {
            if (y1 > arr[1]) arr[1] = y1;
            if (y1 < arr[0]) arr[0] = y1;
        }

        if (second) {
            double y2 = evaluate(i, 1);

            if (!isnan(y2) && !isinf(y2))
            {
                if (y2 > arr[1]) arr[1] = y2;
                if (y2 < arr[0]) arr[0] = y2;
            }
        }
    }
    
    return arr;
}


int draw_graph() {
    
    char *screen_buffer = malloc((WIDTH + 1) * HEIGHT + 1);
    
    make_screen_array(screen_buffer);
    int *bounds = calc_y_bounds();
    
    int x_axis_location;
    
    if (bounds[0] == bounds[1]) {
        x_axis_location = (HEIGHT / 2);
    } else {
        double x_axis_ratio = (0.0 - bounds[0]) / (bounds[1] - bounds[0]);
        x_axis_location = (int)((HEIGHT - 1) * x_axis_ratio);
    }
    if (x_axis_location < 0) x_axis_location = 0;
    if (x_axis_location > HEIGHT - 1) x_axis_location = HEIGHT - 1;
    
    for (int i = 0; i < WIDTH; i++)
    {
        draw_symbol(i, x_axis_location, screen_buffer, '-');
    }

    for (int i = 0; i < WIDTH; i++)
    {
        double y = evaluate(i, 0);

        if (isnan(y) || isinf(y)) continue;

        int normal_y;
        if (bounds[0] == bounds[1])
        {
            normal_y = HEIGHT / 2;
        }
        else
        {
            normal_y = (int)((HEIGHT - 1) * ((y - bounds[0]) / (bounds[1] - bounds[0])));
        }
        if (normal_y < 0) normal_y = 0;
        if (normal_y > HEIGHT - 1) normal_y = HEIGHT - 1;
        
        draw_symbol(i, normal_y, screen_buffer, 'o');
    }

    if (second)
    {
        for (int i = 0; i < WIDTH; i++)
        {
            double y = evaluate(i, 1);

            if (isnan(y) || isinf(y)) continue;

            int normal_y;
            if (bounds[0] == bounds[1])
            {
                normal_y = HEIGHT / 2;
            }
            else
            {
                normal_y = (int)((HEIGHT - 1) * ((y - bounds[0]) / (bounds[1] - bounds[0])));
            }
            if (normal_y < 0) normal_y = 0;
            if (normal_y > HEIGHT - 1) normal_y = HEIGHT - 1;
            
            draw_symbol(i, normal_y, screen_buffer, 'x');
        }
        
    }
    
    draw_max_min_scale(bounds, screen_buffer);
    
    printf("\x1b[H");
    printf(GREEN "%s" RESET, screen_buffer);
    fflush(stdout);
    
    free(screen_buffer);
    free(bounds);

    return 0;
}

int draw_max_min_scale(int *bounds, char *screen_buffer) {
    int min = bounds[0], max = bounds[1];
    char min_string[20], max_string[20];

    if (min == max)
    {
        snprintf(max_string, sizeof(max_string), "%d", max);
        
        int location = (HEIGHT / 2) * (WIDTH + 1);
        size_t flat_len = strlen(max_string);
        if (flat_len > WIDTH)
        {
            flat_len = WIDTH;
        }
        
        memcpy(&screen_buffer[location], max_string, flat_len);

        return 0;
    }
    
    snprintf(min_string, sizeof(min_string), "%d", min);
    snprintf(max_string, sizeof(max_string), "%d", max);

    size_t max_len = strlen(max_string);
    if (max_len > WIDTH) {
        max_len = WIDTH;
    }
    
    memcpy(&screen_buffer[0], max_string, max_len);

    int location = (HEIGHT - 1) * (WIDTH + 1);
    size_t min_len = strlen(min_string);
    if (min_len > WIDTH)
    {
        min_len = WIDTH;
    }
    
    memcpy(&screen_buffer[location], min_string, min_len);

    return 0;
}
