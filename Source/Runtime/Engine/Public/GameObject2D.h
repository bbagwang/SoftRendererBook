#pragma once
#include <algorithm>
#include <string>

class GameObject2D
{
public:
	GameObject2D() = default;
	GameObject2D(std::string Name) : _Name(Name), _MeshPtr(nullptr) { }
	GameObject2D(std::string Name, const Mesh* InMeshPtr) : _Name(Name), _MeshPtr(InMeshPtr) { }

public:
	FORCEINLINE Transform2D& GetTransform() { return _Transform; }
	FORCEINLINE const Mesh* GetMesh() { return _MeshPtr; }
	FORCEINLINE bool HasMesh() { return (_MeshPtr != nullptr); }
	FORCEINLINE std::string GetName() const { return _Name; }
	FORCEINLINE virtual std::string GetClassName() const { return GameObject2D::ClassName; }
protected:
	Transform2D _Transform;
	const Mesh* _MeshPtr = nullptr;
	std::string _Name;
	//Statics
	static unsigned int NameCount;
	static std::string ClassName;
private:
	//같은 이름이 만들어질 가능성이 있음
	static std::string GenerateRandomString(size_t length = 16)
	{
		auto randchar = []() -> char
		{
			const char charset[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";
			const size_t max_index = (sizeof(charset) - 1);
			return charset[rand() % max_index];
		};
		std::string str(length, 0);
		std::generate_n(str.begin(), length, randchar);
		return str;
	}
	//문제가 있음
	/*template<typename Object>
	std::string GenerateClassNameBasedNumberString();*/
};