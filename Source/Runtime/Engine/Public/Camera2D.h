#pragma once
#include "GameObject2D.h"

class Camera2D : public GameObject2D
{
public:
	Camera2D() = default;
	Camera2D(std::string Name) : GameObject2D(Name) {}
	Matrix3x3 GetViewMatrix() const;
};

