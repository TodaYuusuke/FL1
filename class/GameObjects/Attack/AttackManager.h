#pragma once
#include <Adapter.h>
#include "BulletBase.h"
#include "AttackBase.h"
#include "Melee/MeleeAttack.h"
#include "AttackConfig.h"

using BulletCreator = std::function<BulletBase* (const AttackData&, Actor*, const LWP::Math::Vector3&, int, const LWP::Math::Vector3&)>;
using ImpactCreator = std::function<AttackBase* (const ImpactData&, const LWP::Math::Vector3&, int)>;
using MeleeCreator = std::function<BulletBase* (const AttackData&, LWP::Object::TransformQuat*, int)>;
/// <summary>
/// 弾管理クラス
/// </summary>
class AttackManager final : public LWP::Utility::ISingleton<AttackManager> {
	friend class LWP::Utility::ISingleton<AttackManager>;// ISingletonをフレンドクラスにしてコンストラクタを呼び出せるように
private:
	// コンストラクタ
	AttackManager();
	// デストラクタ
	~AttackManager();

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
	/// ゲームシーンの最後に呼ぶ処理
	/// </summary>
	void EndFrame();
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
	void CreateJsonData(LWP::Utility::JsonIO& json, AttackData& data, const std::string& name);
	void CreateJsonData(LWP::Utility::JsonIO& json, ImpactData& data, const std::string& name);
	/// <summary>
	/// 選択した弾の調整項目を表示
	/// </summary>
	/// <param name="json"></param>
	/// <param name="data"></param>
	void SelectBulletDataGui(LWP::Utility::JsonIO& json, AttackData& data);
	void SelectBulletDataGui(LWP::Utility::JsonIO& json, ImpactData& data);
	/// <summary>
	/// 弾を選択
	/// </summary>
	/// <param name="selectedType"></param>
	/// <param name="label"></param>
	void SelectBulletType(int& selectedType, std::string label);
	void SelectType(std::vector<std::string> list, int& selectedType, std::string label);
	/// <summary>
	/// 弾属性を選択
	/// </summary>
	/// <param name="selectedType"></param>
	/// <param name="label"></param>
	void SelectBulletElement(int& selectedType, std::string label);

	/// <summary>
	/// 弾の生成
	/// </summary>
	void AddBullet(AttackBase* attack, float attackMultiply, int belongFragBit);

public:// アクセサ
	/// <summary>
	/// 特定の弾を検索
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	AttackBase* FindBullet(const std::string& name) {
		auto result = std::find_if(attackList_.begin(), attackList_.end(),
			[&](AttackBase* attack) { return attack->GetName() == name; }
		);
		if (result == attackList_.end()) return nullptr;

		return *result;
	}

	template<class T>
	void BulletRegister(int type) {
		bulletCreators_[type] = [](const AttackData& data, Actor* target, const LWP::Math::Vector3& pos, int hitFragBit, const LWP::Math::Vector3& dir = { 0,0,0 }) {
			return new T(data, target, pos, hitFragBit, dir);  // ← 派生クラスを new
			};
	}
	template<class U>
	void MeleeRegister(int type) {
		meleeCreators_[type] = [](const AttackData& data, LWP::Object::TransformQuat* target, int hitFragBit) {
			return new U(data, target, hitFragBit);  // ← 派生クラスを new
			};
	}
	template<class V>
	void ImpactRegister(int type) {
		impactCreators_[type] = [](const ImpactData& data, const LWP::Math::Vector3& pos, int hitFragBit) {
			return new V(data, pos, hitFragBit);  // ← 派生クラスを new
			};
	}

	/// <summary>
	/// 弾生成
	/// </summary>
	/// <param name="type">種類(BulletType参照)</param>
	/// <param name="pos">生成座標</param>
	/// <param name="hitFragBit">当たり判定をとる対象のマスク</param>
	/// <param name="dir">方向ベクトル</param>
	void CreateAttack(int type, Actor* target, const LWP::Math::Vector3& pos, int hitFragBit, int belongFragBit, const LWP::Math::Vector3& dir = { 0,0,0 }, float attackMultiply = 1.0f) {
		if (bulletCreators_.contains(type)) {
			AddBullet(bulletCreators_[type](sampleBulletDatas_[type], target, pos, hitFragBit, dir), attackMultiply, belongFragBit);
		}
	}
	/// <summary>
	/// 近距離攻撃生成
	/// </summary>
	/// <param name="type">種類(BulletType参照)</param>
	/// <param name="target">攻撃対象</param>
	/// <param name="hitFragBit">当たり判定をとる対象のマスク</param>
	void CreateAttack(int type, LWP::Object::TransformQuat* target, int hitFragBit, int belongFragBit, float attackMultiply = 1.0f) {
		if (meleeCreators_.contains(type)) {
			AddBullet(meleeCreators_[type](sampleBulletDatas_[type], target, hitFragBit), attackMultiply, belongFragBit);
		}
	}
	/// <summary>
	/// 着弾時の攻撃生成(爆発など)
	/// </summary>
	/// <param name="type">種類(BulletType参照)</param>
	/// <param name="target">攻撃対象</param>
	/// <param name="hitFragBit">当たり判定をとる対象のマスク</param>
	void CreateImpact(int type, const LWP::Math::Vector3& pos, int hitFragBit, int belongFragBit, float attackMultiply = 1.0f) {
		if (impactCreators_.contains(type)) {
			AddBullet(impactCreators_[type](sampleImpactDatas_[type], pos, hitFragBit), attackMultiply, belongFragBit);
		}
	}

#pragma region Getter
	/// <summary>
	/// 調整できる弾一覧
	/// </summary>
	/// <returns></returns>
	std::vector<std::string> GetBulletTypePreview() { return bulletTypePreview_; }
	/// <summary>
	/// 着弾時の処理を取得
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	ImpactData GetImpactData(int type) { return sampleImpactDatas_[type]; }
#pragma endregion

#pragma region Setter

#pragma endregion

private:// 定数
	// jsonファイルのディレクトリパス
	const std::string kJsonDirectoryPath = "Bullets/";

private:// 調整項目
	// 弾のコピー元
	std::map<int, AttackData> sampleBulletDatas_;
	std::map<int, LWP::Utility::JsonIO> jsonBulletDatas_;
	// 着弾時の処理のコピー元
	std::map<int, ImpactData> sampleImpactDatas_;
	std::map<int, LWP::Utility::JsonIO> jsonImpactDatas_;
	// 属性のコピー元
	//std::map<int, AttackData> sampleElementData_;
	//std::map<int, LWP::Utility::JsonIO> elementJsonDatas_;

	// 調整できる弾一覧
	std::vector<std::string> bulletTypePreview_;
	std::vector<std::string> impactTypePreview_;
	std::vector<std::string> movementTypePreview_;
	std::vector<std::string> bulletElementPreview_;

private:
	std::unordered_map<int, BulletCreator> bulletCreators_;
	std::unordered_map<int, ImpactCreator> impactCreators_;
	std::unordered_map<int, MeleeCreator> meleeCreators_;

	// 弾リスト(その他攻撃も含む)
	//std::list<BulletBase*> bullets_;
	std::list<AttackBase*> attackList_;

	int createID_;
};