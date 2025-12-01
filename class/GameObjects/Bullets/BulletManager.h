#pragma once
#include <Adapter.h>
#include "BulletBase.h"
#include "Melee/MeleeAttack.h"
#include "BulletConfig.h"

using BulletCreator = std::function<BulletBase* (const BulletData&, const LWP::Math::Vector3&, int, const LWP::Math::Vector3&)>;
using MeleeCreator = std::function<BulletBase* (const BulletData&, LWP::Object::TransformQuat*, int)>;
/// <summary>
/// 弾管理クラス
/// </summary>
class BulletManager final : public LWP::Utility::ISingleton<BulletManager> {
	friend class LWP::Utility::ISingleton<BulletManager>;// ISingletonをフレンドクラスにしてコンストラクタを呼び出せるように
private:
	// コンストラクタ
	BulletManager();
	// デストラクタ
	~BulletManager();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

private:
	/// <summary>
	/// jsonファイル作成
	/// </summary>
	/// <param name="json"></param>
	/// <param name="data"></param>
	/// <param name="name"></param>
	void CreateJsonData(LWP::Utility::JsonIO& json, BulletData& data, const std::string& name);
	/// <summary>
	/// 選択した弾の調整項目を表示
	/// </summary>
	/// <param name="json"></param>
	/// <param name="data"></param>
	void SelectBulletDataGui(LWP::Utility::JsonIO& json, BulletData& data);
	/// <summary>
	/// 弾を選択
	/// </summary>
	/// <param name="selectedType"></param>
	/// <param name="label"></param>
	void SelectBulletType(int& selectedType, std::string label);

	/// <summary>
	/// 弾の生成
	/// </summary>
	void AddBullet(BulletBase* bullet, float attackMultiply);

public:// アクセサ
	/// <summary>
	/// 特定の弾を検索
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	BulletBase* FindBullet(const std::string& name) {
		auto result = std::find_if(bullets_.begin(), bullets_.end(),
			[&](BulletBase* bullet) { return bullet->GetName() == name; }
		);
		if (result == bullets_.end()) return nullptr;

		return *result;
	}

	template<class T>
	void BulletRegister(int type) {
		bulletCreators_[type] = [](const BulletData& data, const LWP::Math::Vector3& pos, int hitFragBit, const LWP::Math::Vector3& dir = { 0,0,0 }) {
			return new T(data, pos, hitFragBit, dir);  // ← 派生クラスを new
			};
	}
	template<class U>
	void MeleeRegister(int type) {
		meleeCreators_[type] = [](const BulletData& data, LWP::Object::TransformQuat* target, int hitFragBit) {
			return new U(data, target, hitFragBit);  // ← 派生クラスを new
			};
	}

	/// <summary>
	/// 弾生成
	/// </summary>
	/// <param name="type">種類(BulletType参照)</param>
	/// <param name="pos">生成座標</param>
	/// <param name="hitFragBit">当たり判定をとる対象のマスク</param>
	/// <param name="dir">方向ベクトル</param>
	void CreateBullet(int type, const LWP::Math::Vector3& pos, int hitFragBit, const LWP::Math::Vector3& dir = { 0,0,0 }, float attackMultiply = 1.0f) {
		if (bulletCreators_.contains(type)) {
			AddBullet(bulletCreators_[type](sampleData_[type], pos, hitFragBit, dir), attackMultiply);
		}
	}
	/// <summary>
	/// 近距離攻撃生成
	/// </summary>
	/// <param name="type">種類(BulletType参照)</param>
	/// <param name="target">攻撃対象</param>
	/// <param name="hitFragBit">当たり判定をとる対象のマスク</param>
	void CreateMelee(int type, LWP::Object::TransformQuat* target, int hitFragBit, float attackMultiply = 1.0f) {
		if (meleeCreators_.contains(type)) {
			AddBullet(meleeCreators_[type](sampleData_[type], target, hitFragBit), attackMultiply);
		}
	}

#pragma region Getter
	/// <summary>
	/// 調整できる弾一覧
	/// </summary>
	/// <returns></returns>
	std::vector<std::string> GetBulletTypePreview() { return bulletTypePreview_; }
#pragma endregion

#pragma region Setter

#pragma endregion

private:// 定数
	// jsonファイルのディレクトリパス
	const std::string kJsonDirectoryPath = "Bullets/";

private:// 調整項目
	// 弾のコピー元
	std::map<int, BulletData> sampleData_;
	std::map<int, LWP::Utility::JsonIO> jsonDatas_;

	// 調整できる弾一覧
	std::vector<std::string> bulletTypePreview_;

private:
	std::unordered_map<int, BulletCreator> bulletCreators_;
	std::unordered_map<int, MeleeCreator> meleeCreators_;

	// 弾リスト
	std::list<BulletBase*> bullets_;

	int createID_;
};