#include "Affin.h"

Affin::Affin(Model* model_) {

}

//�X�P�[�����O�ݒ�
void Affin::ConversionScale(float& x, float& y, float& z) {
	// X,Y,Z�����̃X�P�[�����O��ݒ�
	worldTransform_.scale_ = {x, y, z};
	//�X�P�[�����O�s���錾
	Matrix4 matScale;

	//�X�P�[�����O�{�����s��ɐݒ�
	matScale = {
	  worldTransform_.scale_.x,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  worldTransform_.scale_.y,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  worldTransform_.scale_.z,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f};

	// matScale���|���Z���đ��
	worldTransform_.matWorld_ *= matScale;
}

//��]�p�ݒ�
void Affin::ConversionRot(float& x, float& y, float& z) {
	// X,Y,Z������̉�]�p��ݒ�
	worldTransform_.rotation_ = {x, y, z};
	//�����p��]�s���錾
	Matrix4 matRot;
	//�e����]�s���錾
	Matrix4 matRotX, matRotY, matRotZ;

	// Z����]�s���錾
	matRotZ = {
	  cos(worldTransform_.rotation_.z),
	  sin(worldTransform_.rotation_.z),
	  0.0f,
	  0.0f,
	  -sin(worldTransform_.rotation_.z),
	  cos(worldTransform_.rotation_.z),
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f};

	// X����]�s���錾
	matRotX = {
	  1.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  cos(worldTransform_.rotation_.x),
	  sin(worldTransform_.rotation_.x),
	  0.0f,
	  0.0f,
	  -sin(worldTransform_.rotation_.x),
	  cos(worldTransform_.rotation_.x),
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f};

	// Y����]�s���錾
	matRotY = {
	  cos(worldTransform_.rotation_.y),
	  0.0f,
	  -sin(worldTransform_.rotation_.y),
	  0.0f,
	  0.0f,
	  1.0f,
	  0.0f,
	  0.0f,
	  sin(worldTransform_.rotation_.y),
	  0.0f,
	  cos(worldTransform_.rotation_.y),
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f};

	//�e���̉�]�s�������
	matRotZ *= matRotX;
	matRotZ *= matRotY;
	matRot = matRotZ;

	// matRot���|���Z���đ��
	worldTransform_.matWorld_ *= matRot;
}

//���s�ړ��ݒ�
void Affin::ConversionTrans(float& x, float& y, float& z) {
	// X,Y,Z������̕��s�ړ���ݒ�
	worldTransform_.translation_ = {x, y, z};
	//���s�ړ��s���錾
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//�ړ��ʂ��s��ɐݒ肷��
	matTrans = {
	  1.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f,
	  0.0f,
	  worldTransform_.translation_.x,
	  worldTransform_.translation_.y,
	  worldTransform_.translation_.z,
	  1.0f};

	// matTrans���|���Z���đ��
	worldTransform_.matWorld_ *= matTrans;
}
