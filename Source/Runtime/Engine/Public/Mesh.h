#pragma once

class Mesh
{
public:
	std::vector<Vector2> _Vertices;
	std::vector<int> _Indices;

	FORCEINLINE std::vector<Vector2>& GetVetrices() { return _Vertices; }
	FORCEINLINE std::vector<int>& GetIndices() { return _Indices; }
};

