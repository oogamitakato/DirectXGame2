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
	worldTransform_.translation_ = {0.0f,2.0f,50.0f};

}

//�X�V
void Enemy::Update() { 
	//���W���ړ�������
	worldTransform_.translation_ += velocity_;

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