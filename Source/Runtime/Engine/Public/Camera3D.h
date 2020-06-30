#pragma once

class Camera3D
{
public:
	Camera3D() { }
	~Camera3D() { }

public:
	Transform3D& GetTransform() { return _Transform; }
	Matrix4x4 GetViewMatrix() const;
	Matrix4x4 GetPerspectiveMatrix(int InX, int InY) const;
	void SetLookAtRotation(const Vector3& InTargetPosition);
protected:
	Transform3D _Transform;
	//Near «¡∑ØΩ∫≈“
	float NearZ = 5.5f;
	//Far «¡∑ØΩ∫≈“
	float FarZ = 1000.f;
	//Field Of View
	float FOV = 60.f;
};

