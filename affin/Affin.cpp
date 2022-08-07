#include "Affin.h"

//�X�P�[�����O�ݒ�
void ConversionScale(float& x, float& y, float& z, WorldTransform worldTransform) {
	// X,Y,Z�����̃X�P�[�����O��ݒ�
	worldTransform.scale_ = {x, y, z};

	//�X�P�[�����O�s���錾
	Matrix4 matScale;

	//�X�P�[�����O�{�����s��ɐݒ�
	matScale = {
	  worldTransform.scale_.x,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  worldTransform.scale_.y,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  worldTransform.scale_.z,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f};

	// matScale���|���Z���đ��
	worldTransform.matWorld_ *= matScale;
}

//��]�p�ݒ�
void ConversionRot(float x, float y, float z, WorldTransform worldTransform) {
	// X,Y,Z������̉�]�p��ݒ�
	Vector3 rotation = {x, y, z};

	//�����p��]�s���錾
	Matrix4 matRot;
	//�e����]�s���錾
	Matrix4 matRotX, matRotY, matRotZ;

	// Z����]�s���錾
	matRotZ = {
	  cos(rotation.z),
	  sin(rotation.z),
	  0.0f,
	  0.0f,
	  -sin(rotation.z),
	  cos(rotation.z),
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
	  cos(rotation.x),
	  sin(rotation.x),
	  0.0f,
	  0.0f,
	  -sin(rotation.x),
	  cos(rotation.x),
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f};

	// Y����]�s���錾
	matRotY = {
	  cos(rotation.y),
	  0.0f,
	  -sin(rotation.y),
	  0.0f,
	  0.0f,
	  1.0f,
	  0.0f,
	  0.0f,
	  sin(rotation.y),
	  0.0f,
	  cos(rotation.y),
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
	worldTransform.matWorld_ *= matRot;
}

//���s�ړ��ݒ�
void ConversionTrans(float x, float y, float z, WorldTransform worldTransform) {
	// X,Y,Z������̕��s�ړ���ݒ�
	Vector3 translation = {x, y, z};

	//���s�ړ��s���錾
	Matrix4 matTrans;

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
	  translation.x,
	  translation.y,
	  translation.z,
	  1.0f};

	// matTrans���|���Z���đ��
	worldTransform.matWorld_ *= matTrans;
}
