#include "BulletEffector.h"

BulletEffector::~BulletEffector()
{
	// リスト内の全データクリア
	for (IBulletEffect* effect : effects_) {
		delete effect;
	}
	effects_.clear();
}

void BulletEffector::Init(LWP::Object::Camera* mainCamera)
{
	// ポインタ受け取り
	mainCamera_ = mainCamera;

	// リスト内の全データクリア
	for (IBulletEffect* effect : effects_) {
		delete effect;
	}
	effects_.clear();
}

void BulletEffector::Update() {
	// 終了フラグがたったエフェクト削除
	effects_.remove_if([&](IBulletEffect* e) {
		// 終了フラグが立っていれば
		if (e->GetIsEnd()) {
			// ポインタ解放
			delete e;
			return true;
		}

		// エフェクト更新
		e->Update();
		return false;
	});
}

IBulletEffect* BulletEffector::CreateBulletLineEffect(const LWP::Math::Vector3& scale, LWP::Object::TransformQuat* transform, const LWP::Math::Vector3* vec)
{
	// エフェクト生成
	IBulletEffect* effect = new BulletLineEffect(mainCamera_, scale, transform, vec);

	// エフェクト配列に追加
	effects_.push_back(effect);

	// 生成したエフェクトを返す
	return effects_.back();
}

IBulletEffect* BulletEffector::CreateExplosiveBulletEffect(const LWP::Math::Vector3& scale, LWP::Object::TransformQuat* transform)
{
	// エフェクト生成
	IBulletEffect* effect = new ExplosiveBulletEffect(scale, transform);

	// エフェクト配列に追加
	effects_.push_back(effect);

	// 生成したエフェクトを返す
	return effects_.back();
}