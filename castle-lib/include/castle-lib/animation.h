#pragma once
#include <vector>
#include "internal.h"

namespace castle {
	namespace rdm {
		namespace v1002 {
			enum class KeyType : unsigned int {
				LINEAR = 1
			};

			template<typename T> 
			struct Key {
				int time;
				T value;
			};

			template<typename T, typename Y>
			struct DKey {
				int time;
				T value0;
				Y value1;
			};

			struct Object {
				std::string objectName;
				Vector3 pivot;
				Vector3 position;
				Quaternion rotation;
				Vector3 scale;
				std::vector<Key<Vector3>> positionKeys;
				std::vector<Key<Quaternion>> rotationKeys;
				std::vector<DKey<Vector4, Vector3>> scaleKeys;
				KeyType posType;
				KeyType rotType;
				KeyType scaleType;
			};

			struct Bone {
				std::string boneName;
				Vector3 position;
				std::vector<int> childIndices;
				std::vector<Key<Vector3>> positionKeys;
				std::vector<Key<Quaternion>> rotationKeys;
			};

			struct Animation {
				std::string fileName;
				std::string filePath;
				std::string MAXFilePath;
				unsigned int animationRange;
				unsigned int TPS;
				unsigned int FPS;
				
				std::vector<Bone> bones;
				std::vector<Object> objects;
			};
		
			Animation* read(char* data, size_t size);
			size_t write(Animation* mdl, char*& data);
			void free(Animation* mdl);
			size_t calcSize(Animation* mdl);
		}
	}
}
