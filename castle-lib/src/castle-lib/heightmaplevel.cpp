#include <castle-lib/heightmaplevel.h>
#include <cstring>

using namespace castle::hlm;

HeightMapLevel* castle::hlm::read(char* data, size_t size) {
    HeightMapLevel* hlm = new HeightMapLevel{};
    
	size_t count = size / sizeof(HLMEntry);
	hlm->entries.resize(count);
	memcpy(hlm->entries.data(), data, size);

    return hlm;
}

size_t castle::hlm::write(HeightMapLevel* hlm, char*& data) {
    size_t size = calcSize(hlm);
	data = new char[size];

	memcpy(data, (char*)hlm->entries.data(), size);

	return size;
}

void castle::hlm::free(HeightMapLevel* hlm) {
    delete hlm;
}

size_t castle::hlm::calcSize(HeightMapLevel* hlm) {
    return hlm->entries.size() * sizeof(HLMEntry);
}
