#pragma once
#include "NecromaLib/GameData/GameObject2D.h"
#include "NecromaLib/GameData/Camera.h"

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
	void DisplayObject(DirectX::Model* model);

	void ResetMoveTime() { m_moveTime = 0; }

private:

	float m_moveTime;

	DirectX::SimpleMath::Vector3 m_modelPos;

	std::unique_ptr<Camera> m_camera;

	// �e�L�X�g�{�b�N�X�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

};