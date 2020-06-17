
#include "Precompiled.h"

void Transform3D::CalculateLocalAxis()
{
	float cy, sy, cp, sp, cr, sr;
	Math::GetSinCos(sy, cy, _Rotation.Yaw);
	Math::GetSinCos(sp, cp, _Rotation.Pitch);
	Math::GetSinCos(sr, cr, _Rotation.Roll);

	_Right = Vector3(cy * cr + sy * sp * sr, cp * sr, -sy * cr + cy * sp * sr);
	_Up = Vector3(-cy * sr + sy * sp * cr, cp * cr, sy * sr + cy * sp * cr);
	_Forward = Vector3(sy * cp, -sp, cy * cp);
}

