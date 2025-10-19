#include "LeadingSystem.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"
#include "../../Enemy/EnemyManager.h"
#include "../../../Componets/Math.h"

using namespace FLMath;
using namespace LWP::Math;

LeadingSystem::LeadingSystem(LWP::Object::Camera* camera) {
	pCamera_ = camera;
}

void LeadingSystem::Init() {
	leadingTarget_ = nullptr;
	leadingTargetScreenPos_ = { 0.0f,0.0f };
}

void LeadingSystem::Update() {
	// 偏差対象の選択
	SelectLeadingTarget();

	// 偏差対象解除
	ClearLeadingTarget();
}

void LeadingSystem::DebugGui() {
	if (ImGui::TreeNode("LeadingSystem")) {
		ImGui::DragFloat("LeadingWorldRange", &leadingWorldRange_, 0.1f);
		ImGui::DragFloat("LeadingScreenRange", &leadingScreenRange_, 0.1f);
		ImGui::DragFloat("LeadingAccuracy", &leadingAccuracy_, 0.01f);

		ImGui::SameLine();
		if (leadingTarget_) {
			ImGui::Text("Leading!");
			ImGui::DragFloat3("Translation", &leadingTarget_->GetWorldTF()->translation.x);
			ImGui::DragFloat3("FutureTranslation", &targetFuture_.x);
		}
		else {
			ImGui::Text("Not Target");
		}
		ImGui::TreePop();
	}
}

void LeadingSystem::SelectLeadingTarget() {
	// 偏差対象がいるなら終了
	if (leadingTarget_) { return; }
	// 敵がいないなら終了
	if (pEnemyManager_->GetEnemyList().empty()) { return; }

	Actor* actorBuf = nullptr;
	for (Actor* actor : pEnemyManager_->GetEnemyList()) {
		// 自機と敵の距離
		Vector3 distance = actor->GetWorldTF()->GetWorldPosition() - pCamera_->worldTF.GetWorldPosition();
		// カメラと敵の距離が一定以上なら終了
		if (leadingWorldRange_ < distance.Length()) { continue; }

		// 敵がレティクルの一定範囲にいるならロックオン(判定はスクリーン座標で行う)
		Vector2 screenPos = ConvertWorldToScreen(actor->GetWorldTF()->GetWorldPosition(), pCamera_->GetViewProjection());
		// 画面外に敵がいるなら終了
		if (screenPos.x < 0.0f && screenPos.x > LWP::Info::GetWindowWidthF() &&
			screenPos.y < 0.0f && screenPos.y > LWP::Info::GetWindowHeightF()) {
			continue;
		}

		// カメラの後ろ側なら終了
		if (!IsObjectInFront(actor->GetWorldTF()->GetWorldPosition(), pCamera_->worldTF.GetWorldPosition(), pCamera_->worldTF.rotation)) {
			continue;
		}

		// 画面中心から一定以上の距離に敵がいるなら終了(スクリーン座標で計算を行う)
		Vector2 screenCenterPos = {
			LWP::Info::GetWindowWidthF() / 2.0f,
			LWP::Info::GetWindowHeightF() / 2.0f
		};
		if (leadingScreenRange_ < Vector2::Distance(screenCenterPos, screenPos)) { continue; }

		// 前回調べた敵よりも画面中心に敵が近いか
		if (std::fabsf(screenCenterPos.x - leadingTargetScreenPos_.x) > std::fabsf(screenCenterPos.x - screenPos.x) &&
			std::fabsf(screenCenterPos.y - leadingTargetScreenPos_.y) > std::fabsf(screenCenterPos.y - screenPos.y)) {
			actorBuf = nullptr;
			actorBuf = actor;
			leadingTargetScreenPos_ = screenPos;
		}
	}

	// 偏差対象更新
	leadingTarget_ = actorBuf;
}

void LeadingSystem::ClearLeadingTarget() {
	// 偏差対象がいないなら偏差対象解除
	if (!leadingTarget_) { return; }
	// 敵がいないなら偏差対象解除
	if (pEnemyManager_->GetEnemyList().empty()) { return; }

	// 自機と敵の距離
	Vector3 distance = leadingTarget_->GetWorldTF()->GetWorldPosition() - pCamera_->worldTF.GetWorldPosition();
	// カメラと敵の距離が一定以上なら偏差対象解除
	if (leadingWorldRange_ < distance.Length()) {
		leadingTarget_ = nullptr;
		leadingTargetScreenPos_ = { 0.0f,0.0f };
		return;
	}

	// 敵がレティクルの一定範囲にいるならロックオン(判定はスクリーン座標で行う)
	Vector2 screenPos = ConvertWorldToScreen(leadingTarget_->GetWorldTF()->GetWorldPosition(), pCamera_->GetViewProjection());
	// 画面外に敵がいるなら偏差対象解除
	if (screenPos.x < 0.0f && screenPos.x > LWP::Info::GetWindowWidthF() &&
		screenPos.y < 0.0f && screenPos.y > LWP::Info::GetWindowHeightF()) {
		leadingTarget_ = nullptr;
		leadingTargetScreenPos_ = { 0.0f,0.0f };
		return;
	}

	// カメラの後ろ側なら偏差対象解除
	if (!IsObjectInFront(leadingTarget_->GetWorldTF()->GetWorldPosition(), pCamera_->worldTF.GetWorldPosition(), pCamera_->worldTF.rotation)) {
		leadingTarget_ = nullptr;
		leadingTargetScreenPos_ = { 0.0f,0.0f };
		return;
	}

	// 画面中心から一定以上の距離に敵がいるなら偏差対象解除(スクリーン座標で計算を行う)
	Vector2 screenCenterPos = {
		LWP::Info::GetWindowWidthF() / 2.0f,
		LWP::Info::GetWindowHeightF() / 2.0f
	};
	if (leadingScreenRange_ < Vector2::Distance(screenCenterPos, screenPos)) {
		leadingTarget_ = nullptr;
		leadingTargetScreenPos_ = { 0.0f,0.0f };
		return;
	}
}

float LeadingSystem::PlusMin(float a, float b) {
	if (a < 0 && b < 0) return 0;
	if (a < 0) return b;
	if (b < 0) return a;
	return a < b ? a : b;
}

void LeadingSystem::CalFutureTargetPos(const Vector3& shooterPos, float bulletSpeed) {
	if (!leadingTarget_) {
		targetFuture_ = { 0,0,0 };
		return;
	}

	//目標の1フレームの移動速度
	Vector3 v3_Mv = leadingTarget_->GetWorldTF()->GetWorldPosition() - leadingTarget_->GetPreTranslation();
	//射撃する位置から見た目標位置
	Vector3 v3_Pos = leadingTarget_->GetWorldTF()->GetWorldPosition() - shooterPos;

	//ピタゴラスの定理から２つのベクトルの長さが等しい場合の式を作り
	//二次方程式の解の公式を使って弾が当たる予測時間を計算する
	float A = (v3_Mv.x * v3_Mv.x + v3_Mv.y * v3_Mv.y + v3_Mv.z * v3_Mv.z) - bulletSpeed * bulletSpeed;
	float B = 2 * (v3_Pos.x * v3_Mv.x + v3_Pos.y * v3_Mv.y + v3_Pos.z * v3_Mv.z);
	float C = (v3_Pos.x * v3_Pos.x + v3_Pos.y * v3_Pos.y + v3_Pos.z * v3_Pos.z);

	//0割り禁止処理
	if (A == 0) {
		if (B == 0) {
			targetFuture_ = leadingTarget_->GetWorldTF()->GetWorldPosition();
			return;
		}
		else {
			leadingTarget_->GetWorldTF()->GetWorldPosition() + v3_Mv * (-C / B);
			return;
		}
	}

	//弾が当たる時間のフレームを計算する
	float flame1, flame2;
	//二次方程式の解の公式の判別式で分類
	float D = B * B - 4 * A * C;
	if (D > 0) {
		float E = std::sqrtf(D);
		flame1 = (-B - E) / (2 * A);
		flame2 = (-B + E) / (2 * A);
		//解は2つなので正の数の最小値を使う
		flame1 = PlusMin(flame1, flame2);
	}
	else {
		//虚数解
		//当たらないので今の位置を狙う
		flame1 = 0;
	}

	// 的の未来位置
	targetFuture_ = leadingTarget_->GetWorldTF()->GetWorldPosition() + v3_Mv * flame1;
}

Vector3 LeadingSystem::GetLeadingShotAngle(const Vector3& shooterPos, float bulletSpeed) {
	if (!leadingTarget_) { return Vector3{ 0,0,0 }; }

	//目標の1フレームの移動速度
	Vector3 v3_Mv = leadingTarget_->GetWorldTF()->GetWorldPosition() - leadingTarget_->GetPreTranslation();
	//射撃する位置から見た目標位置
	Vector3 v3_Pos = leadingTarget_->GetWorldTF()->GetWorldPosition() - shooterPos;

	//ピタゴラスの定理から２つのベクトルの長さが等しい場合の式を作り
	//二次方程式の解の公式を使って弾が当たる予測時間を計算する
	float A = (v3_Mv.x * v3_Mv.x + v3_Mv.y * v3_Mv.y + v3_Mv.z * v3_Mv.z) - bulletSpeed * bulletSpeed;
	float B = 2 * (v3_Pos.x * v3_Mv.x + v3_Pos.y * v3_Mv.y + v3_Pos.z * v3_Mv.z);
	float C = (v3_Pos.x * v3_Pos.x + v3_Pos.y * v3_Pos.y + v3_Pos.z * v3_Pos.z);

	//0割り禁止処理
	if (A == 0) {
		if (B == 0) {
			return leadingTarget_->GetWorldTF()->GetWorldPosition();
		}
		else {
			return leadingTarget_->GetWorldTF()->GetWorldPosition() + v3_Mv * (-C / B);
		}
	}

	//弾が当たる時間のフレームを計算する
	float flame1, flame2;
	//二次方程式の解の公式の判別式で分類
	float D = B * B - 4 * A * C;
	if (D > 0) {
		float E = std::sqrtf(D);
		flame1 = (-B - E) / (2 * A);
		flame2 = (-B + E) / (2 * A);
		//解は2つなので正の数の最小値を使う
		flame1 = PlusMin(flame1, flame2);
	}
	else {
		//虚数解
		//当たらないので今の位置を狙う
		flame1 = 0;
	}

	// 方向ベクトル
	Vector3 result = (leadingTarget_->GetWorldTF()->GetWorldPosition() + v3_Mv * flame1) - shooterPos;

	return result.Normalize();
}
