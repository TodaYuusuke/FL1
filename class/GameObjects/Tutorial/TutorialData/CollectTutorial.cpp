#include "BaseTutorialData.h"
#include "CollectTutorial.h"
#include "../../Player/Player.h"

using namespace FLMath;
using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;

CollectTutorial::CollectTutorial(Player* player, EnemyManager* enemyManager) {
	vCon_ = VirtualController::GetInstance();
	weaponManager_ = WeaponManager::GetInstance();
	player_ = player;
	enemyManager_ = enemyManager;

	centerPos_ = { LWP::Info::GetWindowWidthF() / 2.0f, 200.0f, 0.0f };

	// 開始スプライト
	introFont_.LoadTexture("UI/Tutorial/collect_tutorial.png");
	introFont_.worldTF.translation = { LWP::Info::GetWindowWidthF() / 2.0f, 200.0f, 0.0f };
	introFont_.material.color.A = 0;
	introFont_.isActive = false;

	guideFont_[CollectTutorial::CollectGuideSequence::kCollect].anchorPoint = { 0.5f, 0.5f };
	guideFont_[CollectTutorial::CollectGuideSequence::kCollect].LoadTexture("UI/Tutorial/pickup_tutorial.png");
	echoFont_[CollectTutorial::CollectGuideSequence::kCollect].LoadTexture("UI/Tutorial/pickup_tutorial.png");
	guideFont_[CollectTutorial::CollectGuideSequence::kCollect].isActive = false;

	for (auto& [key, sprite] : guideFont_) {
		echoFont_[key] = sprite;
	}

	// 落とす武器
	dropWeapons_[0] = (int)WeaponType::kMachineGun;
	dropWeapons_[1] = (int)WeaponType::kShotGun;

	// 武器の座標
	Vector3 weaponPos = player_->GetWorldTF()->GetWorldPosition();
	weaponPos.y += 20.0f;
	// 自機の向いている方向に武器を出す
	weaponPos += Vector3{ 0.0f,0.0f,1.0f } * Matrix4x4::CreateRotateXYZMatrix(player_->GetWorldTF()->rotation) * 40.0f;
	// 持たせる武器を作成
	for (int i = 0; i < dropWeapons_.size(); i++) {
		std::vector<std::string> weaponNames = weaponManager_->GetWeaponNamePreview(dropWeapons_[i]);
		IWeapon* weapon = WeaponManager::GetInstance()->CreateWeapon(dropWeapons_[i], weaponNames[0]);

		weaponManager_->DropWeapon(weapon, weaponPos);
	}
}

void CollectTutorial::Init() {
	sequence_ = CollectTutorial::CollectGuideSequence::kCollect;
	isFinish_ = false;
}

void CollectTutorial::Update() {
	for (auto& [key, sprite] : guideFont_) {
		sprite.worldTF.translation = centerPos_;
		echoFont_[key].worldTF.translation = centerPos_;

		// 該当のチュートリアル以外画像非表示
		if (key == sequence_) {
			sprite.isActive = true;
			continue;
		}
		sprite.isActive = false;
	}

	switch (sequence_) {
	case CollectTutorial::CollectGuideSequence::kIntro:
		// 開始演出
		StartEffect();

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(0.0f)) {
			sequence_ = CollectTutorial::CollectGuideSequence::kCollect;
			isNextGuide_ = false;
		}

		break;
	case CollectTutorial::CollectGuideSequence::kCollect:
		// 武器を拾ったか
		if (vCon_->GetPress(BindActionType::kCollect)) {
			for (int i = 0; i < (int)WeaponSide::kCount; i++) {
				if (!player_->GetWeaponController()->GetWeaponSlot((WeaponSide)i)->GetIsFullWeapon()) {
					isNextGuide_ = true;
					break;
				}
			}
		}

		// 成功演出
		if (isNextGuide_) { SuccessEffect(CollectTutorial::CollectGuideSequence::kCollect); }

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) {
			sequence_ = CollectTutorial::CollectGuideSequence::kCount;
			isNextGuide_ = false;
		}

		break;
	default:
		// 終了演出
		FinishEffect();

		break;
	}
}

void CollectTutorial::SuccessEffect(CollectGuideSequence target) {
	const float endFrame = 30.0f;
	if (nextGuideInterval_ >= endFrame) {
		echoFont_[target].isActive = false;
		return;
	}

	// 衝撃波
	echoFont_[target].isActive = true;
	echoFont_[target].worldTF.scale = LWP::Utility::Interp::Lerp(Vector3{ 1,1,1 }, Vector3{ 2,2,2 }, Easing::OutExpo(nextGuideInterval_ / endFrame));
	echoFont_[target].material.color.A = static_cast<int>(LWP::Utility::Interp::LerpF(200.0f, 0.0f, Easing::OutExpo(nextGuideInterval_ / endFrame)));
}