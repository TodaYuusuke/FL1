#include "EffectManager.h"

using namespace LWP::Math;
using namespace LWP::Utility;

EffectManager::~EffectManager()
{
	// 全てのエミッタの削除
	for (Emitter* e : emitters_) {
		delete e;
	}
	// 配列クリア
	emitters_.clear();

	// 全てのデータの削除
	for (auto& [key, ptr] : effectDatas_) {
		delete ptr;
	}
	// 配列クリア
	effectDatas_.clear();
}

void EffectManager::Init()
{
	// 全てのエミッタの削除
	for (Emitter* e : emitters_) {
		delete e;
	}
	// 配列クリア
	emitters_.clear();

	// 全てのデータの削除
	for (auto& [key, ptr] : effectDatas_) {
		delete ptr;
	}
	// 配列クリア
	effectDatas_.clear();

	// jsonIOの初期化
	jsonIO_.Init(filePath_);

	// エフェクトデータのロード
	LoadEffectData();
}

void EffectManager::Update()
{
	// デルタタイム取得
	float deltaTime = LWP::Info::GetDeltaTimeF();

	// 生存時間を超過したエミッタを削除
	emitters_.remove_if([&](Emitter* e) {
		// 粒子の終了フラグがTrueのとき
		if (e->GetIsEnd()) {
			// ポインタ解放
			delete e;
			return true;
		}

		// 粒子の更新を行う
		e->Update(deltaTime, playSpeed_);
		return false;
	});
}

void EffectManager::DebugGUI()
{
	
}

Emitter* EffectManager::CreateNewEmitter(std::string effectName, const LWP::Math::Vector3& pos, bool isInfinite, LWP::Object::TransformQuat* parent)
{
	// 新規に生成しようとしている対象が配列内に存在しない場合
	if (!effectDatas_.contains(effectName)) {
		// 早期リターン
		return nullptr;
	}

	// データの値を取得
	LWP::Effect::EffectSaveData data = *effectDatas_[effectName];

	//空のエミッタ生成
	Emitter* newEmitter = nullptr;

	// 生成される粒子の種類によって処理を分岐
	switch (data.ParticleType)
	{
	case Emitter::Surface: // 平面
		newEmitter = new Emitter(LWP::Resource::LoadTexture("Particle/" + data.TexPath), data.SurfaceType, pos);
		break;
	case Emitter::Sequence: // 連番
		newEmitter = new Emitter(LWP::Resource::LoadTexture("Particle/" + data.TexPath), data.SurfaceType, pos, data.SplitSize, data.AnimTime, data.IsAnimLoop);
		break;
	case Emitter::Model3D: // 3Dモデル
		newEmitter = new Emitter(data.ModelPath, pos);
		break;
	}

	// エミッタ初期化
	newEmitter->Init(data.EmitAliveTime, data.EmitTime, data.EmitCount, data.MaxEmitCount)
		.SetIsInfinite(isInfinite)
		.SetIsWaitDeleteAllParticles(data.IsWaitDeleteAllParticles)
		.SetParticleAliveTimeAmp(data.AliveTimeAmp.min, data.AliveTimeAmp.max)
		.SetRotateVelocity(data.PVelocityRotate)
		.SetScaleEasing(data.PEasingScale, data.unificationRandomScale)
		.SetColorEasing(data.PEasingColor);

	// 位置座標に関しては個別で設定
	if (data.PVelocityTranslate.isUsed) {
		newEmitter->SetTranslateVelocity(data.PVelocityTranslate);
	}
	else {
		newEmitter->SetTranslateEasing(data.PEasingTranslate);
	}

	// 親のトランスフォームが渡されている場合親子付けを行う
	if (parent != nullptr) {
		newEmitter->SetParent(parent);
	}

	// 生成したエミッタを配列に追加
	emitters_.push_back(newEmitter);

	// 生成したエミッタを返す
	return emitters_.back();
}

LWP::Effect::EffectSaveData* EffectManager::CreateNewData(const std::string& dataName)
{
	// 新規に生成しようとしている対象が配列内に存在しない場合
	if (!effectDatas_.contains(dataName)) {
		// 新規生成
		effectDatas_.emplace(dataName, new LWP::Effect::EffectSaveData());
	}

	// ポインタを返す
	return effectDatas_[dataName];
}

std::vector<std::string> EffectManager::GetDatas()
{
	// 結果返還用
	std::vector<std::string> result{};

	// キーを配列に代入していく
	for (auto& [key, ptr] : effectDatas_) {
		result.push_back(key);
	}
	// 結果を返還
	return result;
}

void EffectManager::SaveAllData()
{
	// jsonIOの初期化
	jsonIO_.Init(filePath_);

	// 配列内のデータ全てをjsonに追加
	for (auto& [key, ptr] : effectDatas_) {
		ptr->AddValue(key, jsonIO_);
	}

	// 保存
	jsonIO_.Save();
}

void EffectManager::LoadEffectData()
{
	// 一度配列内のデータを削除
	for (auto& [key, ptr] : effectDatas_) {
		delete ptr;
	}
	// 配列クリア
	effectDatas_.clear();

	// グループ名の取得
	NestedList nameList = JsonIO::LoadGroupNames(filePath_);

	// グループ名リストが空の場合早期リターン
	if (nameList.empty()) {
		return;
	}

	// グループ名を元にマップ配列を作成
	for (auto itr = nameList.begin(); itr != nameList.end(); ++itr) {
		effectDatas_.emplace(itr->name, new LWP::Effect::EffectSaveData());
		effectDatas_[itr->name]->AddValue(itr->name, jsonIO_);
	};

	// データロード
	jsonIO_.Load();
}
