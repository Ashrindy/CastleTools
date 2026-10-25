#pragma once
#include <cstdio>
#include <string>
#include <filesystem>
#ifdef __WIN32
#include <fstream>
#endif

static size_t readFile(char* filepath, char*& buffer) {
    FILE* file = fopen(filepath, "rb");
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    buffer = new char[size];
    fread(buffer, 1, size, file);
    fclose(file);
    return size;
}

#ifdef __WIN32
static void writeFile(const std::filesystem::path& filepath, char* buffer, size_t size) {
    std::ofstream newFile{ filepath, std::ios::binary };
    newFile.write(buffer, size);
}
#else
static void writeFile(char* filepath, char* buffer, size_t size) {
    FILE* newFile = fopen(filepath, "wb");
    fwrite(buffer, size, 1, newFile);
    fclose(newFile);
}
inline static void writeFile(const std::filesystem::path& filepath, char* buffer, size_t size) {
    writeFile(filepath.string().c_str(), buffer, size);
}
#endif

static inline void deleteFile(char* data) {
    delete[] data;
}

static std::string getFileExtension(const std::string& filePath) {
    size_t dotPos = filePath.find_last_of('.');
    if (dotPos != std::string::npos)
        return filePath.substr(dotPos + 1);
    return "";
}

static std::string getFilePathWithoutExtension(const std::string& filePath) {
    size_t dotPos = filePath.find_last_of('.');
    if (dotPos != std::string::npos)
        return filePath.substr(0, dotPos + 1);
    return filePath;
}
