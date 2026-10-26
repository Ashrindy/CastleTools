#pragma once
#include <initializer_list>

struct File {
	typedef void Execution(char* filepath);

    std::initializer_list<const char*> extensions;
    const char* description;
    Execution* execution;
};

void parseArchive(char* filepath);
//void parseModel(char* filepath);

static File files[] {
    {
        { "rda", "RDA" },
        "Archive file format.",
        parseArchive
    }
    /*,
    {
        { "rdo", "RDO", "rdw" },
        "Model file format.",
        parseModel
    }*/
};
