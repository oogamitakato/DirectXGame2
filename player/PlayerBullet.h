#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Affin.h"

//���L�����̒e
class PlayerBullet {
  public:
	  //������
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	//�X�V
	void Update();

	//�`��
	void Draw(const ViewProjection& viewProjection);

  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���x
	Vector3 velocity_;
};
