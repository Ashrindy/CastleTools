#include <castle-lib/terrainlevelmap.h>
#include <cstring>

using namespace castle::tlm;

TerrainLevelMap* castle::tlm::read(char* data, size_t size) {
    TerrainLevelMap* tlm = new TerrainLevelMap{};
    
	size_t count = size / sizeof(int);
	tlm->entries.resize(count);
	memcpy(tlm->entries.data(), data, size);

    return tlm;
}

size_t castle::tlm::write(TerrainLevelMap* tlm, char*& data) {
    size_t size = calcSize(tlm);
	data = new char[size];

	memcpy(data, (char*)tlm->entries.data(), size);

	return size;
}

void castle::tlm::free(TerrainLevelMap* tlm) {
    delete tlm;
}

size_t castle::tlm::calcSize(TerrainLevelMap* tlm) {
    return tlm->entries.size() * sizeof(int);
}
