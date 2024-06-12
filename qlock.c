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
    const char *src = "#include <stdio.h>\n#include <string.h>\n#include <time.h>\n#include <unistd.h>\n\n#define FONT_COUNT 11\nint font[FONT_COUNT] = {31599,19812,14479,31207,23524,29411,29679,30866,31727,31719,1040};\n#define FONT_ROWS 5\n#define FONT_COLS 3\n#define DIGITS_COUNT 8\n#define DIGITS_PAD 2\n#define DISPLAY_WIDTH (FONT_COLS + DIGITS_PAD)*DIGITS_COUNT\n#define DISPLAY_HEIGHT FONT_ROWS\n\nvoid pixel(int ch, int x, int y, int digits[DIGITS_COUNT])\n{\n\n    int i = x/(FONT_COLS + DIGITS_PAD);\n    int dx = x%(FONT_COLS + DIGITS_PAD);\n    if (y < FONT_ROWS && dx < FONT_COLS && (font[digits[i]]>>((FONT_ROWS - y - 1)*FONT_COLS + dx))&1) {\n        printf(\"\\033[1;31m%c\\033[0m\", ch);\n    } else {\n        printf(\"%c\", ch);\n    }\n}\n\nint main()\n{\n    const char *src = \"?\";\n    size_t src_n = strlen(src);\n    for (;;) {\n        time_t t = time(NULL);\n        struct tm *tm = localtime(&t);\n\n        int digits[DIGITS_COUNT];\n        digits[0] = tm->tm_hour/10;\n        digits[1] = tm->tm_hour%10;\n        digits[2] = 10;\n        digits[3] = tm->tm_min/10;\n        digits[4] = tm->tm_min%10;\n        digits[5] = 10;\n        digits[6] = tm->tm_sec/10;\n        digits[7] = tm->tm_sec%10;\n\n        int x = 0;\n        int y = 0;\n        for (size_t i = 0; i < src_n && y < FONT_ROWS; ++i) {\n            switch (src[i]) {\n                case 63: {\n                    for (size_t j = 0; j < src_n; ++j) {\n                        switch (src[j]) {\n                            case '\\n': {\n                                pixel('\\\\', x, y, digits);\n                                x += 1;\n                                pixel('n', x, y, digits);\n                                x += 1;\n                            } break;\n                            case '\"': {\n                                pixel('\\\\', x, y, digits);\n                                x += 1;\n                                pixel('\\\"', x, y, digits);\n                                x += 1;\n                            } break;\n                            case '\\\\': {\n                                pixel('\\\\', x, y, digits);\n                                x += 1;\n                                pixel('\\\\', x, y, digits);\n                                x += 1;\n                            } break;\n                            default: {\n                                pixel(src[j], x, y, digits);\n                                x += 1;\n                            }\n                        }\n                    }\n                } break;\n\n                case '\\n': {\n                    y += 1;\n                    x = 0;\n                    pixel('\\n', x, y, digits);\n                } break;\n\n                default: {\n                    pixel(src[i], x, y, digits);\n                    x += 1;\n                }\n            }\n        }\n    }\n    return 0;\n}\n";
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
