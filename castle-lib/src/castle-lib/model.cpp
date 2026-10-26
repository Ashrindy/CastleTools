#include <castle-lib/model.h>
#include <cstring>

using namespace castle::rdo::v2011;

struct RawMaterial {
	char materialName[256];
	RGB32 ambient;
	RGB32 diffuse;
	RGB32 specular;
	float transparency;
	Vector2 uvTilling;
	Vector2 uvOffset;
	char textureName[256];
	int unknown;
};

struct RawBone {
	char boneName[256];
	Vector3 position;
	unsigned int childIndexCount;
	unsigned int objectCount;
	int unknown;
};

struct RawMesh {
	unsigned int version;
	char meshName[256];
	Vector3 pivot;
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	unsigned int posCount;
	unsigned int faceCount;
	unsigned int tPosCount;
	unsigned int tFaceCount;
	Vector3 boundingBoxCenter;
	Vector3 boundingBoxCorners[8];
private:
	char empty[16];
public:
	int materialID;
private:
	char empty1[4];
};

struct RawModel {
	unsigned int version;
	char fileName[256];
	char filePath[256];
	unsigned int objectCount;
	unsigned int boneCount;
	unsigned int materialCount;
	unsigned int unknownCount;
private:
	char empty[124];
public:
	char maxFilePath[256];
};

Model* castle::rdo::v2011::read(char* data, size_t size) {
    size_t pos = 0;
    Model* mdl = new Model{};

	RawModel* rawRdo = (RawModel*)data;
	mdl->version = rawRdo->version;
	if (mdl->version != 2011)
		ErrorBox({ "Unknown RDO version! " + mdl->version });
	mdl->fileName = rawRdo->fileName;
	mdl->filePath = rawRdo->filePath;
	mdl->MAXFilePath = rawRdo->maxFilePath;
	pos += sizeof(RawModel);

	std::vector<RawMesh*> tempRawMesh{};
	for (auto i = 0; i < rawRdo->objectCount; i++) {
		RawMesh* rawMesh = (RawMesh*)&data[pos];
		Mesh& mesh = mdl->meshes.emplace_back();
		mesh.version = rawMesh->version;
		if (mesh.version != 3001 && mesh.version != 3005)
			ErrorBox({ "Unknown RDO Mesh version! " + mesh.version });
		mesh.materialID = rawMesh->materialID;
		mesh.meshName = rawMesh->meshName;
		mesh.position = rawMesh->position;
		mesh.pivot = rawMesh->pivot;
		mesh.rotation = rawMesh->rotation;
		mesh.scale = rawMesh->scale;
		mesh.boundingBoxCenter = rawMesh->boundingBoxCenter;
		memcpy(mesh.boundingBoxCorners, rawMesh->boundingBoxCorners, sizeof(rawMesh->boundingBoxCorners));
		mesh.materialID = rawMesh->materialID;
		tempRawMesh.push_back(rawMesh);
		pos += sizeof(RawMesh);
	}
	for (auto i = 0; i < rawRdo->objectCount; i++) {
		auto* rawMesh = tempRawMesh[i];
		auto& mesh = mdl->meshes[i];

		mesh.positions.resize(rawMesh->posCount);
		memcpy(mesh.positions.data(), (Vector3*)&data[pos], rawMesh->posCount * sizeof(Vector3));
		pos += rawMesh->posCount * sizeof(Vector3);

		mesh.faces.resize(rawMesh->faceCount);
		memcpy(mesh.faces.data(), (Face*)&data[pos], rawMesh->faceCount * sizeof(Face));
		pos += rawMesh->faceCount * sizeof(Face);

		mesh.tPositions.resize(rawMesh->tPosCount);
		memcpy(mesh.tPositions.data(), (Vector2*)&data[pos], rawMesh->tPosCount * sizeof(Vector2));
		pos += rawMesh->tPosCount * sizeof(Vector2);

		mesh.tFaces.resize(rawMesh->tFaceCount);
		memcpy(mesh.tFaces.data(), (Face*)&data[pos], rawMesh->tFaceCount * sizeof(Face));
		pos += rawMesh->tFaceCount * sizeof(Face);

		mesh.faceNormals.resize(rawMesh->faceCount);
		memcpy(mesh.faceNormals.data(), (Vector3*)&data[pos], rawMesh->faceCount * sizeof(Vector3));
		pos += rawMesh->faceCount * sizeof(Vector3);

		mesh.positionNormals.resize(rawMesh->posCount);
		memcpy(mesh.positionNormals.data(), (Vector3*)&data[pos], rawMesh->posCount * sizeof(Vector3));
		pos += rawMesh->posCount * sizeof(Vector3);
	}
	tempRawMesh.clear();

	std::vector<RawBone*> tempRawBone{};
	for (auto i = 0; i < rawRdo->boneCount; i++) {
		RawBone* rawBone = (RawBone*)&data[pos];
		Bone& bone = mdl->bones.emplace_back();
		bone.boneName = rawBone->boneName;
		bone.position = rawBone->position;
		tempRawBone.push_back(rawBone);
		pos += sizeof(RawBone);
	}
	for (auto i = 0; i < rawRdo->boneCount; i++) {
		auto* rawBone = tempRawBone[i];
		auto& bone = mdl->bones[i];

		bone.childIndices.resize(rawBone->childIndexCount);
		memcpy(bone.childIndices.data(), (int*)&data[pos], rawBone->childIndexCount * sizeof(int));
		pos += rawBone->childIndexCount * sizeof(int);

		if (rawBone->objectCount > 0) {
			pos += 4;
			bone.objectIndices.resize(rawBone->objectCount);
			memcpy(bone.objectIndices.data(), &data[pos], sizeof(int) * rawBone->objectCount);
			pos += sizeof(int) * rawBone->objectCount;
			int weightCount = *(int*)&data[pos];
			pos += 12 + sizeof(int);
			int* ids = (int*)&data[pos];
			for (auto x = 0; x < weightCount; x++) {
				Weight& weight = bone.weights.emplace_back();
				weight.id = ids[x];
			}
			pos += weightCount * sizeof(int);
			float* weights = (float*)&data[pos];
			for (auto x = 0; x < weightCount; x++)
				bone.weights[x].weight = weights[x];
			pos += weightCount * sizeof(float);
		}
	}
	tempRawBone.clear();

	for (int i = 0; i < rawRdo->materialCount; i++) {
		RawMaterial* rawMat = (RawMaterial*)&data[pos];
		Material& mat = mdl->materials.emplace_back();
		mat.materialName = rawMat->materialName;
		mat.diffuse = rawMat->diffuse;
		mat.ambient = rawMat->ambient;
		mat.specular = rawMat->specular;
		mat.transparency = rawMat->transparency;
		mat.uvOffset = rawMat->uvOffset;
		mat.uvTilling = rawMat->uvTilling;
		mat.textureName = rawMat->textureName;
		pos += sizeof(RawMaterial);
	}

    return mdl;
}

size_t castle::rdo::v2011::write(Model* mdl, char*& data) {
    size_t size = calcSize(mdl);
	data = new char[size];

	size_t pos = 0;
	RawModel* rawRdo = (RawModel*)data;
	rawRdo->version = mdl->version;
	strcpy(rawRdo->fileName, mdl->fileName.c_str());
	strcpy(rawRdo->filePath, mdl->filePath.c_str());
	rawRdo->objectCount = mdl->meshes.size();
	rawRdo->boneCount = mdl->bones.size();
	rawRdo->materialCount = mdl->materials.size();
	rawRdo->unknownCount = 0;
	strcpy(rawRdo->maxFilePath, mdl->MAXFilePath.c_str());
	pos += sizeof(RawModel);

	std::vector<RawMesh*> tempRawMesh{};
	for (auto& mesh : mdl->meshes) {
		RawMesh* rawMesh = (RawMesh*)&data[pos];
		rawMesh->version = mesh.version;
		strcpy(rawMesh->meshName, mesh.meshName.c_str());
		rawMesh->pivot = mesh.pivot;
		rawMesh->position = mesh.position;
		rawMesh->rotation = mesh.rotation;
		rawMesh->scale = mesh.scale;
		rawMesh->posCount = mesh.positions.size();
		rawMesh->faceCount = mesh.faces.size();
		rawMesh->tPosCount = mesh.tPositions.size();
		rawMesh->tFaceCount = mesh.tFaces.size();
		rawMesh->boundingBoxCenter = mesh.boundingBoxCenter;
		memcpy(rawMesh->boundingBoxCorners, mesh.boundingBoxCorners, sizeof(rawMesh->boundingBoxCorners));
		rawMesh->materialID = mesh.materialID;
		tempRawMesh.push_back(rawMesh);
		pos += sizeof(RawMesh);
	}
	for (auto i = 0; i < tempRawMesh.size(); i++) {
		auto* rawMesh = tempRawMesh[i];
		auto& mesh = mdl->meshes[i];

		memcpy((Vector3*)&data[pos], mesh.positions.data(), rawMesh->posCount * sizeof(Vector3));
		pos += rawMesh->posCount * sizeof(Vector3);

		memcpy((Face*)&data[pos], mesh.faces.data(), rawMesh->faceCount * sizeof(Face));
		pos += rawMesh->faceCount * sizeof(Face);

		memcpy((Vector2*)&data[pos], mesh.tPositions.data(), rawMesh->tPosCount * sizeof(Vector2));
		pos += rawMesh->tPosCount * sizeof(Vector2);

		memcpy((Face*)&data[pos], mesh.tFaces.data(), rawMesh->tFaceCount * sizeof(Face));
		pos += rawMesh->tFaceCount * sizeof(Face);

		memcpy((Vector3*)&data[pos], mesh.faceNormals.data(), rawMesh->faceCount * sizeof(Vector3));
		pos += rawMesh->faceCount * sizeof(Vector3);

		memcpy((Vector3*)&data[pos], mesh.positionNormals.data(), rawMesh->posCount * sizeof(Vector3));
		pos += rawMesh->posCount * sizeof(Vector3);
	}
	tempRawMesh.clear();

	std::vector<RawBone*> tempRawBone{};
	for (auto& bone : mdl->bones) {
		RawBone* rawBone = (RawBone*)&data[pos];
		strcpy(rawBone->boneName, bone.boneName.c_str());
		rawBone->position = bone.position;
		rawBone->childIndexCount = bone.childIndices.size();
		rawBone->objectCount = bone.objectIndices.size();
		rawBone->unknown = 0;
		tempRawBone.push_back(rawBone);
		pos += sizeof(RawBone);
	}
	for (auto i = 0; i < tempRawBone.size(); i++) {
		auto* rawBone = tempRawBone[i];
		auto& bone = mdl->bones[i];

		memcpy((int*)&data[pos], bone.childIndices.data(), rawBone->childIndexCount * sizeof(int));
		pos += rawBone->childIndexCount * sizeof(int);

		if (rawBone->objectCount > 0) {
			pos += 4;
			memcpy(&data[pos], bone.objectIndices.data(), sizeof(int) * rawBone->objectCount);
			pos += sizeof(int) * rawBone->objectCount;
			int weightCount = bone.weights.size();
			*(int*)&data[pos] = weightCount;
			pos += 12 + sizeof(int);
			int* ids = (int*)&data[pos];
			for (auto x = 0; x < bone.weights.size(); x++) {
				ids[x] = bone.weights[x].id;
			}
			pos += weightCount * sizeof(int);
			float* weights = (float*)&data[pos];
			for (auto x = 0; x < weightCount; x++)
				weights[x] = bone.weights[x].weight;
			pos += weightCount * sizeof(float);
		}
	}
	tempRawBone.clear();

	for (auto& mat : mdl->materials) {
		RawMaterial* rawMat = (RawMaterial*)&data[pos];
		strcpy(rawMat->materialName, mat.materialName.c_str());
		rawMat->diffuse = mat.diffuse;
		rawMat->ambient = mat.ambient;
		rawMat->specular = mat.specular;
		rawMat->transparency = mat.transparency;
		rawMat->uvOffset = mat.uvOffset;
		rawMat->uvTilling = mat.uvTilling;
		strcpy(rawMat->textureName, mat.textureName.c_str());
		pos += sizeof(RawMaterial);
	}

    return size;
}

void castle::rdo::v2011::free(Model* mdl) {
    delete mdl;
}

size_t castle::rdo::v2011::calcSize(Model* mdl) {
    size_t size = sizeof(RawModel);

	size += sizeof(RawMesh) * mdl->meshes.size();
	for (auto& mesh : mdl->meshes) {
		size += sizeof(Vector3) * mesh.positions.size();
		size += sizeof(Face) * mesh.faces.size();
		size += sizeof(Vector2) * mesh.tPositions.size();
		size += sizeof(Face) * mesh.tFaces.size();
		size += sizeof(Vector3) * mesh.faceNormals.size();
		size += sizeof(Vector3) * mesh.positionNormals.size();
	}
	
	size += sizeof(RawBone) * mdl->bones.size();
	for (auto& bone : mdl->bones) {
		size += sizeof(int) * bone.childIndices.size();
		if (bone.weights.size() > 0) {
			size += 4;
			size += sizeof(int) * bone.objectIndices.size();
			size += 12 + sizeof(int);
			size += sizeof(int) * bone.weights.size();
			size += sizeof(float) * bone.weights.size();
		}
	}

	size += sizeof(RawMaterial) * mdl->materials.size();

    return size;
}
