
#include "Precompiled.h"

Matrix4x4 Camera3D::GetViewMatrix() const
{
	return Matrix4x4(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-_Transform._Position, true));
}

void Camera3D::SetLookAtRotation(const Vector3& InTargetPosition)
{
	_Transform._Forward = (_Transform._Position - InTargetPosition).Normalize();
	if (_Transform._Forward.EqualsInTolerance(Vector3::UnitY) || _Transform._Forward.EqualsInTolerance(-Vector3::UnitY))
	{
		_Transform._Right = Vector3::UnitX;
	}
	else
	{
		_Transform._Right = Vector3::UnitY.Cross(_Transform._Forward).Normalize();
	}
	_Transform._Up = _Transform._Forward.Cross(_Transform._Right).Normalize();
}
