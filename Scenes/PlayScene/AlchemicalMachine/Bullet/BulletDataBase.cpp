#include "pch.h"
#include "BulletDataBase.h"

#define MaxLv	5

// �o���b�g�̌��f�[�^�z��
BulletStatus dataBase[AlchemicalMachineObject::MACHINE_ELEMENT::SUM]
{
	// ������	�U�����@�������ԁZ�@�X�s�[�h�Z�@���ˊԊu���@����́�
	{20.0f,5.0f,3.0f,1.0f,8.0f},

	// �Α���	�U���Z�@�������ԁ��@�X�s�[�h�Z�@���ˊԊu�Z�@����́�
	{25.0f,3.5f,3.0f,0.8f,20.0f },

	// ������	�U�����@�������ԁ��@�X�s�[�h���@���ˊԊu���@����́�
	{20.0f,8.0f,2.5f,1.0f,8.0f},

	// ������	�U���~�@�������ԁZ�@�X�s�[�h�Z�@���ˊԊu���@����́Z
	{10.0f,5.0f,6.5f,0.4f,10.0f },

	// �y����	�U�����@�������ԁZ�@�X�s�[�h���@���ˊԊu�~�@����́Z
	{30.0f,4.0f,2.5f,1.2f,10.0f},
};

BulletStatus GetBaseBulletStatus(AlchemicalMachineObject::MACHINE_ELEMENT element)
{
	// �U����,��������,�X�s�[�h,���ˊԊu,�����
    return dataBase[element];
}

BulletStatus RecalculationStatus(AlchemicalMachineObject::MACHINE_ELEMENT element, int lv)
{
	//���ˊԊu�ȊO�@���X�e�[�^�X * lv / MaxLv + 2
	//���ˊԊu		(1 + ���X�e�[�^�X) * MaxLv / lv - 1


	BulletStatus status;

	// �U����
	status.damage = (dataBase[element].damage)	* lv / MaxLv;
	// ��������
	status.life = (dataBase[element].life)		* lv / MaxLv;
	// �X�s�[�h
	status.speed = (dataBase[element].speed)	* lv / MaxLv;
	// �����
	status.lossMp = (dataBase[element].lossMp)  * lv / MaxLv;
	// ���ˊԊu
	status.span = dataBase[element].span / lv ;

    return status;
}
