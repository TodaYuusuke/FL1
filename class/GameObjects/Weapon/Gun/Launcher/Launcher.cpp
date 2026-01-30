#include "Launcher.h"

using namespace FLMath;

Launcher::Launcher(WeaponData gunData)
	: IGun(gunData)
{
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
