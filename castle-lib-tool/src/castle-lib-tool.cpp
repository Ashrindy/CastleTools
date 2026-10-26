#include <cstring>
#include "utils.h"
#include "files.h"
#include "commands.h"
#ifdef __WIN32
#include <windows.h>
#endif

inline bool isCommand(char** args) {
    return args[0][0] == '-';
}

void parseCommand(char** args) {
    for (auto& cmd : commands) {
        if (strcmp(&args[0][1], cmd.fullArg) == 0 || strcmp(&args[0][1], cmd.shortArg) == 0) {
            cmd.execution(args);
            break;
        }
    }
}

inline void readFile(char* filepath) {
    auto fileExt = getFileExtension(filepath);
    for (auto& file : files) {
        for (auto* ext : file.extensions) {
            if (fileExt == ext) {
                file.execution(filepath);
                break;
            }
        }
    }
}

void parseFiles(char** args, int argCount) {
    for (auto x = 0; x < argCount; x++)
        readFile(args[x]);
}

int main(int argc, char* argv[]) {
#ifdef __WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    int argCount = argc - 1;
    char** args = nullptr;
    if (argCount > 0)
        args = &argv[1];
    else {
        printf("No arguments were provided, try running -h or --help for detailed instructions.\n");
        return 0;
    }

    if (isCommand(args))
        parseCommand(args);
    else
        parseFiles(args, argCount);

    return 0;
}
