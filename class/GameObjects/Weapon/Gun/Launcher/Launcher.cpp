#include "Launcher.h"

using namespace FLMath;

Launcher::Launcher(WeaponData gunData)
	: IGun(gunData)
{
	// 射撃エフェクト生成
	attackEffectEmitter_ = EffectManager::GetInstance()->CreateNewEmitter("MuzzleFlash", { 0.0f, 0.0f, 0.0f }, true);
	// 射撃エフェクトを武器モデルのマズルに親子付け
	attackEffectEmitter_->SetParent(&body_, "Muzzle");
	// 粒子の自動生成OFF
	attackEffectEmitter_->SetIsAutoEmit(false);
}

void Launcher::Destroy() {

}

void Launcher::SetWeaponSide(int weaponSide) {
	// 向きのある武器の名前を決める
	std::string side{};
	// 左側
	if (weaponSide == (int)WeaponSide::kLeft || weaponSide == (int)WeaponSide::kLeftShoulder) {
		side = "_L";
	}
	// 右側
	else if (weaponSide == (int)WeaponSide::kRight || weaponSide == (int)WeaponSide::kRightShoulder) {
		side = "_R";
	}

	// モデル名
	std::string modelName = data_.modelName;
	// 向き名を差し込む
	std::size_t pos = modelName.find('.');
	if (pos != std::string::npos && pos > 0) {
		modelName.insert(pos, side);
	}
	// モデル再読み込み
	body_.LoadFullPath(modelName);
}
