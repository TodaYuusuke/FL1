#pragma once
#include "../../Componets/BehaviourTree/Actor/Actor.h"
#include <Adapter.h>

/// <summary>
/// オブジェクトの押し出し
/// </summary>
class PenetrationResolver final : public LWP::Utility::ISingleton<PenetrationResolver> {
	friend class LWP::Utility::ISingleton<PenetrationResolver>;
private:
	struct PushOut {
		Actor* target;				// 
		LWP::Math::Vector3 offset;	// 押し出し
		float range = 2.0f;		// 押し出し検出範囲
		int priority;
	};

private:
	// コンストラクタ
	PenetrationResolver() = default;
	// デストラクタ
	~PenetrationResolver();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	/// <summary>
	/// 押し出すかを確認
	/// </summary>
	void CheckPushOutObject();

public:// アクセサ
	/// <summary>
	/// 押し出したいオブジェクト登録
	/// </summary>
	/// <param name="target"></param>
	void RegisterObject(Actor* target, int priority = 0, float range = 5.0f) { pushObjcts_.push_back(PushOut{ target, {}, range, priority }); }

private:
	std::list<PushOut> checkPushList_;
	std::list<PushOut> pushObjcts_;
};