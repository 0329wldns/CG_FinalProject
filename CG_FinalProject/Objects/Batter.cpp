#include "pch.h" // 전처리기 헤더 파일을 포함합니다. 이 파일은 프로젝트의 공통 헤더를 포함하고 있어 컴파일 시간을 단축시킵니다.
#include "Objects/Batter.h" // Batter 클래스의 정의를 포함합니다.
#include "Managers/MeshManager.h" // MeshManager 클래스의 정의를 포함합니다.
#include "Managers/ShaderManager.h" // ShaderManager 클래스의 정의를 포함합니다.

void Batter::update()
{
	// 오브젝트의 상태를 업데이트하는 로직을 여기에 작성합니다.
	// 예: 위치, 속도, 애니메이션 상태 등을 업데이트할 수 있습니다.
}

void Batter::render() const
{
    auto& shader = ShaderManager::getInstance();
    auto& meshes = MeshManager::getInstance();

    shader.setFloat("default", "transparency", 1.0f);
    shader.setBool("default", "useTexture", false);

    const Vector3 jersey{ 0.08f, 0.15f, 0.35f };
    const Vector3 pants{ 0.88f, 0.88f, 0.84f };
    const Vector3 skin{ 0.84f, 0.62f, 0.43f };

    // 관절 기준에서 큐브 중심을 옮기고, 해당 부위 크기로 그린다.
    auto drawPart = [&](const Matrix4& joint, const Vector3& offset,
        const Vector3& size, const Vector3& color)
        {
            const Matrix4 model = glm::scale(glm::translate(joint, offset), size);
            shader.setMat4("default", "model", model);
            shader.setVec3("default", "color", color);
            meshes.drawMesh("cube");
        };

    // root는 발밑, pelvis는 골반 중심, torso는 몸통 중심이다.
    const Matrix4 root = glm::translate(Matrix4{ 1.0f }, getPos());
    const Matrix4 pelvis = glm::translate(root, Vector3{ 0.0f, 0.91f, 0.0f });
    const Matrix4 torso = glm::translate(pelvis, Vector3{ 0.0f, 0.34f, 0.0f });

    drawPart(pelvis, Vector3{ 0.0f }, Vector3{ 0.34f, 0.16f, 0.23f }, pants);
    drawPart(torso, Vector3{ 0.0f }, Vector3{ 0.42f, 0.55f, 0.24f }, jersey);
    drawPart(torso, Vector3{ 0.0f, 0.30f, 0.0f },
        Vector3{ 0.11f, 0.08f, 0.11f }, skin); // 목
    drawPart(torso, Vector3{ 0.0f, 0.44f, 0.0f },
        Vector3{ 0.20f, 0.22f, 0.20f }, skin); // 머리

    // X 좌표 부호를 바꿔 양쪽 팔과 다리를 같은 방식으로 만든다.
    for (float side : { -1.0f, 1.0f })
    {
        const Matrix4 shoulder = glm::translate(
            torso, Vector3{ side * 0.265f, 0.22f, 0.0f });
        const Matrix4 elbow = glm::translate(
            shoulder, Vector3{ 0.0f, -0.27f, 0.0f });
        const Matrix4 wrist = glm::translate(
            elbow, Vector3{ 0.0f, -0.24f, 0.0f });

        drawPart(shoulder, Vector3{ 0.0f, -0.135f, 0.0f },
            Vector3{ 0.12f, 0.27f, 0.13f }, jersey);
        drawPart(elbow, Vector3{ 0.0f, -0.12f, 0.0f },
            Vector3{ 0.10f, 0.24f, 0.10f }, skin);
        drawPart(wrist, Vector3{ 0.0f, -0.05f, 0.0f },
            Vector3{ 0.10f, 0.10f, 0.09f }, skin);

        const Matrix4 hip = glm::translate(
            pelvis, Vector3{ side * 0.095f, -0.04f, 0.0f });
        const Matrix4 knee = glm::translate(
            hip, Vector3{ 0.0f, -0.40f, 0.0f });
        const Matrix4 ankle = glm::translate(
            knee, Vector3{ 0.0f, -0.37f, 0.0f });

        drawPart(hip, Vector3{ 0.0f, -0.20f, 0.0f },
            Vector3{ 0.16f, 0.40f, 0.17f }, pants);
        drawPart(knee, Vector3{ 0.0f, -0.185f, 0.0f },
            Vector3{ 0.13f, 0.37f, 0.14f }, pants);
        drawPart(ankle, Vector3{ 0.0f, -0.05f, 0.045f },
            Vector3{ 0.14f, 0.10f, 0.23f }, jersey);
    }
}