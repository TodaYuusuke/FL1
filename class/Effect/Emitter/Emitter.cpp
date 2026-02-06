#include "Emitter.h"

using namespace LWP::Math;
using namespace LWP::Primitive;

Emitter::Emitter(LWP::Resource::Texture texID, int surfaceType, const LWP::Math::Vector3& pos)
{
	// テクスチャIDの取得
	tex_ = texID;
	// 平面タイプの設定
	particleType_ = Surface;
	// 平面タイプの取得
	surfaceType_ = surfaceType;

	// エミッタ座標の初期設定
	transform_.translation = pos;
}

Emitter::Emitter(const std::string& path, const LWP::Math::Vector3& pos)
{
	// ファイルパスの取得
	modelPath_ = path;

	// 平面タイプの設定
	particleType_ = Model3D;

	// エミッタ座標の初期設定
	transform_.translation = pos;
}

Emitter::Emitter(LWP::Resource::Texture texID, int surfaceType, const LWP::Math::Vector3& pos, const LWP::Math::Vector2& splitSize, float animTime, bool isAnimLoop)
{
	// テクスチャIDの取得
	tex_ = texID;
	// タイプ指定
	particleType_ = Sequence;
	// 平面タイプの取得
	surfaceType_ = surfaceType;

	// エミッタ座標の初期設定
	transform_.translation = pos;

	// 各種変数の値受け取り
	splitSize_ = splitSize;
	animTime_ = animTime;
	isAnimLoop_ = isAnimLoop;
}

Emitter::~Emitter()
{
	// 全ての粒子要素の解放
	for (IParticle* p : particles_) {
		delete p;
	}
	// 配列クリア
	particles_.clear();
}

Emitter& Emitter::Init(const float aliveTime, const float emitTime, const int32_t emitCount, const int32_t maxEmitCount)
{
	// 各種値の設定
	aliveTime_		= aliveTime;	// エミッタ自体の生存秒数
	emitTime_		= emitTime;		// 生成秒数
	emitCount_		= emitCount;	// 一度に生成される個数
	maxEmitCount_	= maxEmitCount; // 最大生成数

	// タイマーの開始
	aliveTimer_.Start(aliveTime_);
	emitTimer_.Start(emitTime_);

	// 自身を返す
	return *this;
}

void Emitter::Update(const float deltaTime, const float playSpeed)
{
	// 生存時間を超過したパーティクルを削除
	particles_.remove_if([&](IParticle* p) {
		// 粒子の終了フラグがTrueのとき
		if (p->GetIsEnd()) {
			// ポインタ解放
			delete p;
			return true;
		}

		// 粒子の更新を行う
		p->Update(deltaTime, playSpeed);
		return false;
	});

	// 生存時間超過時
	if (aliveTimer_.GetIsFinish()) {
		// 全てのパーティクルが終了まで待機するか
		if (isWaitDeleteAllParticles_) {
			// 生成するフラグが立っている場合これ以上生成しないように
			if (isEmit_) { isEmit_ = false; }

			// 配列内のパーティクルが全て消えたら終了
			if (particles_.size() <= 0) {
				isEnd_ = true;
				// これ以降の処理を無視
				return;
			}
		}
		else {
			// 強制終了
			isEnd_ = true;
			// これ以降の処理を無視
			return;
		}
	}

	// 粒子生成のフラグが立っている場合
	if (isEmit_) {
		// 粒子を生成
		Emit();
		
		// 生成フラグfalse
		isEmit_ = false;
		// 生成時間の振れ幅を元に生成秒数を求める
		float time = emitTime_ + emitTimeAmp_.Random();
		// 生成秒数が0以下の場合は0にする
		if (time < 0) { time = 0; }
		// 生成タイマーを再度開始
		emitTimer_.Start(time);
	}

	// 生成タイマー終了時
	if (emitTimer_.GetIsFinish()) {
		// 粒子生成の開始
		if (!isEmit_) { isEmit_ = true; }
	}
	
	// 各種タイマーの更新
	if(!isInfinite_){ aliveTimer_.Update(); }
	if(isAutoEmit_){ emitTimer_.Update(); }
}

void Emitter::SetParent(LWP::Resource::SkinningModel* model, const std::string& jointName)
{
	if (model != nullptr) {
		// 親子付け処理
		transform_.Parent(model, jointName);
	}
	else {
		// 親子付け処理
		transform_.ClearParent();
	}
}

Emitter& Emitter::SetRotation(const LWP::Math::Quaternion& rotation)
{
	// 回転角を行列から求める
	transform_.rotation = rotation;

	return *this;
}

Emitter& Emitter::SetIsInfinite(const bool isInfinite)
{
	// 無限状態
	isInfinite_ = isInfinite;

	return *this;
}

Emitter& Emitter::SetIsWaitDeleteAllParticles(const bool isWait)
{
	// フラグの状態変更
	isWaitDeleteAllParticles_ = isWait;

	return *this;
}

Emitter& Emitter::SetEmitTimeAmp(const float minTime, const float maxTime)
{
	// 各データの受け渡し
	emitTimeAmp_.min = minTime;
	emitTimeAmp_.max = maxTime;

	return *this;
}

Emitter& Emitter::SetParticleAliveTimeAmp(const float minTime, const float maxTime)
{
	// 各データの受け渡し
	pAliveTime_.min = minTime;
	pAliveTime_.max = maxTime;

	return *this;
}

Emitter& Emitter::SetTranslateVelocity(const LWP::Effect::VelocityData<LWP::Math::Vector3>& data)
{
	// 各データの受け渡し
	pVelocityPos_ = data;

	// フラグ切り替え
	pVelocityPos_.isUsed = true;
	pEasingPos_.isUsed = false;

	return *this;
}

Emitter& Emitter::SetTranslateEasing(const LWP::Effect::EasingData<LWP::Math::Vector3>& data)
{
	// 各データの受け渡し
	pEasingPos_ = data;

	// フラグ切り替え
	pEasingPos_.isUsed = true;
	pVelocityPos_.isUsed = false;

	return *this;
}

Emitter& Emitter::SetRotateVelocity(const LWP::Effect::VelocityData<LWP::Math::Vector3>& data)
{
	// 各データの受け渡し
	pVelocityRotate_ = data;

	return *this;
}

Emitter& Emitter::SetScaleEasing(const LWP::Effect::EasingData<LWP::Math::Vector3>& data, const bool isUnificationScale)
{
	// 各データの受け渡し
	pEasingScale_ = data;
	// フラグの受け取り
	unificationRandomScale_ = isUnificationScale;

	return *this;
}

Emitter& Emitter::SetColorEasing(const LWP::Effect::EasingData<LWP::Math::Vector4>& data)
{
	// 各データの受け渡し
	pEasingColor_ = data;

	return *this;
}

void Emitter::Emit()
{
	// エミッタのワールド座標格納用
	Vector3 emitWorldPos{};

	// 生成個数を元に粒子生成
	for (int i = 0; i < emitCount_; i++) {
		// 最大生成数を超過していた場合はループを破棄する
		if (particles_.size() >= maxEmitCount_) { break; }
		
		// 生成されるパーティクルの種類によって処理を分岐
		switch (particleType_)
		{
		case Emitter::Surface: // 平面
			EmitSurface();
			break;
		case Emitter::Sequence: // 連番
			EmitSequence();
			break;
		case Emitter::Model3D: // 3Dモデル
			EmitModel();
			break;
		}
	}
}

void Emitter::EmitSurface()
{
	// 空の平面を用意
	std::unique_ptr<IPlane> generatePlane = nullptr;

	// 生成する平面のタイプによって処理を分岐
	switch (surfaceType_)
	{
	case Emitter::Normal: // 通常平面
		generatePlane = std::make_unique<LWP::Primitive::NormalSurface>();
		break;
	case Emitter::Billboard: // ビルボード平面
		generatePlane = std::make_unique<LWP::Primitive::NormalBillboard2D>();
		break;
	case Emitter::HorizontalBillboard: // 縦軸固定ビルボード
		generatePlane = std::make_unique<LWP::Primitive::NormalHorizontalBillboard>();
		break;
	case Emitter::VerticalBillboard: // 横軸固定ビルボード
		generatePlane = std::make_unique<LWP::Primitive::NormalVerticalBillboard>();
		break;
	case Emitter::StretchBillboard:	// ストレッチビルボード
		// ストレッチビルボードの生成
		generatePlane = std::make_unique<LWP::Primitive::NormalStretchedBillboard>();
		break;
	}

	// 平面テクスチャの変更
	generatePlane->material.texture = tex_;

	if (surfaceType_ == StretchBillboard) {};

	// パーティクルインスタンス生成
	PlaneParticle* newParticle = new PlaneParticle(std::move(generatePlane), surfaceType_ == StretchBillboard);
	// 初期化、データの受け渡し
	newParticle->Init()
		.SetParent(&transform_)
		.SetAliveTime(pAliveTime_.Random())
		.SetRotateVelocity(pVelocityRotate_.Convert())
		.SetScaleEasing(pEasingScale_.Convert(), unificationRandomScale_)
		.SetColorEasing(pEasingColor_.Convert());

	// 個別設定
	if (pVelocityPos_.isUsed) {
		newParticle->SetVelocity(pVelocityPos_.Convert());
	}
	else {
		newParticle->SetEasing(pEasingPos_.Convert());
	}

	// パーティクルを配列に追加
	particles_.push_back(newParticle);
}

void Emitter::EmitSequence()
{
	// 空の平面を用意
	std::unique_ptr<IPlane> generatePlane = nullptr;
	int* index = nullptr;

	// 生成する平面のタイプによって処理を分岐
	switch (surfaceType_)
	{
	case Emitter::Normal: // 通常平面
	{
		std::unique_ptr<LWP::Primitive::SequenceSurface> ss = std::make_unique<LWP::Primitive::SequenceSurface>();
		ss->SetSplitSize(splitSize_);
		index = &ss->index;
		generatePlane = std::move(ss);
		break;
	}
	case Emitter::Billboard: // ビルボード平面
	{
		std::unique_ptr<LWP::Primitive::SequenceBillboard2D> sb2D = std::make_unique<LWP::Primitive::SequenceBillboard2D>();
		sb2D->SetSplitSize(splitSize_);
		index = &sb2D->index;
		generatePlane = std::move(sb2D);
		break;
	}
	case Emitter::HorizontalBillboard: // 縦軸固定ビルボード
	{
		std::unique_ptr<LWP::Primitive::SequenceHorizontalBillboard> shb = std::make_unique<LWP::Primitive::SequenceHorizontalBillboard>();
		shb->SetSplitSize(splitSize_);
		index = &shb->index;
		generatePlane = std::move(shb);
		break;
	}
	case Emitter::VerticalBillboard: // 横軸固定ビルボード
	{
		std::unique_ptr<LWP::Primitive::SequenceVerticalBillboard> svb = std::make_unique<LWP::Primitive::SequenceVerticalBillboard>();
		svb->SetSplitSize(splitSize_);
		index = &svb->index;
		generatePlane = std::move(svb);
		break;
	}
		
	case Emitter::StretchBillboard:	// ストレッチビルボード
	{
		// ストレッチビルボードの生成
		std::unique_ptr<LWP::Primitive::SequenceStretchedBillboard> ssb = std::make_unique<LWP::Primitive::SequenceStretchedBillboard>();
		ssb->SetSplitSize(splitSize_);
		index = &ssb->index;
		generatePlane = std::move(ssb);
		break;
	}		
	}

	// 平面テクスチャの変更
	generatePlane->material.texture = tex_;

	// パーティクルインスタンス生成
	SequenceParticle* newParticle = new SequenceParticle(std::move(generatePlane), index, splitSize_, animTime_, isAnimLoop_, surfaceType_ == StretchBillboard);
	// 初期化、データの受け渡し
	newParticle->Init()
		.SetParent(&transform_)
		.SetAliveTime(pAliveTime_.Random())
		.SetRotateVelocity(pVelocityRotate_.Convert())
		.SetScaleEasing(pEasingScale_.Convert(), unificationRandomScale_)
		.SetColorEasing(pEasingColor_.Convert());

	// 個別設定
	if (pVelocityPos_.isUsed) {
		newParticle->SetVelocity(pVelocityPos_.Convert());
	}
	else {
		newParticle->SetEasing(pEasingPos_.Convert());
	}

	// パーティクルを配列に追加
	particles_.push_back(newParticle);
}

void Emitter::EmitModel()
{
	// モデルロード
	std::unique_ptr<LWP::Resource::RigidModel> generateModel = std::make_unique<LWP::Resource::RigidModel>();
	generateModel->LoadShortPath(modelPath_);

	// パーティクルインスタンス生成
	ModelParticle* newParticle = new ModelParticle(std::move(generateModel));
	// 初期化、データの受け渡し
	newParticle->Init()
		.SetParent(&transform_)
		.SetAliveTime(pAliveTime_.Random())
		.SetRotateVelocity(pVelocityRotate_.Convert())
		.SetScaleEasing(pEasingScale_.Convert(), unificationRandomScale_)
		.SetColorEasing(pEasingColor_.Convert());

	// 個別設定
	if (pVelocityPos_.isUsed) {
		newParticle->SetVelocity(pVelocityPos_.Convert());
	}
	else {
		newParticle->SetEasing(pEasingPos_.Convert());
	}

	// パーティクルを配列に追加
	particles_.push_back(newParticle);
}
