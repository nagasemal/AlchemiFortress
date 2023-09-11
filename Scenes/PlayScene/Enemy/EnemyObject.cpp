#include "pch.h"
#include "EnemyObject.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/GameData/Easing.h"

// �R�}���h
#include "Scenes/PlayScene/Enemy/EnemyList/EnemyCommander.h"

#include "Scenes/PlayScene/Enemy/EnemyCommand_Identifier.h"

#define GRAVITY 0.2f

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
	m_targetPos()
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
	m_data.rage.y = Easing::EaseInCirc(0.25f, 0.35f, sinf(m_aliveTimer));
	m_data.rage.x = Easing::EaseInCirc(0.25f, 0.35f, cosf(m_aliveTimer));
	m_data.rage.z = Easing::EaseInCirc(0.25f, 0.35f, cosf(m_aliveTimer));

	// �d�͌v�Z
	m_data.pos.y -= GRAVITY;
	if (m_data.pos.y <= 0.0f)	m_data.pos.y = 0.0f;

	// �^�[�Q�b�g�Ɏ����������鏈��
	SimpleMath::Vector3 targetDiff = m_targetPos - m_data.pos;
	targetDiff.y = 0;
	targetDiff.Normalize();
	m_rotation = SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::UnitX, targetDiff);

	// �ړ����~�߂鏈��
	if (!m_stopFlag)
	{
		// �|�C���^�[���R�}���h�ɓn��
		for (auto& command : m_moveCommands)
		{
			command->SetEnemyPtr(*this);
		}

		// �R�}���h�Đ���ނ̐؂�ւ�
		if (m_moveType == "ALL")m_commander->Execute_ALL();
		if (m_moveType == "ONE")m_commander->Execute_One();

		// ���W�̌v�Z
		m_data.pos += m_lengthVec * deltaTime;
	}

	m_stopFlag = false;

	// ������
	m_lengthVec = SimpleMath::Vector3();
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

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelMatrix, pSD.GetView(), pSD.GetProjection());

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

	m_data.pos -= m_lengthVec * deltaTime;

}

void EnemyObject::SetEnemyData(Enemy_Data data)
{
	m_hp		= data.hp;
	m_exp		= data.exp;
	m_power		= data.power;
	m_enemyType = data.type;
	m_moveType  = data.moveType;

	m_commander = std::make_unique<EnemyCommander>();

	for (auto& moveData : data.moveData)
	{
		// �R�}���h�N���X�擾
		ICommand_Enemy* command = ChangeEnemyMoveCommand(moveData.moveName);
		// �l�擾
		MoveParameter moveParam = MoveParameter();

		moveParam.delay = moveData.delay;
		moveParam.time = moveData.time;
		moveParam.value = moveData.value;

		command->SetParam(moveParam);

		m_moveCommands.push_back(command);
	}

	for (auto& command : m_moveCommands)
	{
		m_commander->AddCommand(command);
	}

}