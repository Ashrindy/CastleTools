#pragma once
#include <ctime>
#include <string>
#include <vector>

namespace castle {
	namespace rda {
		namespace v11 {
			struct File {
				std::string filePath;
				size_t fileSize;
				char* fileData;
				time_t fileDate;
				int compressionLevel;
			};

			struct Archive {
				std::vector<File> files;
			};
		
			Archive* read(char* data, size_t size);
			size_t write(Archive* archive, char*& data);
			void free(Archive* archive);
			size_t calcSize(Archive* archive);
		}
	}
}
