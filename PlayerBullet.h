#pragma once
#include "Model.h"
#include "WorldTransform.h"

//���L�����̒e
class PlayerBullet {
  public:
	  //������
	void Initialize(Model* model, const Vector3& position);

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

};