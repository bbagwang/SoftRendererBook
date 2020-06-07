#pragma once
#include <array>

class QuadTree
{
	static const int QUAD_TREE_NODE_COUNT = 4;
	static const int MAX_DEPTH = 50;
public:
	QuadTree() :Boundary(Vector2::One * -30000, Vector2::One * 30000) {}
	QuadTree(Rectangle Boundary) :Boundary(Boundary) {}
	QuadTree(Vector2 Min, Vector2 Max) :Boundary(Min, Max) {}
	
	enum QT_Dimension
	{
		//�ϵ� (�� 1 ��и�)
		NorthEast = 0,
		//�ϼ� (�� 2 ��и�)
		NorthWest = 1,
		//���� (�� 3 ��и�)
		SouthWest = 2,
		//���� (�� 4 ��и�)
		SouthEast = 3,

		//�ܰ����� ���� ���
		HIT = 999
	};

protected:
	//�� ������ �簢��
	Rectangle Boundary;
	//�ڽ� ��� ������
	std::array<std::unique_ptr<QuadTree>, 4> InnerDimensions;
	//Intersect �Ǿ ����ִ� ������Ʈ��
	std::vector<GameObject2D*> HoldingObjects;
	//����
	int Depth = 0;
	//���� �������
	bool bLeafNode = true;

public:
	void Insert(GameObject2D* Object, const Rectangle& ObjectBoundary);
	QT_Dimension CheckDimension(const Rectangle& ObjectBoundary);
	void GetAllTargetBoundaryObjects(const Rectangle& TargetBoundary, std::vector<GameObject2D*>& Outter);
	
	FORCEINLINE bool IsLeafNode() const { return bLeafNode; }
};