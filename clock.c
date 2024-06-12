#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define FONT_COUNT 11
int font[FONT_COUNT] = {31599,19812,14479,31207,23524,29411,29679,30866,31727,31719,1040};
#define FONT_ROWS 5
#define FONT_COLS 3
#define DIGITS_COUNT 8
#define DIGITS_PAD 2
#define DISPLAY_WIDTH (FONT_COLS + DIGITS_PAD)*DIGITS_COUNT
#define DISPLAY_HEIGHT FONT_ROWS

int main(void)
{
    for (;;) {
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);

        int digits[DIGITS_COUNT];
        digits[0] = tm->tm_hour/10;
        digits[1] = tm->tm_hour%10;
        digits[2] = 10;
        digits[3] = tm->tm_min/10;
        digits[4] = tm->tm_min%10;
        digits[5] = 10;
        digits[6] = tm->tm_sec/10;
        digits[7] = tm->tm_sec%10;
        for (int y = 0; y < DISPLAY_HEIGHT; ++y) {
            for (int x = 0; x < DISPLAY_WIDTH; ++x) {
                int i = x/(FONT_COLS + DIGITS_PAD);
                int dx = x%(FONT_COLS + DIGITS_PAD);
                if (dx < FONT_COLS && (font[digits[i]]>>((FONT_ROWS - y - 1)*FONT_COLS + dx))&1) {
                    // printf("\033[1;33m#\033[0m");
                    printf("\033[1;31m█\033[0m");
                } else {
                    printf("█");
                }
            }
            printf("\n");
        }

        printf("\033[%dA\033[%dD", DISPLAY_HEIGHT, DISPLAY_WIDTH);
        sleep(1);
    }
    return 0;
}
