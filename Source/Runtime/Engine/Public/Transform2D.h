#pragma once

class Transform2D
{
public:
	typedef std::vector<Transform2D*>::iterator ChildrenIterator;
	Transform2D() = default;

public:
	void SetLocalPosition(const Vector2& InPosition) { Position = InPosition; Update(); }
	void AddLocalPosition(const Vector2& InPosition) { Position += InPosition; Update(); }
	void SetLocalRotation(float InDegree) { Rotation = InDegree; Update(); }
	void AddLocalRotation(float InDegree) { Rotation += InDegree; Update(); }
	void SetLocalScale(const Vector2& InScale) { Scale = InScale; Update(); }
	void AddLocalScale(const Vector2& InScale) { Scale += InScale; Update(); }

	void SetWorldPosition(const Vector2& InPosition);
	void AddWorldPosition(const Vector2& InPosition);
	void SetWorldScale(const Vector2& InScale);
	void AddWorldScale(const Vector2& InScale);
	void SetWorldRotation(float InDegree);
	void AddWorldRotation(float InDegree);
	
	Vector2 GetLocalPosition() const { return Position; }
	float GetLocalRotation() const { return Rotation; }
	Vector2 GetLocalScale() const { return Scale; }

	Vector2 GetWorldPosition() const { return WorldPosition; }
	float GetWorldRotation() const { return WorldRotation; }
	Vector2 GetWorldScale() const { return WorldScale; }

	//void SetPosition(const Vector2& InPosition) { SetLocalPosition(InPosition); }
	//void AddPosition(const Vector2& InPosition) { Position += InPosition; }
	//void SetScale(const Vector2& InScale) { SetLocalScale(InScale); }
	//void AddScale(const Vector2& InScale) { Scale += InScale; }
	//void SetRotation(float InDegree) { SetLocalRotation(InDegree); }
	//void AddRotation(float InDegree) { Rotation += InDegree; CalculateLocalAxis(); }

	//Vector2 GetPosition() const { return Position; }
	//Vector2 GetScale() const { return Scale; }
	//float GetRotation() const { return Rotation; }

	Matrix3x3 GetModelingMatrix() const;
	Matrix3x3 GetInvertedModelingMatrix() const;
	FORCEINLINE Matrix3x3 GetWorldModelingMatrix() const { return _WorldTRS; }
	Matrix3x3 GetInvertedWorldModrlingMatrix() const;
	
	const Vector2& GetLocalX() const { return Right; }
	const Vector2& GetLocalY() const { return Up; }

	bool SetParent(Transform2D* InTransformPtr);

	void Update();

	void CalculateMatrices();

	FORCEINLINE Transform2D* Transform2D::GetParent() const { return _ParentPtr; }

	FORCEINLINE Transform2D& Transform2D::GetRoot()
	{
		Transform2D* current = this;
		Transform2D* parent = nullptr;
		while ((parent = current->GetParent()) != nullptr)
		{
			current = parent;
		}
		return *current;
	}

	FORCEINLINE Transform2D& Transform2D::GetChild(BYTE InIndex) const
	{
		assert(InIndex < _Children.size());
		return *_Children[InIndex];
	}

	FORCEINLINE ChildrenIterator Begin() { return _Children.begin(); }
	FORCEINLINE ChildrenIterator End() { return _Children.end(); }
	FORCEINLINE ChildrenIterator Find(const Transform2D* InTransformPtr)
	{
		ChildrenIterator it, itEnd = End();
		for (it = Begin(); it != itEnd; ++it)
		{
			if (*it == InTransformPtr)
			{
				return it;
			}
		}
		return itEnd;
	}

private:
	void CalculateLocalAxis();

	Vector2 Position = Vector2::Zero;
	float Rotation = 0.f;
	Vector2 Scale = Vector2::One;

	Vector2 WorldPosition = Vector2::Zero;
	float WorldRotation = 0.f;
	Vector2 WorldScale = Vector2::One;

	Vector2 Right = Vector2::UnitX;
	Vector2 Up = Vector2::UnitY;

	friend class GameObject2D;

	//부모 트렌스폼 포인터
	Transform2D* _ParentPtr = nullptr;
	//자식들
	std::vector<Transform2D*> _Children;

	//로컬 행렬
	Matrix3x3 _LocalTRS;
	//월드 행렬
	Matrix3x3 _WorldTRS;
};

