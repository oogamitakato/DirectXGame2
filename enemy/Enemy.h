#pragma once
#include "Affin.h"
#include "DebugText.h"
#include "Model.h"
#include "WorldTransform.h"

class Enemy {
  private:
	//�s���t�F�[�Y
	enum class Phase {
		Approach, //�ڋ߂���
		Leave,    //���E����
	};

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���x
	Vector3 approachVelocity_ = {0.0f, 0.0f, -0.1f};
	Vector3 leaveVelocity_ = {0.1f, 0.1f, 0.0f};

  public:
	//������
	Enemy(Model* model);

	//�X�V
	void Update();

	//�`��
	void Draw(const ViewProjection& viewProjection);

	/*�t�F�[�Y���Ƃ̊֐�*/
	//�ڋ߃t�F�[�Y
	void Approach();
	//���E�t�F�[�Y
	void Leave();
};