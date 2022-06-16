#include "PlayerBullet.h"
#include <cassert>

//������
void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("tex1.png");


}

//�X�V
void PlayerBullet::Update();

//�`��
void PlayerBullet::Draw(const ViewProjection& viewProjection);