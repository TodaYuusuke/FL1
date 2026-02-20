#include "Player.h"
#include "../Collision/CollisionMask.h"
#include "../Weapon/WeaponManager.h"
#include "../World/IWorld.h"
#include <algorithm>

using namespace LWP;
using namespace LWP::Utility;
using namespace LWP::Math;
using namespace LWP::Object;

Player::Player(FollowCamera* camera, IWorld* world, const LWP::Math::Vector3& centerPos) {
	// 初期位置の設定
	tag_ = "Player";
	name_ = "Player";
	attackPower_ = 10;
	centerPos_ = centerPos;
	world_ = world;

	// 黒板生成
	blackBoard_ = new BlackBoard();
	blackBoard_->SetValue<Actor*>("Player", this);

	// モデル生成
	model_.LoadShortPath("Player/Player.gltf");
	model_.Update();
	model_.materials["EmissionMaterial"].enableLighting = false;
	for (auto key : model_.skeleton.jointMap) {
		if (!key.first.empty()) jointName_.push_back(key.first);
	}

	// 体の判定生成
	bodyCollision_.SetFollow(&model_.worldTF);
	bodyCollision_.isActive = true;
	bodyCollision_.worldTF.translation = { 0.0f, 1.0f, 0.0f };
	// 自機の所属しているマスクを設定
	bodyCollision_.mask.SetBelongFrag(GameMask::player);
	// 当たり判定をとる対象のマスクを設定
	bodyCollision_.mask.SetHitFrag(GameMask::attack);
	bodyCollision_.enterLambda = [this](LWP::Object::Collision* hitTarget) {
		OnCollision(hitTarget);
		};
	bodyAABB_.min = { -3.0f, -0.5f, -1.0f };
	bodyAABB_.max = { 3.0f, 9.5f, 1.0f };

	// jsonファイルの読み込みor作成
	json_.Init(kJsonFileDirectoryPath + "Player.json")
		// 武器
		.BeginGroup("Weapon")
		// 手
		.BeginGroup("Hand")
		.BeginGroup("Left")
		.AddValue<int>("Type", &weaponTypies_[(int)WeaponSide::kLeft])
		.AddValue<int>("Name", &weaponNamesIndex_[(int)WeaponSide::kLeft])
		.EndGroup()
		.BeginGroup("Right")
		.AddValue<int>("Type", &weaponTypies_[(int)WeaponSide::kRight])
		.AddValue<int>("Name", &weaponNamesIndex_[(int)WeaponSide::kRight])
		.EndGroup()
		.EndGroup()
		// 肩
		.BeginGroup("Shoulder")
		.BeginGroup("Left")
		.AddValue<int>("Type", &weaponTypies_[(int)WeaponSide::kLeftShoulder])
		.AddValue<int>("Name", &weaponNamesIndex_[(int)WeaponSide::kLeftShoulder])
		.EndGroup()
		.BeginGroup("Right")
		.AddValue<int>("Type", &weaponTypies_[(int)WeaponSide::kRightShoulder])
		.AddValue<int>("Name", &weaponNamesIndex_[(int)WeaponSide::kRightShoulder])
		.EndGroup()
		.EndGroup()
		.EndGroup()
		// 当たり判定
		.BeginGroup("Collider")
		.AddValue<float>("Max", &bodyAABB_.max.x)
		.AddValue<float>("Min", &bodyAABB_.min.x)
		.EndGroup()
		// HP
		.AddValue<float>("Value", &maxHp_)
		.BeginGroup("HP")
		.AddValue<LWP::Math::Vector4>("Color", &hpGaugeColor_)
		.AddValue<LWP::Math::Vector3>("Position", &hpGaugePosition_)
		.AddValue<LWP::Math::Vector3>("Scale", &hpGaugeScale_)
		.AddValue<float>("Rotate", &hpGaugeRotate_)
		.EndGroup()
		.CheckJsonFile();

	// 偏差射撃機能
	leadingSystem_ = std::make_unique<LeadingSystem>(camera->GetCamera(), blackBoard_);

	// 移動系統の管理
	moveController_ = std::make_unique<MoveController>(blackBoard_);
	// 武器系統の管理
	weaponController_ = std::make_unique<WeaponController>(leadingSystem_.get(), this);
	weaponController_->SetCenterDist(centerPos_);
	// デバッグ用の武器の持ち主を設定
	weaponController_->SetDebugWeaponOwner(this);

	// HP
	hp_ = std::make_unique<Health>(maxHp_);

	// 初期武器を設定
	void (Player:: * setWeapon[(int)WeaponSide::kCount])(IWeapon*) = {
		&Player::SetLeftWeapon,
		&Player::SetRightWeapon,
		&Player::SetLeftShoulderWeapon,
		&Player::SetRightShoulderWeapon
	};
	for (int i = 0; i < weaponTypies_.size(); i++) {
		// 武器なしは含めない
		if (weaponTypies_[i] >= (int)WeaponType::kCount) { continue; }

		// 持たせる武器を作成
		std::vector<std::string> weaponNames = WeaponManager::GetInstance()->GetWeaponNamePreview(weaponTypies_[i]);
		IWeapon* weapon = WeaponManager::GetInstance()->CreateWeapon(weaponTypies_[i], weaponNames[weaponNamesIndex_[i]]);
		// 武器の付与
		(this->*setWeapon[i])(weapon);
	}

	// アニメーションマネージャー作成
	animManager_ = std::make_unique<RobotAnimManager>("resources/model/Player/Player.gltf", &model_, GetVelocity(), &model_.worldTF.rotation);
	animManager_->Init();

	// 各アニメーション再生
	animManager_->PlayQue("Other_Idle", RobotAnimManager::PlayType::Other, 0.0f, true);
	animManager_->PlayQue("Idle", RobotAnimManager::PlayType::LeftArm, 0.0f, true);
	animManager_->PlayQue("Idle", RobotAnimManager::PlayType::RightArm, 0.0f, true);
	animManager_->PlayQue("Idle", RobotAnimManager::PlayType::LeftShoulder, 0.0f, true);
	animManager_->PlayQue("Idle", RobotAnimManager::PlayType::RightShoulder, 0.0f, true);

	//HP
	hpGauge_ = std::make_unique<SingleGauge>();
	hpGauge_->Initialize(true);
}

Player::~Player() {
	delete blackBoard_;
}

void Player::Init() {
	leadingSystem_->Init();
	moveController_->Init();
	weaponController_->Init();
}

void Player::Update() {
	//HP表示
	hpGauge_->SetRatio(GetHP()->GetHealth() / GetHP()->GetMaxHealth());
	hpGauge_->SetAnchor(hpGaugePosition_);
	hpGauge_->SetSize(hpGaugeScale_);
	hpGauge_->SetRotate(hpGaugeRotate_);
	hpGauge_->SetColor(hpGaugeColor_);
	hpGauge_->Update();

	// RGBずらしが有効、かつ演出中でなければ
	RGBShift* shiftEffector = CameraEffectHandler::GetInstance()->GetRGBShiftEffector();
	if (shiftEffector->GetIsState() != 1 && !shiftEffector->GetIsStaging()) {
		shiftEffector->Finish(0.5f);
	}

	if (hp_->GetIsDead()) {
		// 全てのループSEの再生停止
		moveController_->StopAllLoopSE();

		isAlive_ = false;
		return;
	}
	else {
		isAlive_ = true;
	}

	// HP
	hp_->Update();

	// 偏差射撃
	leadingSystem_->Update();

	// 移動系統の行動
	moveController_->Update();
	// 武器由来の行動
	weaponController_->Update();

	weaponVel_.x = std::clamp<float>(weaponVel_.x, -5.0f, 5.0f);
	weaponVel_.y = std::clamp<float>(weaponVel_.y, -5.0f, 5.0f);
	weaponVel_.z = std::clamp<float>(weaponVel_.z, -5.0f, 5.0f);
	
	// 速度適用
	velocity_ = moveController_->GetVel() + weaponVel_;
	// 座標
	model_.worldTF.translation += velocity_;
	// 移動制限
	world_->LimitMoveArea(model_.worldTF.translation);

	// 演出用の角度
	effectRot_ = moveController_->GetEffectRot();

	// 体の向きを決める
	AdjustRotate();

	model_.worldTF.rotation *= effectRot_;

	weaponVel_ = { 0.0f,0.0f,0.0f };

	// アニメーション更新
	animManager_->Update();
}

void Player::DrawGui() {
	if (ImGui::TreeNode("Player")) {

		// 武器
		if (ImGui::TreeNode("Default Weapon")) {
			// 調整項目
			if (ImGui::Button("Save")) {
				json_.Save();
			}
			ImGui::SameLine();
			if (ImGui::Button("Load")) {
				json_.Load();
			}
			weaponTypePreview_ = WeaponManager::GetInstance()->GetWeaponTypePreview();
			weaponTypePreview_.push_back("None");
			//weaponRarityPreview_ = WeaponManager::GetInstance()->GetWeaponNamePreview();

			// 手武器を選択
			if (ImGui::TreeNode("Select hand weapon")) {
				std::vector<std::string> lWeaponNamePreview;
				std::vector<std::string> rWeaponNamePreview;
				bool isClickCombo = false;
				int preType;

				// 左の武器種選択
				preType = weaponTypies_[(int)WeaponSide::kLeft];
				SelectType(weaponTypePreview_,weaponTypies_[(int)WeaponSide::kLeft], "Left##0", isClickCombo);
				// タブクリック時の処理
				if (isClickCombo) {
					if (weaponTypies_[(int)WeaponSide::kLeft] != preType) { weaponNamesIndex_[(int)WeaponSide::kLeft] = 0; }
				}
				// 武器名の選択
				lWeaponNamePreview = WeaponManager::GetInstance()->GetWeaponNamePreview(weaponTypies_[(int)WeaponSide::kLeft]);
				SelectType(lWeaponNamePreview, weaponNamesIndex_[(int)WeaponSide::kLeft], "Left##1");

				// 右の武器種選択
				preType = weaponTypies_[(int)WeaponSide::kRight];
				SelectType(weaponTypePreview_, weaponTypies_[(int)WeaponSide::kRight], "Right##0", isClickCombo);
				// タブクリック時の処理
				if (isClickCombo) {
					if (weaponTypies_[(int)WeaponSide::kRight] != preType) { weaponNamesIndex_[(int)WeaponSide::kRight] = 0; }
				}
				// 武器名の選択
				rWeaponNamePreview = WeaponManager::GetInstance()->GetWeaponNamePreview(weaponTypies_[(int)WeaponSide::kRight]);
				SelectType(rWeaponNamePreview, weaponNamesIndex_[(int)WeaponSide::kRight], "Right##1");

				ImGui::TreePop();
			}
			// 肩武器を選択
			if (ImGui::TreeNode("Select shoulder weapon")) {
				std::vector<std::string> lWeaponNamePreview;
				std::vector<std::string> rWeaponNamePreview;
				bool isClickCombo = false;
				int preType;

				// 左の武器種選択
				preType = weaponTypies_[(int)WeaponSide::kLeftShoulder];
				SelectType(weaponTypePreview_, weaponTypies_[(int)WeaponSide::kLeftShoulder], "Left##2", isClickCombo);
				// タブクリック時の処理
				if (isClickCombo) {
					if (weaponTypies_[(int)WeaponSide::kLeftShoulder] != preType) { weaponNamesIndex_[(int)WeaponSide::kLeftShoulder] = 0; }
				}
				// 武器名の選択
				lWeaponNamePreview = WeaponManager::GetInstance()->GetWeaponNamePreview(weaponTypies_[(int)WeaponSide::kLeftShoulder]);
				SelectType(lWeaponNamePreview, weaponNamesIndex_[(int)WeaponSide::kLeftShoulder], "Left##3");

				// 右の武器種選択
				preType = weaponTypies_[(int)WeaponSide::kRightShoulder];
				SelectType(weaponTypePreview_, weaponTypies_[(int)WeaponSide::kRightShoulder], "Right##2", isClickCombo);
				// タブクリック時の処理
				if (isClickCombo) {
					if (weaponTypies_[(int)WeaponSide::kRightShoulder] != preType) { weaponNamesIndex_[(int)WeaponSide::kRightShoulder] = 0; }
				}
				// 武器名の選択
				rWeaponNamePreview = WeaponManager::GetInstance()->GetWeaponNamePreview(weaponTypies_[(int)WeaponSide::kRightShoulder]);
				SelectType(rWeaponNamePreview, weaponNamesIndex_[(int)WeaponSide::kRightShoulder], "Right##3");

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		// 体力
		if (ImGui::TreeNode("HP")) {
			// 調整項目
			if (ImGui::Button("Save")) {
				json_.Save();
			}
			ImGui::SameLine();
			if (ImGui::Button("Load")) {
				json_.Load();
			}
			ImGui::DragFloat("Value", &maxHp_, 0.01f);
			ImGui::DragFloat3("Position", &hpGaugePosition_.x, 1.0f);
			ImGui::DragFloat3("Scale", &hpGaugeScale_.x, 0.01f);
			ImGui::DragFloat("Rotate", &hpGaugeRotate_, 0.01f);
			ImGui::ColorEdit4("Color",&hpGaugeColor_.x,0);
			ImGui::TreePop();
		}
		// 当たり判定
		if (ImGui::TreeNode("Collider")) {
			// 調整項目
			if (ImGui::Button("Save")) {
				json_.Save();
			}
			ImGui::SameLine();
			if (ImGui::Button("Load")) {
				json_.Load();
			}
			ImGui::DragFloat3("Min", &bodyAABB_.min.x, 0.01f);
			ImGui::DragFloat3("Max", &bodyAABB_.max.x, 0.01f);
			ImGui::TreePop();
		}
		// 武器
		weaponController_->DebugGui();

		// モデル
		if (ImGui::TreeNode("Model")) {
			model_.DebugGUI();
			ImGui::TreePop();
		}

		// 行動
		if (ImGui::TreeNode("Actions")) {
			moveController_->DebugGui();
			ImGui::TreePop();
		}
		// 機能
		if (ImGui::TreeNode("Systems")) {
			leadingSystem_->DebugGui();
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	static bool isDebugMode = false;
	if (ImGui::Button("DebugMode:ON")) {
		isDebugMode = true;
		bodyCollision_.isActive = false;
		isAlive_ = true;
		hp_->SetHealth(999999);
	}
	ImGui::SameLine();
	if (ImGui::Button("DebugMode:OFF")) {
		isDebugMode = false;
		bodyCollision_.isActive = true;
		hp_->SetHealth(maxHp_);
	}
	if (isDebugMode) ImGui::Text("DebugMode:ON");
	else ImGui::Text("DebugMode:OFF");
}

void Player::OnCollision(LWP::Object::Collision* hitTarget) {
	//if (hitTarget->mask.GetHitFrag() != bodyCollision_.mask.GetBelongFrag()) { return; }
	hp_->SetIsHit(true);
	// 多重被弾回避
	std::vector<std::string> name = hp_->GetDamageAttackerName();
	if (!name.empty()) {
		auto result = std::find(name.begin(), name.end(), hitTarget->name);
		if (result != name.end()) {
			return;
		}
	}

	// ダメージ値取得
	float damageValue = world_->FindAttackPower(hitTarget->name);
	// 最大ダメージを元に補間用Tを求める
	float damageT = damageValue / 50.0f;

	// 振動強度を求める
	float shakeStrength = LWP::Utility::Interp::LerpF(0.025f, 0.07f, damageT);
	// カメラ振動
	CameraEffectHandler::GetInstance()->StartShake({ shakeStrength, shakeStrength, 0.0f }, 0.5f);
	// ずらし強度の最大値を求める
	float rgbshiftStrength = LWP::Utility::Interp::LerpF(0.025f, 0.1f, damageT);
	// ずらし強度をランダムに求める
	LWP::Math::Vector3 randomRGB = LWP::Utility::Random::GenerateVector3(LWP::Math::Vector3(-rgbshiftStrength, -rgbshiftStrength, -rgbshiftStrength), LWP::Math::Vector3(rgbshiftStrength, rgbshiftStrength, rgbshiftStrength));
	// ずらし強度設定
	CameraEffectHandler::GetInstance()->GetRGBShiftEffector()->SetMaxStrength(randomRGB);
	// ずらしエフェクト開始
	CameraEffectHandler::GetInstance()->GetRGBShiftEffector()->Start(0.1f);

	// 被弾音を鳴らす
	AudioPlayer::GetInstance()->PlayRandomAudio("HitSound.mp3", 4, 1.0f, AudioConfig::Enviroment);

	// ダメージを受ける
	hp_->Damage(damageValue, hitTarget->name);
}

void Player::PlayAttackAnim(int weaponSide)
{
	// 武器の情報取得
	if (weaponController_->GetWeaponSlot(static_cast<WeaponSide>(weaponSide)) == nullptr) { return; }
	WeaponData data = weaponController_->GetWeaponSlot(static_cast<WeaponSide>(weaponSide))->GetFrontWeapon()->GetWeaponData();

	// アニメーション名と効果音名の取得
	std::string animName = data.animName;
	// アニメーション名がない場合
	if (animName == "") { return; }

	// 武器タイプがミサイルか近接武器の場合アニメーション名変更
	// ミサイルか近接武器の場合待機アニメーションを変更する
	if (data.type == static_cast<int>(WeaponType::kMissile)) {
		animName = "Tilt" + animName;
	}

	std::string seName = data.attackSEFileName;

	// 射撃アニメーション再生
	animManager_->PlayDirect(animName, weaponSide + 2)
		.AddEvent("PlaySE", 1, [this, weaponSide, seName]() { AudioPlayer::GetInstance()->PlayAudio(seName, 1.0f, LWP::AudioConfig::Player); });

	// ミサイルか近接武器の場合待機アニメーションを変更する
	if (data.type == static_cast<int>(WeaponType::kMissile) || data.type == static_cast<int>(WeaponType::kMelee)) {
		// 待機アニメーションをキューに入れる
		animManager_->PlayQue("TiltIdle", weaponSide + 2);
	}
	else {
		// 待機アニメーションをキューに入れる
		animManager_->PlayQue("Idle", weaponSide + 2);
	}
}

void Player::PlayPickUpAnim(const int weaponSide, const WeaponData& data)
{
	// 取得アニメーション再生
	animManager_->PlayDirect("PickUp", weaponSide + 2)
		.AddEvent("PlaySE", 1, [&]() { AudioPlayer::GetInstance()->PlayAudio("WeaponPickUp.mp3", 1.0f, LWP::AudioConfig::Player); });

	// ミサイルか近接武器の場合待機アニメーションを変更する
	if (data.type == static_cast<int>(WeaponType::kMissile) || data.type == static_cast<int>(WeaponType::kMelee)) {
		// 待機アニメーションをキューに入れる
		animManager_->PlayQue("TiltIdle", weaponSide + 2);
	}
	else {
		// 待機アニメーションをキューに入れる
		animManager_->PlayQue("Idle", weaponSide + 2);
	}
}

void Player::AdjustRotate() {
	Quaternion result{ 0,0,0,1 };
	Quaternion qCurr = model_.worldTF.rotation;
	Quaternion qNext{ 0,0,0,1 };
	float t = 0.3f;
	moveRot_ *= moveController_->GetRot();

	if (leadingSystem_->GetLeadingTarget() && preMoveRot_ == moveRot_) {
		if (!isTriggerLockOn_) moveRot_ = (Quaternion{ 0,0,0,1 });

		t = 0.6f;
		Vector3 targetPos = leadingSystem_->GetLeadingTarget()->GetModel()->GetJointWorldPosition("LockOnAnchor");
		Vector3 playerDir = (Vector3{ 0,0,1 } *Matrix4x4::CreateRotateXYZMatrix(model_.worldTF.rotation)).Normalize();
		playerDir.y = 0.0f;
		Vector3 targetDir = (targetPos - model_.worldTF.GetWorldPosition()).Normalize();
		targetDir.y = 0.0f;

		float dot = Vector3::Dot(playerDir, targetDir);
		float crossY = playerDir.x * targetDir.z - playerDir.z * targetDir.x;

		float signedAngle = atan2(crossY, dot);

		Quaternion q = Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, -signedAngle);
		qNext = (q * qCurr * moveRot_).Normalize();
		lockOnOmega_ = qNext;

		isTriggerLockOn_ = true;
	}
	else {
		if (isTriggerLockOn_) moveRot_ = (Quaternion{ 0,0,0,1 });
		qNext = (lockOnOmega_ * moveRot_).Normalize();

		isTriggerLockOn_ = false;
	}

	result = Interp::SlerpQuaternion(qCurr, qNext, t);
	quat_ = result;
	model_.worldTF.rotation = result;

	preMoveRot_ = moveRot_;
	preLockOnOmega_ = lockOnOmega_;
}

void Player::SelectType(std::vector<std::string> list, int& selectedType, std::string label, bool& isClickCombo) {
	isClickCombo = false;
	if (!list.empty()) {
		const char* combo_preview_value = list[selectedType].c_str();
		if (ImGui::BeginCombo(label.c_str(), combo_preview_value)) {
			for (int n = 0; n < list.size(); n++) {
				const bool is_selected = ((int)selectedType == n);
				std::string selectableLabel = list[n];
				if (ImGui::Selectable(selectableLabel.c_str(), is_selected)) {
					selectedType = n;
					isClickCombo = true;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else {
		ImGui::TextDisabled(("Not found element"));
	}
}

void Player::SelectType(std::vector<std::string> list, int& selectedType, std::string label) {
	if (!list.empty()) {
		const char* combo_preview_value = list[selectedType].c_str();
		if (ImGui::BeginCombo(label.c_str(), combo_preview_value)) {
			for (int n = 0; n < list.size(); n++) {
				const bool is_selected = ((int)selectedType == n);
				std::string selectableLabel = list[n];
				if (ImGui::Selectable(selectableLabel.c_str(), is_selected)) {
					selectedType = n;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else {
		ImGui::TextDisabled(("Not found element"));
	}
}

void Player::SelectWeaponType(int& selectedWeaponType, std::string label) {
	// 読み込むbehaviorTreeのプレビュー作成
	if (!weaponTypePreview_.empty()) {
		const char* combo_preview_value = weaponTypePreview_[selectedWeaponType].c_str();
		if (ImGui::BeginCombo(label.c_str(), combo_preview_value)) {
			for (int n = 0; n < weaponTypePreview_.size(); n++) {
				const bool is_selected = ((int)selectedWeaponType == n);
				std::string selectableLabel = weaponTypePreview_[n];
				if (ImGui::Selectable(selectableLabel.c_str(), is_selected)) {
					selectedWeaponType = n;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else {
		ImGui::TextDisabled(("Not found weapon"));
	}
}

void Player::SelectWeaponRarity(int& selectedWeaponRarity, std::string label) {
	// 読み込むbehaviorTreeのプレビュー作成
	if (!weaponRarityPreview_.empty()) {
		const char* combo_preview_value = weaponRarityPreview_[selectedWeaponRarity].c_str();
		if (ImGui::BeginCombo(label.c_str(), combo_preview_value)) {
			for (int n = 0; n < weaponRarityPreview_.size(); n++) {
				const bool is_selected = ((int)selectedWeaponRarity == n);
				std::string selectableLabel = weaponRarityPreview_[n];
				if (ImGui::Selectable(selectableLabel.c_str(), is_selected)) {
					selectedWeaponRarity = n;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else {
		ImGui::TextDisabled(("Not found weapon"));
	}
}

LWP::Math::Vector3 Player::GetCenterPosition() {
	return model_.worldTF.GetWorldPosition() + centerPos_ * LWP::Math::Matrix4x4::CreateRotateXYZMatrix(model_.worldTF.rotation);
}
