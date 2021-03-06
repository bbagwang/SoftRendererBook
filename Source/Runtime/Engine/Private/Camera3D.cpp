
#include "Precompiled.h"

Matrix4x4 Camera3D::GetViewMatrix() const
{
	Matrix4x4 InverseRotation = Matrix4x4(Vector4(_Transform._Right, false), Vector4(_Transform._Up, false), Vector4(_Transform._Forward, false), Vector4::UnitW).Tranpose();
	Matrix4x4 InverseTranslate = Matrix4x4(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-_Transform._Position, true));
	return InverseRotation * InverseTranslate;
}

Matrix4x4 Camera3D::GetPerspectiveMatrix(int InX, int InY) const
{
	float invertedAspectRatio = (float)InY / (float)InX;

	float FocalLength = 1.f / tanf(Math::Deg2Rad(FOV) * 0.5f);
	float invertedNearFar = 1.f / (NearZ - FarZ);
	float k = (FarZ + NearZ) * invertedNearFar;
	float l = 2.f * FarZ * NearZ * invertedNearFar;
	return Matrix4x4
	(
		Vector4::UnitX * invertedAspectRatio * FocalLength,
		Vector4::UnitY * FocalLength,
		Vector4(0.f, 0.f, k, -1.f),
		Vector4(0.f, 0.f, l, 0.f)
	);
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
