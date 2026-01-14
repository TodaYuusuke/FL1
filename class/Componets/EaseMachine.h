#pragma once
#include <Adapter.h>
#include <list>
#include <assert.h>
#include <functional>
#include <variant>

class EaseMachine {
public: // 構造体
	// アニメーションを行うときのパラメータ
	struct AnimData {
		std::variant<LWP::Math::Vector4*, LWP::Math::Vector3*, LWP::Math::Vector2*, float*, int*> target;	// アニメーションの対象
		float currentFrame;													// 現在のフレーム
		float startFrame;													// アニメーション開始フレーム
		float endFrame;														// アニメーション終了フレーム
		float t;															// 変化量
		std::variant<LWP::Math::Vector4, LWP::Math::Vector3, LWP::Math::Vector2, float, int> start;			// 始めの値
		std::variant<LWP::Math::Vector4, LWP::Math::Vector3, LWP::Math::Vector2, float, int> end;			// 終了時の値
		bool isActive;  													// アニメーションをしているか
		bool isTimeScale = true;											// 時間の流れの速さを適用するか
		int id;																// アニメーションの番号
		std::function<float(float)> easeFunc;								// 使用するイージング関数
	};

public: // メンバ関数
	// コンストラクタ
	EaseMachine();
	// デストラクタ
	~EaseMachine() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	//void Start();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

public:// アクセサ
	// 全てのアニメーションを削除
	void AllClearAnimation() { animData_.clear(); }

	// アニメーション情報をリセット
	void ResetData() {
		// 全てのデータを初期化
		for (std::list<AnimData>::iterator it = animData_.begin(); it != animData_.end(); ++it) {
			it->t = 0;
			it->currentFrame = 0;
			it->isActive = false;
		}
		isEnd_ = false;
		isStart_ = false;
	}

#pragma region Getter
	// アニメーションを始めるかを取得
	bool GetIsStart() { return isStart_; }
	// アニメーションが終了しているかを取得
	bool GetIsEnd() { return isEnd_; }
#pragma endregion

#pragma region Setter
	// アニメーションを始めるかを設定
	void SetIsStart(bool isStart) {
		if (isStart_ != isStart) {
			for (std::list<AnimData>::iterator it = animData_.begin(); it != animData_.end(); ++it) {
				// 最初のアニメーションを起動
				if (it->id == 0) {
					it->isActive = isStart;
					break;
				}
			}
		}
		isStart_ = isStart;
	}

	/// <summary>
	/// アニメーションに必要なパラメータをリストに登録
	/// </summary>
	/// <param name="target">アニメーションする対象</param>
	/// <param name="start">最初のパラメータ</param>
	/// <param name="end">終了時のパラメータ</param>
	/// <param name="endFrame">アニメーション終了時間</param>
	/// <param name="easeFunc">使用するイージング関数</param>
	void SetAnimData(std::variant<LWP::Math::Vector4*, LWP::Math::Vector3*, LWP::Math::Vector2*, float*, int*> target, std::variant<LWP::Math::Vector4, LWP::Math::Vector3, LWP::Math::Vector2, float, int> start, std::variant<LWP::Math::Vector4, LWP::Math::Vector3, LWP::Math::Vector2, float, int> end, float endFrame, std::function<float(float)> easeFunc, bool isTimeScale = true);
	// アニメーションに必要なパラメータを先頭のリスト情報を上書き
	// 無い場合はリストに登録
	void SetFirstAnimData(std::variant<LWP::Math::Vector4*, LWP::Math::Vector3*, LWP::Math::Vector2*, float*, int*> target, std::variant<LWP::Math::Vector4, LWP::Math::Vector3, LWP::Math::Vector2, float, int> start, std::variant<LWP::Math::Vector4, LWP::Math::Vector3, LWP::Math::Vector2, float, int> end, float endFrame, std::function<float(float)> easeFunc, bool isTimeScale = true);
	/// <summary>
	/// 任意のアニメーション情報を上書き
	/// </summary>
	/// <param name="index">対象の要素番号</param>
	/// <param name="target">アニメーションする対象</param>
	/// <param name="start">最初のパラメータ</param>
	/// <param name="end">終了時のパラメータ</param>
	/// <param name="endFrame">アニメーション終了時間</param>
	/// <param name="easeFunc">使用するイージング関数</param>
	void AnimDataOverride(int index, std::variant<LWP::Math::Vector4*, LWP::Math::Vector3*, LWP::Math::Vector2*, float*, int*> target, std::variant<LWP::Math::Vector4, LWP::Math::Vector3, LWP::Math::Vector2, float, int> start, std::variant<LWP::Math::Vector4, LWP::Math::Vector3, LWP::Math::Vector2, float, int> end, float endFrame, std::function<float(float)> easeFunc, bool isTimeScale = true);
#pragma endregion

private: // メンバ変数
	std::list<AnimData> animData_;
	// アニメーションの順番
	int animId_;
	// アニメーションスタート
	bool isStart_;
	// アニメーションが終了しているか
	bool isEnd_;
};