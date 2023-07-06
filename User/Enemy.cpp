#include "Enemy.h"
#include "Player.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	//FBX�I�u�W�F�N�g���
	delete fbxObject3d_;
	delete fbxModel_;
	for (int i = 0; i < 4; i++) { delete fbxWinpObject3d_[i]; }
	delete fbxWinpModel_;
	for (int i = 0; i < 4; i++) { delete obstacleObj_[i]; }
	delete obstacleModel_;
	delete shootModel_;
	delete shootObj_;

}

void Enemy::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	input_ = input;

	fbxModel_ = FbxLoader::GetInstance()->LoadModelFromFile("enemyfanneru");
	fbxWinpModel_ = FbxLoader::GetInstance()->LoadModelFromFile("enemystand");
	// �f�o�C�X���Z�b�g
	FBXObject3d::SetDevice(dxCommon->GetDevice());
	// �O���t�B�b�N�X�p�C�v���C������
	FBXObject3d::CreateGraphicsPipeline();

	//�ҋ@(Boss)
	fbxObject3d_ = new FBXObject3d;
	fbxObject3d_->Initialize();
	fbxObject3d_->SetModel(fbxModel_);
	fbxObject3d_->wtf.position = { 0.0f,0.1f,0.0f };
	fbxObject3d_->wtf.scale = { 0.04f,0.04f,0.04f };
	fbxObject3d_->PlayAnimation(0.5f, false);

	//�G���G
	for (int i = 0; i < 4; i++) {
		fbxWinpObject3d_[i] = new FBXObject3d;
		fbxWinpObject3d_[i]->Initialize();
		fbxWinpObject3d_[i]->SetModel(fbxWinpModel_);
		fbxWinpObject3d_[i]->wtf.scale = { 0.05f,0.05f,0.05f };
		fbxWinpObject3d_[i]->PlayAnimation(1.0f, true);
	}
	//�G���G�̋����ƃ|�W�V����
	//�ŏ��ɏo�Ă���0~3�̓G
	fbxWinpObject3d_[0]->wtf.position = { 2.4f,  1.3f, 2.0f };  //�E��{  0.4f, 0.3f,2.0f };   
	fbxWinpObject3d_[1]->wtf.position = { -2.8f,  1.4f, 2.0f }; //����{ -0.4f, 0.2f,2.0f }; 
	fbxWinpObject3d_[2]->wtf.position = { 4.0f, 0.6f, 2.0f }; //�E��{  0.4f,-0.3f,2.0f }; 
	fbxWinpObject3d_[3]->wtf.position = { -4.0f, -0.2f, 2.0f }; //����{ -0.4f,-0.3f,2.0f };


	shootModel_ = Model::LoadFromOBJ("eneboll");
	shootObj_ = Object3d::Create();
	shootObj_->SetModel(shootModel_);
	shootObj_->wtf.position = { fbxWinpObject3d_[0]->wtf.position.x,fbxWinpObject3d_[0]->wtf.position.y, fbxWinpObject3d_[0]->wtf.position.z };
	shootObj_->wtf.scale = { 0.04f,0.04f,0.04f };

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
	//�ŏ��̃{�X�̏����ĎG���G���o�Ă���܂ł̋���
	fbxObject3d_->Update();
	shootObj_->Update();
	if (bossGostMove == 0) {fbxObject3d_->wtf.position.y -= 0.003f;}
	if(fbxObject3d_->wtf.position.y <= -0.1f) {bossGostMove = 1;}
	if(bossGostMove == 1){ fbxObject3d_->wtf.position.z += 0.08f; }
	if (fbxObject3d_->wtf.position.z >= 6.0f) {bossGostAt = true;}
	if (fbxObject3d_->wtf.position.z >= 12.0f) { fbxObject3d_->wtf.position.z = 100.0f;}

	for (int i = 0; i < 4; i++) {
		fbxWinpObject3d_[i]->Update();
	}
	for (int i = 0; i < 4; i++) {
		obstacleObj_[i]->Update();
	}

	//�G���G�������n�߂�
	if (bossGostAt == true) {
		if (isAliveFlag == true) {
			isShootTimer++;
			//�E��G��
			fbxWinpObject3d_[0]->wtf.position.x -= 0.05f;
			fbxWinpObject3d_[0]->wtf.position.y -= 0.025f;
			if (fbxWinpObject3d_[0]->wtf.position.x <= 0.4f) { fbxWinpObject3d_[0]->wtf.position.x = 0.4f; }
			if (fbxWinpObject3d_[0]->wtf.position.y <= 0.2f) { fbxWinpObject3d_[0]->wtf.position.y = 0.2f; }
			//����G��
			fbxWinpObject3d_[1]->wtf.position.x += 0.05f;
			fbxWinpObject3d_[1]->wtf.position.y -= 0.025f;
			if (fbxWinpObject3d_[1]->wtf.position.x >= -0.4f) { fbxWinpObject3d_[1]->wtf.position.x = -0.4f; }
			if (fbxWinpObject3d_[1]->wtf.position.y <= 0.3f) { fbxWinpObject3d_[1]->wtf.position.y = 0.3f; }
			fbxWinpObject3d_[2]->wtf.position.x -= 0.005f;
			fbxWinpObject3d_[3]->wtf.position.x += 0.005f;

			

			//�e����
			if (isShootTimer >= 30) {
				isShootFlag = true;
			}
			if (isShootFlag == true) {
				shootObj_->wtf.position.z -= 0.05f;
			}
			else {
				shootObj_->wtf.position = { fbxWinpObject3d_[0]->wtf.position.x,fbxWinpObject3d_[0]->wtf.position.y, fbxWinpObject3d_[0]->wtf.position.z };
			}
			if (shootObj_->wtf.position.z <= -2.0f) {
				isShootFlag = false;
				isShootTimer = 0;
			}
		}

		//��Q��
		for (int i = 0; i < 4; i++) {
			obstacleObj_[i]->wtf.position.z -= 0.04f;
		}
	}



	//�����蔻��(���@�e�ƎG���G)
	if (coll.CircleCollision(player_->GetBulletWorldPosition(), GetWinpWorldPosition(), 0.1f, 0.3f)) {
		OnColision();
	};

	
}

void Enemy::Draw()
{
	if (isAliveFlag == true) {
		if (isShootFlag == true) {
			shootObj_->Draw();
		}
	}

	for (int i = 0; i < 4; i++) {
		obstacleObj_[i]->Draw();
	}
}

void Enemy::FbxDraw()
{

	fbxObject3d_->Draw(dxCommon->GetCommandList());


	if (isAliveFlag == true) {
		for (int i = 0; i < 4; i++) {
			fbxWinpObject3d_[i]->Draw(dxCommon->GetCommandList());
		}
	}

}

Vector3 Enemy::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	fbxObject3d_->wtf.UpdateMat();
	//���[���h�s��̕��s�ړ�����
	worldPos.x = fbxObject3d_->wtf.matWorld.m[3][0];
	worldPos.y = fbxObject3d_->wtf.matWorld.m[3][1];
	worldPos.z = fbxObject3d_->wtf.matWorld.m[3][2];

	return worldPos;
}

Vector3 Enemy::GetWinpWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 WinpWorldPos;
	fbxWinpObject3d_[0]->wtf.UpdateMat();
	//���[���h�s��̕��s�ړ�����
	WinpWorldPos.x = fbxWinpObject3d_[0]->wtf.matWorld.m[3][0];
	WinpWorldPos.y = fbxWinpObject3d_[0]->wtf.matWorld.m[3][1];
	WinpWorldPos.z = fbxWinpObject3d_[0]->wtf.matWorld.m[3][2];

	return WinpWorldPos;
}

void Enemy::OnColision()
{
	isAliveFlag = false;
}

