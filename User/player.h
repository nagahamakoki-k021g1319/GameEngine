#pragma once
#include "DirectXCommon.h"
#include "Object3d.h"
#include "Input.h"
#include "Camera.h"

#include "ParticleManager.h"
#include "Audio.h"

#include "FBXModel.h"
#include "FbxLoader.h"
#include "FBXObject3d.h"


class Player {
public:
	Player();
	~Player();

	void Initialize(DirectXCommon* dxCommon, Input* input);
	void Update();

	void Draw();
	void FbxDraw();

	//プレイヤーの行動一覧
	void PlayerAction();

	Vector3 bVelocity(Vector3& velocity, Transform& worldTransform);

	////ワールド座標を取得
	Vector3 GetWorldPosition();

	//ワールド座標を取得(弾)
	Vector3 GetBulletWorldPosition();

	//ワールド座標を取得(レティクル)
	Vector3 GetRetWorldPosition();

	Vector3 GetPos() { return fbxObject3d_->wtf.position; };


	/// <summary>
	/// ポジション
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(Vector3 pos) {fbxObject3d_->wtf.position = pos; };
	void SetCamera(Camera* cam) { camera = cam; };

public:
	//音を止める関数
	IXAudio2SourceVoice* pSourceVoice[10] = { 0 };

private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Audio* audio = nullptr;
	//待機
	FBXModel* fbxModel_ = nullptr;
	FBXObject3d* fbxObject3d_ = nullptr;
	
	//弾発射
	Object3d* shootObj_ = nullptr;
	Model* shootModel_ = nullptr;
	bool isShootFlag = false;

	//切り払いモーション
	FBXModel* fbxSlashModel_ = nullptr;
	FBXObject3d* fbxSlashObject3d_ = nullptr;
	bool isSlashFlag = false;
	float isSlashTimer = 0.0f;

	//盾モーション
	FBXModel* fbxGardModel_ = nullptr;
	FBXObject3d* fbxGardObject3d_ = nullptr;
	bool isGardFlag = false;
	float isGardTimer = 0.0f;

	//ヒットボックス
	Object3d* hitboxObj_ = nullptr;
	Model* hitboxModel_ = nullptr;
	
	//レティクル
	Object3d* retObj_ = nullptr;
	Model* retModel_ = nullptr;
	Vector3 enemylen;
	Vector3 len;

	const float moveSpeed_ = 0.1f;
	const float rotaSpeed_ = 0.1f;

	Camera* camera = nullptr;
	Transform* camTransForm = nullptr;
	Vector3 targetPos;
	Vector3 eyePos;
	Vector3 centerPos;
	float targetTheta;
	float targetDistance = 10;
	float camMoveSpeed = 0.2f;
  
	Vector2 camRotaSpeed = { PI / 1800, PI / 1800};

	bool isAliveFlag = true;

	

};