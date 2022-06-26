#pragma once
#include "Affin.h"
#include "DebugText.h"
#include "Model.h"
#include "WorldTransform.h"

//���L�����̒e
class PlayerBullet {
  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���x
	Vector3 velocity_;

	//�e�̎���
	static const int32_t kLifeTimer = 60 * 5;
	//�f�X�^�C�}�[
	int32_t debugTimer_ = kLifeTimer;
	//�f�X�t���O
	bool isDead_ = false;

  public:
	//������
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	//�X�V
	void Update();

	//�`��
	void Draw(const ViewProjection& viewProjection);

	//�Q�b�^�[
	bool IsDead() const { return isDead_; }
};
