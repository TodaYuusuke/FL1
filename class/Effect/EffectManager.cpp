#include "EffectManager.h"

using namespace LWP::Math;
using namespace LWP::Utility;

EffectManager::~EffectManager()
{
	// 全てのエミッタの削除
	for (Emitter* e : emitters_) {
		delete e;
	}
	// 配列クリア
	emitters_.clear();
}

void EffectManager::Init()
{
	// 全てのエミッタの削除
	for (Emitter* e : emitters_) {
		delete e;
	}
	// 配列クリア
	emitters_.clear();
}

void EffectManager::Update()
{
	// デルタタイム取得
	float deltaTime = LWP::Info::GetDeltaTimeF();

	// 生存時間を超過したエミッタを削除
	emitters_.remove_if([&](Emitter* e) {
		// 粒子の終了フラグがTrueのとき
		if (e->GetIsEnd()) {
			// ポインタ解放
			delete e;
			return true;
		}

		// 粒子の更新を行う
		e->Update(deltaTime, playSpeed_);
		return false;
	});
}

void EffectManager::DebugGUI()
{
	
}

void EffectManager::CreateNewEmitter(std::string ParticleName, const Vector3& pos, LWP::Object::TransformQuat* parent)
{
	
}

void EffectManager::ReciveNewEmitter(Emitter* newEmitter)
{
	// 新規エミッタを配列に追加
	emitters_.push_back(newEmitter);
}
