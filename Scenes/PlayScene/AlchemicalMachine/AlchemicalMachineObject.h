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
class SelectionBox;

class AlchemicalMachineObject : public GameObjct3D
{
public:

	// �L�[��񋓌^�Ƃ��Ď����Ă�����悤�ɂ���
	enum MACHINE_TYPE : int
	{
		NONE			 = 0,	// �w��Ȃ�
		ATTACKER		 = 1,	// �U���^
		UPEER			 = 2,	// �͈͓������^
		DEFENSER		 = 3,	// �h��^
		MINING			 = 4,	// �̌@�^
		RECOVERY		 = 5,	// ���͉���^

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

	// ���ʂ��ăA�b�v�f�[�g�������鏈��
	void SelectUpdate_Common();

	// ���̃I�u�W�F�N�g���m�̓����蔻��
	void HitToObject(MousePointer* pMP);

	// ���ʔ͈͓���3D�I�u�W�F�N�g��������
	bool OnCollisionEnter_MagicCircle(GameObjct3D* object);
	
	/// <summary>
	/// ���f���`��n
	/// </summary>
	/// <param name="model">�@�@�@ �x�[�X�̃��f���@</param>
	/// <param name="ring">�@�@�@�@�ǉ��p�[�c</param>
	void ModelRender(DirectX::Model* model,DirectX::Model* ring = nullptr);

	// ����
	void SummonAM(DirectX::SimpleMath::Vector3 pos);

// �A�N�Z�T
public:

	// ���݂��Ă��邩�ۂ�
	const bool GetActive()									const { return m_active; }

	// �}�E�X���G��Ă��邩�ۂ�
	const bool GetHitMouse()								const { return m_hitMouseFlag;}

	// �I�u�W�F�N�g�̖��O
	const std::string GetObjectName()						const { return m_objectName; }

	// �I�u�W�F�N�g�̃^�C�v
	const MACHINE_TYPE GetModelID()							const { return m_machineID;}

	// ���ݖ��g�p
	const float GetMachineEffectNum()						const { return m_machineEffectNum; }

	// ���ʔ����܂ł̎���
	const float GetSpan()									const { return m_span;}
	
	// ���݃��x��
	const int	  GetLv()									const { return m_lv; }

	// ����HP
	const int	  GetHP()									const { return m_hp; }

	// ����MAXHP
	const int	  GetMAXHP()								const { return m_maxHp; }

	// ���ʔ͈�
	const Circle GetMagicCircle()							const { return m_magicCircle; }

	// �ۗL���Ă���F���
	// HP��0�Ȃ�΍���Ԃ��܂�
	const DirectX::SimpleMath::Color GetColor()				const { return m_hp <= 0 ? DirectX::SimpleMath::Color(0, 0, 0, 1): m_color;}

	// �����̗v���ŕω����󂯂Ă�����
	const bool GetPowerUpFlag()								const { return m_powerUPFlag; }

	// ���݂̑���
	const MACHINE_ELEMENT GetElement()						const { return m_element; }

	// ���݂ǂ̃��C����ɑ��݂��Ă��邩
	const int GetLine()										const { return m_line; }

	void SetPos(DirectX::SimpleMath::Vector3 pos)		{ m_data.pos = pos; }
	void SetMagicCircle(Circle circle)					{ m_magicCircle = circle;}
	void SetActive(bool flag)							{ m_active = flag; }
	void SetLine(int line)								{ m_line = line; }
	void SetPowerUpFlag(bool flag)						{ m_powerUPFlag = flag; }
	void SetSelectModeFlag(bool flag)					{ m_selectModeFlag = flag;}

private:

protected:

	// Lv�̍ő�l
	static const int MAX_LV = 5;

	// �}�V��ID�@�L�[�Ƃ��ă��f���̎󂯎����s��
	MACHINE_TYPE m_machineID;

	// �}�V���̑ϋv�l
	int m_hp;

	// �}�V���̍ő�ϋv�l
	int m_maxHp;

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

	// �I�����[�h���̃t���O
	bool m_selectModeFlag;

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

	// �I�u�W�F�N�g�̃��C���ʒu(�ǂ̉~�`����ɂ��邩)
	int m_line;

	// LvUp�p�̑I���{�b�N�X
	std::unique_ptr<SelectionBox> m_selectLvUpBox;

	// �C���p�̑I���{�b�N�X
	std::unique_ptr<SelectionBox> m_repairBox;

};