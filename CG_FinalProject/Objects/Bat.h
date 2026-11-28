#pragma once
#include "Objects/Object.h"

class Bat : public Object
{
public:
    Bat();

    void update() override {}
    void render() const override;

    void setPose(const Matrix4& transform, bool swinging);
    Vector3 getDirection() const { return Vector3(model[1]); }
    Vector3 getBarrelStart() const { return getPos() + getDirection() * 0.48f; }
    Vector3 getBarrelTip() const { return getPos() + getDirection() * 0.78f; }
    float getBarrelRadius() const { return 0.033f; }
    bool isSwinging() const { return swinging; }

private:
    Matrix4 model{ 1.0f };
    bool swinging{ false };
};
