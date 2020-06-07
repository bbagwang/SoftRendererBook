#pragma once

#include "Precompiled.h"
#include <iostream>

void QuadTree::Insert(GameObject2D* Object, const Rectangle& ObjectBoundary)
{
	//너무 깊이 탐색되는 것을 방지
	if (Depth >= MAX_DEPTH)
	{
		HoldingObjects.push_back(Object);
		return;
	}

	QT_Dimension Dimension = CheckDimension(ObjectBoundary);

	//외곽선에 닿으면 HoldingObject에 들어감
	if (Dimension == QT_Dimension::HIT)
	{
		HoldingObjects.push_back(Object);
		return;
	}

	Rectangle NewBoundary;
	Vector2 NewBoundary_Min, NewBoundary_Max;
	Vector2 Center, Extent;
	Boundary.GetCenterAndExtent(Center, Extent);

	//제 1 사분면부터 제 4 사분면까지
	switch (Dimension)
	{
	case QT_Dimension::NorthEast:
		NewBoundary_Min = Center;
		NewBoundary_Max = Boundary.Max;
		break;
	case QT_Dimension::NorthWest:
		NewBoundary_Min = Vector2(Boundary.Min.X, Center.Y);
		NewBoundary_Max = Vector2(Center.X, Boundary.Max.Y);
		break;
	case QT_Dimension::SouthWest:
		NewBoundary_Min = Boundary.Min;
		NewBoundary_Max = Center;
		break;
	case QT_Dimension::SouthEast:
		NewBoundary_Min = Vector2(Center.X, Boundary.Min.Y);
		NewBoundary_Max = Vector2(Boundary.Max.X, Center.Y);
		break;
	default:
		return;
	}

	//새로운 바운더리 생성
	NewBoundary.Min = NewBoundary_Min;
	NewBoundary.Max = NewBoundary_Max;

	//공간 없으면 생성
	if (!InnerDimensions[Dimension])
	{
		InnerDimensions[Dimension] = std::make_unique<QuadTree>(NewBoundary);
		InnerDimensions[Dimension]->Depth = Depth + 1;
		bLeafNode = false;
	}
	//HIT 아니니까 내부 공간에서 또 탐색 (Recursive)
	InnerDimensions[Dimension]->Insert(Object, ObjectBoundary);

}

QuadTree::QT_Dimension QuadTree::CheckDimension(const Rectangle& ObjectBoundary)
{
	Vector2 Center, Extent;
	Boundary.GetCenterAndExtent(Center, Extent);

	std::cout << "Cente X : " << Center.X << " Y : " << Center.Y << std::endl;
	if (ObjectBoundary.Min.Y > Center.Y)
	{
		if (ObjectBoundary.Min.X > Center.X)
			return QT_Dimension::NorthEast;
		else if (ObjectBoundary.Max.X < Center.X)
			return QT_Dimension::NorthWest;
	}
	else if (ObjectBoundary.Max.Y < Center.Y)
	{
		if (ObjectBoundary.Max.X < Center.X)
			return QT_Dimension::SouthWest;
		else if (ObjectBoundary.Min.X > Center.X)
			return QT_Dimension::SouthEast;
	}
	//공간에 안들어있다면 Intersected 되었다고 볼 수 있음.
	return QT_Dimension::HIT;
}

void QuadTree::GetAllTargetBoundaryObjects(const Rectangle& TargetBoundary, std::vector<GameObject2D*>& Outter)
{
	if (Boundary.Intersect(TargetBoundary))
	{
		if (!HoldingObjects.empty())
		{
			//오브젝트가 있는 경우다.
			for (auto& ObjectIter : HoldingObjects)
			{
				Outter.push_back(ObjectIter);
			}
		}
		//하위 공간 탐색 (Recursive)
		for (auto& Iter : InnerDimensions)
		{
			if (Iter)
				Iter->GetAllTargetBoundaryObjects(TargetBoundary, Outter);
		}
	}
}
