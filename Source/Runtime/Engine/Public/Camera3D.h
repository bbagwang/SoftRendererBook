#pragma once

class Camera3D
{
public:
	Camera3D() { }
	~Camera3D() { }

public:
	Transform3D& GetTransform() { return _Transform; }
	Matrix4x4 GetViewMatrix() const
	{
		Matrix4x4 InverseRotation = Matrix4x4(Vector4(_Transform._Right, false), Vector4(_Transform._Up, false), Vector4(_Transform._Forward, false), Vector4::UnitW).Tranpose();
		Matrix4x4 InverseTranslate = Matrix4x4(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-_Transform._Position, true));
		return InverseRotation * InverseTranslate;
	}

	void SetLookAtRotation(const Vector3& InTargetPosition);
protected:
	Transform3D _Transform;
};

