#version 330 core

in vec2 ScreenPosition; // 화면 좌표 (NDC)
out vec4 FragColor; // 최종 색상 출력

uniform mat4 inverseView; // 카메라 뷰 행렬의 역행렬
uniform float tanHalfFovy; // 카메라의 수직 FOV의 절반에 대한 탄젠트 값
uniform float aspect; // 화면 종횡비
uniform bool orthographic; // 직교 투영 여부

void main()
{
    // 화면 위치에 해당하는 카메라 기준 방향
    vec3 viewDirection;

    if (orthographic) // 직교 투영일 경우, 카메라 기준 방향은 항상 -Z 방향
    {
        viewDirection = vec3(0.0, 0.0, -1.0);
    }
    else // 원근 투영일 경우, 화면 좌표를 이용하여 카메라 기준 방향 계산
    {
        viewDirection = vec3(
            ScreenPosition.x * aspect * tanHalfFovy,
            ScreenPosition.y * tanHalfFovy,
            -1.0
        ); // 카메라 기준 방향은 -Z 방향을 향함
    }

    // 카메라 기준 방향을 월드 기준 방향으로 변환
    vec3 worldDirection = normalize(
        mat3(inverseView) * viewDirection
    ); // 역행렬을 사용하여 카메라 기준 방향을 월드 기준 방향으로 변환

    vec3 horizonColor = vec3(0.78, 0.88, 0.96); // 지평선 색상
    vec3 zenithColor = vec3(0.20, 0.48, 0.82); // 하늘 꼭대기 색상

    // 위를 볼수록 1에 가까워진다.
    float height = clamp(worldDirection.y, 0.0, 1.0); // 높이 값은 0에서 1 사이로 제한
    float blend = pow(height, 0.55); // 높이에 따라 색상을 블렌딩 (pow 함수를 사용하여 부드럽게)

    vec3 skyColor = mix(horizonColor, zenithColor, blend); // 지평선 색상과 하늘 꼭대기 색상을 높이에 따라 혼합

    FragColor = vec4(skyColor, 1.0); // 최종 색상 출력 (알파 값은 1.0으로 설정)
}