#pragma once
#include "Emitter/Emitter.h"

// クラスの前方宣言
class EffectManager;

/// <summary>
/// エフェクトの編集、保存を行うクラス
/// </summary>
class EffectEditor final : public LWP::Utility::ISingleton<EffectEditor> {
	friend class LWP::Utility::ISingleton<EffectEditor>;
private: // プライベートなサブクラス

	/// <summary>
	/// 粒子の移動モード列挙子
	/// </summary>
	enum MoveMode {
		VELOCITY,
		EASING
	};

	enum PopUpMode {
		None,
		New,
		Save,
		Load
	};

public: // コンストラクタ

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EffectEditor() = default;

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

public: // アクセッサ等

	/// <summary>
	/// エフェクトマネージャーのセッター
	/// </summary>
	/// <param name="effectManager">エフェクトマネージャー</param>
	void SetEffectManager(EffectManager* effectManager) { effectManager_ = effectManager; }

private: // プライベートなメンバ関数

	/// <summary>
	/// パーティクルタイプ編集用UI
	/// </summary>
	/// <param name="type">編集対象</param>
	void EditParticleTypeGUI(int& type);

	/// <summary>
	/// 平面タイプ編集用UI
	/// </summary>
	/// <param name="type">編集対象</param>
	void EditSurfaceTypeGUI(int& type);

	/// <summary>
	/// イージング編集用GUI
	/// </summary>
	/// <param name="type">編集対象</param>
	/// <param name="eType">保存用</param>
	void EditEasingTypeGUI(LWP::Utility::Easing::Type& type, int& eType);

	/// <summary>
	/// Vector3用編集GUI
	/// </summary>
	/// <param name="id">ImGui表示名</param>
	/// <param name="data">編集対象</param>
	/// <param name="isClampZero">0でクランプするか</param>
	/// <param name="isUnification">均一化するか</param>
	void EditVector3GUI(const std::string& id, LWP::Math::Vector3& data, const bool isUnification, const bool isClampZero = false);

	/// <summary>
	/// 色用編集GUI
	/// </summary>
	/// <param name="id">ImGui表示名</param>
	/// <param name="color">編集対象</param>
	void EditColorGUI(const std::string& id, LWP::Math::Vector4& color);

	/// <summary>
	/// VelocityData用編集GUI
	/// </summary>
	/// <param name="id">ImGui表示名</param>
	/// <param name="data">編集対象</param>
	/// <param name="isUnification">値を均一化するか</param>
	void EditVelocityGUI(const std::string& id, LWP::Effect::VelocityData<LWP::Math::Vector3>& data, const bool isUnification = false);

	/// <summary>
	/// EasingData用編集GUI
	/// </summary>
	/// <param name="id">ImGui表示名</param>
	/// <param name="data">編集対象</param>
	/// <param name="isUnification">値を均一化するか</param>
	void EditEasingGUI(const std::string& id, LWP::Effect::EasingData<LWP::Math::Vector3>& data, const bool isUnification = false);

	/// <summary>
	/// ポップアップ関連の更新
	/// </summary>
	void PopUpUpdate();

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

	// 現在読み込み中のファイルパス
	std::string nowLoadName_ = "Test Particle";

	// 編集中データ
	LWP::Effect::EffectSaveData* editData_ = nullptr;

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

	// ポップアップのモード
	int popupMode_ = EffectEditor::PopUpMode::None;

	// 作成するエフェクト名
	std::string createEffectName_{};

};
