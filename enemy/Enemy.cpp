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
	worldTransform_.translation_ = {-10.0f, -10.0f, 20.0f};

	//�ڋ߃t�F�[�Y������
	InitApproach();
}

//�G�̒e����
void Enemy::Fire() {
	//�e�̑��x
	const float kBulletSpeed = -0.5f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();

	newBullet->Initialize(
	  model_,
	  {
	    worldTransform_.translation_.x,
	    worldTransform_.translation_.y,
	    worldTransform_.translation_.z,
	  },
	  velocity);

	//�e��o�^����
	bullets_.push_back(std::move(newBullet));
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

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });


	/*ConversionTrans(
	  approachVelocity_.x, approachVelocity_.y, approachVelocity_.z, worldTransform_);*/
	worldTransform_.matWorld_.m[3][0] = worldTransform_.translation_.x;
	worldTransform_.matWorld_.m[3][1] = worldTransform_.translation_.y;
	worldTransform_.matWorld_.m[3][2] = worldTransform_.translation_.z;

	//�s��X�V
	worldTransform_.TransferMatrix();

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

//�`��
void Enemy::Draw(const ViewProjection& viewProjection) {
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

/*�t�F�[�Y���Ƃ̊֐�*/
//�ڋ߃t�F�[�Y������
void Enemy::InitApproach() {
	//���˃^�C�}�[��������
	//fireTimer = kFireInterval;
}

//�ڋ߃t�F�[�Y
void Enemy::Approach() {
	//���W���ړ�������
	worldTransform_.translation_ += approachVelocity_;
	//�K��̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

	////���˃^�C�}�[�J�E���g�_�E��
	//fireTimer--;

	////�w�莞�ԂɒB����
	//if (fireTimer <= 0) {
	//	//�e����
	//	Fire();
	//	//���˃^�C�}�[��������
	//	fireTimer = kFireInterval;
	//}
}

//�ڋ߃t�F�[�Y������
void Enemy::InitLeave() {
	//���˃^�C�}�[��������
	fireTimer = kFireInterval;
}

//���E�t�F�[�Y
void Enemy::Leave() {
	//�ړ�
	worldTransform_.translation_ += leaveVelocity_;

	//���˃^�C�}�[�J�E���g�_�E��
	fireTimer--;

	//�w�莞�ԂɒB����
	if (fireTimer <= 0) {
		//�e����
		Fire();
		//���˃^�C�}�[��������
		fireTimer = kFireInterval;
	}
}