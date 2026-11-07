#pragma once

// C/C++ 표준 라이브러리 및 Windows 헤더
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <random>
#include <windows.h>
#include <cmath>
#include <array>

// OpenGL / GLEW / GLFW 그래픽스 라이브러리
#include <GL/glew.h>
#include <GL/glfw3.h>

// GLM 수학 라이브러리
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

using namespace std;

// 프로젝트 전역 타입 별칭 (GLM 타입 단축어)
using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;

using Matrix4 = glm::mat4;

using Color = glm::vec3;
