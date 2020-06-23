#pragma once

class Camera3D
{
public:
	Camera3D() { }
	~Camera3D() { }

public:
	Transform3D& GetTransform() { return _Transform; }
	Matrix4x4 GetViewMatrix() const;

	void SetLookAtRotation(const Vector3& InTargetPosition);
protected:
	Transform3D _Transform;
};

