#pragma once

struct Command {
	typedef void Execution(char** args);

    const char* fullArg;
    const char* shortArg;
    const char* description;
    const char* args;
    Execution* execution;
};

void helpCommand(char** args);
void versionCommand(char** args);
void searchRdaCommand(char** args);
void searchDirectoryRdaCommand(char** args);
void replaceRdaFile(char** args);

static Command commands[] {
    {
        "-help",
        "h",
        "Shows a description of every command.",
        "",
        helpCommand
    },
    {
        "-version",
        "v",
        "Shows the version of the tool.",
        "",
        versionCommand
    },
    {
        "-search",
        "s",
        "Searches for a file in a specified RDA file.",
        "{search-value} {RDA-filepath}",
        searchRdaCommand
    },
    {
        "-search-batch",
        "sb",
        "Searches a directory of RDA's for a file.",
        "{search-value} {directory-of-RDA}",
        searchDirectoryRdaCommand
    },
    {
        "-replace-file",
        "r",
        "Replaces a file in a specified RDA file.",
        "{search-value} {RDA-filepath} {replacement-filepath}",
        replaceRdaFile
    }
};
