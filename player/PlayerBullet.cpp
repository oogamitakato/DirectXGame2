#include "PlayerBullet.h"
#include "Affin.h"
#include "Player.h"
#include <cassert>

//������
void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("tex1.png");

	//���[���h�ϊ��f�[�^�̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = {position.x, position.y, position.z};

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
}

//�X�V
void PlayerBullet::Update() {

	//���W���ړ�������
	worldTransform_.translation_ += velocity_;

	worldTransform_.matWorld_.m[3][0] = worldTransform_.translation_.x;
	worldTransform_.matWorld_.m[3][1] = worldTransform_.translation_.y;
	worldTransform_.matWorld_.m[3][2] = worldTransform_.translation_.z;

	//�s��X�V
	worldTransform_.TransferMatrix();
}

//�`��
void PlayerBullet::Draw(const ViewProjection& viewProjection) {

	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}