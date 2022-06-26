#pragma once
#include "Affin.h"
#include "DebugText.h"
#include "Model.h"
#include "WorldTransform.h"

class Enemy {
  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���x
	Vector3 velocity_ = {0.0f,0.0f,-0.1f};

  public:
	 //������
	Enemy(Model* model);

	//�X�V
	void Update();

	//�`��
	void Draw(const ViewProjection& viewProjection);

};