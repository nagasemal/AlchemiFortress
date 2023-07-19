#include "pch.h"
#include "ParticleUtility.h"
#include "NecromaLib/Singleton/DeltaTime.h"

ParticleUtility::ParticleUtility(float life,
	DirectX::SimpleMath::Vector3 pos,
	DirectX::SimpleMath::Vector3 velocity,
	DirectX::SimpleMath::Vector3 accele,
	DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,
	DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor)
{
	m_startLife = m_life = life;

	m_position = pos;
	m_velocity = velocity;
	m_accele = accele;

	m_startScale = m_nowScale = startScale;
	m_endScale = endScale;


	m_startColor = m_nowColor = startColor;
	m_endColor = endColor;

	m_quaternion = DirectX::SimpleMath::Quaternion();
	m_rotate = DirectX::SimpleMath::Vector3();
}

ParticleUtility::~ParticleUtility()
{
}

bool ParticleUtility::Update()
{
	//処理に使う秒速計(1秒で1.0f)を取得する。
	float elapsedTime = DeltaTime::GetInstance().GetDeltaTime();

	// 回転の計算
	DirectX::SimpleMath::Quaternion q;

	// ヨー　
	q *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitX, DirectX::XMConvertToRadians(m_rotate.x));

	// ピッチ
	q *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, DirectX::XMConvertToRadians(m_rotate.y));

	// ロール
	q *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitZ, DirectX::XMConvertToRadians(m_rotate.z));

	// 姿勢に回転を加える
	m_quaternion = q * m_quaternion;

	// スケール
	m_nowScale = DirectX::SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);
	// 色
	m_nowColor = DirectX::SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);
	// 速度の計算
	m_velocity += m_accele * elapsedTime;
	// 座標の計算
	m_position += m_velocity * elapsedTime;
	m_life -= elapsedTime;

	// ライフが０で自身を消してもらう
	if (m_life < 0.0f)
	{
		return false;
	}

	return true;
}
