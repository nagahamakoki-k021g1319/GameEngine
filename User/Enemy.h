#pragma once
#include "DirectXCommon.h"
#include "Object3d.h"
#include "Input.h"
#include "Camera.h"

class Player;
#include "Collision.h"
#include "ParticleManager.h"
#include "Audio.h"

#include "FBXModel.h"
#include "FbxLoader.h"
#include "FBXObject3d.h"

class Enemy
{
public:
	Enemy();
	~Enemy();

	void Initialize(DirectXCommon* dxCommon, Input* input);
	void Update();

	void Draw();
	void FbxDraw();

	////���[���h���W���擾
	Vector3 GetWorldPosition();

	void OnColision();

	/// <summary>
	/// �|�W�V����
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(Vector3 pos) { fbxObject3d_->wtf.position = pos; };

public:
	//�����~�߂�֐�
	IXAudio2SourceVoice* pSourceVoice[10] = { 0 };

private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Audio* audio = nullptr;
	Player* player_ = nullptr;
	Collision coll;
	//�ҋ@(������Boss)
	FBXModel* fbxModel_ = nullptr;
	FBXObject3d* fbxObject3d_ = nullptr;
	int bossGostMove = 0;
	bool bossGostAt = false;

	//�G���G
	FBXModel* fbxWinpModel_ = nullptr;
	FBXObject3d* fbxWinpObject3d_[4] = {0};

	

	//��Q��
	Object3d* obstacleObj_[4] = { 0 };
	Model* obstacleModel_ = nullptr;
	int obstacleTimer = 0;


	const float moveSpeed_ = 0.1f;
	const float rotaSpeed_ = 0.1f;

	bool isAliveFlag = true;

	//���[���h���W������ϐ�
	Vector3 worldPos;

};

