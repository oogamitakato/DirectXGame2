#include "Enemy.h"
#include <cassert>

//������(�R���X�g���N�^)
Enemy::Enemy(Model* model) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = TextureManager::Load("3Denemy.png");
	//���[���h�ϊ��f�[�^�̏�����
	worldTransform_.Initialize();

	//�������W
	worldTransform_.translation_ = {0.0f, 5.0f, 50.0f};
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};

	//�ڋ߃t�F�[�Y������
	InitApproach();
}

//�G�̒e����
void Enemy::Fire() {
	//�e�̑��x
	const float kBulletSpeed = 0.5f;
	//���L�����̃��[���h���W���擾
	Vector3 velocity = player_->GetWorldPosition();
	//�G�L�����̃��[���h���W���擾
	Vector3 enemyPos = GetWorldPosition();
	//�G�L���������L�����̍����x�N�g�������߂�
	velocity -= enemyPos;
	//�׃N�g���̐��K��
	velocity /= sqrt(pow(velocity.x, 2.0f) + pow(velocity.y, 2.0f) + pow(velocity.z, 2.0f));
	//�x�N�g���̒������A�����ɍ��킹��
	velocity *= kBulletSpeed;

	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

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


	worldTransform_.matWorld_.m[0][0] = worldTransform_.scale_.x;
	worldTransform_.matWorld_.m[1][1] = worldTransform_.scale_.y;
	worldTransform_.matWorld_.m[2][2] = worldTransform_.scale_.z;

	worldTransform_.matWorld_.m[3][0] = worldTransform_.translation_.x;
	worldTransform_.matWorld_.m[3][1] = worldTransform_.translation_.y;
	worldTransform_.matWorld_.m[3][2] = worldTransform_.translation_.z;

	//�s��X�V
	worldTransform_.TransferMatrix();

	//�e�X�V
	if (isDead_ == false) {
		for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
			bullet->Update();
		}
	}
}

//�`��
void Enemy::Draw(const ViewProjection& viewProjection) {
	//���f���̕`��
	if (isDead_ == false) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_);

		for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
			bullet->Draw(viewProjection);
		}
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
	if (worldTransform_.translation_.z < 20.0f) {
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
	if (worldTransform_.translation_.x > 30) {
		leaveVelocity_ = -leaveVelocity_;
	} else if (worldTransform_.translation_.x < -30) {
		leaveVelocity_ = -leaveVelocity_;
	}
	//�K��̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z > 60.0f) {
		phase_ = Phase::Approach;
	}

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

//���[���h���W���擾
Vector3 Enemy::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

//�Փ˂����o������Ăяo�����֐�
void Enemy::OnCollision() { 

	hp--;
	if (hp <= 0) {
		isDead_ = true;	
	}
}