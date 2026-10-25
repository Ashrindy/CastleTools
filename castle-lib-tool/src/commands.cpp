#include <castle-lib/archive.h>
#include <thread>
#include <castle-lib/internal.h>
#include "utils.h"
#include "files.h"
#include "commands.h"

void helpCommand(char** args) {
    printf("FORMATS\n");
    for (auto& file : files) {
        std::string extensions;
        for (auto* ext : file.extensions)
            extensions += "." + std::string{ ext } + " ";
        printf("%-22s%-35s\n", extensions.c_str(), file.description);
    }

    printf("COMMANDS\n");
    for (auto& cmd : commands)
        printf("-%-15s-%-5s%-55s%-35s\n", cmd.fullArg, cmd.shortArg, cmd.args, cmd.description);
}

void versionCommand(char** args) {
    printf("castle-lib-tool version %s\n", CASTLELIBTOOL_VERSION_S);
}

void searchRdaCommand(char** args) {
    char* searchValue = args[1];
    std::string searchValueS = convertToUtf8(searchValue);
    char* rdaFilePath = args[2];
    
    char* fileData = nullptr;
    size_t fileSize = readFile(rdaFilePath, fileData);

    castle::rda::v11::Archive* archive = castle::rda::v11::read(fileData, fileSize);
    deleteFile(fileData);

    for (auto& file : archive->files)
        if (file.filePath.find(searchValueS) != std::string::npos)
            printf("%s\n", file.filePath.c_str());

    castle::rda::v11::free(archive);
}

void searchDirectoryRdaCommand(char** args) {
    char* searchValue = args[1];
    std::string searchValueS = convertToUtf8(searchValue);
    char* rdaDirectory = args[2];

    std::vector<std::thread> searchThreads;
    for (auto& filepath : std::filesystem::directory_iterator{ rdaDirectory }) {
        auto ext = getFileExtension(filepath.path().string());
        if (ext != "RDA" && ext != "rda") continue;

        searchThreads.emplace_back([](const std::filesystem::directory_entry& filepath, const std::string& searchValueS) {
            char* fileData = nullptr;
            size_t fileSize = readFile((char*)filepath.path().string().c_str(), fileData);

            castle::rda::v11::Archive* archive = castle::rda::v11::read(fileData, fileSize);
            deleteFile(fileData);

            for (auto& file : archive->files)
                if (file.filePath.find(searchValueS) != std::string::npos)
                    printf("%s: %s\n", filepath.path().filename().string().c_str(), file.filePath.c_str());

            castle::rda::v11::free(archive);
            return;
        }, filepath, searchValueS);
    }

    for (auto& thread : searchThreads)
        thread.join();
}

void replaceRdaFile(char** args) {
    char* searchValue = args[1];
    std::string searchValueS = convertToUtf8(searchValue);
    char* rdaFilePath = args[2];
    char* replaceFilePath = args[3];
    char* fileData = nullptr;
    size_t fileSize = readFile(rdaFilePath, fileData);

    castle::rda::v11::Archive* archive = castle::rda::v11::read(fileData, fileSize);
    deleteFile(fileData);

    for (auto& file : archive->files) {
        if (file.filePath == searchValueS) {
            file.fileSize = readFile(replaceFilePath, file.fileData);
            break;
        }
    }

    fileSize = castle::rda::v11::write(archive, fileData);

    writeFile(rdaFilePath, fileData, fileSize);
    deleteFile(fileData);

    castle::rda::v11::free(archive);
}
