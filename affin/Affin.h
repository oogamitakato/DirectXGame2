#pragma once
#include "Model.h"
#include "WorldTransform.h"

//�X�P�[�����O�ݒ�
void ConversionScale(float& x, float& y, float& z, WorldTransform& worldTransform);

//��]�p�ݒ�
void ConversionRot(float &x, float &y, float &z, WorldTransform &worldTransform);

//���s�ړ��ݒ�
void ConversionTrans(float x, float y, float z,WorldTransform &worldTransform);