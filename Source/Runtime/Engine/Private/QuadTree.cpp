#pragma once

#include "Precompiled.h"
#include <iostream>

void QuadTree::Insert(GameObject2D* Object, const Rectangle& ObjectBoundary)
{
	//�ʹ� ���� Ž���Ǵ� ���� ����
	if (Depth >= MAX_DEPTH)
	{
		HoldingObjects.push_back(Object);
		return;
	}

	QT_Dimension Dimension = CheckDimension(ObjectBoundary);

	//�ܰ����� ������ HoldingObject�� ��
	if (Dimension == QT_Dimension::HIT)
	{
		HoldingObjects.push_back(Object);
		return;
	}

	Rectangle NewBoundary;
	Vector2 NewBoundary_Min, NewBoundary_Max;
	Vector2 Center, Extent;
	Boundary.GetCenterAndExtent(Center, Extent);

	//�� 1 ��и���� �� 4 ��и����
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

	//���ο� �ٿ���� ����
	NewBoundary.Min = NewBoundary_Min;
	NewBoundary.Max = NewBoundary_Max;

	//���� ������ ����
	if (!InnerDimensions[Dimension])
	{
		InnerDimensions[Dimension] = std::make_unique<QuadTree>(NewBoundary);
		InnerDimensions[Dimension]->Depth = Depth + 1;
		bLeafNode = false;
	}
	//HIT �ƴϴϱ� ���� �������� �� Ž�� (Recursive)
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
	//������ �ȵ���ִٸ� Intersected �Ǿ��ٰ� �� �� ����.
	return QT_Dimension::HIT;
}

void QuadTree::GetAllTargetBoundaryObjects(const Rectangle& TargetBoundary, std::vector<GameObject2D*>& Outter)
{
	if (Boundary.Intersect(TargetBoundary))
	{
		if (!HoldingObjects.empty())
		{
			//������Ʈ�� �ִ� ����.
			for (auto& ObjectIter : HoldingObjects)
			{
				Outter.push_back(ObjectIter);
			}
		}
		//���� ���� Ž�� (Recursive)
		for (auto& Iter : InnerDimensions)
		{
			if (Iter)
				Iter->GetAllTargetBoundaryObjects(TargetBoundary, Outter);
		}
	}
}
