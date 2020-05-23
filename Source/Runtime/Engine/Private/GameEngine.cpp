
#include "Precompiled.h"
#include "InputManager.h"
#include "Mesh.h"
#include "Transform2D.h"
#include "GameObject2D.h"
#include "Camera2D.h"
#include "GameEngine.h"

bool GameEngine::Init()
{
	if (!_InputManager.GetXAxis || !_InputManager.GetYAxis || !_InputManager.SpacePressed)
	{
		return false;
	}

	if (!LoadResources())
	{
		return false;
	}

	if (!LoadScene())
	{
		return false;
	}

	return true;
}

bool GameEngine::LoadResources()
{
	_Resources["Mesh"] = std::make_unique<Mesh>();

	constexpr float squareHalfSize = 0.5f;
	constexpr int vertexCount = 4;
	constexpr int triangleCount = 2;
	constexpr int indexCount = triangleCount * 3;

	_Resources["Mesh"].get()->_Vertices = {
		Vector2(-squareHalfSize, -squareHalfSize),
		Vector2(-squareHalfSize, squareHalfSize),
		Vector2(squareHalfSize, squareHalfSize),
		Vector2(squareHalfSize, -squareHalfSize)
	};

	_Resources["Mesh"].get()->_Indices = {
		0, 2, 1, 0, 3, 2
	};

	return true;
}

bool GameEngine::LoadScene()
{
	static float squareScale = 100.f;

	for (int i = 0 ; i < 10; i++)
	{
		_Scene.push_back(std::make_unique<GameObject2D>(std::string("1"),_Resources["Mesh"].get()));
		Vector2 InitPos(rand() % 400, rand() % 400);
		_Scene.back().get()->GetTransform().SetPosition(InitPos);
		_Scene.back().get()->GetTransform().SetScale(Vector2::One * squareScale);
	}

	_Scene.push_back(std::make_unique<GameObject2D>("Player", _Resources["Mesh"].get()));
	_Scene.back().get()->GetTransform().SetScale(Vector2::One * squareScale);
	_Scene.push_back(std::make_unique<Camera2D>("Camera"));
	return true;
}
