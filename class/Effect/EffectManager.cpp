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

	// エディタ解放
	delete effectEditor_;
}

void EffectManager::Init()
{
	// エフェクトエディタの作成
	effectEditor_ = new EffectEditor(this);
	effectEditor_->Init();
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
	// エディタの更新
	effectEditor_->Update();
}

void EffectManager::SendNewEmitter(Emitter* newEmitter)
{
	// 新規エミッタを配列に追加
	emitters_.push_back(newEmitter);
}
