#include "PenetrationResolver.h"
#include <queue>

using namespace LWP;
using namespace LWP::Math;

PenetrationResolver::~PenetrationResolver() {
	pushObjcts_.clear();
}

void PenetrationResolver::Init() {

}

void PenetrationResolver::Update() {
	CheckPushOutObject();

	// 対象が死んでいるなら削除
	pushObjcts_.erase(
		std::remove_if(pushObjcts_.begin(), pushObjcts_.end(),
			[&](PushOut obj) {
				if (!obj.target->GetIsAlive()) {
					return true; // vectorから削除対象
				}
				return false;
			}),
		pushObjcts_.end()
	);
}

void PenetrationResolver::CheckPushOutObject() {
	// 優先度の高いオブジェクト順にする比較関数
	auto compare = [&](PushOut current, PushOut next) {
		// 昇順
		return current.priority > next.priority;
		};
	std::priority_queue<
		PushOut,				// 要素の型
		std::vector<PushOut>,	// 内部コンテナ
		decltype(compare)       // コンパレータ
	> que{ compare };

	for (std::list<PushOut>::iterator itr = pushObjcts_.begin(); itr != pushObjcts_.end(); ++itr) {
		que.push((*itr));
	}

	checkPushList_.clear();
	while (!que.empty()) {
		checkPushList_.push_back(que.top());
		que.pop();
	}
	pushObjcts_ = checkPushList_;

	// リスト内のペアを総当たり
	for (std::list<PushOut>::iterator itrA = pushObjcts_.begin(); itrA != pushObjcts_.end(); ++itrA) {
		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<PushOut>::iterator itrB = itrA;
		itrB++;
		for (; itrB != pushObjcts_.end(); ++itrB) {
			// 相手との距離
			Vector3 dist = (*itrB).target->GetWorldTF()->GetWorldPosition() - (*itrA).target->GetWorldTF()->GetWorldPosition();
			// 押し出し範囲以上なら終了
			if (dist.Length() > (*itrA).range + (*itrB).range) { continue; }

			// 押し出し量決定
			float value = dist.Length() - ((*itrA).range + (*itrB).range);
			(*itrA).offset = (dist.Normalize() * value);
			LWP::Math::Vector3 newTranslation = (*itrA).target->GetWorldTF()->GetWorldPosition() + (*itrA).offset;
			// Y座標は維持
			newTranslation.y = (*itrA).target->GetWorldTF()->GetWorldPosition().y;
			(*itrA).target->SetTranslation(newTranslation);
		}
	}
}
