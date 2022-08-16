#include "skydome.h"
#include "DebugText.h"
#include "Input.h"
#include <cassert>
#include <math.h>

void Skydome::Initialize(Model* model, uint32_t textureHandle) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;

	//���[���h�ϊ��f�[�^�̏�����
	worldTransform_.Initialize();

	worldTransform_.matWorld_.m[0][0] = 1000.0f;
	worldTransform_.matWorld_.m[1][1] = 1000.0f;
	worldTransform_.matWorld_.m[2][2] = 1000.0f;

		//�s��X�V
	worldTransform_.TransferMatrix();
}

void Skydome::Update() {

}

void Skydome::Draw(ViewProjection viewProjection) {
	//3D���f���`��
	model_->Draw(worldTransform_, viewProjection);
}