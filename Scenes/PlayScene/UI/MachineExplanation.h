#pragma once
#include "NecromaLib/GameData/GameObject2D.h"
#include "NecromaLib/GameData/Camera.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"

class MachineExplanation : public GameObjct2D
{
public:
	MachineExplanation();
	~MachineExplanation();

	// ������
	void Initialize()			override;

	// �X�V
	void Update()				override;

	// �`��
	void Draw()					override;

	// �I������
	void Finalize()				override;

	// ���f����`�悷�邽�߂̊֐�
	void DisplayObject(DirectX::Model* model,DirectX::Model* secondModel = nullptr,AlchemicalMachineObject* object = nullptr);

	void ResetMoveTime() { m_moveTime = 0; m_hitFlag = false;}

public:

	// �}�E�X�������E�B���h�E�ɐG��Ă���
	bool OnMouse();

private:

	float m_moveTime;

	DirectX::SimpleMath::Vector3 m_modelPos;

	std::unique_ptr<Camera> m_camera;

	// �e�L�X�g�{�b�N�X�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	bool m_hitFlag;

};