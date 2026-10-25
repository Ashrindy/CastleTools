#include <castle-lib/archive.h>
#include <castle-lib/internal.h>
#include <cstring>
#include <string>
#include <thread>
#include <zlib.h>

using namespace castle::rda::v11;

struct RawFile {
	char filePath[256];
	int dataPtr;
	int compressedSize;
	int uncompressedSize;
	int compressionLevel;
	int fileDate;
};

struct RawArchive {
	char signature[256];
	int fileCount;
};

Archive* castle::rda::v11::read(char* data, size_t size) {
    Archive* archive = new Archive{};

    size_t pos = 0;
	auto* rawArc = (RawArchive*)data;
	pos += sizeof(RawArchive);

#ifdef CASTLELIB_RDA_MULTITHREADED_UNCOMPRESS
	std::vector<std::vector<char>> uncompressedFiles{ (size_t)rawArc->fileCount };
	std::vector<std::thread> threads{};
	std::vector<RawFile*> rawFiles{};

	for (auto x = 0; x < rawArc->fileCount; x++) {
		auto* i = (RawFile*)&data[pos];
		auto* compData = &data[i->dataPtr];
		threads.emplace_back([](char* compressedData, size_t compressedSize, size_t uncompressedSize, std::vector<char>* data) {
			data->resize(uncompressedSize);
			if (compressedSize == uncompressedSize)
				memcpy(data->data(), compressedData, uncompressedSize);
			else
				uncompress((Bytef*)data->data(), (uLongf*)&uncompressedSize, (const Bytef*)compressedData, (uLong)compressedSize);
			return;
		}, compData, i->compressedSize, i->uncompressedSize, &uncompressedFiles[x]);
		rawFiles.push_back(i);
		pos += sizeof(RawFile);
	}

	for (auto& thread : threads) {
		thread.join();
	}

	for (auto x = 0; x < rawArc->fileCount; x++) {
		auto* i = rawFiles[x];
		File& y = archive->files.emplace_back();
		y.filePath = convertToUtf8({ i->filePath });
		y.fileDate = i->fileDate;
		y.compressionLevel = i->compressionLevel;
		y.fileSize = i->uncompressedSize;
		y.fileData = new char[i->uncompressedSize];
		memcpy(y.fileData, uncompressedFiles[x].data(), y.fileSize);
	}
#else
	for (auto x = 0; x < rawArc->fileCount; x++) {
		auto* i = (RawFile*)&data[pos];
		File& y = archive->files.emplace_back();
		y.filePath = convertToUtf8({ i->filePath });
		y.fileDate = i->fileDate;
		y.compressionLevel = i->compressionLevel;
		y.fileSize = i->uncompressedSize;
		y.fileData = new char[i->uncompressedSize];
		auto* compData = &data[i->dataPtr];
		if (i->compressedSize == i->uncompressedSize)
			memcpy(y.fileData, compData, i->uncompressedSize);
		else
			uncompress((Bytef*)y.fileData, (uLongf*)&i->uncompressedSize, (const Bytef*)compData, (uLong)i->compressedSize);
		pos += sizeof(RawFile);
	}
#endif

    return archive;
}

// Should rather compare the file extension.
inline bool shouldCompress(const castle::rda::v11::File& file) {
	return memcmp(file.fileData, "RIFF", 4) != 0 && memcmp(file.fileData, "OggS", 4) != 0;
}

size_t castle::rda::v11::write(Archive* archive, char*& data) {
    size_t pos = 0;
    data = new char[calcSize(archive)];

	auto* header = (RawArchive*)data;
	strcpy(header->signature, "Resource File V1.1");
	header->fileCount = archive->files.size();

	pos += sizeof(RawArchive);
	RawFile* rawFiles = (RawFile*)&data[pos];
	for (auto x = 0; x < archive->files.size(); x++) {
		auto& i = rawFiles[x];
		auto& file = archive->files[x];
		strcpy(i.filePath, convertFromUtf8(file.filePath).c_str());
		i.dataPtr = 0;
		i.compressedSize = 0;
		i.uncompressedSize = file.fileSize;
		i.compressionLevel = file.compressionLevel;
		i.fileDate = file.fileDate;
	}

#ifdef CASTLELIB_RDA_MULTITHREADED_COMPRESS
	std::vector<std::vector<char>> compressedFiles{};
	std::vector<std::thread> threads{};
	compressedFiles.resize(archive->files.size());
	for (auto x = 0; x < archive->files.size(); x++) {
		auto& file = archive->files[x];
		threads.emplace_back([](char* ogData, size_t ogSize, std::vector<char>* data, bool shouldCompress) {
			if (shouldCompress) {
				auto compressedSize = compressBound(ogSize);
				data->resize(compressedSize);
				compress((Bytef*)data->data(), &compressedSize, (const Bytef*)ogData, ogSize);
				data->resize(compressedSize);
			}
			else {
				data->resize(ogSize);
				memcpy(data->data(), ogData, ogSize);
			}
			return;
		}, file.fileData, file.fileSize, &compressedFiles[x], shouldCompress(file));
	}

	for (auto& thread : threads) {
		thread.join();
	}

	pos += sizeof(RawFile) * archive->files.size();
	for (auto x = 0; x < archive->files.size(); x++) {
		auto& i = rawFiles[x];
		i.dataPtr = pos;
		i.compressedSize = compressedFiles[x].size();
		memcpy(&data[pos], compressedFiles[x].data(), i.compressedSize);
		pos += i.compressedSize;
	}
#else
	pos += sizeof(RawFile) * archive->files.size();
	for (auto x = 0; x < archive->files.size(); x++) {
		auto& i = rawFiles[x];
		auto& file = archive->files[x];
		i.dataPtr = pos;
		if (shouldCompress(file)) {
			auto compressedSize = compressBound(file.fileSize);
			char* compressedData = &data[pos];
			compress((Bytef*)compressedData, &compressedSize, (const Bytef*)file.fileData, file.fileSize);
			i.compressedSize = compressedSize;
		}
		else {
			i.compressedSize = i.uncompressedSize;
		}
		pos += i.compressedSize;
	}
#endif

    return pos;
}

void castle::rda::v11::free(Archive* archive) {
	for (auto& file : archive->files)
		delete[] file.fileData;
    delete archive;
}

size_t castle::rda::v11::calcSize(Archive* archive) {
    size_t size = 0;
	
	size += sizeof(RawArchive);
	size += sizeof(RawFile) * archive->files.size();
	for (auto& file : archive->files)
		size += compressBound(file.fileSize);
    
	return size;
}
