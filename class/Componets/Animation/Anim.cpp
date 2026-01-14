#include "Anim.h"

Anim::Anim(LWP::Resource::Animation* anim, const LWP::Animation::AnimData& data)
{
	// 情報受け取り
	anim_ = anim;
	data_ = data;
}

void Anim::Start() {

	// トラックタイプを取得
	LWP::Resource::Animation::TrackType tType = static_cast<LWP::Resource::Animation::TrackType>(data_.trackType);
	// アニメーション再生
	anim_->Play(data_.AnimName, data_.TransitionTime, 0.0f, tType);
	// ループフラグ切り替え
	anim_->Loop(data_.isLoop, tType);

}

void Anim::Update() {

	// アニメーションがループでない、かつアニメーション終了時
	if (!data_.isLoop && !anim_->GetPlaying(data_.AnimName)) {
		// アニメーション終了
		isEnd_ = true;
	}
	else if(data_.isLoop && anim_->GetEndOneLoop()) { // 1ループが終了した場合
		// 全イベントフラグリセット
		for (int i = 0; i < data_.Events.size(); i++) {
			data_.Events[i].IsDone = false;
		}
	}
	
	// イベント関係の処理
	for (int i = 0; i < data_.Events.size(); i++) {
		// 経過秒数が開始フレームを超過し、イベントが実行されていない場合
		if(data_.Events[i].BeginFrame <= static_cast<int>(std::floor(anim_->GetProgressSeconds() * 60.0f)) && 
			!data_.Events[i].IsDone) {
			// イベントの関数を実行
			data_.Events[i].Func();
			// イベント実行フラグをTrue
			data_.Events[i].IsDone = true;
		}
	}	

}

Anim& Anim::SetTrackType(const int type)
{
	// 再生トラックの設定
	data_.trackType = type;
	// 自身を返還
	return *this;
}

Anim& Anim::AddEvent(const std::string eventID, const int beginFrame, std::function<void()> func)
{
	// 新規イベントデータ作成
	LWP::Animation::EventData newEvent{};

	// データ受け渡し
	newEvent.Name = eventID;
	newEvent.BeginFrame = beginFrame;
	newEvent.Func = func;

	// イベント配列に追加
	data_.Events.push_back(newEvent);

	// 自身を返還
	return *this;
}

