#include "pch.h"
#include "EnemyState.h"

#include "NecromaLib/Singleton/ModelShader.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "Scenes/PlayScene/Enemy/EnemyList/ICommand_Enemy.h"
#include "Scenes/PlayScene/Enemy/EnemyList/EnemyCommander.h"
#include "Scenes/PlayScene/Enemy/EnemyObject.h"

#include "Scenes/PlayScene/Shadow/MagicCircle.h"

#define ENEMY_MAGICCIRCLE_NUM 6

Enemy_IdlingState::Enemy_IdlingState()
{

	m_state = ENEMY_STATE::Idoling;

	m_magicCircle = std::make_unique<MagicCircle>();
	m_magicCircle->Initialize();
}

void Enemy_IdlingState::Update(EnemyObject* object)
{
	object->SetAliveTimer(object->GetAliveTimer() - DeltaTime::GetInstance().GetDeltaTime());

	// �����蔻����Ƃ�Ȃ�
	object->SetColliderActive(false);

	m_magicCircle->Update();

	// 5�b�O�ɖ��@�w��W�J����
	if (object->GetAliveTimer() <= 5.0f)
	{
		m_magicCircle->CreateMagicCircle(object->GetData().pos, 1.0f,SimpleMath::Color(0.0f,0.0f,0.0f,1.0));
	}

	//  �������Ԃ�0.0f�ɂȂ����珢������
	if (object->GetAliveTimer() <= 0.0f)
	{

		// ���@�w���폜����
		m_magicCircle->DeleteMagicCircle();
		m_enemyContext->TransitionTo(new Enemy_MoveingState());

	}
}

void Enemy_IdlingState::Render(EnemyObject* object,Model* model)
{
	model;
	object;

	m_magicCircle->CreateWorld();
	m_magicCircle->Render(ENEMY_MAGICCIRCLE_NUM);
}



Enemy_MoveingState::Enemy_MoveingState()
{

	m_state = ENEMY_STATE::Moveing;

}

void Enemy_MoveingState::Update(EnemyObject* object)
{

	object->SetAliveTimer(object->GetAliveTimer() + DeltaTime::GetInstance().GetDeltaTime());
	// �����蔻����Ƃ�
	object->SetColliderActive(true);

	//�@�ړ��R�}���h�Đ���ނ̐؂�ւ�
	if (object->GetMoveType()) object->GetEnemyCommand()->Execute_ALL();
	else					   object->GetEnemyCommand()->Execute_One();

	//�@�������Ȃ��悤�ɂ���
	if (object->GetStopFlag()) object->Bouns();

	// �^�[�Q�b�g�����ɐg�̂�������
	SimpleMath::Vector3 length = object->GetLengthVec();
	length.y = 0.0f;
	object->SetLengthVec(length);
	object->SetRotate(SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::UnitX, length));

	// ���S��ԂɑJ�ڂ���
	if (object->GetHp() <= 0.0f)
	{
		m_enemyContext->TransitionTo(new Enemy_DethState());
	}

}

void Enemy_MoveingState::Render(EnemyObject* object,Model* model)
{
	ShareData& pSD = ShareData::GetInstance();

	// �d�Ȃ����ہA�e��`��
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), object->GetModleMatrix(), pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			ModelShader::GetInstance().SilhouetteShader();
		});

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), object->GetModleMatrix(), pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			// �[�x�X�e���V���X�e�[�g�̐ݒ�
			pSD.GetContext()->OMSetDepthStencilState(nullptr, 3);
		});
}



Enemy_StoppingState::Enemy_StoppingState()
{

	m_state = ENEMY_STATE::Stopping;

}

void Enemy_StoppingState::Update(EnemyObject* object)
{

	// ����}�V�����痣�ꂽ��Ăѓ����o��
	if (!object->GetStopFlag())
	{
		m_enemyContext->TransitionTo(new Enemy_MoveingState());
	}

}

void Enemy_StoppingState::Render(EnemyObject* object,Model* model)
{
	ShareData& pSD = ShareData::GetInstance();

	// �d�Ȃ����ہA�e��`��
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), object->GetModleMatrix(), pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			ModelShader::GetInstance().SilhouetteShader();
		});

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), object->GetModleMatrix(), pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			// �[�x�X�e���V���X�e�[�g�̐ݒ�
			pSD.GetContext()->OMSetDepthStencilState(nullptr, 3);
		});
}



Enemy_KnockBackState::Enemy_KnockBackState(SimpleMath::Vector3 direction, float power)
{

	m_state = ENEMY_STATE::Idoling;

	m_knockBackVec = direction;
	m_knockBackVec.y = 1.0f;

	m_knockBackPower = power / 4.0f;

	m_knockBackVec.Normalize();

}

void Enemy_KnockBackState::Update(EnemyObject* object)
{

	m_knockBackVec.y -= DeltaTime::GetInstance().GetDeltaTime();

	// �����蔻����Ƃ�Ȃ�
	object->SetColliderActive(false);

	object->SetAccele(m_knockBackVec * m_knockBackPower);

	// �^�[�Q�b�g�����ɐg�̂�������
	SimpleMath::Vector3 length = object->GetLengthVec();
	length.y = 0.0f;
	object->SetLengthVec(length);
	object->SetRotate(SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::UnitX, length));

	// �n�ʂɕt������Ăѓ����o��
	if (m_knockBackVec.y <= 0.0f && object->GetPos().y <= 0.0f)
	{
		m_enemyContext->TransitionTo(new Enemy_MoveingState());
	}
}

void Enemy_KnockBackState::Render(EnemyObject* object,Model* model)
{

	ShareData& pSD = ShareData::GetInstance();

	SimpleMath::Color color = SimpleMath::Color(0.75f + m_knockBackVec.y, 0.75f, 0.75f, 1.0f);

	model->UpdateEffects([&](IEffect* effect)
		{
			// ����̓��C�g�����~����
			auto lights = dynamic_cast<IEffectLights*>(effect);
			// �F�ύX
			lights->SetLightDiffuseColor(0, color);
			lights->SetLightDiffuseColor(1, color);
			lights->SetLightDiffuseColor(2, color);

		});


	// �d�Ȃ����ہA�e��`��
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), object->GetModleMatrix(), pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			ModelShader::GetInstance().SilhouetteShader();
		});

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), object->GetModleMatrix(), pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			// �[�x�X�e���V���X�e�[�g�̐ݒ�
			pSD.GetContext()->OMSetDepthStencilState(nullptr, 3);
		});

}

Enemy_DethState::Enemy_DethState()
{
	m_dethTime = 0.0f;
	m_state = ENEMY_STATE::Deth;

}

void Enemy_DethState::Update(EnemyObject* object)
{

	// �����蔻����Ƃ�Ȃ�
	object->SetColliderActive(false);

	m_dethTime += DeltaTime::GetInstance().GetDeltaTime();

	object->SetAccele(SimpleMath::Vector3(0.0f,m_dethTime,0.0f));

	// ��苗���܂Ŕ�񂾂����
	object->SetDethFlag(m_dethTime >= 10.0f);

}

void Enemy_DethState::Render(EnemyObject* object,Model* model)
{
	ShareData& pSD = ShareData::GetInstance();

	// �������`��
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), object->GetModleMatrix(), pSD.GetView(), pSD.GetProjection(), false, [&]
		{

			// �[�x�X�e���V���X�e�[�g�̐ݒ�
			ModelShader::GetInstance().ToransparentShader();

		});
}
