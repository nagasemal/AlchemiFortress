#pragma once
#include "NecromaLib/GameData/GameObject2D.h"

class MachineSelect : public GameObjct2D
{
public:
	MachineSelect();
	~MachineSelect();

	// ������
	void Initialize()			override;

	// �X�V
	void Update()				override;

	// �`��
	void Draw()					override;

	// �I������
	void Finalize()				override;

	// ���f����`�悷�邽�߂̊֐�
	void DisplayObject(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,DirectX::Model* model);

private:

};