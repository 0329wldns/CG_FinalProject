#include "pch.h"
#include "Managers/TextureManager.h"
// pch.h의 using namespace std와 Windows byte 이름의 충돌을 방지한다.
namespace Gdiplus { using ::byte; }// Gdiplus.h에서 byte를 정의하지만 Windows.h에서도 byte를 정의한다.
#include <gdiplus.h>
#include <cstring>

#pragma comment(lib, "gdiplus.lib")

bool TextureManager::init() // 텍스처를 초기화하고 로드한다. init()에서 기본 텍스처를 로드한다.
{
	if (!imageLoaderToken) // Gdiplus 초기화
	{
		Gdiplus::GdiplusStartupInput input;
		if (Gdiplus::GdiplusStartup(&imageLoaderToken, &input, nullptr) != Gdiplus::Ok) // Gdiplus 초기화 실패
			return false;
	}

	return loadTexture("fence_ad", "res/fence_ad_diamond.png")
		&& loadTexture("power", "res/fence_ad_power_drive.png")
		&& loadTexture("burger", "res/fence_ad_home_run_burger.png")
		&& loadTexture("ace", "res/fence_ad_ace_athletics.png")
		&& loadTexture("coffee", "res/fence_ad_grand_slam_coffee.png");
}

bool TextureManager::loadTexture(const string& name, const string& path)
{
	if (textureMap.find(name) != textureMap.end()) // 이미 로드된 텍스처 이름이면 true를 반환한다.
		return true;
	if (!imageLoaderToken) // Gdiplus 초기화가 안 되어 있으면 false를 반환한다.
		return false;

	const int pathLength = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, 
		path.c_str(), -1, nullptr, 0); // UTF-8 문자열을 UTF-16 문자열로 변환하기 위해 필요한 버퍼 크기를 계산한다. - 안전하게 동적할당하고 버퍼 오버플로우를 방지한다.
	if (!pathLength) // 변환 실패
		return false;
	std::wstring widePath(pathLength, L'\0');
	MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
		path.c_str(), -1, widePath.data(), pathLength);
	Gdiplus::Bitmap image(widePath.c_str());
	if (image.GetLastStatus() != Gdiplus::Ok) 
	{
		cerr << "Texture load failed: " << path << '\n';
		return false;
	}

	const UINT width = image.GetWidth();
	const UINT height = image.GetHeight();
	Gdiplus::Rect bounds(0, 0, width, height); // 이미지의 전체 영역을 나타내는 사각형을 생성한다.
	Gdiplus::BitmapData data{}; // 이미지 데이터를 읽기 위해 비트맵을 잠근다.
	if (image.LockBits(&bounds, Gdiplus::ImageLockModeRead,
		PixelFormat32bppARGB, &data) != Gdiplus::Ok)
		return false;

	// PNG의 위쪽 행을 OpenGL 텍스처의 위쪽(v = 1)에 맞춘다.
	const size_t rowBytes = static_cast<size_t>(width) * 4;
	vector<unsigned char> pixels(rowBytes * height);
	for (UINT row = 0; row < height; ++row)
	{
		const auto* source = static_cast<const unsigned char*>(data.Scan0)
			+ static_cast<ptrdiff_t>(row) * data.Stride;
		std::memcpy(pixels.data() + (height - 1 - row) * rowBytes, source, rowBytes); // PNG의 위쪽 행을 OpenGL 텍스처의 위쪽(v = 1)에 맞춘다.
	}
	image.UnlockBits(&data);

	GLint previousTexture{};
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTexture); // 현재 바인딩된 텍스처를 저장한다.
	GLuint texture{};
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, pixels.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, previousTexture);
	textureMap.emplace(name, texture);
	return true;
}

bool TextureManager::bindTexture(const string& name) const
{
	const auto iter = textureMap.find(name); // 찾은 텍스처 이름이 있으면 해당 텍스처를 바인딩하고 true를 반환한다. 없으면 0번 텍스처를 바인딩하고 false를 반환한다.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iter == textureMap.end() ? 0 : iter->second);
	return iter != textureMap.end();
}

void TextureManager::release()
{
	for (const auto& entry : textureMap)
		glDeleteTextures(1, &entry.second);
	textureMap.clear();
}

TextureManager::~TextureManager()
{
	// Core 초기화 도중 생성되어 Core보다 먼저 소멸한다.
	release();
	if (imageLoaderToken)
		Gdiplus::GdiplusShutdown(imageLoaderToken);
}
