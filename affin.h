#pragma once
#include "Model.h"
#include "WorldTransform.h"

struct Affin {
  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;

  public:
	
	//�X�P�[�����O�ݒ�
	void ConversionScale(float& x, float& y, float& z);

	//��]�p�ݒ�
	void ConversionRot(float& x, float& y, float& z);

	//���s�ړ��ݒ�
	void ConversionTrans(float& x, float& y, float& z);

};
