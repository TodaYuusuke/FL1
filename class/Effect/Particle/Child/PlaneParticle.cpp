#include "PlaneParticle.h"

using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Primitive;

PlaneParticle::PlaneParticle(std::unique_ptr<IPlane> plane, bool isStretchBillboard)
{
	// 平面のポインタを受け取る
	plane_ = std::move(plane);

	// ストレッチビルボードのみ特殊な処理を行う
	if (isStretchBillboard) {
		// ストレッチビルボード型にキャスト
		if (NormalStretchedBillboard* stretch = dynamic_cast<NormalStretchedBillboard*>(plane_.get())) {
			// アドレス取得
			stretchVector_ = &stretch->velocity;
		}
	}
}

PlaneParticle::~PlaneParticle()
{
	// 各コンポーネントの解放
	delete moveComponent_;
	delete rotateComponent_;
	delete scaleComponent_;
	delete colorComponent_;
}

PlaneParticle& PlaneParticle::Init()
{
	// フラグリセット
	isEnd_ = false;

	// 自身を返還する
	return *this;
}

void PlaneParticle::Update(const float deltaTime, const float playSpeed)
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
		// 色の更新
		plane_->material.color = color_;
	}

	// 生存時間タイマー更新
	aliveTimer_.Update();
}

PlaneParticle& PlaneParticle::SetParent(LWP::Object::TransformQuat* parent)
{
	// 親子付け設定
	plane_->worldTF.Parent(parent);

	// 自身を返す
	return *this;
}

PlaneParticle& PlaneParticle::SetTransform(const LWP::Math::Vector3& scale, const LWP::Math::Quaternion& rotate, const LWP::Math::Vector3& translate)
{
	// 座標設定
	plane_->worldTF.scale		= scale;
	plane_->worldTF.rotation	= rotate;
	plane_->worldTF.translation = translate;

	// 自身を返す
	return *this;
}

PlaneParticle& PlaneParticle::SetAliveTime(const float aliveTime)
{
	// 粒子の生存時間を渡す
	aliveTime_ = aliveTime;
	// タイマー開始
	aliveTimer_.Start(aliveTime);

	// 自身を返す
	return *this;
}

PlaneParticle& PlaneParticle::SetVelocity(const LWP::Effect::VelocityData<Vector3>& data)
{
	// 移動用コンポーネントが何も設定されていない場合
	if (moveComponent_ == nullptr) {
		// 初期座標の設定
		plane_->worldTF.translation = plane_->worldTF.translation + data.startValue;

		// 新規生成
		moveComponent_ = new VelocityComponent(&plane_->worldTF.translation, data.velocity, data.acceleration, stretchVector_);
		moveComponent_->Init();
	}

	return *this;
}

PlaneParticle& PlaneParticle::SetEasing(const LWP::Effect::EasingData<LWP::Math::Vector3>& data)
{
	// 移動用コンポーネントが何も設定されていない場合
	if (moveComponent_ == nullptr) {
		// 初期座標の設定
		plane_->worldTF.translation = data.startValue;

		// 新規生成
		moveComponent_ = new EasingComponent(&plane_->worldTF.translation, data.endValue, aliveTime_, data.type);
		moveComponent_->Init();
	}

	return *this;
}

PlaneParticle& PlaneParticle::SetRotateVelocity(const LWP::Effect::VelocityData<LWP::Math::Vector3>& data)
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
		plane_->worldTF.rotation = q;

		// 新規生成
		rotateComponent_ = new VelocityComponent(&plane_->worldTF.rotation, data.velocity, data.acceleration);
		rotateComponent_->Init();
	}

	return *this;
}

PlaneParticle& PlaneParticle::SetScaleEasing(const LWP::Effect::EasingData<LWP::Math::Vector3>& data, const bool isUnificationScale)
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
		plane_->worldTF.scale = startValue;

		// 新規生成
		scaleComponent_ = new EasingComponent(&plane_->worldTF.scale, endValue, aliveTime_, data.type);
		scaleComponent_->Init();
	}

	return *this;
}

PlaneParticle& PlaneParticle::SetColorEasing(const LWP::Effect::EasingData<LWP::Math::Vector4>& data)
{
	// 移動用コンポーネントが何も設定されていない場合
	if (colorComponent_ == nullptr) {
		// 開始色の取得
		color_ = data.startValue;
		plane_->material.color = color_;

		// 新規生成
		colorComponent_ = new EasingComponent(&color_, data.endValue, aliveTime_, data.type);
		// 初期化
		colorComponent_->Init();
	}

	// 自身を返す
	return *this;
}
