#include <pch.h>
#include "ScreenToWorld.h"

DirectX::SimpleMath::Vector3 CalcScreenToWorldN(int sX, int sY, float fZ, int screen_W, int screen_H, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix prj)
{
	// 各行列の逆行列を算出
	DirectX::XMMATRIX invView, invPrj, vP, invViewport = DirectX::XMMATRIX::XMMATRIX();

	invView = DirectX::XMMatrixInverse(nullptr, view);
	invPrj = DirectX::XMMatrixInverse(nullptr, prj);
	vP = DirectX::XMMatrixIdentity();

	DirectX::XMFLOAT4X4 matrix = DirectX::XMFLOAT4X4::XMFLOAT4X4();

	matrix._11 = screen_W / 2.f;
	matrix._22 = -screen_H / 2.f;
	matrix._41 = screen_W / 2.f;
	matrix._42 = screen_H / 2.f;

	vP += DirectX::XMLoadFloat4x4(&matrix);

	invViewport = DirectX::XMMatrixInverse(nullptr, vP);

	// 逆変換
	DirectX::SimpleMath::Matrix tmp = invViewport * invPrj * invView;

	DirectX::SimpleMath::Vector3 pOut = DirectX::SimpleMath::Vector3::Zero;

	pOut = DirectX::XMVector3TransformCoord(DirectX::SimpleMath::Vector3(sX, sY, fZ), tmp);

	return pOut;
}

DirectX::SimpleMath::Vector3 CalcScreenToXZN(int sX, int sY, int screen_W, int screen_H, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix prj)
{
	DirectX::SimpleMath::Vector3 nearPos, farPos, ray = DirectX::SimpleMath::Vector3::Zero;

	// 最近点
	nearPos = CalcScreenToWorldN(sX, sY, 0.0f, screen_W, screen_H, view, prj);

	// 最遠点
	farPos = CalcScreenToWorldN(sX, sY, 1.0f, screen_W, screen_H, view, prj);

	ray = farPos - nearPos;
	ray.Normalize();

	DirectX::SimpleMath::Vector3 pOut = DirectX::SimpleMath::Vector3::Zero;

	// カメラから見てYが無限に続くかどうか　交点が存在するかどうか
	if (ray.y <= 0)
	{
		// 床交点
		DirectX::SimpleMath::Vector3 lRay = DirectX::XMVector3Dot(ray, DirectX::SimpleMath::Vector3(0, 1, 0));
		DirectX::SimpleMath::Vector3 lp0 = DirectX::XMVector3Dot(-nearPos, DirectX::SimpleMath::Vector3(0, 1, 0));

		pOut = nearPos + (lp0 / lRay) * ray;

	}
	else
	{
		pOut = farPos;
	}

	return pOut;
}