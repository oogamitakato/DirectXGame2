#include "Field.h"
#include "Affin.h"
#include <memory>
#include <list>
#include <cassert>

Field::Field(Model* model, const Vector3 position) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("mario.jpg");

	for (int i = 0; i < 20; i++) {
		//���[���h�ϊ��f�[�^�̏�����
		worldTransform_[i].Initialize();

		//�����Ŏ󂯎�����������W���Z�b�g
		worldTransform_[i].translation_ = {position.x, position.y, position.z};

		//ConversionScale(scale.x, scale.y, scale.z, worldTransform_[i]);
		worldTransform_[i].matWorld_.m[0][0] = scale.x;
		worldTransform_[i].matWorld_.m[1][1] = scale.y;
		worldTransform_[i].matWorld_.m[2][2] = scale.z;

		worldTransform_[i].matWorld_.m[3][0] = -50 + (i / 2 )* 10;
		worldTransform_[i].matWorld_.m[3][1] = 20 * pow(-1,i);

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