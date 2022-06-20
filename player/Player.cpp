#include "Player.h"
#include "Input.h"
#include "DebugText.h"
#include <math.h>
#include <cassert>

//������(�R���X�g���N�^)
Player::Player(Model* model, uint32_t textureHandle) 
{
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
}

//�X�V
void Player::Update() { 
	Move();
	Rotate();

	//�L�����N�^�[�U������
	Attack();

	//�e�X�V
	if (bullet_) {
		bullet_->Update();
	}
}

//�ړ�
void Player::Move() {
	Vector3 move = {0, 0, 0};

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
	Matrix4 matTrans(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, move.x, move.y, move.z, 1);

	worldTransform_.matWorld_ *= matTrans;

	//�s��X�V
	worldTransform_.TransferMatrix();

	//�L�����N�^�[�̍��W����ʕ\�����鏈��
	debugText_->SetPos(50, 100);
	debugText_->Printf(
	  "Player : {%f,%f,%f}", worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
	  worldTransform_.matWorld_.m[3][2]);
}

//��]
void Player::Rotate() { 
	
	Vector3 rotate = {0.0f, 0.0f, 0.0f};

	if (input_->PushKey(DIK_Q)) {
		rotate.y += 0.05;
	}

	if (input_->PushKey(DIK_E)) {
		rotate.y -= 0.05;
	}

	//���W�ړ�
	Matrix4 matRotY(
	  cos(rotate.y), 0.0f, sin(rotate.y), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -sin(rotate.y), 0.0f,
	  cos(rotate.y), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	worldTransform_.matWorld_ *= matRotY;

	//�s��X�V
	worldTransform_.TransferMatrix();
}

//�U��
void Player::Attack() { 
	
	if (input_->PushKey(DIK_SPACE)) {
		//�e�𐶐����A������
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(
		  model_, {worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
		           worldTransform_.matWorld_.m[3][2]});

		//�e��o�^����
		bullet_ = newBullet;
	}
};

//�`��
void Player::Draw(ViewProjection viewProjection){

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�`��
	if (bullet_) {
		bullet_->Draw(viewProjection);
	}
}

