#pragma once

class Transform3D
{
public:
	Transform3D() = default;

public:
	void SetPosition(const Vector3& InPosition) { _Position = InPosition; }
	void AddPosition(const Vector3& InDeltaPosition) { _Position += InDeltaPosition; }
	void SetRotation(const Rotator& InRotation) { _Rotation = InRotation; }
	
	void AddYawRotation(float InDegree) { _Rotation.Yaw += InDegree; _Rotation.Clamp(); }
	void AddRollRotation(float InDegree) { _Rotation.Roll += InDegree; _Rotation.Clamp(); }
	void AddPitchRotation(float InDegree) { _Rotation.Pitch += InDegree; _Rotation.Clamp(); }
	
	void SetScale(const Vector3& InScale) { _Scale = InScale; }

	Vector3 GetPosition() const { return _Position; }
	Rotator GetRotation() const { return _Rotation; }
	Vector3 GetScale() const { return _Scale; }

	Matrix4x4 GetModelingMatrix() const
	{
		Matrix4x4 translate = Matrix4x4(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(_Position, true));
		Matrix4x4 rotate = Matrix4x4(Vector4(_Right, false), Vector4(_Up, false), Vector4(_Forward, false), Vector4::UnitW);
		Matrix4x4 scale = Matrix4x4(Vector4::UnitX * _Scale.X, Vector4::UnitY * _Scale.Y, Vector4::UnitZ * _Scale.Z, Vector4::UnitW);
		
		return translate * rotate * scale;
	}
	const Vector3& GetLocalX() const { return _Right; }
	const Vector3& GetLocalY() const { return _Up; }
	const Vector3& GetLocalZ() const { return _Forward; }

private:
	void CalculateLocalAxis();

	Vector3 _Position = Vector3::Zero;
	Rotator _Rotation;
	Vector3 _Scale = Vector3::One;

	Vector3 _Right = Vector3::UnitX;
	Vector3 _Up = Vector3::UnitY;
	Vector3 _Forward = Vector3::UnitZ;

	friend class GameObject3D;
	friend class Camera3D;
};

