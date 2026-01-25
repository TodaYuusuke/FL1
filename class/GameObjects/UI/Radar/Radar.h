#pragma once
#include <Adapter.h>

class Radar final : public LWP::Utility::ISingleton<Radar> {
	friend class LWP::Utility::ISingleton<Radar>;// ISingletonをフレンドクラスにしてコンストラクタを呼び出せるように
public:

	//レーダー用ユニットの区分
	enum UnitType
	{
		PLAYER,
		ENEMY,
		WEAPON,
		COUNT
	};

	struct UnitData
	{
		LWP::Math::Vector3 position;
		UnitType type;
	};


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

	void SetPlayerTransform(LWP::Object::TransformQuat* transform) { playerTransform_ = transform; };
	void SetParent(LWP::Object::TransformQuat* transform) { centerTransform_.Parent(transform); };

	//フレームの最初に呼ぶ
	void ClearVector() { unitDatas_.clear(); };

	/// <summary>
	/// 表示要素追加
	/// </summary>
	void AppendTarget(LWP::Math::Vector3 position, UnitType type) { unitDatas_.push_back({position,type}); };
	static void AppendTargetWeapon(LWP::Math::Vector3 position) {GetInstance()->AppendTarget(position, WEAPON); };
	static void AppendTargetEnemy(LWP::Math::Vector3 position) { GetInstance()->AppendTarget(position, ENEMY); };

private:
	Radar() {};
	~Radar() {};

	// jsonファイルのディレクトリパス
	const std::string kJsonDirectoryPath = "MiniMap/";
	LWP::Utility::JsonIO json_;
	float mapScale_;
	float viewBorder_;

	//プレイヤーの位置(これを基準に計算する)
	LWP::Primitive::Cube player_;
	const static size_t kMaxModels_ = 30;
	std::array<LWP::Primitive::Cube, kMaxModels_> unitModels_;
	std::vector<UnitData> unitDatas_;
	std::array<LWP::Utility::Color, UnitType::COUNT> colorSample_;
	//プレイヤーの座標
	LWP::Object::TransformQuat* playerTransform_;
	LWP::Object::TransformQuat centerTransform_;

	std::function<void(LWP::Math::Vector3)> appendEnemy_;
};
