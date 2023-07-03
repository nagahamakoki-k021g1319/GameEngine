#include "Enemy.h"
#include "Player.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	//FBXオブジェクト解放
	delete fbxObject3d_;
	delete fbxModel_;
	for (int i = 0; i < 4; i++) { delete fbxWinpObject3d_[i];}
	delete fbxWinpModel_;
	for (int i = 0; i < 4; i++) { delete obstacleObj_[i]; }
	delete obstacleModel_;
}

void Enemy::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	input_ = input;

	fbxModel_ = FbxLoader::GetInstance()->LoadModelFromFile("enemyfanneru");
	fbxWinpModel_ = FbxLoader::GetInstance()->LoadModelFromFile("enemystand");
	// デバイスをセット
	FBXObject3d::SetDevice(dxCommon->GetDevice());
	// グラフィックスパイプライン生成
	FBXObject3d::CreateGraphicsPipeline();

	//待機(Boss)
	fbxObject3d_ = new FBXObject3d;
	fbxObject3d_->Initialize();
	fbxObject3d_->SetModel(fbxModel_);
	fbxObject3d_->wtf.position = { 0.0f,0.15f,0.0f };
	fbxObject3d_->wtf.scale = { 0.04f,0.04f,0.04f };
	fbxObject3d_->PlayAnimation(0.5f, false);

	//雑魚敵
	for (int i = 0; i < 4; i++) {
		fbxWinpObject3d_[i] = new FBXObject3d;
		fbxWinpObject3d_[i]->Initialize();
		fbxWinpObject3d_[i]->SetModel(fbxWinpModel_);
		fbxWinpObject3d_[i]->wtf.scale = { 0.05f,0.05f,0.05f };
		fbxWinpObject3d_[i]->PlayAnimation(1.0f, true);
	}
	//雑魚敵の挙動とポジション
	//最初に出てくる0~3の敵
	fbxWinpObject3d_[0]->wtf.position = {  2.4f,  1.3f, 2.0f };  //右上{  0.4f, 0.3f,2.0f };   
	fbxWinpObject3d_[1]->wtf.position = { -2.8f,  1.5f, 2.0f }; //左上{ -0.4f, 0.3f,2.0f }; 
	fbxWinpObject3d_[2]->wtf.position = {  4.0f, 0.6f, 2.0f }; //右下{  0.4f,-0.3f,2.0f }; 
	fbxWinpObject3d_[3]->wtf.position = {  -4.0f, -0.2f, 2.0f }; //左下{ -0.4f,-0.3f,2.0f };

	obstacleModel_ = Model::LoadFromOBJ("rock");
	for (int i = 0; i < 4; i++) {
		obstacleObj_[i] = Object3d::Create();
		obstacleObj_[i]->SetModel(obstacleModel_);
		obstacleObj_[i]->wtf.scale = { 0.05f,0.05f,0.05f };
	}
	obstacleObj_[0]->wtf.position = { 0.3f,0.3f,12.0f };
	obstacleObj_[1]->wtf.position = { -0.3f,0.1f,17.0f };
	obstacleObj_[2]->wtf.position = { -0.1f,0.3f,22.0f };
	obstacleObj_[3]->wtf.position = { 0.3f,-0.1f,27.0f };


}

void Enemy::Update()
{
	//最初のボスの消えて雑魚敵が出てくるまでの挙動
	fbxObject3d_->Update();
	if (bossGostMove == 0) {fbxObject3d_->wtf.position.y -= 0.003f;}
	if(fbxObject3d_->wtf.position.y <= -0.1f) {bossGostMove = 1;}
	if(bossGostMove == 1){ fbxObject3d_->wtf.position.z += 0.08f; }
	if (fbxObject3d_->wtf.position.z >= 6.0f) {bossGostAt = true;}
	if (fbxObject3d_->wtf.position.z >= 12.0f) { fbxObject3d_->wtf.position.z = 12.0f;}
	
	for (int i = 0; i < 4; i++) {
		fbxWinpObject3d_[i]->Update();
	}
	for (int i = 0; i < 4; i++) {
		obstacleObj_[i]->Update();
	}
	
	//雑魚敵が動き始める
	if (bossGostAt == true) {
		//右上雑魚
		fbxWinpObject3d_[0]->wtf.position.x -= 0.05f;
		fbxWinpObject3d_[0]->wtf.position.y -= 0.025f;
		if (fbxWinpObject3d_[0]->wtf.position.x <= 0.4f) {fbxWinpObject3d_[0]->wtf.position.x = 0.4f;}
		if (fbxWinpObject3d_[0]->wtf.position.y <= 0.3f) {fbxWinpObject3d_[0]->wtf.position.y = 0.3f;}
		//左上雑魚
		fbxWinpObject3d_[1]->wtf.position.x += 0.05f;
		fbxWinpObject3d_[1]->wtf.position.y -= 0.025f;
		if (fbxWinpObject3d_[1]->wtf.position.x >= -0.4f) { fbxWinpObject3d_[1]->wtf.position.x = -0.4f; }
		if (fbxWinpObject3d_[1]->wtf.position.y <= 0.3f) { fbxWinpObject3d_[1]->wtf.position.y = 0.3f; }
		fbxWinpObject3d_[2]->wtf.position.x -= 0.005f;
		fbxWinpObject3d_[3]->wtf.position.x += 0.005f;

		//障害物
		for (int i = 0; i < 4; i++) {
			obstacleObj_[i]->wtf.position.z -= 0.04f;
		}


	}



	/*if(isAliveFlag == true){
		if (coll.CircleCollision(player_->GetWorldPosition(), GetWorldPosition(), 1.0f, 1.0f)) {
			OnColision();
		};
	}*/

}

void Enemy::Draw()
{
	for (int i = 0; i < 4; i++) {
		obstacleObj_[i]->Draw();
	}
}

void Enemy::FbxDraw()
{
	if (isAliveFlag == true) {
		fbxObject3d_->Draw(dxCommon->GetCommandList());
	}

	for (int i = 0; i < 4; i++) {
		fbxWinpObject3d_[i]->Draw(dxCommon->GetCommandList());
	}

}

Vector3 Enemy::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	fbxObject3d_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos.x = fbxObject3d_->wtf.matWorld.m[3][0];
	worldPos.y = fbxObject3d_->wtf.matWorld.m[3][1];
	worldPos.z = fbxObject3d_->wtf.matWorld.m[3][2];

	return worldPos;
}

void Enemy::OnColision()
{
	isAliveFlag = false;
}
