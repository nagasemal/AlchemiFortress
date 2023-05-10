//
// Camera.h
//
#pragma once

#include <SimpleMath.h>

/// <summary>
/// �f�o�b�O�p�J�����N���X
/// </summary>
class Camera
{
public:
	static const float DEFAULT_CAMERA_DISTANCE;	// �J�����̋���

private:

	// �r���[�s��
	DirectX::SimpleMath::Matrix mView;

	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix mProjection;

	// �J�������W
	DirectX::SimpleMath::Vector3 mEye;

	// �����_
	DirectX::SimpleMath::Vector3 mTarget;

	// ������x�N�g��
	DirectX::SimpleMath::Vector3 mUp;

public:
	// �R���X�g���N�^
	Camera();

	// �f�X�g���N�^
	virtual ~Camera();

	// �r���[�s��̃A�N�Z�T
	void SetViewMatrix(DirectX::SimpleMath::Matrix view) { mView = view; }

	DirectX::SimpleMath::Matrix GetViewMatrix()
	{
		CalculateViewMatrix();
		return mView;
	}

	// �v���W�F�N�V�����s��̃A�N�Z�T
	void SetProjectionMatrix(DirectX::SimpleMath::Matrix projection) { mProjection = projection; }

	DirectX::SimpleMath::Matrix GetProjectionMatrix()
	{
		CalculateProjectionMatrix();
		return mProjection;
	}

	// �J�������W�̃A�N�Z�T
	void SetEyePosition(DirectX::SimpleMath::Vector3 eye) { mEye = eye; }
	DirectX::SimpleMath::Vector3 GetEyePosition() const { return mEye; }

	// �����_���W�̃A�N�Z�T
	void SetTargetPosition(DirectX::SimpleMath::Vector3 target) { mTarget = target; }
	DirectX::SimpleMath::Vector3 GetTargetPosition() const { return mTarget; }

	// ������x�N�g���̃A�N�Z�T
	void SetUpVector(DirectX::SimpleMath::Vector3 up) { mUp = up; }
	DirectX::SimpleMath::Vector3 GetUpVector() const { return mUp; }

protected:

	// �r���[�s��̎Z�o
	virtual void CalculateViewMatrix();

	// �v���W�F�N�V�����s��̎Z�o
	virtual void CalculateProjectionMatrix();
};
