#pragma once
#include "DebugText.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class RailCamera {
  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	DebugText* debugText_;

  public:

	//������
	void Initialize(Vector3 translation, Vector3 rotation);

	//�X�V
	void Update();
};
