
#include "Precompiled.h"
#include "SoftRenderer.h"

// �׸��� �׸���
void SoftRenderer::DrawGrid2D()
{
	// �׸��� ����
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// ���� ���� ���� �׸���
	ScreenPoint screenHalfSize = _ScreenSize.GetHalf();

	for (int x = screenHalfSize._X; x <= _ScreenSize._X; x += _Grid2DUnit)
	{
		_RSI->DrawFullVerticalLine(x, gridColor);
		if (x > screenHalfSize._X)
		{
			_RSI->DrawFullVerticalLine(2 * screenHalfSize._X - x, gridColor);
		}
	}

	for (int y = screenHalfSize._Y; y <= _ScreenSize._Y; y += _Grid2DUnit)
	{
		_RSI->DrawFullHorizontalLine(y, gridColor);
		if (y > screenHalfSize._Y)
		{
			_RSI->DrawFullHorizontalLine(2 * screenHalfSize._Y - y, gridColor);
		}
	}

	// ���� �� �׸���
	_RSI->DrawFullHorizontalLine(screenHalfSize._Y, LinearColor::Red);
	_RSI->DrawFullVerticalLine(screenHalfSize._X, LinearColor::Green);
}


// ���� ����
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	// �ð��� ���� ȸ����
	static float rotateSpeed = 180.f;
	float deltaRadian = Math::Deg2Rad(rotateSpeed * InDeltaSeconds);

	// ��� ����
	float sin = sinf(deltaRadian);
	float cos = cosf(deltaRadian);
	Vector2 xAxis(cos, sin);
	Vector2 yAxis(-sin, cos);
	Matrix2x2 rotateMat(xAxis, yAxis);

	// �� ȸ���ϱ�
	_CurrentPosition = rotateMat * _CurrentPosition;
}

// ������ ����
void SoftRenderer::Render2D()
{
	// ���� �׸���
	DrawGrid2D();

	// ������ ��ġ�� ������ �������� �� ���
	_RSI->DrawPoint(_CurrentPosition, _CurrentColor);
	_RSI->DrawPoint(_CurrentPosition + Vector2::UnitX, _CurrentColor);
	_RSI->DrawPoint(_CurrentPosition - Vector2::UnitX, _CurrentColor);
	_RSI->DrawPoint(_CurrentPosition + Vector2::UnitY, _CurrentColor);
	_RSI->DrawPoint(_CurrentPosition - Vector2::UnitY, _CurrentColor);
}

