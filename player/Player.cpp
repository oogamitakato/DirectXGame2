#include "Player.h"
#include "Affin.h"
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
	worldTransform_.matWorld_.m[3][1] = 5;
}

//�X�V
void Player::Update() 
{ 
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
	if (input_->PushKey(DIK_UP)) 
	{
		move.y += speed;
	}

	if (input_->PushKey(DIK_DOWN)) 
	{
		move.y -= speed;
	}

	if (input_->PushKey(DIK_RIGHT)) 
	{
		move.x += speed;
	}

	if (input_->PushKey(DIK_LEFT)) 
	{
		move.x -= speed;
	}

	//���W�ړ�	

	Matrix4 matTrans(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		move.x, move.y, move.z, 1);
	
	worldTransform_.matWorld_ *= matTrans;

	//�L�����N�^�[�U������
	Attack();

	//�e�X�V
	if (bullet_) {
		if (input_->PushKey(DIK_SPACE)) {
			bullet_->Update();
		}
	}

	//�s��X�V
	worldTransform_.TransferMatrix();

	//�L�����N�^�[�̍��W����ʕ\�����鏈��
	debugText_->SetPos(50, 100);
	debugText_->Printf(
	  "Player : {%f,%f,%f}", 
		worldTransform_.matWorld_.m[3][0],
		worldTransform_.matWorld_.m[3][1],
		worldTransform_.matWorld_.m[3][2]);
}

//�`��
void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	if (bullet_) {
		bullet_->Draw(viewProjection);
	}
}

//�U��
void Player::Attack() {

	if (input_->PushKey(DIK_SPACE)) {
		//�e�𐶐����A������
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(
		  model_, {
		            worldTransform_.matWorld_.m[3][0],
		            worldTransform_.matWorld_.m[3][1],
		            worldTransform_.matWorld_.m[3][2],
		          });

		//�e��o�^����
		bullet_ = newBullet;
	}
}

//void Player::ConversionScale(float& x, float& y, float& z) {
//
//	// X,Y,Z�����̃X�P�[�����O��ݒ�
//	worldTransform_.scale_ = {x, y, z};
//	//�X�P�[�����O�s���錾
//	Matrix4 matScale;
//
//	//�X�P�[�����O�{�����s��ɐݒ�
//	matScale = {
//	  worldTransform_.scale_.x,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  worldTransform_.scale_.y,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  worldTransform_.scale_.z,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  1.0f};
//
//	// matScale���|���Z���đ��
//	worldTransform_.matWorld_ *= matScale;
//}
//
//void Player::ConversionRot(float& x, float& y, float& z) {
//
//	// X,Y,Z������̉�]�p��ݒ�
//	worldTransform_.rotation_ = {x, y, z};
//	//�����p��]�s���錾
//	Matrix4 matRot;
//	//�e����]�s���錾
//	Matrix4 matRotX, matRotY, matRotZ;
//
//	// Z����]�s���錾
//	matRotZ = {
//	  cos(worldTransform_.rotation_.z),
//	  sin(worldTransform_.rotation_.z),
//	  0.0f,
//	  0.0f,
//	  -sin(worldTransform_.rotation_.z),
//	  cos(worldTransform_.rotation_.z),
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  1.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  1.0f};
//
//	// X����]�s���錾
//	matRotX = {
//	  1.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  cos(worldTransform_.rotation_.x),
//	  sin(worldTransform_.rotation_.x),
//	  0.0f,
//	  0.0f,
//	  -sin(worldTransform_.rotation_.x),
//	  cos(worldTransform_.rotation_.x),
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  1.0f};
//
//	// Y����]�s���錾
//	matRotY = {
//	  cos(worldTransform_.rotation_.y),
//	  0.0f,
//	  -sin(worldTransform_.rotation_.y),
//	  0.0f,
//	  0.0f,
//	  1.0f,
//	  0.0f,
//	  0.0f,
//	  sin(worldTransform_.rotation_.y),
//	  0.0f,
//	  cos(worldTransform_.rotation_.y),
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  1.0f};
//
//	//�e���̉�]�s�������
//	matRotZ *= matRotX;
//	matRotZ *= matRotY;
//	matRot = matRotZ;
//
//	// matRot���|���Z���đ��
//	worldTransform_.matWorld_ *= matRot;
//}
//
//void Player::ConversionTrans(float& x, float& y, float& z) {
//
//	// X,Y,Z������̕��s�ړ���ݒ�
//	worldTransform_.translation_ = {x, y, z};
//	//���s�ړ��s���錾
//	Matrix4 matTrans = MathUtility::Matrix4Identity();
//
//	//�ړ��ʂ��s��ɐݒ肷��
//	matTrans = {
//	  1.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  1.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  1.0f,
//	  0.0f,
//	  worldTransform_.translation_.x,
//	  worldTransform_.translation_.y,
//	  worldTransform_.translation_.z,
//	  1.0f};
//
//	// matTrans���|���Z���đ��
//	worldTransform_.matWorld_ *= matTrans;
//}