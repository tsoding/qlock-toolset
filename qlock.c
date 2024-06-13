#include <stdio.h>
#include <string.h>
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

void pixel(int ch, int x, int y, int digits[DIGITS_COUNT])
{
    int i = x/(FONT_COLS + DIGITS_PAD);
    int dx = x%(FONT_COLS + DIGITS_PAD);
    if (y < FONT_ROWS && dx < FONT_COLS && (font[digits[i]]>>((FONT_ROWS - y - 1)*FONT_COLS + dx))&1) {
        printf("\033[1;31m%c\033[0m", ch);
    } else {
        printf("%c", ch);
    }
}

int main()
{
    const char *src = "?";
    size_t src_n = strlen(src);
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

        int x = 0;
        int y = 0;
        for (size_t i = 0; i < src_n && y < FONT_ROWS; ++i) {
            switch (src[i]) {
                case 63: {
                    for (size_t j = 0; j < src_n; ++j) {
                        switch (src[j]) {
                            case '\n': {
                                pixel('\\', x, y, digits);
                                x += 1;
                                pixel('n', x, y, digits);
                                x += 1;
                            } break;
                            case '"': {
                                pixel('\\', x, y, digits);
                                x += 1;
                                pixel('\"', x, y, digits);
                                x += 1;
                            } break;
                            case '\\': {
                                pixel('\\', x, y, digits);
                                x += 1;
                                pixel('\\', x, y, digits);
                                x += 1;
                            } break;
                            default: {
                                pixel(src[j], x, y, digits);
                                x += 1;
                            }
                        }
                    }
                } break;

                case '\n': {
                    y += 1;
                    x = 0;
                    pixel('\n', x, y, digits);
                } break;

                default: {
                    pixel(src[i], x, y, digits);
                    x += 1;
                }
            }
        }

        printf("\033[%dA\033[%dD", y, x);
        sleep(1);
    }
    return 0;
}
