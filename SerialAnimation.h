#pragma once
#include <Adapter.h>

class SerialAnimation {
public:
	// コンストラクタ
	SerialAnimation();
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

public:// アクセサ
	void Start() {};
	/// <summary>
	/// 使用する画像変更
	/// </summary>
	void ChangeSprite(const std::string& spriteName) {}

#pragma region Getter

#pragma endregion

#pragma region Setter

#pragma endregion

private:
	// 連番画像
	LWP::Primitive::ClipSprite sprite_;

	// アニメーション速度(次の画像に遷移するまでの時間)
	float nextAnimInterval_;
	// 経過時間
	float currentFrame_;
	// 待機時間
	float waitFrame_;

	// ループ再生するか
	bool isLoop_;
	// 開始
	bool isStart_;
	// 終了
	bool isEnd_;
};

