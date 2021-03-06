
#include "Precompiled.h"
#include "SoftRenderer.h"

void SoftRenderer::DrawGizmo3D()
{
}

// 게임 로직
void SoftRenderer::Update3D(float InDeltaSeconds)
{
	static float rotateSpeed = 180.f;
	InputManager input = _GameEngine3D.GetInputManager();

	Transform3D& playerTransform = _GameEngine3D.GetPlayer()->GetTransform();
	playerTransform.AddYawRotation(input.GetXAxis() * rotateSpeed * InDeltaSeconds);
	playerTransform.AddRollRotation(input.GetYAxis() * rotateSpeed * InDeltaSeconds);
	playerTransform.AddPitchRotation(input.SpacePressed() * rotateSpeed * InDeltaSeconds);
}

// 렌더링 로직
void SoftRenderer::Render3D()
{
	// 격자 그리기
	DrawGizmo3D();

	Matrix4x4 viewMat = _GameEngine3D.GetCamera()->GetViewMatrix();
	Matrix4x4 PerspectiveMat = _GameEngine3D.GetCamera()->GetPerspectiveMatrix(_ScreenSize.X, _ScreenSize.Y);
	for (auto it = _GameEngine3D.GoBegin(); it != _GameEngine3D.GoEnd(); ++it) 
	{
		GameObject3D* gameObject = it->get();
		const Mesh3D* mesh = _GameEngine3D.GetMesh(gameObject->GetMeshKey());
		Transform3D& transform = gameObject->GetTransform();
		Matrix4x4 finalMat = PerspectiveMat * viewMat * transform.GetModelingMatrix();

		size_t vertexCount = mesh->_Vertices.size();
		size_t indexCount = mesh->_Indices.size();
		size_t triangleCount = indexCount / 3;

		// 렌더러가 사용할 정점 버퍼와 인덱스 버퍼 생성
		Vector4* vertices = new Vector4[vertexCount];
		for (int vi = 0; vi < vertexCount; ++vi)
		{
			vertices[vi] = Vector4(mesh->_Vertices[vi], true);
		}

		int* indices = new int[indexCount];
		std::memcpy(indices, mesh->_Indices.data(), sizeof(int) * indexCount);

		// 각 정점에 행렬을 적용
		for (int vi = 0; vi < vertexCount; ++vi)
		{
			vertices[vi] = finalMat * vertices[vi];
			float invW = 1.f / vertices[vi].W;
			vertices[vi].X *= invW;
			vertices[vi].Y *= invW;
			vertices[vi].Z *= invW;

			vertices[vi].X *= (_ScreenSize.X * 0.5f);
			vertices[vi].Y *= (_ScreenSize.Y * 0.5f);
		}

		// 변환된 정점을 잇는 선 그리기
		for (int ti = 0; ti < triangleCount; ++ti)
		{
			int bi = ti * 3;

			Vector3 edge1 = (vertices[indices[bi+1]] - vertices[indices[bi]]).ToVector3();
			Vector3 edge2 = (vertices[indices[bi+2]] - vertices[indices[bi]]).ToVector3();
			Vector3 faceNormal = edge1.Cross(edge2).Normalize();
			static Vector3 cameraDir = -Vector3::UnitZ;
			if (cameraDir.Dot(faceNormal) > 0.f)
			{
				continue;
			}

			_RSI->DrawLine(vertices[indices[bi]].ToVector2(), vertices[indices[bi + 1]].ToVector2(), gameObject->GetColor());
			_RSI->DrawLine(vertices[indices[bi]].ToVector2(), vertices[indices[bi + 2]].ToVector2(), gameObject->GetColor());
			_RSI->DrawLine(vertices[indices[bi + 1]].ToVector2(), vertices[indices[bi + 2]].ToVector2(), gameObject->GetColor());
		}

		delete[] vertices;
		delete[] indices;
	}
}

