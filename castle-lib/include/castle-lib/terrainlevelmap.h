#pragma once
#include <vector>

namespace castle {
	namespace tlm {
		struct TerrainLevelMap {
			std::vector<int> entries;
		};

		TerrainLevelMap* read(char* data, size_t size);
		size_t write(TerrainLevelMap* tlm, char*& buffer);
		void free(TerrainLevelMap* tlm);
		size_t calcSize(TerrainLevelMap* tlm);
	}
}
