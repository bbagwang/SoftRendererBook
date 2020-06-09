
#include "Precompiled.h"
#include <random>

const std::string GameEngine::QuadMeshKey("SM_Quad");
const std::string GameEngine::PlayerKey("Player");

const std::string GameEngine::SunKey("SUN");
const std::string GameEngine::EarthKey("EARTH");
const std::string GameEngine::MoonKey("MOON");
//SPACE X GAZUAAAAA
const std::string GameEngine::MarsKey("MARS");

bool GameEngine::Init(const ScreenPoint& InScreenSize)
{
	if (!_InputManager.GetXAxis || !_InputManager.GetYAxis || !_InputManager.SpacePressed)
	{
		return false;
	}

	if (!LoadResources())
	{
		return false;
	}

	if (!LoadScene(InScreenSize))
	{
		return false;
	}

	return true;
}

bool GameEngine::LoadResources()
{
	auto quadMesh = std::make_unique<Mesh2D>();

	float squareHalfSize = 0.5f;
	int vertexCount = 4;
	int triangleCount = 2;
	int indexCount = triangleCount * 3;

	quadMesh->_Vertices = {
		Vector2(-squareHalfSize, -squareHalfSize),
		Vector2(-squareHalfSize, squareHalfSize),
		Vector2(squareHalfSize, squareHalfSize),
		Vector2(squareHalfSize, -squareHalfSize)
	};

	quadMesh->_Indices = {
		0, 2, 1, 0, 3, 2
	};

	quadMesh->CalculateBounds();

	_Meshes.insert({ GameEngine::QuadMeshKey , std::move(quadMesh) });

	return true;
}

bool GameEngine::LoadScene(const ScreenPoint& InScreenSize)
{
	static float squareScale = 10.f;

	// 플레이어 설정
	auto player = std::make_unique<GameObject2D>(GameEngine::PlayerKey);
	player->SetMesh(GameEngine::QuadMeshKey);
	player->GetTransform().SetLocalScale(Vector2::One * squareScale);
	player->SetColor(LinearColor::Blue);
	_GameObjects.push_back(std::move(player));
	
	//태양계 데모
	//태양
	auto Sun = std::make_unique<GameObject2D>(GameEngine::SunKey);
	Sun->SetMesh(GameEngine::QuadMeshKey);
	Sun->GetTransform().SetLocalScale(Vector2::One*squareScale*5);
	Sun->GetTransform().SetWorldPosition(Vector2::Zero);

	Sun->SetColor(LinearColor::Red);
	_GameObjects.push_back(std::move(Sun));
	//지구
	auto Earth = std::make_unique<GameObject2D>(GameEngine::EarthKey);
	Earth->SetMesh(GameEngine::QuadMeshKey);
	Earth->GetTransform().SetWorldScale(Vector2::One * squareScale*5);
	Earth->GetTransform().SetLocalPosition(Vector2::One * squareScale*10);
	Earth->SetColor(LinearColor::Green);
	_GameObjects.push_back(std::move(Earth));
	GetEarth()->GetTransform().SetParent(&GetSun()->GetTransform());
	//달
	auto Moon = std::make_unique<GameObject2D>(GameEngine::MoonKey);
	Moon->SetMesh(GameEngine::QuadMeshKey);
	Moon->GetTransform().SetWorldScale(Vector2::One);
	Moon->GetTransform().SetLocalPosition(Vector2::One * squareScale*5);
	Moon->SetColor(LinearColor::Black);
	_GameObjects.push_back(std::move(Moon));
	GetMoon()->GetTransform().SetParent(&GetEarth()->GetTransform());
	////화성
	//auto Mars = std::make_unique<GameObject2D>(GameEngine::MarsKey);
	//Mars->SetMesh(GameEngine::QuadMeshKey);
	//Mars->GetTransform().SetWorldPosition(Vector2::One * squareScale*5);
	//Mars->GetTransform().SetWorldScale(Vector2::One * squareScale);
	//Mars->SetColor(LinearColor::Magenta);
	//_GameObjects.push_back(std::move(Mars));
	//GetMars()->GetTransform().SetParent(&GetSun()->GetTransform());

	// 카메라 설정
	_Camera = std::make_unique<Camera2D>();
	_Camera->SetCameraViewSize(InScreenSize);

	//쿼드트리 설정
	_QuadTree = std::make_unique<QuadTree>();

	//쿼드 트리에 플레이어 넣음
	GameObject2D* CurrentObject = GetPlayer();
	Rectangle ObjectBoudary = GetMesh(CurrentObject->GetMeshKey())->GetRectangleBound();
	ObjectBoudary.Min += CurrentObject->GetTransform().GetLocalPosition();
	ObjectBoudary.Max += CurrentObject->GetTransform().GetLocalPosition();
	_QuadTree->Insert(CurrentObject, ObjectBoudary);

	// 랜덤한 배경 설정
	std::mt19937 generator(0);
	std::uniform_real_distribution<float> dist(-15000.f, 15000.f);

	//// 100만개의 배경 게임 오브젝트 생성
	//for (int i = 0; i < 1000000; ++i)
	//{
	//	char name[64];
	//	std::snprintf(name, sizeof(name), "GameObject%d", i);
	//	auto newGo = std::make_unique<GameObject2D>(name);
	//	newGo->GetTransform().SetPosition(Vector2(dist(generator), dist(generator)));
	//	newGo->GetTransform().SetScale(Vector2::One * squareScale);
	//	newGo->SetMesh(GameEngine::QuadMeshKey);
	//	_GameObjects.push_back(std::move(newGo));
	//	GameObject2D* CurrentObject = _GameObjects.back().get();
	//	Rectangle ObjectBoudary = GetMesh(CurrentObject->GetMeshKey())->GetRectangleBound();
	//	ObjectBoudary.Min += CurrentObject->GetTransform().GetPosition();
	//	ObjectBoudary.Max += CurrentObject->GetTransform().GetPosition();
	//	_QuadTree->Insert(CurrentObject, ObjectBoudary);
	//}

	return true;
}
