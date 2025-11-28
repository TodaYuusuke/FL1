#pragma once

#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <setupapi.h>
#include <locale>
#include <thread>
#include "Adapter.h"
#include <memory>
//#include "SimpleKalman.h"
//#include "../externals/KalmanFilter/Kalman.h"

//マイコンからの受信データ形式(マイコン側の構造体と合わせる)
struct RecvData{
	float accelG[3];
	float gyroDps[3];
	//float temp;
};

//スティック入力+ボタン二個
struct  RecvStickTest {
	int16_t x, y;
	bool button0, button1;
};

//レバー一個分のデータ
struct StickData {
	LWP::Math::Vector2 lever;
	bool button0, button1;
};

//使用する全データをまとめたやつ
struct GameUsedData {
	union multiSticks{
		struct stick2Shaft {
			StickData stickLeft;
			StickData stickRight;
		} multiSticks;
		StickData sticks[2];
	} stick;
	//ペダル分追加予定
};
/*
//ジャイロ二つ分の簡易カルマンフィルタまとめたやつ
struct kalman2Shaft {
	kalman2Shaft() { x.reset(new SimpleKalman); z.reset(new SimpleKalman);};
	std::unique_ptr <SimpleKalman> x, z;
};
*/
class ControllerReceiver
{
public:
	//インスタンス取得
	static ControllerReceiver* GetInstance() {
		static ControllerReceiver instance;
		return &instance;
	}


	void Initialize();

	//終了時処理
	void EndPort();

	//ポートを閉める
	void ClosePort();

	//ポートを再度開く
	void ReOpenPort();

	GameUsedData GetData() { return usedData_; };
	float GetDeltaTime() { return deltaTime_; };
	RecvStickTest GetDebugData() { return data_; };

	//生データを取得する
	LWP::Math::Vector3 GetRawData() { return rawData_; };

	//ポートが開いてるか(入力が有効か)
	bool IsOpen() { return isStartThread_ && !isEndThread_; };

	void SetDeadZone(float deadZone) { deadZone_ = deadZone; };
	float GetDeadZone() { return deadZone_; };

private:
	ControllerReceiver() { Initialize(); };
	~ControllerReceiver() {};
	ControllerReceiver(const ControllerReceiver&) = delete;
	ControllerReceiver operator=(const ControllerReceiver&) = delete;

	std::wstring ToWstring(const std::string& str);
	//USBポートの名前を取ってくる
	std::wstring GetUSBPort();

	//スレッドで回す受信処理
	void Loop();
	//受け取ったデータを回転に変換する
	///Vector3 CalcRotate(RecvData in,kalman2Shaft* kalman);

	//ジョイスティックから受け取った生データを0~1に変換する
	LWP::Math::Vector2 CalcRotateFromJoyStick(RecvStickTest in);

	//ポートが閉まった際の処理
	bool CheckEndPort(char cord);

	HANDLE arduino;
	bool Ret = false;

	//パラメータ格納用
	GameUsedData usedData_ = {0};

	//受信データ
	RecvStickTest data_ = { 0 };
	
	//通信用スレッド
	std::thread receiveThread_;
	bool isStartThread_ = false;
	bool isEndThread_ = false;

	float deltaTime_ = 0;
	LWP::Math::Vector3 rawData_;

	//std::unique_ptr<kalman2Shaft> kalman_[2];
	LWP::Math::Vector3 preAngle_ = {0};

	//スティックの入力の変換につかうやつ
	static const int16_t kInputStickCenter_ = 510;
	static const float kInputStickRange_;

	float deadZone_ = 0.02f;
};