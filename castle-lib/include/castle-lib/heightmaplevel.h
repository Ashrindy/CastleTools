#pragma once
#include <vector>

namespace castle {
	namespace hlm {
		struct HLMEntry {
			float x, y, z;
			float unk0, unk1;
			float unk2, unk3, unk4;
			int unk5;
		};

		struct HeightMapLevel {
			std::vector<HLMEntry> entries;
		};

		HeightMapLevel* read(char* data, size_t size);
		size_t write(HeightMapLevel* tlm, char*& data);
		void free(HeightMapLevel* tlm);
		size_t calcSize(HeightMapLevel* tlm);
	}
}
