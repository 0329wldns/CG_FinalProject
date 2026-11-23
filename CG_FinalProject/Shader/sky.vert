#version 330 core

layout (location = 0) in vec2 aPos; // 기존 사각형의 메쉬 좌표

out vec2 ScreenPosition; // Output variable to pass the screen position to the fragment shader

void main()
{
	ScreenPosition = aPos * 2.0; // 좌표를 -1 ~ 1로 확대해 화면 전체를 덮음 

	gl_Position = vec4(ScreenPosition, 1.0, 1.0); // 스크린 좌표를 gl_Position에 전달하여 화면 전체를 덮는 사각형을 렌더링
}