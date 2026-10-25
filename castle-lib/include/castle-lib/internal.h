#pragma once
#include <iostream>
#include <iconv.h>
#ifdef _WIN32
#include <windows.h>
#endif

struct Vector2 {
public:
	float x, y;
	inline Vector2(float x = 0, float y = 0) : x(x), y(y) {}

	inline Vector2 operator+(const Vector2& other) const {
		return Vector2(x + other.x, y + other.y);
	}

	inline Vector2 operator-(const Vector2& other) const {
		return Vector2(x - other.x, y - other.y);
	}

	inline Vector2 operator*(float scalar) const {
		return Vector2(x * scalar, y * scalar);
	}

	inline Vector2 operator/(float scalar) const {
		return Vector2(x / scalar, y / scalar);
	}

	inline bool operator==(const Vector2& other) const {
		return x == other.x && y == other.y;
	}

	inline bool operator!=(const Vector2& other) const {
		return !(*this == other);
	}
};

struct Vector3 {
public:
	float x, y, z;
	inline Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

	inline Vector3 operator+(const Vector3& other) const {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	inline Vector3 operator-(const Vector3& other) const {
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	inline Vector3 operator*(float scalar) const {
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	inline Vector3 operator/(float scalar) const {
		return Vector3(x / scalar, y / scalar, z / scalar);
	}

	inline bool operator==(const Vector3& other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	inline bool operator!=(const Vector3& other) const {
		return !(*this == other);
	}
};

struct Vector4 {
public:
	float x, y, z, w;
	inline Vector4(float x = 0, float y = 0, float z = 0, float w = 0) : x(x), y(y), z(z), w(w) {}

	inline Vector4 operator+(const Vector4& other) const {
		return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	inline Vector4 operator-(const Vector4& other) const {
		return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	inline Vector4 operator*(float scalar) const {
		return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	inline Vector4 operator/(float scalar) const {
		return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	inline bool operator==(const Vector4& other) const {
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	inline bool operator!=(const Vector4& other) const {
		return !(*this == other);
	}
};

struct Quaternion {
public:
	float w, x, y, z;
	Quaternion(float w = 0, float x = 0, float y = 0, float z = 0) : w(w), x(x), y(y), z(z) {}

	inline Quaternion operator+(const Quaternion& other) const {
		return Quaternion(w + other.w, x + other.x, y + other.y, z + other.z);
	}

	inline Quaternion operator-(const Quaternion& other) const {
		return Quaternion(w - other.w, x - other.x, y - other.y, z - other.z);
	}

	inline Quaternion operator*(float scalar) const {
		return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);
	}

	inline Quaternion operator/(float scalar) const {
		return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
	}

	inline bool operator==(const Quaternion& other) const {
		return w == other.w && x == other.x && y == other.y && z == other.z;
	}

	inline bool operator!=(const Quaternion& other) const {
		return !(*this == other);
	}
};

struct RGB32 {
public:
	float r, g, b;
	inline RGB32(float r = 0, float g = 0, float b = 0) : r(r), g(g), b(b) {}
};

template<bool crash=true>
static void ErrorBox(std::string msg) {
	printf("%s\n", msg.c_str());
#ifdef _WIN32
	MessageBoxA(NULL, msg.c_str(), "Error", MB_ICONERROR);
#endif
	if (crash)
		exit(1);
}

static std::string convertToUtf8(const std::string& input) {
	iconv_t cd = iconv_open("UTF-8", "ISO-8859-15");

	std::string output(input.size() * 4 + 16, '\0');
	char* in = (char*)input.data();
	size_t inLeft = input.size();
	char* out = output.data();
	size_t outLeft = output.size();

	size_t result = iconv(cd, &in, &inLeft, &out, &outLeft);
	iconv_close(cd);

	output.resize(output.size() - outLeft);
	return output;
}

static std::string convertFromUtf8(const std::string& input) {
	iconv_t cd = iconv_open("ISO-8859-15", "UTF-8");

	std::string output(input.size() * 2 + 16, '\0');
	char* in = (char*)input.data();
	size_t inLeft = input.size();
	char* out = output.data();
	size_t outLeft = output.size();

	size_t result = iconv(cd, &in, &inLeft, &out, &outLeft);
	iconv_close(cd);

	output.resize(output.size() - outLeft);
	return output;
}
