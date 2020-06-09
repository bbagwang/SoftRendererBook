
#include "Precompiled.h"

void Transform2D::SetWorldPosition(const Vector2& InPosition)
{
	WorldPosition = InPosition;
	
	Position = _ParentPtr ? _ParentPtr->GetInvertedWorldModrlingMatrix() * WorldPosition : WorldPosition;
	
	Update();
}

void Transform2D::AddWorldPosition(const Vector2& InPosition)
{
	WorldPosition += InPosition;

	Position = _ParentPtr ? _ParentPtr->GetInvertedWorldModrlingMatrix() * WorldPosition : WorldPosition;

	Update();
}

void Transform2D::SetWorldScale(const Vector2& InScale)
{
	WorldScale = InScale;

	if (_ParentPtr == nullptr)
	{
		Scale = WorldScale;
	}
	else
	{
		Vector2 invScale = _ParentPtr->WorldScale.Invert();
		Scale = Vector2(WorldScale.X * invScale.X, WorldScale.Y * invScale.Y);
	}

	Update();
}

void Transform2D::AddWorldScale(const Vector2& InScale)
{
	WorldScale += InScale;
	if (_ParentPtr == nullptr)
	{
		Scale = WorldScale;
	}
	else
	{
		Vector2 invScale = _ParentPtr->WorldScale.Invert();
		Scale = Vector2(WorldScale.X * invScale.X, WorldScale.Y * invScale.Y);
	}

	Update();
}

void Transform2D::SetWorldRotation(float InDegree)
{
	WorldRotation = InDegree;

	Rotation = _ParentPtr ? (WorldRotation - _ParentPtr->WorldRotation) : WorldRotation;

	Update();
}

void Transform2D::AddWorldRotation(float InDegree)
{
	WorldRotation += InDegree;

	Rotation = _ParentPtr ? (WorldRotation - _ParentPtr->WorldRotation) : WorldRotation;

	Update();
}

Matrix3x3 Transform2D::GetModelingMatrix() const
{
	Matrix3x3 translateMat(Vector3::UnitX, Vector3::UnitY, Vector3(Position));

	Matrix3x3 rotationMat(
		Vector3(Right, false),
		Vector3(Up, false),
		Vector3::UnitZ);

	Matrix3x3 scaleMat(Vector3::UnitX * Scale.X, Vector3::UnitY * Scale.Y, Vector3::UnitZ);
	return translateMat * rotationMat * scaleMat;
}

Matrix3x3 Transform2D::GetInvertedModelingMatrix() const
{
	//T
	Matrix3x3 T(Matrix3x3::Identity);
	T.Cols[2] = Vector3(-Position, true);

	//ȸ�� �������
	float sin, cos;
	Math::GetSinCos(sin, cos, WorldRotation);
	Vector2 WorldRight = Vector2(cos, sin);
	Vector2 WorldUp = Vector2(-sin, cos);

	//R
	Matrix3x3 R(Vector3(WorldRight, false), Vector3(WorldUp, false), Vector3::UnitZ);
	R = R.Tranpose();

	//S
	Matrix3x3 S(Vector3::UnitX * WorldScale.Invert().X, Vector3::UnitY * WorldScale.Invert().Y, Vector3::UnitZ);

	//S R T
	return S * R * T;
}

Matrix3x3 Transform2D::GetInvertedWorldModrlingMatrix() const
{
	//T
	Matrix3x3 T(Matrix3x3::Identity);
	T.Cols[2] = Vector3(-WorldPosition, false);

	//R
	Matrix3x3 R(Vector3(Right, false), Vector3(Up, false), Vector3::UnitZ);
	R = R.Tranpose();

	//S
	Matrix3x3 S(Vector3::UnitX * (1.f / Scale.X), Vector3::UnitY * (1.f / Scale.Y), Vector3::UnitZ);
	//Matrix3x3 S(Vector3::UnitX * Scale.Invert().X, Vector3::UnitY * Scale.Invert().Y, Vector3::UnitZ);

	//S R T
	Matrix3x3 InvertedWorldModelingMatrix = S * R * T;

	//��Ʈ üũ�� ���� �𵨸� ��İ�
	if (_ParentPtr)
		InvertedWorldModelingMatrix = _ParentPtr->GetInvertedWorldModrlingMatrix() * InvertedWorldModelingMatrix;

	return InvertedWorldModelingMatrix;
}

bool Transform2D::SetParent(Transform2D* InTransformPtr)
{
	if (InTransformPtr == nullptr)
		return false;

	// ���� ������ �̹� �����Ǿ� �ִ��� üũ
	Transform2D* current;
	for (current = InTransformPtr; current != nullptr; current = current->GetParent())
	{
		if (this == current)
			return false;
	}

	// �̹� �θ� �ִ� ��� �ش� �θ��� �ڽ� ��Ͽ��� �ڽ��� ����.
	Transform2D* parent = GetParent();
	if (parent != nullptr)
	{
		ChildrenIterator it = parent->Find(this);
		assert(it != parent->End());
		parent->_Children.erase(it);
		_ParentPtr = nullptr;
	}

	// ���ο� �θ� �ڽ��� ����ϰ� �θ�� ���
	InTransformPtr->_Children.push_back(this);
	_ParentPtr = InTransformPtr;

	// �θ�κ��� ���� ������ ����
	Vector2 invScale = _ParentPtr->Scale.Invert();
	Scale = Vector2(invScale.X * Scale.X, invScale.Y * Scale.Y); // ���� ���� ������
	
	float invRotation = -_ParentPtr->Rotation;
	Rotation = invRotation + Rotation; // ���� ���� ȸ��

	Position = _ParentPtr->GetInvertedWorldModrlingMatrix() * Position; // ȸ���� �̵� ���Ϳ� Inv������ ����  ( ���� ���� ��ġ )
	
	Update();

	return true;
}

void Transform2D::Update()
{
	CalculateLocalAxis(); // �ڽ��� ���� �� ������ ������
	CalculateMatrices(); // ����� �̸� ���صα�

	ChildrenIterator it;  // �ڽĵ鵵 ��� ������ �����Ƿ� ��������� ��� ����
	for (it = Begin(); it != End(); ++it)
	{
		(*it)->Update(); // World ������ ����
	}
}

void Transform2D::CalculateMatrices()
{
	_LocalTRS = GetModelingMatrix();

	if (_ParentPtr == nullptr)
	{
		WorldPosition = Position;
		WorldRotation = Rotation;
		WorldScale = Scale;
		_WorldTRS = _LocalTRS;  // ��Ʈ�� ��� ���� TRS�� ���� TRS�� ����

	}
	else
	{
		WorldPosition = _ParentPtr->_WorldTRS * Position;
		WorldRotation = _ParentPtr->WorldRotation * Rotation;
		WorldScale = Vector2(_ParentPtr->WorldScale.X * Scale.X, _ParentPtr->WorldScale.Y * Scale.Y);
		_WorldTRS = _ParentPtr->_WorldTRS * _LocalTRS; // �θ��� ���� TRS�� ���� TRS�� ���Ͽ� ���� TRS ����� ����
	}
}

void Transform2D::CalculateLocalAxis()
{
	float sin, cos;
	Math::GetSinCos(sin, cos, Rotation);

	Right = Vector2(cos, sin);
	Up = Vector2(-sin, cos);
}
