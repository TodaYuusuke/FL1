#pragma once
#include <Adapter.h>

class SerialAnimation {
public:
	// コンストラクタ
	SerialAnimation(const std::string& spriteName, const LWP::Math::Vector2& splitSize);
	// デストラクタ
	~SerialAnimation() = default;

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

public:// アクセサ
	/// <summary>
	/// 開始
	/// </summary>
	void Start(float nextAnimInterval, float waitFrame, bool isLoop = false);
	/// <summary>
	/// 使用する画像変更
	/// </summary>
	void ChangeSprite(const std::string& spriteName, const LWP::Math::Vector2& splitSize);

#pragma region Getter
	float GetCurrentFrame() { return currentFrame_; }
	bool GetIsFinish() {
		if (isEnd_ && waitFrame_ <= currentFrame_) { return true; }
		return false;
	}
	bool GetIsLoop() { return isLoop_; }
	bool GetIsStart() { return isStart_; }
#pragma endregion

#pragma region Setter
	void SetPos(const LWP::Math::Vector3& pos) { sprite_.worldTF.translation = pos; }
	void SetScale(const LWP::Math::Vector3& scale) { sprite_.worldTF.scale = scale; }
	void SetNextAnimInterval(float value) { nextAnimInterval_ = value; }
	void SetWaitFrame(float waitFrame) { waitFrame_ = waitFrame; }
	void SetIsLoop(bool isLoop) { isLoop_ = isLoop; }
#pragma endregion

private:
	// 連番画像
	LWP::Primitive::SequenceSprite sprite_;

	// アニメーション速度(次の画像に遷移するまでの時間)
	float nextAnimInterval_;
	// 経過時間
	float currentFrame_;
	// 待機時間
	float waitFrame_;

	// 
	int splitNum_;

	// ループ再生するか
	bool isLoop_;
	// 開始
	bool isStart_;
	// 終了
	bool isEnd_;
};

