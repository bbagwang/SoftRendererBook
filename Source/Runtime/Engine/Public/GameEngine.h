#pragma once

class GameEngine
{
public:
	GameEngine() = default;

public:
	bool Init(const ScreenPoint& InScreenSize);
	bool LoadResources();
	bool LoadScene(const ScreenPoint& InScreenSize);
	InputManager& GetInputManager() { return _InputManager; }
	std::vector<std::unique_ptr<GameObject2D>>& GetGameObjects() { return _GameObjects; }
	const Mesh2D* GetMesh(const std::string& InMeshKey) { return _Meshes[InMeshKey].get(); }
	GameObject2D* GetPlayer() { return _GameObjects[0].get(); }
	Camera2D* GetCamera() { return _Camera.get(); }
	QuadTree* GetQuadTree() { return _QuadTree.get(); }

	std::vector<std::unique_ptr<GameObject2D>>::const_iterator GoBegin() const { return _GameObjects.begin(); }
	std::vector<std::unique_ptr<GameObject2D>>::const_iterator GoEnd() const { return _GameObjects.end(); }

	const static std::string QuadMeshKey;
	const static std::string PlayerKey;

	bool _bUsingQuadTreeMode = true;

private:
	InputManager _InputManager;
	std::vector<std::unique_ptr<GameObject2D>> _GameObjects;
	std::unordered_map<std::string, std::unique_ptr<Mesh2D>> _Meshes;
	std::unique_ptr<Camera2D> _Camera;
	std::unique_ptr<QuadTree> _QuadTree;
};

