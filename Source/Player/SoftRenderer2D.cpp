
#include "Precompiled.h"
#include "SoftRenderer.h"

// 그리드 그리기
void SoftRenderer::DrawGrid2D()
{
	// 그리드 색상
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// 뷰의 영역 계산
	Vector2 viewPos = _GameEngine.GetCamera()->GetTransform().GetPosition();
	Vector2 extent = Vector2(_ScreenSize.X * 0.5f, _ScreenSize.Y * 0.5f);

	// 좌측 하단에서부터 격자 그리기
	int xGridCount = _ScreenSize.X / _Grid2DUnit;
	int yGridCount = _ScreenSize.Y / _Grid2DUnit;

	// 그리드가 시작되는 좌하단 좌표 값 계산
	Vector2 minPos = viewPos - extent;
	Vector2 minGridPos = Vector2(ceilf(minPos.X / (float)_Grid2DUnit), ceilf(minPos.Y / (float)_Grid2DUnit)) * (float)_Grid2DUnit;
	ScreenPoint gridBottomLeft = ScreenPoint::ToScreenCoordinate(_ScreenSize, minGridPos - viewPos);

	for (int ix = 0; ix < xGridCount; ++ix)
	{
		_RSI->DrawFullVerticalLine(gridBottomLeft.X + ix * _Grid2DUnit, gridColor);
	}

	for (int iy = 0; iy < yGridCount; ++iy)
	{
		_RSI->DrawFullHorizontalLine(gridBottomLeft.Y - iy * _Grid2DUnit, gridColor);
	}

	// 월드의 원점
	ScreenPoint worldOrigin = ScreenPoint::ToScreenCoordinate(_ScreenSize, -viewPos);
	_RSI->DrawFullHorizontalLine(worldOrigin.Y, LinearColor::Red);
	_RSI->DrawFullVerticalLine(worldOrigin.X, LinearColor::Green);
}


// 게임 로직
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	static float moveSpeed = 1000.f;

	InputManager input = _GameEngine.GetInputManager();

	Transform2D& PlayerTransform = _GameEngine.GetPlayer()->GetTransform();
	PlayerTransform.AddPosition(Vector2(input.GetXAxis(), input.GetYAxis()) * moveSpeed * InDeltaSeconds);

	_CurrentColor = input.SpacePressed() ? LinearColor::Red : LinearColor::Blue;

	Transform2D& CameraTransform = _GameEngine.GetCamera()->GetTransform();
	Vector2 PlayerPosition = PlayerTransform.GetPosition();
	Vector2 PrevCameraPosition = CameraTransform.GetPosition();
	if ((PlayerPosition - PrevCameraPosition).SizeSquared() < 0.1f)
		CameraTransform.SetPosition(PlayerPosition);
	else
	{
		Vector2 newCameraPosition = PrevCameraPosition + (PlayerPosition - PrevCameraPosition) * InDeltaSeconds*10.f;
		CameraTransform.SetPosition(newCameraPosition);
	}
}

// 렌더링 로직
void SoftRenderer::Render2D()
{
	// 격자 그리기
	DrawGrid2D();

	// 카메라의 뷰 행렬
	Matrix3x3 viewMat = _GameEngine.GetCamera()->GetViewMatrix();

	for (auto& Iter : _GameEngine.GetSceneObjects())
	{
		// 플레이어 게임 오브젝트의 트랜스폼
		Transform2D& transform = Iter->GetTransform();
		Matrix3x3 finalMat = viewMat * transform.GetModelingMatrix();

		// 게임 오브젝트 트랜스폼 정보의 출력
		_RSI->PushStatisticTexts(finalMat.ToStrings());

		// 플레이어 게임 오브젝트의 메시
		Mesh* mesh = _GameEngine.FindResourceByName("Mesh");
		size_t vertexCount = mesh->GetVetrices().size();
		size_t indexCount = mesh->GetIndices().size();
		size_t triangleCount = indexCount / 3;

		// 렌더러가 사용할 정점 버퍼와 인덱스 버퍼 생성
		Vector2* vertices = new Vector2[vertexCount];
		std::memcpy(vertices, &mesh->GetVetrices()[0], sizeof(Vector2) * vertexCount);
		int* indices = new int[indexCount];
		std::memcpy(indices, &mesh->GetIndices()[0], sizeof(int) * indexCount);

		// 각 정점에 행렬을 적용
		for (int vi = 0; vi < vertexCount; ++vi)
		{
			vertices[vi] = finalMat * vertices[vi];
		}

		// 변환된 정점을 잇는 선 그리기
		for (int ti = 0; ti < triangleCount; ++ti)
		{
			int bi = ti * 3;
			_RSI->DrawLine(vertices[indices[bi]], vertices[indices[bi + 1]], _CurrentColor);
			_RSI->DrawLine(vertices[indices[bi]], vertices[indices[bi + 2]], _CurrentColor);
			_RSI->DrawLine(vertices[indices[bi + 1]], vertices[indices[bi + 2]], _CurrentColor);
		}

		delete[] vertices;
		delete[] indices;
	}
}

