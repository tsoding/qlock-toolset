#define NOB_IMPLEMENTATION
#include "nob.h"
#define STB_C_LEXER_IMPLEMENTATION
#include "stb_c_lexer.h"

bool build_exe(Nob_Cmd *cmd, const char *output_path, const char *input_path)
{
    cmd->count = 0;
    nob_cmd_append(cmd, "cc", "-Wall", "-Wextra");
    nob_cmd_append(cmd, "-o", output_path);
    nob_cmd_append(cmd, "-include", "time.h");
    nob_cmd_append(cmd, input_path);
    return nob_cmd_run_sync(*cmd);
}

bool compile_quine_blob(const char *output_path, const char *input_path)
{
    Nob_String_Builder sb = {0};
    if (!nob_read_entire_file(input_path, &sb)) return false;

    FILE *output = fopen(output_path, "wb");
    if (output == NULL) {
        nob_log(NOB_ERROR, "Could not read file %s: %s", output_path, strerror(errno));
        return false;
    }

    for (size_t i = 0; i < sb.count; ++i) {
        if (sb.items[i] == '?') {
            for (size_t j = 0; j < sb.count; ++j) {
                switch (sb.items[j]) {
                    case '\n': fprintf(output, "\\n\"\n\""); break;
                    case '\\': fprintf(output, "\\\\"); break;
                    case '"':  fprintf(output, "\\\""); break;
                    default: fprintf(output, "%c", sb.items[j]);
                }
            }
        } else {
            fprintf(output, "%c", sb.items[i]);
        }
    }

    fclose(output);

    nob_log(NOB_INFO, "Generated %s", output_path);

    return true;
}

bool is_unconcatable(long token)
{
    return (token == CLEX_id || token == CLEX_intlit);
}

bool format_tokens(const char *output_path, const char *input_path)
{
    Nob_String_Builder sb = {0};
    if (!nob_read_entire_file(input_path, &sb)) return false;

    FILE *output = fopen(output_path, "wb");
    if (output == NULL) {
        nob_log(NOB_ERROR, "Could not read file %s: %s", output_path, strerror(errno));
        return false;
    }

    stb_lexer l = {0};
    static char string_store[1024];
    stb_c_lexer_init(&l, sb.items, sb.items + sb.count, string_store, NOB_ARRAY_LEN(string_store));

    int x = 0;
    int prev_token = 0;
    while (stb_c_lexer_get_token(&l)) {
        int n = l.where_lastchar - l.where_firstchar + 1;
        if (is_unconcatable(prev_token) && is_unconcatable(l.token)) {
            fprintf(output, " ");
        }
        prev_token = l.token;
        fprintf(output, "%.*s", n, l.where_firstchar);
        x += n;
        if (x >= 80) {
            fprintf(output, "\n");
            x = 0;
        }
    }
    fclose(output);
    nob_log(NOB_INFO, "Generated %s", output_path);
    return true;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists("./build/")) return 1;

    Nob_Cmd cmd = {0};
    if (!build_exe(&cmd, "./build/quine", "quine.c")) return 1;
    if (!build_exe(&cmd, "./build/clock", "clock.c")) return 1;
    if (!format_tokens("./build/qlock-formatted.c", "qlock.c")) return 1;
    if (!compile_quine_blob("./build/qlock-blob.c", "./build/qlock-formatted.c")) return 1;
    if (!build_exe(&cmd, "./build/qlock", "./build/qlock-blob.c")) return 1;

    return 0;
}
