#include"Player.h"

Player::Player() {

}

Player::~Player() {

	//FBXオブジェクト解放
	delete fbxObject3d_;
	delete fbxModel_;
	delete shootObj_;
	delete shootModel_;
	delete fbxSlashObject3d_;
	delete fbxSlashModel_;
	delete fbxGardObject3d_;
	delete fbxGardModel_;
	delete hitboxObj_;
	delete hitboxModel_;
	delete retObj_;
	delete retModel_;
}

void Player::Initialize(DirectXCommon* dxCommon, Input* input) {
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	input_ = input;
	camTransForm = new Transform();

	fbxModel_ = FbxLoader::GetInstance()->LoadModelFromFile("stand");
	fbxSlashModel_ = FbxLoader::GetInstance()->LoadModelFromFile("strongAttack");
	fbxGardModel_ = FbxLoader::GetInstance()->LoadModelFromFile("mera");
	
	// デバイスをセット
	FBXObject3d::SetDevice(dxCommon->GetDevice());
	// グラフィックスパイプライン生成
	FBXObject3d::CreateGraphicsPipeline();

	//待機
	fbxObject3d_ = new FBXObject3d;
	fbxObject3d_->Initialize();
	fbxObject3d_->SetModel(fbxModel_);
	fbxObject3d_->wtf.position = { 0.0f,-0.3f,0.0f };
	fbxObject3d_->wtf.scale = { 0.05f,0.05f,0.05f };
	fbxObject3d_->PlayAnimation(1.0f,true);

	//斬り払い
	fbxSlashObject3d_ = new FBXObject3d;
	fbxSlashObject3d_->Initialize();
	fbxSlashObject3d_->SetModel(fbxSlashModel_);
	fbxSlashObject3d_->wtf.position = { 0.0f,-0.3f,0.0f };
	fbxSlashObject3d_->wtf.scale = { 0.05f,0.05f,0.05f };
	
	//盾
	fbxGardObject3d_ = new FBXObject3d;
	fbxGardObject3d_->Initialize();
	fbxGardObject3d_->SetModel(fbxGardModel_);
	fbxGardObject3d_->wtf.position = { 0.0f,-0.3f,0.0f };
	fbxGardObject3d_->wtf.scale = { 0.05f,0.05f,0.05f };

	//自機の弾
	shootModel_ = Model::LoadFromOBJ("boll");
	shootObj_ = Object3d::Create();
	shootObj_->SetModel(shootModel_);
	shootObj_->wtf.position = { fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y + 0.07f, fbxObject3d_->wtf.position.z };
	shootObj_->wtf.scale = { 0.03f,0.03f,0.03f };

	//ヒットボックス可視化
	hitboxModel_ = Model::LoadFromOBJ("slash");
	hitboxObj_ = Object3d::Create();
	hitboxObj_->SetModel(hitboxModel_);
	hitboxObj_->wtf.position = { fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y, fbxObject3d_->wtf.position.z };
	hitboxObj_->wtf.scale = { 0.05f,0.05f,0.05f };
	hitboxObj_->wtf.rotation = { 0.0f,80.0f,0.0f };

	//レティクル
	retModel_ = Model::LoadFromOBJ("ster");
	retObj_ = Object3d::Create();
	retObj_->SetModel(retModel_);
	retObj_->wtf.scale = { 0.5f,0.5f,0.5f };
	retObj_->wtf.position = { -1.5f,1.0f,10.0f };
}

void Player::Update() {
	fbxObject3d_->Update();
	fbxSlashObject3d_->Update();
	fbxSlashObject3d_->wtf.position = { fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y, fbxObject3d_->wtf.position.z};
	fbxGardObject3d_->Update();
	fbxGardObject3d_->wtf.position = { fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y, fbxObject3d_->wtf.position.z };
	shootObj_->Update();
	hitboxObj_->wtf.position = { fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y + 0.1f, fbxObject3d_->wtf.position.z + 0.1f };
	hitboxObj_->Update();
	retObj_->Update();
	enemylen = retObj_->wtf.position - shootObj_->wtf.position;
	enemylen.nomalize();

	//プレイヤーの行動一覧
	PlayerAction();

	

}

void Player::Draw() {
	if (isShootFlag == true) {
		shootObj_->Draw();
	}

	if (isSlashFlag == true || isGardFlag == true) {
		hitboxObj_->Draw();
	}

	retObj_->Draw();
}

void Player::FbxDraw(){
	if (isSlashFlag == true) {
		fbxSlashObject3d_->Draw(dxCommon->GetCommandList());
	}
	else if (isGardFlag == true) {
		fbxGardObject3d_->Draw(dxCommon->GetCommandList());
	}
	else
	{
		fbxObject3d_->Draw(dxCommon->GetCommandList());
	}
}

void Player::PlayerAction()
{
	//移動(自機)
	if (input_->PushKey(DIK_W)) {
		fbxObject3d_->wtf.position.y += 0.01f;
	}
	else if (input_->PushKey(DIK_S)) {
		fbxObject3d_->wtf.position.y -= 0.01f;
	}
	else if (input_->PushKey(DIK_A)) {
		fbxObject3d_->wtf.position.x -= 0.01f;
	}
	else if (input_->PushKey(DIK_D)) {
		fbxObject3d_->wtf.position.x += 0.01f;
	}
	//移動(レティクル)
	if (input_->PushKey(DIK_UP)) {
		retObj_->wtf.position.y += 0.08f;
	}
	else if (input_->PushKey(DIK_DOWN)) {
		retObj_->wtf.position.y -= 0.08f;
	}
	else if (input_->PushKey(DIK_LEFT)) {
		retObj_->wtf.position.x -= 0.08f;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		retObj_->wtf.position.x += 0.08f;
	}
	//移動制限(自機とレティクル)
	if (fbxObject3d_->wtf.position.x >= 0.6f) {
		fbxObject3d_->wtf.position.x = 0.6f;
	}
	else if(fbxObject3d_->wtf.position.x <= -0.6f) {
		fbxObject3d_->wtf.position.x = -0.6f;
	}
	else if (fbxObject3d_->wtf.position.y >= 0.19f) {
		fbxObject3d_->wtf.position.y = 0.19f;
	}
	else if (fbxObject3d_->wtf.position.y <= -0.35f) {
		fbxObject3d_->wtf.position.y = -0.35f;
	}

	//弾発射
	float ShortSpeed = 0.05f;

	if (input_->PushKey(DIK_SPACE)) {
		isShootFlag = true;
	}
	if (isShootFlag == true) {
		shootObj_->wtf.position += enemylen;
		len = enemylen;
		len *= ShortSpeed;
		
	}
	else {
		shootObj_->wtf.position = { fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y + 0.07f, fbxObject3d_->wtf.position.z };
	}
	if (shootObj_->wtf.position.z >= 9.0f) {
		isShootFlag = false;
	}

	//斬り払い
	if (input_->TriggerKey(DIK_Q)) {
		isSlashFlag = true;
		fbxSlashObject3d_->PlayAnimation(1.5f, true);
	}
	if (isSlashFlag == true) {
		isSlashTimer++;
	}
	if (isSlashTimer >= 15.0f) {
		isSlashTimer = 0.0f;
		isSlashFlag = false;
	}

	//盾
	if (input_->TriggerKey(DIK_E)) {
		isGardFlag = true;
		fbxGardObject3d_->PlayAnimation(1.5f, true);
	}
	if (isGardFlag == true) {
		isGardTimer++;
	}
	if (isGardTimer >= 15.0f) {
		isGardTimer = 0.0f;
		isGardFlag = false;
	}


}

Vector3 Player::bVelocity(Vector3& velocity, Transform& worldTransform)
{
	Vector3 result = { 0,0,0 };

	//内積
	result.z = velocity.x * worldTransform.matWorld.m[0][2] +
		velocity.y * worldTransform.matWorld.m[1][2] +
		velocity.z * worldTransform.matWorld.m[2][2];

	result.x = velocity.x * worldTransform.matWorld.m[0][0] +
		velocity.y * worldTransform.matWorld.m[1][0] +
		velocity.z * worldTransform.matWorld.m[2][0];

	result.y = velocity.x * worldTransform.matWorld.m[0][1] +
		velocity.y * worldTransform.matWorld.m[1][1] +
		velocity.z * worldTransform.matWorld.m[2][1];

	return result;
}

Vector3 Player::GetWorldPosition(){
	//ワールド座標を入れる変数
	Vector3 worldPos;
	
	fbxObject3d_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos.x = fbxObject3d_->wtf.matWorld.m[3][0];
	worldPos.y = fbxObject3d_->wtf.matWorld.m[3][1];
	worldPos.z = fbxObject3d_->wtf.matWorld.m[3][2];

	return worldPos;
}

Vector3 Player::GetBulletWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 BulletWorldPos;

	shootObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	BulletWorldPos.x = shootObj_->wtf.matWorld.m[3][0];
	BulletWorldPos.y = shootObj_->wtf.matWorld.m[3][1];
	BulletWorldPos.z = shootObj_->wtf.matWorld.m[3][2];

	return BulletWorldPos;
}

Vector3 Player::GetRetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 RetWorldPos;

	shootObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	RetWorldPos.x = retObj_->wtf.matWorld.m[3][0];
	RetWorldPos.y = retObj_->wtf.matWorld.m[3][1];
	RetWorldPos.z = retObj_->wtf.matWorld.m[3][2];

	return RetWorldPos;
}



