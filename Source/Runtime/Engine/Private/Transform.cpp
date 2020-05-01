
#include "Precompiled.h"
#include "Transform.h"

Matrix3x3 Transform::GetModelingMatrix() const
{
	//Translation
	Vector3 Translation(Position.X, Position.Y, 1.f);
	Matrix3x3 T_Matrix;
	T_Matrix[2] = Translation;

	//Rotation
	Matrix3x3 R_Matrix;
	float RotationRadian = Math::Deg2Rad(Rotation);
	R_Matrix[0] = Vector3(cosf(RotationRadian), sinf(RotationRadian), 0.f);
	R_Matrix[1] = Vector3(-sinf(RotationRadian), cosf(RotationRadian), 0.f);

	//Scale
	Matrix3x3 S_Matrix;
	S_Matrix[0] *= Scale.X;
	S_Matrix[1] *= Scale.Y;

	return T_Matrix * R_Matrix * S_Matrix;

	/*return Matrix3x3
		(cosf(Rotation)*Scale.X, -sinf(Rotation)*Scale.Y, Position.X,
		 sinf(Rotation)*Scale.X, cosf(Rotation)*Scale.Y, Position.Y,
		 0, 0, 1);*/
}

void Transform::CalculateLocalAxis()
{
	float sine = 0.f;
	float cosine = 0.f;
	Math::GetSinCos(sine, cosine, Rotation);
	Right = Vector2(cosine, sine);
	Up = Vector2(-sine, cosine);
}
