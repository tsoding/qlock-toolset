#define NOB_IMPLEMENTATION
#include "nob.h"

bool build_exe(Nob_Cmd *cmd, const char *output_path, const char *input_path)
{
    cmd->count = 0;
    nob_cmd_append(cmd, "cc", "-Wall", "-Wextra");
    nob_cmd_append(cmd, "-o", output_path);
    nob_cmd_append(cmd, input_path);
    return  nob_cmd_run_sync(*cmd);
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
                    case '\n': fprintf(output, "\\n"); break;
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

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists("./build/")) return 1;

    Nob_Cmd cmd = {0};
    if (!build_exe(&cmd, "./build/quine", "quine.c")) return 1;
    if (!build_exe(&cmd, "./build/clock", "clock.c")) return 1;
    if (!compile_quine_blob("./build/qlock-blob.c", "qlock.c")) return 1;
    if (!build_exe(&cmd, "./build/qlock", "./build/qlock-blob.c")) return 1;

    return 0;
}
