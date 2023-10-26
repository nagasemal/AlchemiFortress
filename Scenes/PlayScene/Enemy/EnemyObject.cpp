#include "pch.h"
#include "EnemyObject.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/ModelShader.h"

#include "NecromaLib/GameData/Easing.h"

// �R�}���h
#include "Scenes/PlayScene/Enemy/EnemyList/EnemyCommander.h"

#include "Scenes/PlayScene/Enemy/EnemyCommand_Identifier.h"

#define GRAVITY 0.8f

EnemyObject::EnemyObject(ENEMY_TYPE type, SimpleMath::Vector3 startPos, int lv) :
	m_power(1),
	m_hp(10),
	m_lv(lv),
	m_accele(),
	m_lengthVec(),
	m_exp(),
	m_stopFlag(),
	m_enemyType(type),
	m_rotation(),
	m_moveVec(),
	m_aliveTimer(),
	m_targetPos(),
	m_element(ELEMENT::NOMAL),
	m_gravityScale()
{

	m_data.pos = startPos;
	m_data.rage = SimpleMath::Vector3(0.25f, 0.25f, 0.25f);

}

EnemyObject::~EnemyObject()
{
}

void EnemyObject::Initialize()
{
}

void EnemyObject::Update()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();
	m_aliveTimer += deltaTime;

	// �g�k�A�j���[�V����
	m_data.rage.y = Easing::EaseInCirc(0.25f, 0.30f, sinf(m_aliveTimer));
	m_data.rage.x = Easing::EaseInCirc(0.25f, 0.30f, cosf(m_aliveTimer));
	m_data.rage.z = Easing::EaseInCirc(0.25f, 0.30f, cosf(m_aliveTimer));

	// �������擾�����K������
	m_lengthVec = Easing::Moveing(GetTargetPos(),GetData().pos);
	m_lengthVec.Normalize();

	// �����x�ɏd�͂�K������
	m_gravityScale += GRAVITY * deltaTime;



	// �^�[�Q�b�g�Ɏ����������鏈��
	SimpleMath::Vector3 targetDiff = m_targetPos - m_data.pos;
	targetDiff.y = 0;
	targetDiff.Normalize();
	m_rotation = SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::UnitX, targetDiff);

	// �|�C���^�[���R�}���h�ɓn��
	for (auto& command : m_moveCommands)
	{
		command->SetEnemyPtr(*this);
	}

	// �R�}���h�Đ���ނ̐؂�ւ�
	if (m_moveType == "ALL")m_commander->Execute_ALL();
	if (m_moveType == "ONE")m_commander->Execute_One();

	// ���W�̌v�Z
	m_data.pos += (m_lengthVec * m_accele) * deltaTime;

	m_data.pos.y -= m_gravityScale * deltaTime;
	m_data.pos.y += m_accele.y * deltaTime;

	if (m_data.pos.y <= 0.0f)
	{
		m_data.pos.y = 0.0f;
		m_gravityScale = 0.0f;
	}

	m_stopFlag = false;



	// ������
	m_lengthVec = SimpleMath::Vector3();
	m_accele = SimpleMath::Vector3();
}

void EnemyObject::Draw()
{

}

void EnemyObject::Render(Model* model)
{

	ShareData& pSD = ShareData::GetInstance();

	SimpleMath::Matrix modelMatrix = 
		SimpleMath::Matrix::CreateFromQuaternion(m_rotation)
		* SimpleMath::Matrix::CreateScale(m_data.rage * 5.0f)
		* SimpleMath::Matrix::CreateTranslation(m_data.pos);

	model->UpdateEffects([&](IEffect* effect)
		{
			// ����̓��C�g�����~����
			auto lights = dynamic_cast<IEffectLights*>(effect);
			// �F�ύX
			lights->SetLightDiffuseColor(0, m_color);
			lights->SetLightDiffuseColor(1, m_color);
			lights->SetLightDiffuseColor(2, m_color);

		});

	// �d�Ȃ����ہA�e��`��
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelMatrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{

			ModelShader::GetInstance().SilhouetteShader();

			////�摜�p�T���v���[�̓o�^
			//ID3D11SamplerState* sampler[1] = { pSD.GetCommonStates()->LinearWrap() };
			//pSD.GetContext()->PSSetSamplers(0, 1, sampler);
			////�������`��w��
			//ID3D11BlendState* blendstate = pSD.GetCommonStates()->NonPremultiplied();
			//// �������菈��
			//pSD.GetContext()->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

			//// �[�x�X�e���V���X�e�[�g�̐ݒ�
			//pSD.GetContext()->OMSetDepthStencilState(pSD.GetCommonStates()->DepthNone(), 1);

			//// �J�����O�͍�����
			//pSD.GetContext()->RSSetState(pSD.GetCommonStates()->CullNone());

			//// �s�N�Z���V�F�[�_�[�ɓK��
			//pSD.GetContext()->PSSetShader(pSD.GetModelShadowShader().Get(), nullptr, 0);
		});

	pSD.GetContext()->PSSetShader(nullptr, nullptr, 0);
	pSD.GetContext()->OMSetDepthStencilState(nullptr, 0);

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelMatrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			// �[�x�X�e���V���X�e�[�g�̐ݒ�
			pSD.GetContext()->OMSetDepthStencilState(nullptr, 3);
		});


}

void EnemyObject::Finalize()
{
	for (auto& command : m_moveCommands)
	{
		delete command;
	}

	m_moveCommands.clear();
}

bool EnemyObject::GotoTarget(SimpleMath::Vector3 target)
{
	target;

	return 0;
}

void EnemyObject::HitMachine(bool flag)
{
	// true�Ȃ�Ώ������Ȃ�
	if (m_stopFlag == true) return;

	m_stopFlag = flag;

}

void EnemyObject::Bouns()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_data.pos -= (m_lengthVec * m_accele) * deltaTime;

}

void EnemyObject::SetEnemyData(Enemy_Data data)
{
	// lv�ɉ����ăp�����[�^���㏸����
	m_hp		= data.hp		* m_lv;
	m_exp		= data.exp		* m_lv;
	m_power		= data.power	* m_lv;
	m_enemyType = data.type;
	m_moveType  = data.moveType;

	m_element = data.element;
	m_color = ShareJsonData::GetInstance().GetElementColor(m_element);

	// �R�}���h���e�𓮂����N���X
	m_commander = std::make_unique<EnemyCommander>();

	for (auto& moveData : data.moveData)
	{
		// �󂯎�肽�������̓������R�}���h�N���X���擾����
		ICommand_Enemy* command = ChangeEnemyMoveCommand(moveData.moveName);
		// �l�擾
		MoveParameter moveParam = MoveParameter();

		// �����̃p�����[�^���󂯎��
		moveParam.delay = moveData.delay;
		moveParam.time = moveData.time;
		moveParam.value = moveData.value;

		// �R�}���h�N���X�Ƀp�����[�^������
		command->SetParam(moveParam);

		// �v�f�����Ԃɓ����
		m_moveCommands.push_back(command);
	}

	// �R�}���_�[�N���X�ɃR�}���h��o�^����
	for (auto& command : m_moveCommands)
	{
		m_commander->AddCommand(command);
	}

}

void EnemyObject::HitBullet()
{



}
