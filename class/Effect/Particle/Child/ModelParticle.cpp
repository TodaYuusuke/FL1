#include "ModelParticle.h"

using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Primitive;

ModelParticle::ModelParticle(std::unique_ptr<LWP::Resource::RigidModel> model)
{
	// 平面のポインタを受け取る
	model_ = std::move(model);
}

ModelParticle::~ModelParticle()
{
	// 各コンポーネントの解放
	delete moveComponent_;
	delete rotateComponent_;
	delete scaleComponent_;
	delete colorComponent_;
}

ModelParticle& ModelParticle::Init()
{
	// フラグリセット
	isEnd_ = false;

	// 自身を返還する
	return *this;
}

void ModelParticle::Update(const float deltaTime, const float playSpeed)
{
	// 生存時間タイマー終了時
	if (aliveTimer_.GetIsFinish()) {
		// 終了フラグTrue
		isEnd_ = true;
		// 早期リターン
		return;
	}

	// 移動コンポーネント生成時
	if (moveComponent_ != nullptr) {
		moveComponent_->Update(deltaTime, playSpeed);
	}

	// 回転コンポーネント生成時
	if (rotateComponent_ != nullptr) {
		rotateComponent_->Update(deltaTime, playSpeed);
	}

	// 拡縮コンポーネント生成時
	if (scaleComponent_ != nullptr) {
		scaleComponent_->Update(deltaTime, playSpeed);
	}

	// 色コンポーネント生成時
	if (colorComponent_ != nullptr) {
		colorComponent_->Update(deltaTime, playSpeed);

		// 全ての色の更新
		for (auto& [key, ptr] : model_->materials) {
			ptr.color = color_;
		}
	}

	// 生存時間タイマー更新
	aliveTimer_.Update();
}

ModelParticle& ModelParticle::SetParent(LWP::Object::TransformQuat* parent)
{
	// 親子付け設定
	model_->worldTF.Parent(parent);

	// 自身を返す
	return *this;
}

ModelParticle& ModelParticle::SetTransform(const LWP::Math::Vector3& scale, const LWP::Math::Quaternion& rotate, const LWP::Math::Vector3& translate)
{
	// 座標設定
	model_->worldTF.scale		= scale;
	model_->worldTF.rotation	= rotate;
	model_->worldTF.translation = translate;

	// 自身を返す
	return *this;
}

ModelParticle& ModelParticle::SetAliveTime(const float aliveTime)
{
	// 粒子の生存時間を渡す
	aliveTime_ = aliveTime;
	// タイマー開始
	aliveTimer_.Start(aliveTime);

	// 自身を返す
	return *this;
}

ModelParticle& ModelParticle::SetVelocity(const LWP::Effect::VelocityData<Vector3>& data)
{
	// 移動用コンポーネントが何も設定されていない場合
	if (moveComponent_ == nullptr) {
		// 初期座標の設定
		model_->worldTF.translation = data.startValue;

		// 新規生成
		moveComponent_ = new VelocityComponent(&model_->worldTF.translation, data.velocity, data.acceleration);
		moveComponent_->Init();
	}

	return *this;
}

ModelParticle& ModelParticle::SetEasing(const LWP::Effect::EasingData<LWP::Math::Vector3>& data)
{
	// 移動用コンポーネントが何も設定されていない場合
	if (moveComponent_ == nullptr) {
		// 初期座標の設定
		model_->worldTF.translation = data.startValue;

		// 新規生成
		moveComponent_ = new EasingComponent(&model_->worldTF.translation, data.endValue, aliveTime_, data.type);
		moveComponent_->Init();
	}

	return *this;
}

ModelParticle& ModelParticle::SetRotateVelocity(const LWP::Effect::VelocityData<LWP::Math::Vector3>& data)
{
	// 回転用コンポーネントが何も設定されていない場合
	if (rotateComponent_ == nullptr) {
		// 度数法をラジアンに返還
		Vector3 GenerateEuilar = {
			LWP::Utility::DegreeToRadian(data.startValue.x),
			LWP::Utility::DegreeToRadian(data.startValue.y),
			LWP::Utility::DegreeToRadian(data.startValue.z)
		};
		// クォータニオンに変換
		Quaternion q = LWP::Math::Quaternion::ConvertEuler(GenerateEuilar);

		// 初期角度の設定
		model_->worldTF.rotation = q;

		// 新規生成
		rotateComponent_ = new VelocityComponent(&model_->worldTF.rotation, data.velocity, data.acceleration);
		rotateComponent_->Init();
	}

	return *this;
}

ModelParticle& ModelParticle::SetScaleEasing(const LWP::Effect::EasingData<LWP::Math::Vector3>& data, const bool isUnificationScale)
{
	// 拡縮用コンポーネントが何も設定されていない場合
	if (scaleComponent_ == nullptr) {
		// 一時データ格納用
		Vector3 startValue = data.startValue;
		Vector3 endValue = data.endValue;

		// スケールを均一化する場合
		if (isUnificationScale) {
			// それぞれの値をx軸の値で揃える
			startValue	= { startValue.x, startValue.x, startValue.x };
			endValue	= { endValue.x, endValue.x, endValue.x };
		}

		// 初期サイズの設定
		model_->worldTF.scale = startValue;

		// 新規生成
		scaleComponent_ = new EasingComponent(&model_->worldTF.scale, endValue, aliveTime_, data.type);
		scaleComponent_->Init();
	}

	return *this;
}

ModelParticle& ModelParticle::SetColorEasing(const LWP::Effect::EasingData<LWP::Math::Vector4>& data)
{
	// 移動用コンポーネントが何も設定されていない場合
	if (colorComponent_ == nullptr) {
		// 開始色の取得
		color_ = data.startValue;
		// 全てのデータの削除
		for (auto& [key, ptr] : model_->materials) {
			ptr.color = color_;
		}

		// 新規生成
		colorComponent_ = new EasingComponent(&color_, data.endValue, aliveTime_, data.type);
		// 初期化
		colorComponent_->Init();
	}

	// 自身を返す
	return *this;
}
