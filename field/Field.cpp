#include "Field.h"
#include "Affin.h"
#include <memory>
#include <list>
#include <cassert>

Field::Field(Model* model) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("mario.jpg");

	for (int i = 0; i < 20; i++) {
		//���[���h�ϊ��f�[�^�̏�����
		worldTransform_[i].Initialize();

		//�����Ŏ󂯎�����������W���Z�b�g
		ConversionTrans(-10 + i, 4 * pow(-1, i), 0, worldTransform_[i]);
		ConversionScale(scale.x, scale.y, scale.z, worldTransform_[i]);

		worldTransform_[i].TransferMatrix();
	}
}

void Field::Update() {

}

void Field::Draw(const ViewProjection& viewProjection) {
	//���f���̕`��
	for (int i = 0; i < 20; i++) {
		model_->Draw(worldTransform_[i], viewProjection, textureHandle_);
	}
}