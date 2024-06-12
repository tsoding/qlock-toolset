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

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists("./build/")) return 1;

    Nob_Cmd cmd = {0};
    if (!build_exe(&cmd, "./build/quine", "quine.c")) return 1;
    if (!build_exe(&cmd, "./build/clock", "clock.c")) return 1;
    if (!build_exe(&cmd, "./build/qlock", "qlock.c")) return 1;

    return 0;
}
