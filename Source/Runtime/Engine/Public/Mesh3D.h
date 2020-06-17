#pragma once

class Mesh3D
{
public:
	Mesh3D() {};
	~Mesh3D() {};

public:
	std::vector<Vector3> _Vertices;
	std::vector<int> _Indices;

private:
};

