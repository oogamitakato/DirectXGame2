#include "Player.h"
#include "Affin.h"
#include "DebugText.h"
#include "Input.h"
#include <cassert>
#include <math.h>

//������(�R���X�g���N�^)
Player::Player(Model* model, uint32_t textureHandle) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��f�[�^�̏�����
	worldTransform_.Initialize();
	worldTransform_.matWorld_.m[3][0] = 5;
}

//�X�V
void Player::Update() {

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { 
		return bullet->IsDead();
		});

	Rotate();

	move = {0, 0, 0};

	//�ړ����E���W
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 20.0f;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.matWorld_.m[3][0] = max(worldTransform_.matWorld_.m[3][0], -kMoveLimitX);
	worldTransform_.matWorld_.m[3][0] = min(worldTransform_.matWorld_.m[3][0], +kMoveLimitX);
	worldTransform_.matWorld_.m[3][1] = max(worldTransform_.matWorld_.m[3][1], -kMoveLimitY);
	worldTransform_.matWorld_.m[3][1] = min(worldTransform_.matWorld_.m[3][1], +kMoveLimitY);

	//�L�[�{�[�h���͂ɂ��ړ�����
	if (input_->PushKey(DIK_UP)) {
		move.y += speed;
	}

	if (input_->PushKey(DIK_DOWN)) {
		move.y -= speed;
	}

	if (input_->PushKey(DIK_RIGHT)) {
		move.x += speed;
	}

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= speed;
	}

	//���W�ړ�
	matTrans = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, move.x, move.y, move.z, 1};

	worldTransform_.matWorld_ *= matTrans;

	//�L�����N�^�[�U������
	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//�s��X�V
	worldTransform_.TransferMatrix();

	//�L�����N�^�[�̍��W����ʕ\�����鏈��
	debugText_->SetPos(50, 100);
	debugText_->Printf(
	  "Player : {%f,%f,%f}", worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
	  worldTransform_.matWorld_.m[3][2]);
}

//�`��
void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

//�U��
void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {
		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = AttackRotate(velocity, worldTransform_);

		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(
		  model_,
		  {
		    worldTransform_.matWorld_.m[3][0],
		    worldTransform_.matWorld_.m[3][1],
		    worldTransform_.matWorld_.m[3][2],
		  },
		  velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

//��]
void Player::Rotate() {

	//��]�s��
	matRot = {
	  cos(worldTransform_.rotation_.y),
	  0.0f,
	  -sin(worldTransform_.rotation_.y),
	  0.0f,
	  0.0f,
	  1.0f,
	  0.0f,
	  0.0f,
	  sin(worldTransform_.rotation_.y),
	  0.0f,
	  cos(worldTransform_.rotation_.y),
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f};

	//���݂̕��s�ړ����W��ێ�
	Vector3 move2 = {
	  worldTransform_.matWorld_.m[3][0],
	  worldTransform_.matWorld_.m[3][1],
	  worldTransform_.matWorld_.m[3][2],
	};
	//���W���O�ɂ���
	worldTransform_.matWorld_.m[3][0] = 0;
	worldTransform_.matWorld_.m[3][1] = 0;
	worldTransform_.matWorld_.m[3][2] = 0;

	//�L�[�������ĉ�]
	if (input_->PushKey(DIK_Q)) {
		worldTransform_.rotation_.y = 0.05f;
	} else if (input_->PushKey(DIK_E)) {
		worldTransform_.rotation_.y = -0.05f;
	} else {
		worldTransform_.rotation_.y = 0.0f;
	}

	//�s����|���Z
	worldTransform_.matWorld_ *= matRot;
	//���W�����Ƃ̈ʒu�ɖ߂�
	worldTransform_.matWorld_.m[3][0] = move2.x;
	worldTransform_.matWorld_.m[3][1] = move2.y;
	worldTransform_.matWorld_.m[3][2] = move2.z;
}

//�e�̔��˕����̉�]
Vector3 Player::AttackRotate(Vector3 rotation, WorldTransform worldTransform) {

	rotation = {
	  rotation.x * worldTransform_.matWorld_.m[0][0] +
	    rotation.y * worldTransform_.matWorld_.m[1][0] +
	    rotation.z * worldTransform_.matWorld_.m[2][0],
	  rotation.x * worldTransform_.matWorld_.m[0][1] +
	    rotation.y * worldTransform_.matWorld_.m[1][1] +
	    rotation.z * worldTransform_.matWorld_.m[2][1],
	  rotation.x * worldTransform_.matWorld_.m[0][2] +
	    rotation.y * worldTransform_.matWorld_.m[1][2] +
	    rotation.z * worldTransform_.matWorld_.m[2][2],
	};

	return rotation;
}
