#pragma once
#include "NecromaLib/GameData/GameObject3D.h"

#include "Scenes/PlayScene/Mouse/MousePointer.h"
#include "Scenes/PlayScene/Enemy/EnemyObject.h"
#include "Scenes/PlayScene/AlchemicalMachine/Bullet/Bullet.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"
#include <list>
#include <vector>

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
		EARTH	= 4
	};


	// �I�u�W�F�N�g�̌��ݏ󋵂̎擾
	enum STATE_NAME : int
	{
		BRING = 0,		// �^�΂�Ă���
		IDEL  = 1,		// �~�܂��Ă���
		Play  = 2		// �쓮���[�h

	};

	AlchemicalMachineObject();
	~AlchemicalMachineObject();

	// �S�Ẵ}�V���̏����|�C���^�Ƃ��Ď󂯎��(�}�V�����m�ō�p������̂����邽��)
	virtual void AllAlchemicalMachine(AlchemicalMachineObject* object,int maxNum) = 0;

	// �S�ẴG�l�~�[�̏����󂯎�� �o���b�g�ˏo��Request�𑗂�
	virtual bool BulletRequest(std::list<EnemyObject>* enemys) = 0;

	// �A���P�~�J���}�V���̒e���𓾂�
	virtual Bullet GetBulletData() = 0;

	// �I�����̓��ʂȃA�b�v�f�[�g����
	virtual void SelectUpdate() = 0;

	// �I�����A�}�E�X�I���\��UI�̕\���Ɏg�p
	virtual void RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) = 0;

	// ���̃I�u�W�F�N�g���m�̓����蔻��
	void HitToObject(MousePointer* pMP);

	// ���ʔ͈͓���3D�I�u�W�F�N�g��������
	bool OnCollisionEnter_MagicCircle(GameObjct3D* object);
	
	// ���f���`��n
	void ModelRender(DirectX::Model* model);

	void SummonAM(DirectX::SimpleMath::Vector3 pos);


// �A�N�Z�T
public:

	bool GetActiv()									{ return m_activ; }
	bool GetHitMouse()								{ return m_hitMouseFlag;}
	std::string GetObjectName()						{ return m_objectName; }
	MACHINE_TYPE GetModelID()						{ return m_machineID;}
	float GetMachineEffectNum()						{ return m_machineEffectNum; }
	float GetSpan()									{ return m_span;}
	int	  GetLv()									{ return m_lv; }
	Circle GetMagicCircle()							{ return m_magicCircle; }

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
	bool m_activ;

	// �}�E�X���������Ă��邩�ۂ�
	bool m_hitMouseFlag;

	// ���ʔ͈�
	Circle m_magicCircle;

	std::string m_objectName;

	// �I�u�W�F�N�g�̐F
	DirectX::SimpleMath::Color m_color;

};