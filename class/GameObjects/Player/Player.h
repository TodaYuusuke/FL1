#pragma once
#include "../BehaviourTree/Actor/Actor.h"
#include <Adapter.h>

class Player : public Actor{
//const float cCoolTime = 60.f;

public:
	// コンストラクタ
	Player(LWP::Object::Camera* camera);

	// デストラクタ
	~Player();

	// 更新
	void Update(float delta_time) override;

	// 描画
	void Draw() const override;

	//GUIの描画
	void DrawGui() override;

	void InputHandle();

	void DifferentialUpdate(float leftStickY, float rightStickY, float deltaTime);

private:
	//float mSpeed = 4.f;
	// ゲームパッド
	//XINPUT_STATE joyState_;

	LWP::Resource::RigidModel model_;

private:
	float treadWidth = 2.5f;
	float maxSpeed = 0.5f;
	float x = 0.0f, z = 0.0f, angle = 0.0f, omega;
	float vL = 0.0f, vR = 0.0f;
};