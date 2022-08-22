#include "RailCamera.h"

void RailCamera::Initialize(Vector3 translation, Vector3 rotation) {
	debugText_ = DebugText::GetInstance();

	//���[���h�g�����X�t�H�[���̏����ݒ�
	worldTransform_.Initialize();
	worldTransform_.translation_ = translation;
	worldTransform_.rotation_ = rotation;

	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.Initialize();
}

void RailCamera::Update() {
	worldTransform_.translation_ -= {0.0f, 0.0f, 0.1f};
	worldTransform_.rotation_ += {0.0f, 0.0f, 0.1f};
	worldTransform_.TransferMatrix();
	viewProjection_.eye = worldTransform_.translation_;

	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = worldTransform_.rotation_;
	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection_.target.x = viewProjection_.eye.x + forward.x;
	viewProjection_.target.y = viewProjection_.eye.y + forward.y;
	viewProjection_.target.z = viewProjection_.eye.z + forward.z;
	//���[���h������x�N�g��
	Vector3 up(0, 1, 0);
	//���[���J�����̉�]�𔽉f(���[���J�����̏�����x�N�g��)
	viewProjection_.up = worldTransform_.rotation_;
	//�r���[�v���W�F�N�V�������X�V
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	debugText_->SetPos(50, 120);
	debugText_->Printf(
	  "eye : {%f, %f, %f}", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
}
