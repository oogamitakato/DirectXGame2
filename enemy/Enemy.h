#pragma once
#include "Affin.h"
#include "DebugText.h"
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include "Player.h"

//���@�N���X�̑O������
class Player;

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

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//���˃^�C�}�[
	int32_t fireTimer = 0;

  public:
	//������
	Enemy(Model* model);

	//�e����
	void Fire();

	//�X�V
	void Update();

	//�`��
	void Draw(const ViewProjection& viewProjection);

	/*�t�F�[�Y���Ƃ̊֐�*/
	//�ڋ߃t�F�[�Y������
	void InitApproach();
	//�ڋ߃t�F�[�Y
	void Approach();

	//���E�t�F�[�Y������
	void InitLeave();
	//���E�t�F�[�Y
	void Leave();

	//���ˊԊu
	static const int kFireInterval = 60;

	// setter
	void SetPlayer(Player* player) { player_ = player; }

	//���L����
	Player* player_ = nullptr;

	//���[���h���W���擾
	Vector3 GetWorldPosition();
};