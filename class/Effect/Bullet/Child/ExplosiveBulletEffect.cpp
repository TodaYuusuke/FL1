#include "ExplosiveBulletEffect.h"

ExplosiveBulletEffect::ExplosiveBulletEffect(const LWP::Math::Vector3& scale, LWP::Object::TransformQuat* transform)
{
    // 各種ポインタの受け取り
    bulletTransform_ = transform;

    // 平面テクスチャ読み込み
    bulletPlane_.LoadTexture("Particle/LauncherBullet.png");

    // 座標追従
    bulletPlane_.worldTF.Parent(bulletTransform_);
    // 拡縮設定
    bulletPlane_.worldTF.scale = scale;

    // 煙トレイルエミッタ生成
    smokeEffectEmitter_ = EffectManager::GetInstance()->CreateNewEmitter("SmokeTrail", { 0.0f, 0.0f, 0.0f }, true, &bulletPlane_.worldTF);
}

ExplosiveBulletEffect::~ExplosiveBulletEffect()
{
    if (smokeEffectEmitter_ != nullptr) {
        smokeEffectEmitter_->Finish();
    }
}

void ExplosiveBulletEffect::Update()
{
    // 生成を要求
    if (smokeEffectEmitter_ != nullptr) {
        smokeEffectEmitter_->Emit();
    }
}
