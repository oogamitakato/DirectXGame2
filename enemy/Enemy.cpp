#include "Enemy.h"
#include <cassert>

//������(�R���X�g���N�^)
Enemy::Enemy(Model* model) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = TextureManager::Load("uvChecker.png");
	//���[���h�ϊ��f�[�^�̏�����
	worldTransform_.Initialize();

	//�������W
	worldTransform_.translation_ = {0.0f, 2.0f, 20.0f};
}

//�X�V
void Enemy::Update() {

	switch (phase_) {
	case Phase::Approach:
	default:
		Approach();
		break;

	case Phase::Leave:
		Leave();
		break;
	}


	worldTransform_.matWorld_.m[3][0] = worldTransform_.translation_.x;
	worldTransform_.matWorld_.m[3][1] = worldTransform_.translation_.y;
	worldTransform_.matWorld_.m[3][2] = worldTransform_.translation_.z;

	//�s��X�V
	worldTransform_.TransferMatrix();
}

//�`��
void Enemy::Draw(const ViewProjection& viewProjection) {
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

/*�t�F�[�Y���Ƃ̊֐�*/
//�ڋ߃t�F�[�Y
void Enemy::Approach() {
	//���W���ړ�������
	worldTransform_.translation_ += approachVelocity_;
	//�K��̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}
//���E�t�F�[�Y
void Enemy::Leave() {
	//�ړ�
	worldTransform_.translation_ += leaveVelocity_;
}