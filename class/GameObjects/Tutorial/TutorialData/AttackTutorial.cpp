#include "BaseTutorialData.h"
#include "AttackTutorial.h"
#include "../../Player/Player.h"
#include "../../Enemy/EnemyManager.h"
#include "../../Weapon/WeaponManager.h"

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

	// 連番画像
	serialAnim_ = std::make_unique<SerialAnimation>("UI/Tutorial/manual/armShot_manual.png", Vector2{ 700.0f,256.0f });
	serialAnim_->SetPos({ LWP::Info::GetWindowWidthF() / 2.0f, 350.0f, 0.0f });
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

	bool isDropedWeapon = false;
	std::map<WeaponSide, IWeapon*> weapon;
	std::vector<IWeapon*> weapons = WeaponManager::GetInstance()->GetWeapons();
	std::vector<WeaponSide> checkSide;
	switch (sequence_) {
	case AttackTutorial::AttackGuideSequence::kIntro:
		// 開始演出
		StartEffect();

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(0.0f)) {
			sequence_ = AttackTutorial::AttackGuideSequence::kHandAttack;
			isNextGuide_ = false;
			serialAnim_->Start(10.0f, 30.0f, true);
		}

		break;
	case AttackTutorial::AttackGuideSequence::kHandAttack:
		// 手の武器で攻撃したら終了
		weapon[WeaponSide::kLeft] = player_->GetWeaponController()->GetWeaponSlot(WeaponSide::kLeft)->GetFrontWeapon();
		weapon[WeaponSide::kRight] = player_->GetWeaponController()->GetWeaponSlot(WeaponSide::kRight)->GetFrontWeapon();
		if ((weapon[WeaponSide::kLeft] && weapon[WeaponSide::kLeft]->GetIsAttacking()) ||
			(weapon[WeaponSide::kRight] && weapon[WeaponSide::kRight]->GetIsAttacking())) {
			isNextGuide_ = true;
			serialAnim_->Init();
		}

		// 成功演出
		if (isNextGuide_) { SuccessEffect(AttackTutorial::AttackGuideSequence::kHandAttack); }

		// 武器を一つも持っていない状態になったら武器を生成
		checkSide = {
			WeaponSide::kLeft,
			WeaponSide::kRight
		};
		// 所持者のない武器が存在しているかを検出
		for (int i = 0; i < weapons.size(); i++) {
			if (!weapons[i]->GetActor()) {
				isDropedWeapon = true;
				break;
			}
		}
		if (!isDropedWeapon) DropWeapons(checkSide);

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) {
			sequence_ = AttackTutorial::AttackGuideSequence::kShoulderAttack;
			isNextGuide_ = false;
			serialAnim_->ChangeSprite("UI/Tutorial/manual/shoulderShot_manual.png", Vector2{ 700.0f,256.0f });
			serialAnim_->Start(10.0f, 30.0f, true);
		}

		break;
	case AttackTutorial::AttackGuideSequence::kShoulderAttack:
		// 肩の武器で攻撃したら終了
		weapon[WeaponSide::kLeftShoulder] = player_->GetWeaponController()->GetWeaponSlot(WeaponSide::kLeftShoulder)->GetFrontWeapon();
		weapon[WeaponSide::kRightShoulder] = player_->GetWeaponController()->GetWeaponSlot(WeaponSide::kRightShoulder)->GetFrontWeapon();
		if ((weapon[WeaponSide::kLeftShoulder] && weapon[WeaponSide::kLeftShoulder]->GetIsAttacking()) ||
			(weapon[WeaponSide::kRightShoulder] && weapon[WeaponSide::kRightShoulder]->GetIsAttacking())) {
			isNextGuide_ = true;
			serialAnim_->Init();
		}

		// 成功演出
		if (isNextGuide_) { SuccessEffect(AttackTutorial::AttackGuideSequence::kShoulderAttack); }

		// 武器を一つも持っていない状態になったら武器を生成
		checkSide = {
			WeaponSide::kLeftShoulder,
			WeaponSide::kRightShoulder
		};
		// 所持者のない武器が存在しているかを検出
		for (int i = 0; i < weapons.size(); i++) {
			if (!weapons[i]->GetActor()) {
				isDropedWeapon = true;
				break;
			}
		}
		if (!isDropedWeapon) DropWeapons(checkSide);

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) {
			// チュートリアル敵を生成
			enemyManager_->TutorialSpawn();
			sequence_ = AttackTutorial::AttackGuideSequence::kKill;
			isNextGuide_ = false;
		}

		break;
	case AttackTutorial::AttackGuideSequence::kKill:
		// 敵を倒したか
		if (enemyManager_->GetEnemyList().empty()) {
			isNextGuide_ = true;
			serialAnim_->Init();
		}
		// 成功演出
		if (isNextGuide_) { SuccessEffect(AttackTutorial::AttackGuideSequence::kKill); }

		// 武器を一つも持っていない状態になったら武器を生成
		checkSide = {
			WeaponSide::kLeft,
			WeaponSide::kRight,
			WeaponSide::kLeftShoulder,
			WeaponSide::kRightShoulder,
		};
		// 所持者のない武器が存在しているかを検出
		for (int i = 0; i < weapons.size(); i++) {
			if (!weapons[i]->GetActor()) {
				isDropedWeapon = true;
				break;
			}
		}
		if (!isDropedWeapon) DropWeapons(checkSide);

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

	serialAnim_->Update();
}

void AttackTutorial::DropWeapons(std::vector<WeaponSide> checkSide) {
	// 武器を一つも持っていない状態になったら武器を生成
	bool isNoneWeapon = true;
	for (int i = 0; i < checkSide.size(); i++) {
		// 武器を所持している
		if (player_->GetWeaponController()->GetWeaponSlot(checkSide[i])->GetFrontWeapon()) {
			isNoneWeapon = false;
			break;
		}
	}
	if (isNoneWeapon) {
		// 落とす武器
		std::array<int, 4> dropWeapons = {
			(int)WeaponType::kMachineGun,
			(int)WeaponType::kShotGun,
			(int)WeaponType::kMissile,
			(int)WeaponType::kLauncher
		};

		// 武器の座標
		Vector3 weaponPos = player_->GetWorldTF()->GetWorldPosition();
		weaponPos.y += 20.0f;
		// 自機の向いている方向に武器を出す
		weaponPos += Vector3{ 0.0f,0.0f,1.0f } *Matrix4x4::CreateRotateXYZMatrix(player_->GetWorldTF()->rotation) * 40.0f;
		// 持たせる武器を作成
		for (int i = 0; i < dropWeapons.size(); i++) {
			std::vector<std::string> weaponNames = WeaponManager::GetInstance()->GetWeaponNamePreview(dropWeapons[i]);
			IWeapon* weapon = WeaponManager::GetInstance()->CreateWeapon(dropWeapons[i], weaponNames[0]);

			WeaponManager::GetInstance()->DropWeapon(weapon, weaponPos);
		}
	}
}

void AttackTutorial::SuccessEffect(AttackGuideSequence target) {
	const float endFrame = 30.0f;
	if (nextGuideInterval_ >= endFrame) {
		echoFont_[target].isActive = false;
		return;
	}

	// チュートリアル成功時効果音を鳴らす
	if (!echoFont_[target].isActive) { AudioPlayer::GetInstance()->PlayAudio("Success_SE.mp3", 1.0f, LWP::AudioConfig::Other); }

	// 衝撃波
	echoFont_[target].isActive = true;
	echoFont_[target].worldTF.scale = LWP::Utility::Interp::Lerp(Vector3{ 1,1,1 }, Vector3{ 2,2,2 }, Easing::OutExpo(nextGuideInterval_ / endFrame));
	echoFont_[target].material.color.A = static_cast<unsigned char>(LWP::Utility::Interp::LerpF(200.0f, 0.0f, Easing::OutExpo(nextGuideInterval_ / endFrame)));
}