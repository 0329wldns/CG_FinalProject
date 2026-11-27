#include "pch.h" // 전처리기 헤더 파일을 포함합니다. 이 파일은 프로젝트의 공통 헤더를 포함하고 있어 컴파일 시간을 단축시킵니다.
#include "Objects/Bat.h"
#include "Objects/Batter.h" // Batter 클래스의 정의를 포함합니다.
#include "Managers/MeshManager.h" // MeshManager 클래스의 정의를 포함합니다.
#include "Managers/ShaderManager.h" // ShaderManager 클래스의 정의를 포함합니다.

#include "Managers/TimeManager.h"
#include "Managers/KeyManager.h"
#include "Constants.h"

namespace
{
constexpr float thighLength = 0.40f;
constexpr float shinLength = 0.37f;
constexpr float hipDrop = 0.04f;

Matrix4 alignY(const Vector3& position, const Vector3& direction)
{
    const Vector3 y = glm::normalize(direction);
    const Vector3 reference = glm::abs(y.y) < 0.9f ?
        Vector3{ 0.0f, 1.0f, 0.0f } : Vector3{ 1.0f, 0.0f, 0.0f };
    const Vector3 x = glm::normalize(glm::cross(y, reference));
    return Matrix4{ Vector4(x, 0.0f), Vector4(y, 0.0f),
        Vector4(glm::cross(x, y), 0.0f), Vector4(position, 1.0f) };
}
}

void Batter::update()
{
    auto& keys = KeyManager::getInstance();
    const auto pitchInput = keys.getKeyState(KEY::MOUSE_L);
    if (pitchInput == KEY_STATE::TAP && pose == Pose::Ready)
    {
        pose = Pose::Routine;
        poseTime = 0.0f;
    }
    if (pitchInput == KEY_STATE::AWAY && pose == Pose::Routine)
        pose = Pose::Ready;

    if (pose != Pose::Swing && pose != Pose::Recovery
        && keys.getKeyState(KEY::SPACE) == KEY_STATE::TAP)
    {
        swingStartYaw = torsoYaw;
        pose = Pose::Swing;
        poseTime = 0.0f;
    }

    const float dt = TimeManager::getInstance().getDeltaTime();
    switch (pose)
    {
    case Pose::Routine:
    {
        poseTime = glm::min(poseTime + dt, 1.2f);
        const float phase = poseTime / 1.2f;
        torsoYaw = 15.0f * (0.5f - 0.5f * glm::cos(glm::radians(360.0f) * phase));
        break;
    }
    case Pose::Ready:
        torsoYaw = glm::max(0.0f, torsoYaw - 90.0f * dt);
        break;
    case Pose::Swing:
        poseTime += dt;
        if (poseTime < 0.14f)
            torsoYaw = glm::mix(swingStartYaw, 18.0f, glm::smoothstep(0.0f, 0.14f, poseTime));
        else
            torsoYaw = glm::mix(18.0f, -105.0f, glm::smoothstep(0.14f, 0.43f, poseTime));
        if (poseTime >= 0.65f)
        {
            pose = Pose::Recovery;
            poseTime = 0.0f;
        }
        break;
    case Pose::Recovery:
        poseTime += dt;
        torsoYaw = glm::mix(-105.0f, 0.0f, glm::smoothstep(0.0f, 0.35f, poseTime));
        if (poseTime >= 0.35f)
        {
            pose = Pose::Routine;
            poseTime = 0.0f;
        }
        break;
    }
    updatePose();
}

void Batter::updatePose()
{
    // root는 발밑, pelvis는 골반 중심, torso는 몸통 중심이다.
    const Vector3 up{ 0.0f, 1.0f, 0.0f };
    const Matrix4 root = glm::rotate(
        glm::translate(Matrix4{ 1.0f }, getPos()), glm::radians(-90.0f), up);
    const float spread = glm::radians(23.0f);
    const float ankleHeight = 0.10f;
    // 다리를 벌린 뒤에도 신발 밑면이 발밑 기준 높이에 닿게 한다.
    const float pelvisHeight = hipDrop + (thighLength + shinLength) * glm::cos(spread) + ankleHeight;
    // 골반은 손과 배트가 나오기 전에 먼저 열린다.
    float hipTurn = 0.0f;
    if (pose == Pose::Swing)
        hipTurn = glm::smoothstep(0.09f, 0.31f, poseTime);
    else if (pose == Pose::Recovery)
        hipTurn = 1.0f - glm::smoothstep(0.0f, 0.35f, poseTime);

    const float load = pose == Pose::Swing ?
        glm::smoothstep(0.0f, 0.09f, poseTime) *
        (1.0f - glm::smoothstep(0.09f, 0.23f, poseTime)) : 0.0f;
    const float step = pose == Pose::Swing ? glm::smoothstep(0.04f, 0.14f, poseTime)
        : pose == Pose::Recovery ? 1.0f - glm::smoothstep(0.0f, 0.35f, poseTime) : 0.0f;
    const auto footTransform = [&](float side)
    {
        const float footSpacing = 0.095f + (thighLength + shinLength) * glm::sin(spread);
        // 앞발은 배트가 나오기 전에 짧게 내딛고, 팔로스루까지 그 위치를 지킨다.
        const float frontStride = side < 0.0f ? 0.14f * step : 0.0f;
        const float footLift = side < 0.0f ? 0.16f * step * (1.0f - step) : 0.0f;
        const Matrix4 footBase = glm::translate(
            root, Vector3{ side * footSpacing - frontStride, ankleHeight + footLift, 0.0f });
        const float footYaw = (side > 0.0f ? -55.0f : -12.0f) * hipTurn;
        // 뒷발은 발끝 위치를 기준으로 돌아가며 뒤꿈치가 올라온다.
        const Vector3 toeOffset{ 0.0f, -0.10f, 0.16f };
        Matrix4 foot = glm::translate(footBase, toeOffset);
        foot = glm::rotate(foot, glm::radians(footYaw), up);
        if (side > 0.0f)
            foot = glm::rotate(foot, glm::radians(28.0f * hipTurn),
                Vector3{ 1.0f, 0.0f, 0.0f });
        foot = glm::translate(foot, -toeOffset);
        return foot;
    };
    const float hipYaw = glm::radians(-50.0f * hipTurn);
    const float weightTransfer = glm::max(hipTurn, 0.70f * step);
    Vector3 pelvisOffset{ 0.09f * load * (1.0f - step) - 0.16f * weightTransfer,
        pelvisHeight - 0.025f - 0.025f * hipTurn, 0.0f };
    // 양쪽 발목에 닿을 수 있는 높이 안에서 골반을 움직인다.
    const Matrix4 hipFrame = glm::rotate(root, hipYaw, up);
    const float legReachLimit = thighLength + shinLength - 0.001f;
    for (float side : { -1.0f, 1.0f })
    {
        const Vector3 hipAtZeroHeight(root * Vector4(pelvisOffset.x, 0.0f, 0.0f, 1.0f)
            + hipFrame * Vector4(side * 0.095f, -hipDrop, 0.0f, 0.0f));
        const Vector3 toAnkle = Vector3(footTransform(side)[3]) - hipAtZeroHeight;
        const float verticalReach = glm::sqrt(glm::max(0.0f,
            legReachLimit * legReachLimit - toAnkle.x * toAnkle.x - toAnkle.z * toAnkle.z));
        pelvisOffset.y = glm::min(pelvisOffset.y, toAnkle.y + verticalReach);
    }
    const Matrix4 pelvis = glm::translate(root, pelvisOffset);
    const Matrix4 hips = glm::rotate(pelvis, hipYaw, up);
    const Matrix4 torsoPivot = glm::rotate(
        pelvis, glm::radians(torsoYaw), Vector3{ 0.0f, 1.0f, 0.0f });
    const Matrix4 torso = glm::translate(torsoPivot, Vector3{ 0.0f, 0.34f, 0.0f });

    // 준비, 임팩트, 팔로스루에서 손의 위치와 배럴 방향을 각각 지정한다.
    // 전체 스윙에 가감속을 적용해 임팩트에서 배트가 멈추지 않게 한다.
    const float sweep = pose == Pose::Swing ? glm::smoothstep(0.14f, 0.43f, poseTime) : 1.0f;
    const float through = glm::min(sweep / 0.56f, 1.0f);
    const float follow = glm::max((sweep - 0.56f) / 0.44f, 0.0f);
    const float swingWeight = pose == Pose::Swing ? 1.0f :
        pose == Pose::Recovery ? 1.0f - glm::smoothstep(0.0f, 0.35f, poseTime) : 0.0f;
    const Vector3 readyGrip{ 0.24f, 0.20f, 0.24f };
    const Vector3 readyDirection{ 0.48f, 0.85f, -0.18f };
    const Matrix4 gripSpace = glm::translate(pelvis, Vector3{ 0.0f, 0.34f, 0.0f });
    const float loadYaw = pose == Pose::Swing && poseTime < 0.14f ? torsoYaw : 18.0f;
    const Matrix4 loadRotation = glm::rotate(Matrix4{ 1.0f }, glm::radians(loadYaw), up);
    const Vector3 loadedGrip = Vector3(loadRotation * Vector4(readyGrip, 0.0f));
    const Vector3 loadedDirection = Vector3(loadRotation * Vector4(readyDirection, 0.0f));
    // 손은 존 중심보다 위에 두고 배럴이 공 높이까지 내려오게 한다.
    const float impactHandHeight = STRIKE_ZONE::CENTER.y + 0.245f;
    const Vector3 impactGrip{ -0.04f, impactHandHeight - gripSpace[3].y, 0.30f };
    const Vector3 impactWorld = Vector3(gripSpace * Vector4(impactGrip, 1.0f));
    const Vector3 impactDirection = glm::normalize(
        glm::transpose(glm::mat3(root)) * (STRIKE_ZONE::CENTER - impactWorld));
    const auto curve = [](const Vector3& from, const Vector3& control,
                          const Vector3& to, float t)
    {
        return glm::mix(glm::mix(from, control, t), glm::mix(control, to, t), t);
    };
    // 손은 임팩트에서 몸 앞을 가로질러 반대쪽 어깨로 올라간다.
    const Vector3 gripTangent{ -0.26f, 0.17f, 0.13f };
    // 마무리에서는 그립을 반대쪽 어깨 위로 올려 배럴이 상체를 가로지르지 않게 한다.
    const Vector3 finishGrip{ -0.36f, 0.35f, 0.08f };
    const Vector3 swingGrip = sweep <= 0.56f
        ? curve(loadedGrip, impactGrip - gripTangent * 0.56f, impactGrip, through)
        : curve(impactGrip, impactGrip + gripTangent * 0.44f, finishGrip, follow);

    // 배럴 방향을 수평 회전과 높이 각도로 나눠 임팩트 뒤에도 같은 방향으로 회전시킨다.
    const float impactAzimuth = glm::atan(impactDirection.x, impactDirection.z);
    const float loadAzimuth = glm::atan(loadedDirection.x, loadedDirection.z);
    // +X는 포수 쪽이므로 임팩트 전후에 방위각을 줄여 투수 쪽으로 통과한다.
    const float azimuth = glm::mix(glm::mix(loadAzimuth, impactAzimuth, through),
        glm::radians(-112.0f), follow);
    const float contactElevation = glm::asin(impactDirection.y);
    const float settleBat = glm::smoothstep(0.0f, 0.38f, sweep);
    const float liftBat = glm::smoothstep(0.72f, 1.0f, sweep);
    const float elevation = glm::mix(
        glm::mix(glm::asin(glm::normalize(loadedDirection).y), contactElevation, settleBat),
        glm::radians(48.0f), liftBat);
    const Vector3 swingDirection{ glm::sin(azimuth) * glm::cos(elevation),
        glm::sin(elevation), glm::cos(azimuth) * glm::cos(elevation) };
    const Vector3 grip = glm::mix(Vector3(torso * Vector4(readyGrip, 1.0f)),
        Vector3(gripSpace * Vector4(swingGrip, 1.0f)), swingWeight);
    const Vector3 barrel = glm::normalize(glm::mix(
        Vector3(torso * Vector4(readyDirection, 0.0f)),
        Vector3(root * Vector4(swingDirection, 0.0f)), swingWeight));

    bodyPose = { hips, torso, { footTransform(-1.0f), footTransform(1.0f) } };
    bat.setPose(alignY(grip, barrel), pose == Pose::Swing);
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

    const Vector3 up{ 0.0f, 1.0f, 0.0f };
    const Matrix4& hips = bodyPose.hips;
    const Matrix4& torso = bodyPose.torso;
    const Vector3 grip = bat.getPos();
    const Vector3 barrel = bat.getDirection();

    drawPart(hips, Vector3{ 0.0f }, Vector3{ 0.34f, 0.16f, 0.23f }, pants);
    drawPart(torso, Vector3{ 0.0f }, Vector3{ 0.42f, 0.55f, 0.24f }, jersey);
    drawPart(torso, Vector3{ 0.0f, 0.30f, 0.0f },
        Vector3{ 0.11f, 0.08f, 0.11f }, skin); // 목
    const Matrix4 head = glm::rotate(torso, glm::radians(-90.0f - torsoYaw), up);
    drawPart(head, Vector3{ 0.0f, 0.44f, 0.0f },
        Vector3{ 0.20f, 0.22f, 0.20f }, skin); // 머리

    // X 좌표 부호를 바꿔 양쪽 팔과 다리를 같은 방식으로 만든다.
    for (float side : { -1.0f, 1.0f })
    {
        const Vector3 shoulder = Vector3(
            torso * Vector4{ side * 0.265f, 0.22f, 0.0f, 1.0f });
        const Vector3 hand = grip + barrel * (side * 0.043f);
        const Vector3 reach = hand - shoulder;
        const float distance = glm::length(reach);
        const Vector3 axis = reach / distance;
        const Vector3 elbowHint = Vector3(torso * Vector4{ side, -1.0f, 0.0f, 0.0f });
        const Vector3 bend = glm::normalize(elbowHint - axis * glm::dot(elbowHint, axis));
        const float upperLength = 0.31f;
        const float lowerLength = 0.35f; // 팔꿈치부터 손 중심까지
        const float solveDistance = glm::min(distance, upperLength + lowerLength - 0.001f);
        const float along = (upperLength * upperLength - lowerLength * lowerLength
            + solveDistance * solveDistance) / (2.0f * solveDistance);
        const float outward = glm::sqrt(glm::max(0.0f, upperLength * upperLength - along * along));
        const Vector3 elbow = shoulder + axis * along + bend * outward;
        const Vector3 wrist = hand - glm::normalize(hand - elbow) * 0.05f;

        auto drawLimbSegment = [&](const Vector3& from, const Vector3& to,
                                  float width, float depth, const Vector3& color)
        {
            drawPart(alignY((from + to) * 0.5f, to - from), Vector3{ 0.0f },
                Vector3{ width, glm::length(to - from), depth }, color);
        };
        drawLimbSegment(shoulder, elbow, 0.12f, 0.13f, jersey);
        drawLimbSegment(elbow, wrist, 0.10f, 0.10f, skin);
        drawPart(alignY(hand, barrel), Vector3{ 0.0f },
            Vector3{ 0.10f, 0.09f, 0.09f }, skin);

        // 발목 목표는 바닥 기준에 두고, 회전한 골반과 발목 사이에서 무릎을 계산한다.
        const Matrix4 foot = bodyPose.feet[side > 0.0f ? 1 : 0];
        const Vector3 hipPosition(hips * Vector4{ side * 0.095f, -hipDrop, 0.0f, 1.0f });
        const Vector3 anklePosition(foot[3]);
        const Vector3 legReach = anklePosition - hipPosition;
        const float legDistance = glm::length(legReach);
        const Vector3 legAxis = legReach / legDistance;
        const Vector3 footForward(foot[2]);
        const Vector3 kneeForward = glm::normalize(
            footForward - legAxis * glm::dot(footForward, legAxis));
        const float legSolveDistance = glm::min(legDistance, thighLength + shinLength);
        const float kneeAlong = (thighLength * thighLength - shinLength * shinLength
            + legSolveDistance * legSolveDistance) / (2.0f * legSolveDistance);
        const float kneeBend = glm::sqrt(glm::max(0.0f, thighLength * thighLength - kneeAlong * kneeAlong));
        const Vector3 kneePosition = hipPosition + legAxis * kneeAlong + kneeForward * kneeBend;

        drawLimbSegment(hipPosition, kneePosition, 0.16f, 0.17f, pants);
        drawLimbSegment(kneePosition, anklePosition, 0.13f, 0.14f, pants);
        drawPart(foot, Vector3{ 0.0f, -0.05f, 0.045f },
            Vector3{ 0.14f, 0.10f, 0.23f }, jersey);
    }
}
