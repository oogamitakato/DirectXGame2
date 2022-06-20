#include "PlayerBullet.h"
#include "Player.h"
#include <cassert>

//������
void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("tex1.png");

	//���[���h�ϊ��f�[�^�̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W�����Z�b�g
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
}

//�X�V
void PlayerBullet::Update() { 


	//�s��X�V
	worldTransform_.TransferMatrix();
}

//�`��
void PlayerBullet::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}