#pragma once
#include "pch.h"
#include "Singleton.h"

// 이미지 파일을 OpenGL 텍스처로 등록하고 관리한다.
class TextureManager : public Singleton<TextureManager>
{
	friend class Singleton<TextureManager>;

public: // 텍스처를 초기화하고 로드한다. init()에서 기본 텍스처를 로드한다.
	bool init();
	bool loadTexture(const string& name, const string& path);
	bool bindTexture(const string& name) const;
	void release();

private: // 생성자와 소멸자는 Singleton에서만 호출한다.
	TextureManager() = default;
	~TextureManager();

	ULONG_PTR imageLoaderToken{};
	unordered_map<string, GLuint> textureMap{};
};
