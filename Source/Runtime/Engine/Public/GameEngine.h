#pragma once
#include <unordered_map>

class GameEngine
{
public:
	GameEngine() = default;

public:
	bool Init();
	bool LoadResources();
	bool LoadScene();
	InputManager& GetInputManager() { return _InputManager; }
	Camera2D* GetCamera() { return (static_cast<Camera2D*>(FindObjectByName("Camera"))); }
	GameObject2D* GetPlayer() { return (FindObjectByName("Player")); }

	Mesh* FindResourceByName(std::string Name) { return _Resources[Name].get() ? _Resources[Name].get() : nullptr; }

	GameObject2D* FindObjectByName(std::string InName)
	{
		for (auto& Iter : _Scene)
		{
			if (Iter.get()->GetName().compare(InName) == 0)
				return Iter.get();
		}
		return nullptr;
	}

	int GetObjectCount() const { return _Scene.size(); }
	auto& GetSceneObjects() const { return _Scene; }
	auto& GetResourceMap() const { return _Resources; }
private:
	InputManager _InputManager;

	std::vector<std::unique_ptr<GameObject2D>> _Scene;
	//메쉬만 쓰지만 일단 Resource로 선언
	std::unordered_map<std::string, std::unique_ptr<Mesh>> _Resources;
};

