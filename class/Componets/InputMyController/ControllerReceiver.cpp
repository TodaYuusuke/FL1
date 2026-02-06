#include "ControllerReceiver.h"
//#include "VectorFunction.h"
//#include "ConvertString.h"
#include <functional>
#include <chrono>

#pragma comment(lib,"hid.lib")
#pragma comment(lib,"setupapi.lib")

#include <math.h>
#include <time.h>

const float ControllerReceiver::kInputStickRange_ = 270.0f;

std::wstring ControllerReceiver::ToWstring(const std::string& str) {
	if (str.empty()) {
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0) {
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}

std::wstring multi_to_wide_capi(std::string const& src)
{
	std::locale::global(std::locale(""));
	std::size_t converted{};
	std::vector<wchar_t> dest(src.size(), L'\0');
	if (::_mbstowcs_s_l(&converted, dest.data(), dest.size(), src.data(), _TRUNCATE, ::_create_locale(LC_ALL, "jpn")) != 0) {
		throw std::system_error{ errno, std::system_category() };
	}
	dest.resize(std::char_traits<wchar_t>::length(dest.data()));
	dest.shrink_to_fit();
	return std::wstring(dest.begin(), dest.end());
}

std::string convSJIStoUTF8(BYTE* pSource, int* pSize)
{
	*pSize = 0;

	// Convert SJIS -> UTF-16
	const int nSize = ::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pSource, -1, NULL, 0);

	BYTE* buffUtf16 = new BYTE[nSize * 2 + 2];
	::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pSource, -1, (LPWSTR)buffUtf16, nSize);

	// Convert UTF-16 -> UTF-8
	const int nSizeUtf8 = ::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)buffUtf16, -1, NULL, 0, NULL, NULL);
	/*if (!pDist) {
		*pSize = nSizeUtf8;
		delete buffUtf16;
		//return TRUE;
	}*/

	BYTE* buffUtf8 = new BYTE[nSizeUtf8 * 2];
	ZeroMemory(buffUtf8, nSizeUtf8 * 2);
	::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)buffUtf16, -1, (LPSTR)buffUtf8, nSizeUtf8, NULL, NULL);

	*pSize = lstrlen((LPCWSTR)buffUtf8);
	char pDist[256];
	memcpy(pDist, buffUtf8, *pSize);
	std::string ret = pDist;


	delete buffUtf16;
	delete buffUtf8;
	return ret;
	//return TRUE;
}

std::wstring ControllerReceiver::GetUSBPort() {
	HDEVINFO hDevInfo;
	int max = 0;
	std::wstring wName = L"ERROR";
	// デバイス一覧の情報を取得
	hDevInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_COMPORT, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	//if (hDevInfo == INVALID_HANDLE_VALUE) { printf_s("ERROR:COMPORT NOT FOUND\n"); return L"ERROR"; }

	// デバイス情報セット
	SP_DEVINFO_DATA Data = { sizeof(SP_DEVINFO_DATA) };
	Data.cbSize = sizeof(Data);

	// 個々のデバイスの情報を取得
	bool isGetDevice = false;
	while (SetupDiEnumDeviceInfo(hDevInfo, max, &Data)) {
		isGetDevice = true;
		char* port = nullptr;
		char name[256] = { 0 };

		//拡張
		char param[256];
		char newName[128];

		// 個々のデバイスの構成情報のレジストリキーを開く
		HKEY key = SetupDiOpenDevRegKey(hDevInfo, &Data, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);
		if (key != INVALID_HANDLE_VALUE) {
			DWORD type = 0;
			DWORD size = 256;

			//デバイス情報(レジストリキー)の情報群からCOMPortの名前を取り出す
			RegQueryValueEx(key, L"PortName", NULL, &type, (LPBYTE)name, &size);
			port = name;
		}
		else continue;

		DWORD size = 0;
		char* buf = nullptr;

		// デバイスの詳細の取得
		// デバイスの詳細に必要な文字数をsizeに収納
		SetupDiGetDeviceRegistryProperty(hDevInfo, &Data, SPDRP_DEVICEDESC, NULL, NULL, 0, &size);
		buf = new char[size * 2];
		BOOL success = FALSE;
		// デバイスの詳細の文字列をbufに収納
		success = SetupDiGetDeviceRegistryProperty(hDevInfo, &Data, SPDRP_DEVICEDESC, NULL, (PBYTE)buf, size, &size);
		//size = 256;
		memcpy(param, buf, size);

		std::string paramShift = param;
		//std::string paramUtf = 
		//convSJIStoUTF8((BYTE*)param,(int*)&size);

		//ConvSJistoUtf8((BYTE*)param, (BYTE*)param,(int*)&size);
		//余分な\0削除
		for (unsigned int i = 0; i < size/2; i++) {
			newName[i] = name[i * 2];
			param[i] = param[i * 2];
		}


		newName[size] = '\0';
		param[3] = '\0';
	
		std::wstring wParam = ToWstring(param);
		delete[] buf;
		max++;
		//識別がUSBだったら名前を返す
		if (wParam == L"USB") {
			wName = ToWstring(newName);
		}
		else {
			wName = L"NOT CONNECTING USB";
		}
	}

	if (!isGetDevice) {
		//デバイス接続0の時
	}

	SetupDiDestroyDeviceInfoList(hDevInfo); // デバイス情報セットの解放
	return wName;
}


void ControllerReceiver::Initialize() {

	InitializeJson();

	isStartThread_ = false;
	isEndThread_ = false;
	//kalman_[0].reset(new kalman2Shaft);
	//kalman_[1].reset(new kalman2Shaft);
	data_ = { 0 };
	//BYTE data = 1;
	//1.ポートをオープン
	std::wstring name = GetUSBPort();
	arduino = CreateFile(name.c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (arduino == INVALID_HANDLE_VALUE) {
		//ポートオープン失敗(コントローラーに切り替える用の処理入れる)
		return ;
	}

	//2.送受信バッファ初期化
	Ret = SetupComm(arduino, 1024, 1024);
	if (!Ret) {
		CloseHandle(arduino);
		return ;
	}
	Ret = PurgeComm(arduino, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	if (!Ret) {
		CloseHandle(arduino);
		return ;
	}

	//3.基本通信条件の設定
	DCB dcb;
	GetCommState(arduino, &dcb);
	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = 9600;
	dcb.fBinary = TRUE;
	dcb.ByteSize = 8;
	dcb.fParity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	Ret = SetCommState(arduino, &dcb);
	if (!Ret) {
		CloseHandle(arduino);
		return ;
	}

	//通信スレッド開始
	receiveThread_ = std::thread(std::bind(&ControllerReceiver::Loop, this));
	isStartThread_ = true;

}


void ControllerReceiver::InitializeJson() {
	// 管理クラスの調整項目
	json_.Init(kJsonDirectoryPath + "LeverParamater.json")
		.BeginGroup("Left")
		.AddValue<LWP::Math::Vector2>("min", &borders_[0])
		.AddValue<LWP::Math::Vector2>("max", &borders_[1])
		.EndGroup()
		.BeginGroup("Right")
		.AddValue<LWP::Math::Vector2>("min", &borders_[2])
		.AddValue<LWP::Math::Vector2>("max", &borders_[3])
		.EndGroup()

		.CheckJsonFile();
}

void ControllerReceiver::DebugGUI() {
	if (ImGui::BeginTabItem("Lever")) {
		json_.DebugGUI();
		ImGui::EndTabItem();
	}
}

void ControllerReceiver::ReOpenPort() {
	ClosePort();
	isStartThread_ = false;
	isEndThread_ = false;
	
	//ポートオープン
	std::wstring name = GetUSBPort();
	arduino = CreateFile(name.c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (arduino == INVALID_HANDLE_VALUE) {
		//ポートオープン失敗(コントローラーに切り替える用の処理入れる)
		return;
	}

	//2.送受信バッファ初期化
	Ret = SetupComm(arduino, 1024, 1024);
	if (!Ret) {
		CloseHandle(arduino);
		return;
	}
	Ret = PurgeComm(arduino, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	if (!Ret) {
		CloseHandle(arduino);
		return;
	}

	//3.基本通信条件の設定
	DCB dcb;
	GetCommState(arduino, &dcb);
	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = 115200;
	dcb.fBinary = TRUE;
	dcb.ByteSize = 8;
	dcb.fParity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	Ret = SetCommState(arduino, &dcb);
	if (!Ret) {
		CloseHandle(arduino);
		return;
	}

	//通信スレッド開始
	receiveThread_ = std::thread(std::bind(&ControllerReceiver::Loop, this));
	isStartThread_ = true;

}

void ControllerReceiver::Loop() {
	DWORD dwReadSize;
	//DeltaTime計算
	std::chrono::system_clock::time_point nowTime;
	std::chrono::system_clock::time_point preTime;
	nowTime = std::chrono::system_clock::now();
	preTime = std::chrono::system_clock::now();

	char rData;
	//受付開始
	while (!isEndThread_) {
		char rBuff[sizeof(RecvStickTest)];
		RecvStickTest data;
		//データのHeader'S'を取得したらそこから続く実データの取得を行う
		Ret = ReadFile(arduino, &rData, sizeof(rData), &dwReadSize, NULL);
		if (CheckEndPort(rData)) { break; };
		if (rData == 'S') {
			//受信用DeltaTime計算
			nowTime = std::chrono::system_clock::now();
			//実データ取得
			for (int i = 0; i < 2;i++) {
				bool isBreak = false;
				for (size_t num = 0; num < sizeof(RecvStickTest); num++) {
					Ret = ReadFile(arduino, &rBuff[num], sizeof(char), &dwReadSize, NULL);
					if (CheckEndPort(rData)) { isBreak = true; break; };
				}
				if (isBreak) {
					break;
				}
				
				
				

				//構造体にコピー
				memcpy(&data, rBuff, sizeof(RecvStickTest));
				data_ = data;
				rawData_.x = data.x;
				rawData_.y = data.y;
				//usedData_.rotate[i] =  CalcRotate(data, kalman_[i].get());
				usedData_.stick.sticks[i].lever = CalcRotateFromJoyStick(data,i);
				usedData_.stick.sticks[i].button0 = !data.button0;
				usedData_.stick.sticks[i].button1 = !data.button1;

			}
			//右スティックの入力反転
			usedData_.stick.sticks[1].lever.x = -usedData_.stick.sticks[1].lever.x;
			usedData_.stick.sticks[1].lever.y = -usedData_.stick.sticks[1].lever.y;

			auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - preTime).count();
			deltaTime_ = float(dt);
			preTime = nowTime;

		}
	}
}
/*
Vector3 ControllerReceiver::CalcRotate(RecvData in, kalman2Shaft* kalman) {
	Vector3 rotate;
	//加速度から角度算出,座標系変換
	Vector3 accel;
	accel.x = in.accelG[2];
	accel.y = in.accelG[1];
	accel.z = -in.accelG[0];
	rotate.x = -atan2(accel.y, accel.z);
	rotate.z = atan2(accel.x, accel.z);


	//補正
	kalman->x->K = in.gyroDps[2] * deltaTime_;
	kalman->z->K = in.gyroDps[0] * deltaTime_;

	rotate.x = float(kalman->x->update(double(rotate.x)));
	rotate.z = float(kalman->z->update(double(rotate.z)));

	return rotate;
}*/

LWP::Math::Vector2 ControllerReceiver::CalcRotateFromJoyStick(RecvStickTest in,int index) {
	LWP::Math::Vector2 rotate;
	rotate.y = 0;
	int16_t num = in.x - kInputStickCenter_;
	rotate.x = float(num)/ kInputStickRange_;
	num = in.y - kInputStickCenter_;
	rotate.y = float(num) / kInputStickRange_;
	
	rotate = rotate.Normalize();

	//deadZone設定
	if (rotate.x > borders_[index*2].x && rotate.x < borders_[index * 2 + 1].x) {
		rotate.x = 0;
	}
	else if (rotate.x >0) {
		rotate.x = 1.0f;
	}
	else {
		rotate.x = -1.0f;
	}
	if (rotate.y > borders_[index * 2].y && rotate.y < borders_[index * 2 + 1].y) {
		rotate.y = 0;
	}
	else if (rotate.y > 0) {
		rotate.y = 1.0f;
	}
	else {
		rotate.y = -1.0f;
	}
	rotate = rotate.Normalize();

	return rotate;
}

void ControllerReceiver::EndPort() {
	if (isStartThread_) {
		//通信終了
		isEndThread_ = true;
		receiveThread_.join();
		receiveThread_.detach();
	}
	CloseHandle(arduino);
}

void ControllerReceiver::ClosePort() {
	DWORD dwSendSize;
	static char data = 'E';
	//現在のスレッドを強制終了
	if (isStartThread_) {
		Ret = WriteFile(arduino, &data, sizeof(data), &dwSendSize, NULL);
		//通信終了
		isEndThread_ = true;
		receiveThread_.join();
		isStartThread_ = false;
		CloseHandle(arduino);
	}
}

bool ControllerReceiver::CheckEndPort(char cord) {
	//終了検知
	if (cord == 'E') {
		return true;
	}
	return false;
}