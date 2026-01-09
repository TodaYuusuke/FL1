#include "BaseTutorialData.h"
#include "AttackTutorial.h"
#include "../../Player/Player.h"
#include "../../Enemy/EnemyManager.h"

using namespace FLMath;
using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;

AttackTutorial::AttackTutorial(Player* player, EnemyManager* enemyManager) {
	vCon_ = VirtualController::GetInstance();
	player_ = player;
	enemyManager_ = enemyManager;

	centerPos_ = { LWP::Info::GetWindowWidthF() / 2.0f, 200.0f, 0.0f };

	// 開始スプライト
	introFont_.LoadTexture("UI/Tutorial/attack_tutorial.png");
	introFont_.worldTF.translation = { LWP::Info::GetWindowWidthF() / 2.0f, 200.0f, 0.0f };
	introFont_.material.color.A = 0;
	introFont_.isActive = false;

	guideFont_[AttackTutorial::AttackGuideSequence::kHandAttack].anchorPoint = { 0.5f, 0.5f };
	guideFont_[AttackTutorial::AttackGuideSequence::kHandAttack].LoadTexture("UI/Tutorial/attackArm_tutorial.png");
	echoFont_[AttackTutorial::AttackGuideSequence::kHandAttack].LoadTexture("UI/Tutorial/attackArm_tutorial.png");
	guideFont_[AttackTutorial::AttackGuideSequence::kHandAttack].isActive = false;

	guideFont_[AttackTutorial::AttackGuideSequence::kShoulderAttack].anchorPoint = { 0.5f, 0.5f };
	guideFont_[AttackTutorial::AttackGuideSequence::kShoulderAttack].LoadTexture("UI/Tutorial/attackShoulder_tutorial.png");
	echoFont_[AttackTutorial::AttackGuideSequence::kShoulderAttack].LoadTexture("UI/Tutorial/attackShoulder_tutorial.png");
	guideFont_[AttackTutorial::AttackGuideSequence::kShoulderAttack].isActive = false;

	guideFont_[AttackTutorial::AttackGuideSequence::kKill].anchorPoint = { 0.5f, 0.5f };
	guideFont_[AttackTutorial::AttackGuideSequence::kKill].LoadTexture("UI/Tutorial/shot_tutorial.png");
	echoFont_[AttackTutorial::AttackGuideSequence::kKill].LoadTexture("UI/Tutorial/shot_tutorial.png");
	guideFont_[AttackTutorial::AttackGuideSequence::kKill].isActive = false;

	for (auto& [key, sprite] : guideFont_) {
		echoFont_[key] = sprite;
	}
}

void AttackTutorial::Init() {
	sequence_ = AttackTutorial::AttackGuideSequence::kIntro;
	isFinish_ = false;
}

void AttackTutorial::Update() {
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
	case AttackTutorial::AttackGuideSequence::kIntro:
		// 開始演出
		StartEffect();

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(0.0f)) {
			sequence_ = AttackTutorial::AttackGuideSequence::kHandAttack;
			isNextGuide_ = false;
		}

		break;
	case AttackTutorial::AttackGuideSequence::kHandAttack:
		// 攻撃したか
		if (player_->GetWeaponController()->GetWeaponSlot(WeaponSide::kLeft)->GetFrontWeapon()->GetIsAttacking() ||
			player_->GetWeaponController()->GetWeaponSlot(WeaponSide::kRight)->GetFrontWeapon()->GetIsAttacking()) {
			isNextGuide_ = true;
		}

		// 成功演出
		if (isNextGuide_) { SuccessEffect(AttackTutorial::AttackGuideSequence::kHandAttack); }

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) {
			sequence_ = AttackTutorial::AttackGuideSequence::kShoulderAttack;
			isNextGuide_ = false;
		}

		break;
	case AttackTutorial::AttackGuideSequence::kShoulderAttack:
		// 攻撃したか
		if (player_->GetWeaponController()->GetWeaponSlot(WeaponSide::kLeftShoulder)->GetFrontWeapon()->GetIsAttacking() ||
			player_->GetWeaponController()->GetWeaponSlot(WeaponSide::kRightShoulder)->GetFrontWeapon()->GetIsAttacking()) {
			isNextGuide_ = true;
		}

		// 成功演出
		if (isNextGuide_) { SuccessEffect(AttackTutorial::AttackGuideSequence::kShoulderAttack); }

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) {
			// チュートリアル敵を生成
			enemyManager_->TutorialSpawn();
			sequence_ = AttackTutorial::AttackGuideSequence::kKill;
			isNextGuide_ = false;
		}

		break;
	case AttackTutorial::AttackGuideSequence::kKill:
		// 武器を拾ったか
		if (enemyManager_->GetEnemyList().empty()) {
			isNextGuide_ = true;
		}

		// 成功演出
		if (isNextGuide_) { SuccessEffect(AttackTutorial::AttackGuideSequence::kKill); }

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) {
			sequence_ = AttackTutorial::AttackGuideSequence::kCount;
			isNextGuide_ = false;
		}

		break;
	default:
		// 終了演出
		FinishEffect();

		break;
	}
}

void AttackTutorial::SuccessEffect(AttackGuideSequence target) {
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