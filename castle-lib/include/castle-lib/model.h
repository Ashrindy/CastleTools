#pragma once
#include <vector>
#include "internal.h"

namespace castle {
	namespace rdo {
		namespace v2011 {
			struct Material {
				std::string materialName;
				RGB32 diffuse;
				RGB32 specular;
				RGB32 ambient;
				float transparency;
				Vector2 uvTilling;
				Vector2 uvOffset;
				std::string textureName;
			};

			struct Weight {
				int id;
				float weight;
			};

			struct Bone {
				std::string boneName;
				Vector3 position;
				std::vector<int> childIndices;
				std::vector<Weight> weights;
				std::vector<int> objectIndices;
			};

			struct Face {
				unsigned int a, b, c;
				int matid;
			};

			struct Mesh {
				unsigned int version;

				int materialID;
				std::string meshName;

				std::vector<Face> faces;
				std::vector<Face> tFaces;
				std::vector<Vector3> positions;
				std::vector<Vector2> tPositions;
				std::vector<Vector3> faceNormals;
				std::vector<Vector3> positionNormals;

				Vector3 boundingBoxCenter;
				Vector3 boundingBoxCorners[8];

				Vector3 pivot;
				Vector3 position;
				Quaternion rotation;
				Vector3 scale;
			};

			struct Model {
				unsigned int version;

				std::string fileName;
				std::string filePath;
				std::string MAXFilePath;

				std::vector<Mesh> meshes;
				std::vector<Bone> bones;
				std::vector<Material> materials;
			};
		
			Model* read(char* data, size_t size);
			size_t write(Model* mdl, char*& data);
			void free(Model* mdl);
			size_t calcSize(Model* mdl);
		}
	}
}
