#pragma once
#include "NecromaLib/GameData/GameObject3D.h"

#include "Scenes/PlayScene/Mouse/MousePointer.h"
#include "Scenes/PlayScene/Enemy/EnemyObject.h"
#include "Scenes/PlayScene/AlchemicalMachine/Bullet/Bullet.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"
#include "Scenes/PlayScene/Field/FieldObjectManager.h"
#include <list>
#include <vector>

#include "Scenes/DataManager.h"

class EnemyObject;
class Bullet;

class AlchemicalMachineObject : public GameObjct3D
{
public:

	// �L�[��񋓌^�Ƃ��Ď����Ă�����悤�ɂ���
	enum MACHINE_TYPE : int
	{
		NONE			 = 0,	// �w��Ȃ�
		ATTACKER		 = 1,	// �U���^
		DEFENSER		 = 2,	// �h��^
		MINING			 = 3,	// �̌@�^
		RECOVERY		 = 4,	// ���͉���^
		UPEER			 = 5,	// �͈͓������^

		NUM
	};

public:

	enum MACHINE_ELEMENT : int
	{
		NOMAL	= 0,
		FLAME	= 1,
		AQUA	= 2,
		WIND	= 3,
		EARTH	= 4,
		SUM
	};


	// �I�u�W�F�N�g�̌��ݏ󋵂̎擾
	enum STATE_NAME : int
	{
		BRING = 0,		// �^�΂�Ă���
		IDEL  = 1,		// �~�܂��Ă���
		Play  = 2		// �쓮���[�h

	};

	AlchemicalMachineObject();
	virtual ~AlchemicalMachineObject() {};

	// �I�����̓��ʂȃA�b�v�f�[�g����
	virtual void SelectUpdate() = 0;

	// �I�����A�}�E�X�I���\��UI�̕\���Ɏg�p
	virtual void RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) = 0;

	// LvUp��p����
	virtual void LvUp() = 0;

	// ���̃I�u�W�F�N�g���m�̓����蔻��
	void HitToObject(MousePointer* pMP);

	// ���ʔ͈͓���3D�I�u�W�F�N�g��������
	bool OnCollisionEnter_MagicCircle(GameObjct3D* object);
	
	// ���f���`��n
	void ModelRender(DirectX::Model* model,DirectX::Model* ring = nullptr);

	// ����
	void SummonAM(DirectX::SimpleMath::Vector3 pos);


// �A�N�Z�T
public:

	const bool GetActive()									const { return m_active; }
	const bool GetHitMouse()								const { return m_hitMouseFlag;}
	const std::string GetObjectName()						const { return m_objectName; }
	const MACHINE_TYPE GetModelID()							const { return m_machineID;}
	const float GetMachineEffectNum()						const { return m_machineEffectNum; }
	const float GetSpan()									const { return m_span;}
	const int	  GetLv()									const { return m_lv; }
	const Circle GetMagicCircle()							const { return m_magicCircle; }
	const DirectX::SimpleMath::Color GetColor()				const { return m_color; }
	const bool GetPowerUpFlag()								const { return m_powerUPFlag; }
	const MACHINE_ELEMENT GetElement()						const { return m_element; }

	void SetPos(DirectX::SimpleMath::Vector3 pos)	{ m_data.pos = pos; }
	void SetMagicCircle(Circle circle)				{ m_magicCircle = circle;}

private:

protected:

	// �}�V��ID�@�L�[�Ƃ��ă��f���̎󂯎����s��
	MACHINE_TYPE m_machineID;

	// �}�V���̑ϋv�l
	int m_hp;

	// �}�V���̃��x��
	int m_lv;

	// �}�V�����L�̃f�[�^(�U���͂�h�䐫�\�A�̌@���x�Ȃ�)������t����f�[�^
	float m_machineEffectNum;

	// ���̌��ʔ����܂ł̎���
	float m_span;

	// ���݂��Ă��邩�ۂ�
	bool m_active;

	// ��������̉e���ŋ������󂯂Ă��邩
	bool m_powerUPFlag;

	// �}�E�X���������Ă��邩�ۂ�
	bool m_hitMouseFlag;

	// Y����]�����邽�߂̕ϐ�
	float m_rotateAnimation;

	// ���ʔ͈�
	Circle m_magicCircle;

	std::string m_objectName;

	// �I�u�W�F�N�g�̐F
	DirectX::SimpleMath::Color m_color;

	// �I�u�W�F�N�g�̑���
	MACHINE_ELEMENT m_element;

};