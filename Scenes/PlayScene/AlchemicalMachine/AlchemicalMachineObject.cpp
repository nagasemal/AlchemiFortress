#include "pch.h"
#include "AlchemicalMachineObject.h"
#include "NecromaLib/Singleton/InputSupport.h"


#define AM_RAGE DirectX::SimpleMath::Vector3(1, 1, 1)


AlchemicalMachineObject::AlchemicalMachineObject():
	m_hp(),
	m_activ(),
	m_hitMouseFlag(),
	m_objectName(),
	m_magicCircle(),
	m_machineID(MACHINE_TYPE::NONE),
	m_lv(1),
	m_machineEffectNum(),
	m_span()
{
}

AlchemicalMachineObject::~AlchemicalMachineObject()
{
}

void AlchemicalMachineObject::HitToObject(MousePointer* pMP)
{

	m_hitMouseFlag = false;

	InputSupport& pINP = InputSupport::GetInstance();
	Circle mouseWolrdPos = Circle();
	mouseWolrdPos.p = InputSupport::GetInstance().GetMousePosWolrd();
	mouseWolrdPos.r = (pMP->GetRage().x + pMP->GetRage().z) / 2;

	// オブジェクトとマウスポインターの当たり判定
	if (CircleCollider(GetCircle(), mouseWolrdPos)) m_hitMouseFlag = true;

}

bool AlchemicalMachineObject::OnCollisionEnter_MagicCircle(GameObjct3D* object)
{
	return 	CircleCollider(m_magicCircle, object->GetCircle());
}

void AlchemicalMachineObject::ModelRender(DirectX::Model* model)
{

	ShareData& pSD = ShareData::GetInstance();

	// モデル情報(位置,大きさ)
	DirectX::SimpleMath::Matrix modelData = DirectX::SimpleMath::Matrix::Identity;
	modelData = DirectX::SimpleMath::Matrix::CreateScale(m_data.rage);
	modelData *= DirectX::SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y, m_data.pos.z);

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection());

}

void AlchemicalMachineObject::SummonAM(DirectX::SimpleMath::Vector3 pos)
{
	m_data.rage = AM_RAGE;
	m_data.pos = pos;
	m_activ = true;

}
