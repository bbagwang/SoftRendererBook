#pragma once

class GameEngine3D
{
public:
	GameEngine3D() = default;

public:
	bool Init(const ScreenPoint& InScreenSize);
	bool LoadResources();
	bool LoadScene(const ScreenPoint& InScreenSize);
	InputManager& GetInputManager() { return _InputManager; }
	std::vector<std::unique_ptr<GameObject3D>>& GetGameObjects() { return _GameObjects; }
	const Mesh3D* GetMesh(const std::string& InMeshKey) { return _Meshes[InMeshKey].get(); }
	GameObject3D* GetPlayer() { return _GameObjects[0].get(); }
	Camera3D* GetCamera() { return _Camera.get(); }

	std::vector<std::unique_ptr<GameObject3D>>::const_iterator GoBegin() const { return _GameObjects.begin(); }
	std::vector<std::unique_ptr<GameObject3D>>::const_iterator GoEnd() const { return _GameObjects.end(); }

	const static std::string CubeMeshKey;
	const static std::string PlayerKey;

private:
	InputManager _InputManager;
	std::vector<std::unique_ptr<GameObject3D>> _GameObjects;
	std::unordered_map<std::string, std::unique_ptr<Mesh3D>> _Meshes;
	std::unique_ptr<Camera3D> _Camera;
};

