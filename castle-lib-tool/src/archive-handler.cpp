#include "utils.h"
#include <castle-lib/archive.h>

void parseArchive(char* filepath) {
    char* fileData = nullptr;
    size_t fileSize = readFile(filepath, fileData);

    castle::rda::v11::Archive* archive = castle::rda::v11::read(fileData, fileSize);
    deleteFile(fileData);
    
    std::filesystem::path path{ filepath };
    auto parentPath = path.parent_path();

    for (auto& file : archive->files) {
        auto filePath = parentPath / std::filesystem::path{ file.filePath };
        std::filesystem::create_directories(filePath.parent_path());

        writeFile(filePath, file.fileData, file.fileSize);

        auto systemTime = std::chrono::system_clock::from_time_t(file.fileDate);
        auto fileTime = std::chrono::time_point_cast<std::filesystem::file_time_type::duration>(systemTime - std::chrono::system_clock::now() + std::filesystem::file_time_type::clock::now());

        std::filesystem::last_write_time(filePath, fileTime);
    }

    castle::rda::v11::free(archive);
}
