#include "LeadingSystem.h"
#include "../../Enemy/EnemyManager.h"
#include "../../Weapon/IWeapon.h"
#include "../../Player/PlayerConfig.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"
#include "../../../Componets/Math.h"

using namespace FLMath;
using namespace LWP::Math;

using namespace FLMath;
using namespace LWP::Math;

LeadingSystem::LeadingSystem(LWP::Object::Camera* camera, BlackBoard* blackBoard) {
	pCamera_ = camera;
	blackBoard_ = blackBoard;

	json_.Init(kJsonFileDirectoryPath + "LeadingSystem.json")
		.AddValue<float>("LeadingScreenRange", &leadingScreenRange_)
		.CheckJsonFile();

	reticle_.LoadTexture("lockOnReticle.png");
	reticle_.anchorPoint = { 0.5f,0.5f };

	// 画面中央のワールド座標算出
	centerWorldPos_ = ConvertScreenToWorld(pCamera_->GetViewProjection(), Vector2{ LWP::Info::GetWindowWidthF() / 2.0f, LWP::Info::GetWindowHeightF() / 2.0f }, 10.0f);
	targetFuture_ = centerWorldPos_;
	leadingTargetPos_ = centerWorldPos_;

	reticle_.worldTF.translation = {
		ConvertWorldToScreen(leadingTargetPos_, pCamera_->GetViewProjection()).x,
		ConvertWorldToScreen(leadingTargetPos_, pCamera_->GetViewProjection()).y,
		0.0f
	};
}

void LeadingSystem::Init() {
	leadingTarget_ = nullptr;
	leadingTargetScreenPos_ = { 0.0f,0.0f };
}

void LeadingSystem::Update() {
	// 画面中央のワールド座標算出
	centerWorldPos_ = ConvertScreenToWorld(pCamera_->GetViewProjection(), Vector2{ LWP::Info::GetWindowWidthF() / 2.0f, LWP::Info::GetWindowHeightF() / 2.0f }, 10.0f);

	// 偏差対象の選択
	SelectLeadingTarget();

	// 敵が死んでいたら偏差終了
	if (leadingTarget_ && !leadingTarget_->GetIsAlive() || pEnemyManager_->GetEnemyList().empty()) {
		leadingTarget_ = nullptr;
	}
	// 偏差対象解除
	ClearLeadingTarget();

	// 偏差対象がいないなら未来の座標を真ん中に戻す
	if (!leadingTarget_) {
		targetFuture_ = centerWorldPos_;
		leadingTargetPos_ = centerWorldPos_;
	}
	else {
		CalFutureTargetPos(bulletSpeed_);
		leadingTargetPos_ = leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor");
	}

	reticle_.worldTF.translation = {
		ConvertWorldToScreen(leadingTargetPos_, pCamera_->GetViewProjection()).x,
		ConvertWorldToScreen(leadingTargetPos_, pCamera_->GetViewProjection()).y,
		0.0f
	};
}

void LeadingSystem::DebugGui() {
	if (ImGui::TreeNode("LeadingSystem")) {
		// 調整項目
		json_.DebugGUI();

		if (leadingTarget_) {
			ImGui::Text("Leading!");
			ImGui::DragFloat3("Translation", &leadingTarget_->GetWorldTF()->translation.x);
			ImGui::DragFloat3("Pos", &leadingTargetPos_.x);
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

	for (Actor* actor : pEnemyManager_->GetEnemyList()) {
		// 敵がレティクルの一定範囲にいるならロックオン(判定はスクリーン座標で行う)
		Vector2 screenPos = ConvertWorldToScreen(actor->GetModel().GetJointWorldPosition("LockOnAnchor"), pCamera_->GetViewProjection());
		// 画面外に敵がいるなら終了
		if (screenPos.x < 0.0f && screenPos.x > LWP::Info::GetWindowWidthF() &&
			screenPos.y < 0.0f && screenPos.y > LWP::Info::GetWindowHeightF()) {
			continue;
		}

		// 画面中心から一定以上の距離に敵がいるなら終了(スクリーン座標で計算を行う)
		Vector2 screenCenterPos = {
			LWP::Info::GetWindowWidthF() / 2.0f,
			LWP::Info::GetWindowHeightF() / 2.0f
		};
		if (leadingScreenRange_ < Vector2::Distance(screenCenterPos, screenPos)) { continue; }

		// カメラの後ろ側なら終了
		if (!IsObjectInFront(actor->GetModel().GetJointWorldPosition("LockOnAnchor"), pCamera_->worldTF.GetWorldPosition(), pCamera_->worldTF.rotation)) {
			continue;
		}

		leadingTarget_ = actor;
		break;
	}
}

void LeadingSystem::ClearLeadingTarget() {
	// 偏差対象がいないなら偏差対象解除
	if (!leadingTarget_) { return; }
	// 敵がいないなら偏差対象解除
	if (pEnemyManager_->GetEnemyList().empty()) { return; }

	// 画面中心から一定以内の距離に敵がいないなら偏差対象解除(スクリーン座標で計算を行う)
	Vector2 screenPos = ConvertWorldToScreen(leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor"), pCamera_->GetViewProjection());
	Vector2 screenCenterPos = {
		LWP::Info::GetWindowWidthF() / 2.0f,
		LWP::Info::GetWindowHeightF() / 2.0f
	};
	if (leadingScreenRange_ < Vector2::Distance(screenCenterPos, screenPos)) {
		leadingTarget_ = nullptr;
		leadingTargetScreenPos_ = { 0.0f,0.0f };
		targetFuture_ = centerWorldPos_;
		return;
	}
	// 画面外に敵がいるなら偏差対象解除
	if (screenPos.x < 0.0f && screenPos.x > LWP::Info::GetWindowWidthF() &&
		screenPos.y < 0.0f && screenPos.y > LWP::Info::GetWindowHeightF()) {
		leadingTarget_ = nullptr;
		leadingTargetScreenPos_ = { 0.0f,0.0f };
		targetFuture_ = centerWorldPos_;
		return;
	}

	//// カメラの後ろ側なら偏差対象解除
	//if (!IsObjectInFront(leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor"), pCamera_->worldTF.GetWorldPosition(), pCamera_->worldTF.rotation)) {
	//	leadingTarget_ = nullptr;
	//	leadingTargetScreenPos_ = { 0.0f,0.0f };
	//	targetFuture_ = centerWorldPos_;
	//	return;
	//}

}

float LeadingSystem::PlusMin(float a, float b) {
	if (a < 0 && b < 0) return 0;
	if (a < 0) return b;
	if (b < 0) return a;
	return a < b ? a : b;
}

void LeadingSystem::CalFutureTargetPos(float bulletSpeed) {
	// 画面中央のワールド座標算出
	centerWorldPos_ = ConvertScreenToWorld(pCamera_->GetViewProjection(), Vector2{ LWP::Info::GetWindowWidthF() / 2.0f, LWP::Info::GetWindowHeightF() / 2.0f }, 10.0f);

	if (!leadingTarget_) {
		targetFuture_ = centerWorldPos_;
		leadingTargetPos_ = centerWorldPos_;
		return;
	}

	//目標の1フレームの移動速度
	Vector3 v3_Mv = leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor") - leadingTarget_->GetPreTranslation();
	//射撃する位置から見た目標位置
	Vector3 v3_Pos = leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor") - shooterPos_;

	//ピタゴラスの定理から２つのベクトルの長さが等しい場合の式を作り
	//二次方程式の解の公式を使って弾が当たる予測時間を計算する
	float A = (v3_Mv.x * v3_Mv.x + v3_Mv.y * v3_Mv.y + v3_Mv.z * v3_Mv.z) - bulletSpeed * bulletSpeed;
	float B = 2 * (v3_Pos.x * v3_Mv.x + v3_Pos.y * v3_Mv.y + v3_Pos.z * v3_Mv.z);
	float C = (v3_Pos.x * v3_Pos.x + v3_Pos.y * v3_Pos.y + v3_Pos.z * v3_Pos.z);

	//0割り禁止処理
	if (A == 0) {
		if (B == 0) {
			targetFuture_ = leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor");
			return;
		}
		else {
			targetFuture_ = leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor") + v3_Mv * (-C / B);
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
		flame1 = std::clamp<float>(flame1, 0.0f, limitLeadingFrame);
	}
	else {
		//虚数解
		//当たらないので今の位置を狙う
		flame1 = 0;
	}

	// 的の未来位置
	targetFuture_ = leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor") + v3_Mv * flame1;
	leadingTargetPos_ = leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor");
}

void LeadingSystem::CalFutureTargetPos(const Vector3& shooterPos, float bulletSpeed) {
	if (!leadingTarget_) {
		targetFuture_ = { 0,0,0 };
		return;
	}

	//目標の1フレームの移動速度
	Vector3 v3_Mv = leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor") - leadingTarget_->GetPreTranslation();
	//射撃する位置から見た目標位置
	Vector3 v3_Pos = leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor") - shooterPos;

	//ピタゴラスの定理から２つのベクトルの長さが等しい場合の式を作り
	//二次方程式の解の公式を使って弾が当たる予測時間を計算する
	float A = (v3_Mv.x * v3_Mv.x + v3_Mv.y * v3_Mv.y + v3_Mv.z * v3_Mv.z) - (bulletSpeed * bulletSpeed);
	float B = 2 * (v3_Pos.x * v3_Mv.x + v3_Pos.y * v3_Mv.y + v3_Pos.z * v3_Mv.z);
	float C = (v3_Pos.x * v3_Pos.x + v3_Pos.y * v3_Pos.y + v3_Pos.z * v3_Pos.z);

	//0割り禁止処理
	if (A == 0) {
		if (B == 0) {
			targetFuture_ = leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor");
			return;
		}
		else {
			leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor") + v3_Mv * (-C / B);
			return;
		}
	}

	//弾が当たる時間のフレームを計算する
	float flame1, flame2;
	//二次方程式の解の公式の判別式で分類
	float D = (B * B) - (4 * A * C);
	if (D > 0) {
		float E = std::sqrtf(D);
		flame1 = (-B - E) / (2 * A);
		flame2 = (-B + E) / (2 * A);
		//解は2つなので正の数の最小値を使う
		flame1 = PlusMin(flame1, flame2);
		flame1 = std::clamp<float>(flame1, 0.0f, limitLeadingFrame);
	}
	else {
		//虚数解
		//当たらないので今の位置を狙う
		flame1 = 0;
	}

	// 的の未来位置
	targetFuture_ = leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor") + (v3_Mv * flame1);
	leadingTargetPos_ = leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor");
}

Vector3 LeadingSystem::GetLeadingShotAngle(const Vector3& shooterPos, float bulletSpeed) {
	shooterPos_ = shooterPos;

	if (!leadingTarget_) { return Vector3{ 0,0,0 }; }

	//目標の1フレームの移動速度
	Vector3 v3_Mv = leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor") - leadingTarget_->GetPreTranslation();
	//射撃する位置から見た目標位置
	Vector3 v3_Pos = leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor") - shooterPos;

	//ピタゴラスの定理から２つのベクトルの長さが等しい場合の式を作り
	//二次方程式の解の公式を使って弾が当たる予測時間を計算する
	float A = (v3_Mv.x * v3_Mv.x + v3_Mv.y * v3_Mv.y + v3_Mv.z * v3_Mv.z) - (bulletSpeed * bulletSpeed);
	float B = 2 * (v3_Pos.x * v3_Mv.x + v3_Pos.y * v3_Mv.y + v3_Pos.z * v3_Mv.z);
	float C = (v3_Pos.x * v3_Pos.x + v3_Pos.y * v3_Pos.y + v3_Pos.z * v3_Pos.z);

	//0割り禁止処理
	if (A == 0) {
		if (B == 0) {
			return leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor");
		}
		else {
			return leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor") + (v3_Mv * (-C / B));
		}
	}

	//弾が当たる時間のフレームを計算する
	float flame1, flame2;
	//二次方程式の解の公式の判別式で分類
	float D = (B * B) - (4 * A * C);
	if (D > 0) {
		float E = std::sqrtf(D);
		flame1 = (-B - E) / (2 * A);
		flame2 = (-B + E) / (2 * A);
		//解は2つなので正の数の最小値を使う
		flame1 = PlusMin(flame1, flame2);
		flame1 = std::clamp<float>(flame1, 0.0f, limitLeadingFrame);
	}
	else {
		//虚数解
		//当たらないので今の位置を狙う
		flame1 = 0;
	}

	// 的の未来位置
	targetFuture_ = leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor") + (v3_Mv * flame1);
	leadingTargetPos_ = leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor");

	// 方向ベクトル
	Vector3 result = (leadingTarget_->GetModel().GetJointWorldPosition("LockOnAnchor") + (v3_Mv * flame1)) - shooterPos;

	return result.Normalize();
}