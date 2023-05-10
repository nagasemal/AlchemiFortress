#pragma once

enum Direction
{
	LEFT = 1 << 0,			//���ɓ������Ă���
	TOP = 1 << 1,			//��ɓ������Ă���
	RIGHT = 1 << 2,			//�E�ɓ������Ă���
	BOTTOM = 1 << 3,			//���ɓ������Ă���
	FRONT = 1 << 4,			//�O�ɓ������Ă���
	BACK = 1 << 5,			//��ɓ������Ă���
	TOP_LEFT = TOP + LEFT,		//����ɓ������Ă���
	TOP_RIGHT = TOP + RIGHT,		//�E��ɓ������Ă���
	BOTTOM_LEFT = BOTTOM + LEFT,	//�����ɓ������Ă���
	BOTTOM_RIGHT = BOTTOM + RIGHT,	//�E���ɓ������Ă���
	CENTER = LEFT + TOP + RIGHT + BOTTOM		//�㉺���E�̒��S�ɓ������Ă���
};

struct Circle
{
	DirectX::SimpleMath::Vector3 p;

	float r;

	Circle(DirectX::SimpleMath::Vector3 ip = DirectX::SimpleMath::Vector3(), float ir = 0.f) { p = ip, r = ir; }

	Circle(float ix, float iy,float iz, float ir) { p = DirectX::SimpleMath::Vector3(ix, iy,iz), r = ir; }
};

int AABB_XZ(DirectX::SimpleMath::Vector3 box1APos, DirectX::SimpleMath::Vector3 box1BPos, DirectX::SimpleMath::Vector3 box2APos, DirectX::SimpleMath::Vector3 box2BPos);

int AABB_XY(DirectX::SimpleMath::Vector3 box1APos, DirectX::SimpleMath::Vector3 box1BPos, DirectX::SimpleMath::Vector3 box2APos, DirectX::SimpleMath::Vector3 box2BPos);

int AABB_3D(DirectX::SimpleMath::Vector3 box1APos, DirectX::SimpleMath::Vector3 box1BPos, DirectX::SimpleMath::Vector3 box2APos, DirectX::SimpleMath::Vector3 box2BPos);

bool PointerToBox(DirectX::SimpleMath::Vector2 screenPos,DirectX::SimpleMath::Vector2 box1, DirectX::SimpleMath::Vector2 box2);

bool CircleCollider(Circle circle1, Circle circle2);

bool PointerToCircle(Circle circle, DirectX::SimpleMath::Vector3 point);

struct OBB
{
	DirectX::SimpleMath::Vector3 pos;		//�@���S�_
	DirectX::SimpleMath::Vector3 inc[3];	//	�e���W���̌X����\�������x�N�g��
	float length[3];						//	���S�_����ʂ܂ł̒��� * 2
};

bool OBBToOBB(OBB obbA, OBB obbB);

struct Plane
{
	DirectX::SimpleMath::Vector3 pos;		//���S�_
	DirectX::SimpleMath::Vector3 m_local;	//���[�J�����W�ł̖@���x�N�g��
};

// OBB�ƕ���
bool OBBToPlane(OBB obb, Plane plane,float* len);

// �h�b�g�ς����߂�֐�
float Dot3D(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b);

// �N���X��
DirectX::SimpleMath::Vector3 Cross3D(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b);

float LenSegOnSeparateAxis(DirectX::SimpleMath::Vector3 Sep, DirectX::SimpleMath::Vector3 e1, DirectX::SimpleMath::Vector3 e2, DirectX::SimpleMath::Vector3* e3 = 0);