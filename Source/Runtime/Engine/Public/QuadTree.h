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
		//북동 (제 1 사분면)
		NorthEast = 0,
		//북서 (제 2 사분면)
		NorthWest = 1,
		//남서 (제 3 사분면)
		SouthWest = 2,
		//남동 (제 4 사분면)
		SouthEast = 3,

		//외곽선에 맞은 경우
		HIT = 999
	};

protected:
	//이 공간의 사각형
	Rectangle Boundary;
	//자식 노드 공간들
	std::array<std::unique_ptr<QuadTree>, 4> InnerDimensions;
	//Intersect 되어서 들어있는 오브젝트들
	std::vector<GameObject2D*> HoldingObjects;
	//깊이
	int Depth = 0;
	//말단 노드인지
	bool bLeafNode = true;

public:
	void Insert(GameObject2D* Object, const Rectangle& ObjectBoundary);
	QT_Dimension CheckDimension(const Rectangle& ObjectBoundary);
	void GetAllTargetBoundaryObjects(const Rectangle& TargetBoundary, std::vector<GameObject2D*>& Outter);
	
	FORCEINLINE bool IsLeafNode() const { return bLeafNode; }
};