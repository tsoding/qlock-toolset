#include <stdio.h>
#include <string.h>

int main()
{
    const char *src = "#include <stdio.h>\n#include <string.h>\n\nint main()\n{\n    const char *src = \"?\";\n    size_t src_n = strlen(src);\n    for (size_t i = 0; i < src_n; ++i) {\n        if (src[i] == 63) {\n            for (size_t j = 0; j < src_n; ++j) {\n                switch (src[j]) {\n                    case '\\n': printf(\"\\\\n\"); break;\n                    case '\"':  printf(\"\\\\\\\"\"); break;\n                    case '\\\\': printf(\"\\\\\\\\\"); break;\n                    default: printf(\"%c\", src[j]);\n                }\n            }\n        } else {\n            printf(\"%c\", src[i]);\n        }\n    }\n    return 0;\n}\n";
    size_t src_n = strlen(src);
    for (size_t i = 0; i < src_n; ++i) {
        if (src[i] == 63) {
            for (size_t j = 0; j < src_n; ++j) {
                switch (src[j]) {
                    case '\n': printf("\\n"); break;
                    case '"':  printf("\\\""); break;
                    case '\\': printf("\\\\"); break;
                    default: printf("%c", src[j]);
                }
            }
        } else {
            printf("%c", src[i]);
        }
    }
    return 0;
}
