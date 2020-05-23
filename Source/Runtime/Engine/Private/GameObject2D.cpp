
#include "Precompiled.h"
#include "Mesh.h"
#include "Transform2D.h"
#include "GameObject2D.h"
#include <typeinfo>

unsigned int GameObject2D::NameCount = 0;
std::string GameObject2D::ClassName = "GameObject2D";

//template<typename Object>
//std::string GenerateClassNameBasedNumberString() {
//	std::string name = typeid(Object).name();
//	size_t spacePosition = name.find_first_of(" ");
//	if (spacePosition != std::string::npos)
//		return name.substr(spacePosition + 1, name.length());
//	return name;
//}

//std::string GenerateClassNameBasedNumberString()
//{
//	std::string result = GetClassName();
//}