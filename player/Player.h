#pragma once
#include "PlayerBullet.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"

class Player {
  private:
	/*�����o�ϐ�*/
	const float PI = 3.1415;
	float speed = 0.5f;
	Vector3 move;

	Matrix4 matTrans = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, move.x, move.y, move.z, 1};

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Input* input_;
	DebugText* debugText_;

	//�e
	PlayerBullet* bullet_ = nullptr;

  public:
	/*�����o�֐�*/

	//������(�R���X�g���N�^)
	Player(Model* model, uint32_t textureHandle);

	//�X�V
	void Update();

	//�ړ�
	void Move();

	//��]
	void Rotate();

	//�U��
	void Attack();

	//�`��
	void Draw(ViewProjection viewProjection);
};