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

	guideFont_[AttackTutorial::AttackGuideSequence::kLeftAttack].anchorPoint = { 0.5f, 0.5f };
	guideFont_[AttackTutorial::AttackGuideSequence::kLeftAttack].LoadTexture("UI/Tutorial/shot_tutorial.png");
	echoFont_[AttackTutorial::AttackGuideSequence::kLeftAttack].LoadTexture("UI/Tutorial/shot_tutorial.png");
	guideFont_[AttackTutorial::AttackGuideSequence::kLeftAttack].isActive = false;

	guideFont_[AttackTutorial::AttackGuideSequence::kRightAttack].anchorPoint = { 0.5f, 0.5f };
	guideFont_[AttackTutorial::AttackGuideSequence::kRightAttack].LoadTexture("UI/Tutorial/shot_tutorial.png");
	echoFont_[AttackTutorial::AttackGuideSequence::kRightAttack].LoadTexture("UI/Tutorial/shot_tutorial.png");
	guideFont_[AttackTutorial::AttackGuideSequence::kRightAttack].isActive = false;

	guideFont_[AttackTutorial::AttackGuideSequence::kLeftShoulderAttack].anchorPoint = { 0.5f, 0.5f };
	guideFont_[AttackTutorial::AttackGuideSequence::kLeftShoulderAttack].LoadTexture("UI/Tutorial/shot_tutorial.png");
	echoFont_[AttackTutorial::AttackGuideSequence::kLeftShoulderAttack].LoadTexture("UI/Tutorial/shot_tutorial.png");
	guideFont_[AttackTutorial::AttackGuideSequence::kLeftShoulderAttack].isActive = false;

	guideFont_[AttackTutorial::AttackGuideSequence::kRightShoulderAttack].anchorPoint = { 0.5f, 0.5f };
	guideFont_[AttackTutorial::AttackGuideSequence::kRightShoulderAttack].LoadTexture("UI/Tutorial/shot_tutorial.png");
	echoFont_[AttackTutorial::AttackGuideSequence::kRightShoulderAttack].LoadTexture("UI/Tutorial/shot_tutorial.png");
	guideFont_[AttackTutorial::AttackGuideSequence::kRightShoulderAttack].isActive = false;

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
			sequence_ = AttackTutorial::AttackGuideSequence::kKill;
			isNextGuide_ = false;
		}

		break;
	case AttackTutorial::AttackGuideSequence::kLeftAttack:
		// 攻撃したか
		if (player_->GetWeaponController()->GetWeaponSlot(WeaponSide::kLeft)->GetFrontWeapon()->GetIsAttack()) {
			isNextGuide_ = true;
		}

		// 成功演出
		if (isNextGuide_) { SuccessEffect(AttackTutorial::AttackGuideSequence::kLeftAttack); }

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) {
			sequence_ = AttackTutorial::AttackGuideSequence::kRightAttack;
			isNextGuide_ = false;
		}

		break;
	case AttackTutorial::AttackGuideSequence::kRightAttack:
		// 攻撃したか
		if (player_->GetWeaponController()->GetWeaponSlot(WeaponSide::kLeft)->GetFrontWeapon()->GetIsAttack()) {
			isNextGuide_ = true;
		}

		// 成功演出
		if (isNextGuide_) { SuccessEffect(AttackTutorial::AttackGuideSequence::kRightAttack); }

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) {
			sequence_ = AttackTutorial::AttackGuideSequence::kLeftShoulderAttack;
			isNextGuide_ = false;
		}

		break;
	case AttackTutorial::AttackGuideSequence::kLeftShoulderAttack:
		// 攻撃したか
		if (player_->GetWeaponController()->GetWeaponSlot(WeaponSide::kLeft)->GetFrontWeapon()->GetIsAttack()) {
			isNextGuide_ = true;
		}

		// 成功演出
		if (isNextGuide_) { SuccessEffect(AttackTutorial::AttackGuideSequence::kLeftShoulderAttack); }

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) {
			sequence_ = AttackTutorial::AttackGuideSequence::kRightShoulderAttack;
			isNextGuide_ = false;
		}

		break;
	case AttackTutorial::AttackGuideSequence::kRightShoulderAttack:
		// 攻撃したか
		if (player_->GetWeaponController()->GetWeaponSlot(WeaponSide::kLeft)->GetFrontWeapon()->GetIsAttack()) {
			isNextGuide_ = true;
		}

		// 成功演出
		if (isNextGuide_) { SuccessEffect(AttackTutorial::AttackGuideSequence::kRightShoulderAttack); }

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) {
			// 敵出現
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