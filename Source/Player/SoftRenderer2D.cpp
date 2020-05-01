
#include "Precompiled.h"
#include "SoftRenderer.h"

// 그리드 그리기
void SoftRenderer::DrawGrid2D()
{
	// 그리드 색상
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// 가로 세로 라인 그리기
	ScreenPoint screenHalfSize = _ScreenSize.GetHalf();

	for (int x = screenHalfSize.X; x <= _ScreenSize.X; x += _Grid2DUnit)
	{
		_RSI->DrawFullVerticalLine(x, gridColor);
		if (x > screenHalfSize.X)
		{
			_RSI->DrawFullVerticalLine(2 * screenHalfSize.X - x, gridColor);
		}
	}

	for (int y = screenHalfSize.Y; y <= _ScreenSize.Y; y += _Grid2DUnit)
	{
		_RSI->DrawFullHorizontalLine(y, gridColor);
		if (y > screenHalfSize.Y)
		{
			_RSI->DrawFullHorizontalLine(2 * screenHalfSize.Y - y, gridColor);
		}
	}

	// 월드 축 그리기
	_RSI->DrawFullHorizontalLine(screenHalfSize.Y, LinearColor::Red);
	_RSI->DrawFullVerticalLine(screenHalfSize.X, LinearColor::Green);
}
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	float twoPiUnit = GetElapsedTime() * Math::TwoPI;
	float sinWave = sinf(twoPiUnit);

	_CurrentColor.R = sinWave;
	_CurrentColor.G = sinWave;
	_CurrentColor.B = 1- sinWave;

	_Transform.SetRotation(sinWave);
}

void SoftRenderer::Render2D()
{
	DrawGrid2D();

	for (int i = -(int)GetScreenSize().X/2; i < (int)GetScreenSize().X/2; i++)
	{
		_Transform.SetPosition(Vector2(i,25));
		_Transform.SetRotation(i*10);
		Vector2 newPosition = _Transform.GetModelingMatrix()*Vector2::One;

		_RSI->DrawPoint(newPosition, _CurrentColor);
		_RSI->DrawPoint(newPosition + Vector2::UnitX, _CurrentColor);
		_RSI->DrawPoint(newPosition - Vector2::UnitX, _CurrentColor);
		_RSI->DrawPoint(newPosition + Vector2::UnitY, _CurrentColor);
		_RSI->DrawPoint(newPosition - Vector2::UnitY, _CurrentColor);

		for (int j = newPosition.Y; j >= -(int)GetScreenSize().Y / 2; j--)
		{
			_RSI->DrawPoint(Vector2(newPosition.X,(float)j), LinearColor::Blue);
			_RSI->DrawPoint(Vector2(newPosition.X,(float)j) + Vector2::UnitX, LinearColor::Blue);
			_RSI->DrawPoint(Vector2(newPosition.X,(float)j) - Vector2::UnitX, LinearColor::Blue);
			_RSI->DrawPoint(Vector2(newPosition.X,(float)j) + Vector2::UnitY, LinearColor::Blue);
			_RSI->DrawPoint(Vector2(newPosition.X,(float)j) - Vector2::UnitY, LinearColor::Blue);
		}
	}
}