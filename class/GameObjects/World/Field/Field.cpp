#include "Field.h"

using namespace LWP::Base;
using namespace LWP::Math;
using namespace LWP::Utility;

Field::Field() {
	// 貫通防止クラスのインスタンス取得
	resolver_ = PenetrationResolver::GetInstance();

	// jsonIOの初期化
	jsonIO_.Init(filePath_);
	
	// データのロード
	LoadProps();

	// 全ての配置物を貫通防止クラスに登録
	for (Prop* p : props_) {
		// 半径が0以上の物のみ登録する
		if (p->GetData()->Radius > 0.0f) {
			// 貫通防止クラスに登録
			resolver_->RegisterObject(p, p->GetPriority(), p->GetRadius());
		}
	}
}

Field::~Field() {
	// 配列クリア
	for (Prop* p : props_) {
		delete p;
	}
	props_.clear();
}

void Field::DebugGUI()
{
	// デバッグメニューの表示
	std::string displayName = "Prop Editor";
	ImGui::SeparatorText(displayName.c_str());

	// 配置物の追加処理
	ImGuiManager::InputText("Add PropName", addPropName_);
	ImGuiManager::InputText("Add ModelPath", addModelpath_);
	if (ImGui::Button("Add Prop")) {
		CreateNewProp(addPropName_, addModelpath_);
	}
	if (selectingProp_ != nullptr) {
		if (ImGui::Button("Copy Selected Prop")) {
			// データを元に配置物作成
			Prop* prop = new Prop(*selectingProp_->GetData());
			// 配置物配列に追加
			props_.push_back(prop);
		}
	}

	// 配列内の配置物を表示
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(300, 200), ImGuiWindowFlags_NoTitleBar);

	// 配置物配列が空でない場合
	if (!props_.empty()) {
		// 一時変数
		int editID = 0;
		Prop* change = nullptr;

		for (Prop* a : props_) {
			a->ImGuiRadioButton(selectedID_, editID, change);
		}

		// 表示対象が変更されていた場合
		if (change != nullptr) {
			selectingProp_ = change;
		}
	}
	else {
		// 配置物がないことを示す
		ImGui::Text("Props is Empty.");
		// 選択配置物はnullに
		selectingProp_ = nullptr;
		// 選択IDリセット
		selectedID_ = 0;
	}

	ImGui::EndChild();

	// アニメーションが選択されている場合
	if (selectingProp_ != nullptr) {
		// そのGUIを表示
		selectingProp_->DrawGui();
	}
	else {
		ImGui::NewLine();
		ImGui::Text("No Selected!");
		ImGui::NewLine();
	}

	ImGui::NewLine();
	// 全てのデータ保存
	if (ImGui::Button("SaveAll")) {
		SaveAllData();
	}
	ImGui::NewLine();
}

void Field::Update() 
{
	// 配置物更新
	for (Prop* a : props_) {
		a->Update();
		if(a->GetName() == "Prop" || a->GetName() == "Garage")appendMiniMap_(a->GetWorldTF()->GetWorldPosition());
	}
}

void Field::EndFrame()
{
	// 削除フラグがたった配置物を削除
	props_.remove_if([&](Prop* p) {
		// 粒子の終了フラグがTrueのとき
		if (p->GetIsDelete()) {
			p->SetIsAlive(false);
			// 選択中配置物を一応nullptrにしておく
			selectingProp_ = nullptr;

			// ポインタ解放
			delete p;
			return true;
		}

		return false;
	});
}

void Field::CreateNewProp(const std::string& name, const std::string filePath)
{
	// データを元に配置物作成
	Prop* prop = new Prop(name, filePath);
	// 配置物配列に追加
	props_.push_back(prop);
}

void Field::SaveAllData()
{
	// jsonIOの初期化
	jsonIO_.Init(filePath_);
	
	// 名前被りした名称をどれだけ追加したか確認する
	std::map<std::string, int> addedMap{};
	// 配列内のデータ全てをjsonに追加
	for (Prop* p : props_) {
		// 保存プロップの名称で末尾に被り数の番号を入れて重複を回避する
		p->GetData()->AddValue(p->GetName() + "-" + std::to_string(addedMap[p->GetName()]++), jsonIO_);
	}

	// 保存
	jsonIO_.Save();
}

void Field::LoadProps()
{
	// 一度配列クリア
	for (Prop* p : props_) {
		delete p;
	}
	props_.clear();

	// グループ名の取得
	NestedList nameList = JsonIO::LoadGroupNames(filePath_);

	// グループ名リストが空の場合早期リターン
	if (nameList.empty()) {
		return;
	}

	// 配置物の保存データリスト
	std::map<std::string, LWP::Prop::PropSaveData> datas{};

	// グループ名を元にマップ配列を作成
	for (auto itr = nameList.begin(); itr != nameList.end(); ++itr) {
		datas.emplace(itr->name, LWP::Prop::PropSaveData());
		datas[itr->name].AddValue(itr->name, jsonIO_);
	};

	// データロード
	jsonIO_.Load();

	// 読み込みデータを元に配置物読み込み
	for (auto [key, data] : datas) {
		// データを元に配置物作成
		Prop* prop = new Prop(data);

		// 配置物配列に追加
		props_.push_back(prop);
	}
}
