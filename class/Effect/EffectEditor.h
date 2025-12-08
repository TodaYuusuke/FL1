#pragma once
#include "Emitter/Emitter.h"

// クラスの前方宣言
class EffectManager;

/// <summary>
/// エフェクトの編集、保存を行うクラス
/// </summary>
class EffectEditor
{
public: // サブクラス

	/// <summary>
	/// 粒子の移動モード列挙子
	/// </summary>
	enum MoveMode {
		VELOCITY,
		EASING
	};

	/// <summary>
	/// パーティクル保存用構造体
	/// </summary>
	struct EffectSaveData {
		int ParticleType = Emitter::ParticleType::Surface;
		std::string ModelPath = "cube.obj";
		int SurfaceType = Emitter::SurfaceType::Normal;
		std::string TexPath = "circle.png";
		float EmitAliveTime = 5.0f;
		bool IsWaitDeleteAllParticles = false;
		int EmitCount = 3;
		int MaxEmitCount = 100;
		float EmitTime = 0.1f;
		bool unificationRandomScale = false;
		LWP::Effect::RandomData<float>					EmitTimeAmp{};
		LWP::Effect::RandomData<float>					AliveTimeAmp{};
		LWP::Effect::VelocityData<LWP::Math::Vector3>	PVelocityTranslate{};
		LWP::Effect::EasingData<LWP::Math::Vector3>		PEasingTranslate{};
		LWP::Effect::VelocityData<LWP::Math::Vector3>	PVelocityRotate{};
		LWP::Effect::EasingData<LWP::Math::Vector3>		PEasingScale{};
		LWP::Effect::EasingData<LWP::Math::Vector4>		PEasingColor{};
	};

public: // コンストラクタ

	// デフォルトコンストラクタ削除
	EffectEditor() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">粒子マネージャー</param>
	EffectEditor(EffectManager* manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EffectEditor() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

private: // プライベートなメンバ関数

	/// <summary>
	/// パーティクルタイプ編集用UI
	/// </summary>
	/// <param name="type">編集対象</param>
	void EditParticleTypeUI(int& type);

	/// <summary>
	/// 平面タイプ編集用UI
	/// </summary>
	/// <param name="type">編集対象</param>
	void EditSurfaceTypeUI(int& type);

	/// <summary>
	/// イージング編集用GUI
	/// </summary>
	/// <param name="type">編集対象</param>
	void EditEasingUI(LWP::Utility::Easing::Type& type);

	/// <summary>
	/// 保存メニュー
	/// </summary>
	void SaveMenu();

	/// <summary>
	/// 読み込みメニュー(関数名がおかしいのはImGuiの関数と被るから)
	/// </summary>
	void LoadPMenu();

	/// <summary>
	/// 汎用調整項目
	/// </summary>
	void CommonTab();
	
	/// <summary>
	/// 粒子の移動に関するタブ
	/// </summary>
	void MoveTab();

	/// <summary>
	/// 粒子の回転に関するタブ
	/// </summary>
	void RotateTab();

	/// <summary>
	/// 粒子の拡縮に関するタブ
	/// </summary>
	void ScaleTab();

	/// <summary>
	/// 粒子の色に関するタブ
	/// </summary>
	void ColorTab();

	/// <summary>
	/// 粒子生成関数
	/// </summary>
	void CreateParticle();

private: // メンバ変数

	// エフェクトマネージャー
	EffectManager* effectManager_ = nullptr;

	// jsonIO
	LWP::Utility::JsonIO jsonIO_{};

	// 保存されるデフォルトディレクトリ
	std::string defaultDirectory_ = "./resources/json/Particle/";

	// 現在読み込み中のファイルパス
	std::string nowLoadFileName_ = "None";

	// 編集中データ
	EffectSaveData editData_{};

	// 移動方法の切り替えフラグ
	int moveMode_ = VELOCITY;

	// エミッタが生成される座標
	LWP::Math::Vector3 generateEmitPos_{};

	// 粒子タイプ用マップ
	static const std::array<std::string, size_t(Emitter::ParticleType::PTypeCount)> kParticleTypeMap_;

	// 平面タイプ用マップ
	static const std::array<std::string, size_t(Emitter::SurfaceType::STypeCount)> kSurfaceTypeMap_;

	// 補間選択用マップ
	static const std::array<std::string, size_t(LWP::Utility::Easing::Type::EasingCount)> kEaseingTypeMap_;

};

