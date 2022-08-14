#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

class Player {
  private:
	/*�����o�ϐ�*/
	const float PI = 3.1415;
	float speed = 0.5f;
	Vector3 move;

	Matrix4 matTrans;

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Input* input_;
	DebugText* debugText_;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	Matrix4 matRot;

  public:
	/*�����o�֐�*/

	//������(�R���X�g���N�^)
	Player(Model* model, uint32_t textureHandle);

	//�X�V
	void Update();

	//�`��
	void Draw(ViewProjection viewProjection);

	//�U��
	void Attack();

	//��]
	void Rotate();
	//�e�̔��˕����̉�]
	Vector3 AttackRotate(Vector3 rotation, WorldTransform worldTransform);

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����֐�
	void OnCollision();

	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
};