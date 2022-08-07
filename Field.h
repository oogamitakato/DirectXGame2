#pragma once
#include "Affin.h"
#include "DebugText.h"
#include "Model.h"
#include "WorldTransform.h"

class Field 
{
  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_[20];
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Vector3 scale = {5.0f, 5.0f, 5.0f};

  public:
	//������
	Field(Model* model, const Vector3 position);

	//�X�V
	void Update();

	//�`��
	void Draw(const ViewProjection& viewProjection);
};
