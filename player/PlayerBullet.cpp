#include "PlayerBullet.h"
#include "Affin.h"
#include "Player.h"
#include <cassert>

//������
void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("tex1.png");

	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��f�[�^�̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = {position.x, position.y, position.z};
}

//�X�V
void PlayerBullet::Update() {

	matTrans = {
	  1.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f,
	  0.0f,
	  worldTransform_.translation_.x,
	  worldTransform_.translation_.y,
	  worldTransform_.translation_.z,
	  1.0f};

	// matTrans���|���Z���đ��
	worldTransform_.matWorld_ *= matTrans;

	//�s��X�V
	worldTransform_.TransferMatrix();

	//�L�����N�^�[�̍��W����ʕ\�����鏈��
	/*debugText_->SetPos(50, 120);
	debugText_->Printf(
	  "Player : {%f,%f,%f}", worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
	  worldTransform_.matWorld_.m[3][2]);*/
}

//�`��
void PlayerBullet::Draw(const ViewProjection& viewProjection) {

	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}