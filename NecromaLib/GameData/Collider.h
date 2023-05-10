#pragma once

enum Direction
{
	LEFT = 1 << 0,			//左に当たっている
	TOP = 1 << 1,			//上に当たっている
	RIGHT = 1 << 2,			//右に当たっている
	BOTTOM = 1 << 3,			//下に当たっている
	FRONT = 1 << 4,			//前に当たっている
	BACK = 1 << 5,			//後に当たっている
	TOP_LEFT = TOP + LEFT,		//左上に当たっている
	TOP_RIGHT = TOP + RIGHT,		//右上に当たっている
	BOTTOM_LEFT = BOTTOM + LEFT,	//左下に当たっている
	BOTTOM_RIGHT = BOTTOM + RIGHT,	//右下に当たっている
	CENTER = LEFT + TOP + RIGHT + BOTTOM		//上下左右の中心に当たっている
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
	DirectX::SimpleMath::Vector3 pos;		//　中心点
	DirectX::SimpleMath::Vector3 inc[3];	//	各座標軸の傾きを表す方向ベクトル
	float length[3];						//	中心点から面までの長さ * 2
};

bool OBBToOBB(OBB obbA, OBB obbB);

struct Plane
{
	DirectX::SimpleMath::Vector3 pos;		//中心点
	DirectX::SimpleMath::Vector3 m_local;	//ローカル座標での法線ベクトル
};

// OBBと平面
bool OBBToPlane(OBB obb, Plane plane,float* len);

// ドット積を求める関数
float Dot3D(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b);

// クロス積
DirectX::SimpleMath::Vector3 Cross3D(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b);

float LenSegOnSeparateAxis(DirectX::SimpleMath::Vector3 Sep, DirectX::SimpleMath::Vector3 e1, DirectX::SimpleMath::Vector3 e2, DirectX::SimpleMath::Vector3* e3 = 0);