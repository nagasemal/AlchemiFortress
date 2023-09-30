#include <pch.h>
#include "ScreenToWorld.h"

SimpleMath::Vector3 CalcScreenToWorldN(int sX, int sY, float fZ, int screen_W, int screen_H, SimpleMath::Matrix view, SimpleMath::Matrix prj)
{
	// �e�s��̋t�s����Z�o
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

	// �t�ϊ�
	SimpleMath::Matrix tmp = invViewport * invPrj * invView;

	SimpleMath::Vector3 pOut = SimpleMath::Vector3::Zero;

	pOut = DirectX::XMVector3TransformCoord(SimpleMath::Vector3((float)sX, (float)sY, (float)fZ), tmp);

	return pOut;
}

SimpleMath::Vector3 CalcScreenToXZN(int sX, int sY, int screen_W, int screen_H, SimpleMath::Matrix view, SimpleMath::Matrix prj)
{
	SimpleMath::Vector3 nearPos, farPos, ray = SimpleMath::Vector3::Zero;

	// �ŋߓ_
	nearPos = CalcScreenToWorldN(sX, sY, 0.0f, screen_W, screen_H, view, prj);

	// �ŉ��_
	farPos = CalcScreenToWorldN(sX, sY, 1.0f, screen_W, screen_H, view, prj);

	ray = farPos - nearPos;
	ray.Normalize();

	SimpleMath::Vector3 pOut = SimpleMath::Vector3::Zero;

	// �J�������猩��Y�������ɑ������ǂ����@��_�����݂��邩�ǂ���
	if (ray.y <= 0)
	{
		// ����_
		SimpleMath::Vector3 lRay = DirectX::XMVector3Dot(ray, SimpleMath::Vector3(0, 1, 0));
		SimpleMath::Vector3 lp0 = DirectX::XMVector3Dot(-nearPos, SimpleMath::Vector3(0, 1, 0));

		pOut = nearPos + (lp0 / lRay) * ray;

	}
	else
	{
		pOut = farPos;
	}

	return pOut;
}