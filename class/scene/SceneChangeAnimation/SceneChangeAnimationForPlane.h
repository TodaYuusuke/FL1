#pragma once

#include "Adapter.h"

class SceneChangeAnimationPlane
{
public:
	SceneChangeAnimationPlane() {};
	~SceneChangeAnimationPlane() {};

	void Initialize();

	/// <summary>
	/// アニメーション開始
	/// </summary>
	/// <param name="type">0:in 1:out</param>
	void Start(int type);

	void Start(int type, std::function<IScene* ()> func);

	void Update();

	bool GetIsPlay() { return isPlay_; };

	void SetAnimationLength(int length) { animationLength_ = length; };

	void SetParent(LWP::Object::TransformQuat* parent);

	void DebugGui();

private:
	const LWP::Math::Vector2 kTextureSize_ = { 480.0f,270.0f };
	static const size_t kSpritFrame_ = 7;//アニメーション分割数
	static const size_t kSpriteNum_ = 4;
	std::array<LWP::Primitive::SequenceSurface, kSpriteNum_> sprites_;
	int frame_;
	int rect_;//再生向き
	bool isPlay_;
	int animationLength_;//アニメーション再生時間(フレーム)
	int type_;


	// jsonファイルのディレクトリパス
	const std::string kJsonDirectoryPath = "SceneChange/";
	LWP::Utility::JsonIO json_;
};
