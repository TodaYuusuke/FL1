#pragma once
#include <Adapter.h>
#include <vector>

class EasingEditor {
public:
	// コンストラクタ
	EasingEditor();
	// デストラクタ
	~EasingEditor() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	/// <summary>
	/// 曲線の計算
	/// </summary>
	/// <param name="p0"></param>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <param name="p3"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	LWP::Math::Vector2 CatmullRomSpline(const LWP::Math::Vector2& p0, const LWP::Math::Vector2& p1, const LWP::Math::Vector2& p2, const LWP::Math::Vector2& p3, float t);
	LWP::Math::Vector2 CatmullRomSpline(const std::vector<LWP::Math::Vector2>& controlPoints, float t);

public:
	/// <summary>
	/// 作成したイージングを使用
	/// </summary>
	/// <param name="t"></param>
	/// <returns></returns>
	float UseEasing(float t) { return CatmullRomSpline(values_, t).y; }

private:
	size_t segment_ = 100;

	const std::string kDirectoryPath = "Easings/";

	LWP::Utility::JsonIO json_;

private:
	std::vector<LWP::Math::Vector2> values_;
};

