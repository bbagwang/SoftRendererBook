
#include "Precompiled.h"

void GameObject3D::SetMesh(const std::string& InMeshKey)
{
	_MeshKey = InMeshKey;
}

const std::string& GameObject3D::GetMeshKey() const
{
	return _MeshKey;
}
