#include <castle-lib/animation.h>
#include <cstring>

using namespace castle::rdm::v1002;

struct RawLabel {
	unsigned int version;
	char labelName[256];
	int unk0[2];
};

struct RawObject {
	char objectName[256];
	Vector3 pivot;
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	unsigned int posKeyAmount;
	unsigned int rotKeyAmount;
	unsigned int scaleKeyAmount;
	KeyType posType;
	KeyType rotType;
	KeyType scaleType;
	int unk0[9];
};

struct RawBone {
	char boneName[256];
	Vector3 position;
	unsigned int childAmount;
	int unk0;
	unsigned int posKeyAmount;
	unsigned int rotKeyAmount;
	int unk1[2];
	unsigned int linkAmount;
	int unk2;
};

struct RawAnimation {
	unsigned int version;
	char fileName[256];
	char filePath[256];
	unsigned int animationRange;
	unsigned int TPS;
	unsigned int FPS;
	int unk0[4];
	unsigned int boneAmount;
	unsigned int objectAmount;
	unsigned int morphAmount;
	int unk1[2];
	char MAXFilePath[256];
};

Animation* castle::rdm::v1002::read(char* data, size_t size) {
    size_t pos = 0;
    Animation* anim = new Animation{};

	RawAnimation* rawAnim = (RawAnimation*)data;

	if (rawAnim->version != 1002)
		ErrorBox({ "Unknown RDM version! " + rawAnim->version });

	anim->fileName = rawAnim->fileName;
	anim->filePath = rawAnim->filePath;
	anim->MAXFilePath = rawAnim->MAXFilePath;
	anim->animationRange = rawAnim->animationRange;
	anim->TPS = rawAnim->TPS;
	anim->FPS = rawAnim->FPS;

	pos += sizeof(RawAnimation);

	std::vector<RawBone*> tempRawBones{};
	for (auto i = 0; i < rawAnim->boneAmount; i++) {
		RawBone* rawBone = (RawBone*)&data[pos];
		Bone& bone = anim->bones.emplace_back();
		bone.boneName = rawBone->boneName;
		bone.position = rawBone->position;
		tempRawBones.push_back(rawBone);
		pos += sizeof(RawBone);
	}
	for (auto i = 0; i < rawAnim->boneAmount; i++) {
		RawBone* rawBone = tempRawBones[i];
		Bone& bone = anim->bones[i];

		bone.childIndices.resize(rawBone->childAmount);
		memcpy(bone.childIndices.data(), &data[pos], rawBone->childAmount * sizeof(int));
		pos += rawBone->childAmount * sizeof(int);

		bone.positionKeys.resize(rawBone->posKeyAmount);
		memcpy(bone.positionKeys.data(), &data[pos], rawBone->posKeyAmount * sizeof(Key<Vector3>));
		pos += rawBone->posKeyAmount * sizeof(Key<Vector3>);

		bone.rotationKeys.resize(rawBone->rotKeyAmount);
		memcpy(bone.rotationKeys.data(), &data[pos], rawBone->rotKeyAmount * sizeof(Key<Quaternion>));
		pos += rawBone->rotKeyAmount * sizeof(Key<Quaternion>);
	}

	//std::vector<RawObject*> tempRawObjects{};
	for (auto i = 0; i < rawAnim->objectAmount; i++) {
		RawObject* rawObject = (RawObject*)&data[pos];
		Object& object = anim->objects.emplace_back();
		object.objectName = rawObject->objectName;
		object.pivot = rawObject->pivot;
		object.position = rawObject->position;
		object.rotation = rawObject->rotation;
		object.scale = rawObject->scale;
		object.posType = rawObject->posType;
		object.rotType = rawObject->rotType;
		object.scaleType = rawObject->scaleType;
		//tempRawObjects.push_back(rawObject);
		pos += sizeof(RawObject);
	}

    return anim;
}

size_t castle::rdm::v1002::write(Animation* anim, char*& data) {
    // TODO: Implement
    return 0;
}

void castle::rdm::v1002::free(Animation* anim) {
    delete anim;
}

size_t castle::rdm::v1002::calcSize(Animation* anim) {
    // TODO: Implement
    return 0;
}
