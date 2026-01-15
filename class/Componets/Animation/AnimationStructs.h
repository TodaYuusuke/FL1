#pragma once
#include <LWP.h>
#include <functional>

/// <summary>
/// アニメーションで使用するデータ構造体
/// </summary>
namespace LWP::Animation
{	
	/// <summary>
	/// アニメーション内のイベント構造体
	/// </summary>
	struct EventData {
		std::string Name = "";		// イベント名称
		int BeginFrame = 0;			// 開始フレーム
		bool IsDone = false;		// の実行済みフラグ
		std::function<void()> Func;	// 発動時時実行される関数

		/// <summary>
		/// デバッグGUI関数
		/// </summary>
		/// <param name="id">ImGui上の表示名</param>
		void DebugGUI(const std::string& id) {
			if (ImGui::TreeNode(id.c_str())) {
				// イベント名称表示
				std::string eventName = this->Name;
				ImGui::Text(eventName.c_str());
				ImGui::NewLine();

				// 開始フレーム表示
				std::string frameText = "BeginFrame : " + std::to_string(this->BeginFrame);
				ImGui::Text(frameText.c_str());

				// 実行状態表示
				bool result = this->IsDone;
				ImGui::Checkbox("IsDone", &result);
				ImGui::TreePop();
			}
		}
	};

	/// <summary>
	/// アニメーションデータ構造体
	/// </summary>
	struct AnimData {
		std::string AnimName = "";			// 再生されるアニメーション名
		float TransitionTime = 0.0f;		// アニメーション遷移にかかる秒数
		bool isLoop = false;				//　ループするか
		int trackType = 0;					// 再生されるトラックのタイプ
		std::vector<EventData> Events{};	// イベント配列

		// デバッグGUI関数
		void DebugGUI() {
			// デバッグメニューの表示
			ImGui::NewLine();
			ImGui::SeparatorText("DebugMenu");
			ImGui::NewLine();

			// アニメーション名表示
			std::string playAnimName = "PlayAnim : " + AnimName;
			ImGui::Text(playAnimName.c_str());

			// 遷移秒数表示
			std::string transitionText = "TransitionTime : " + std::to_string(this->TransitionTime);
			ImGui::Text(transitionText.c_str());

			// ループ状態表示
			bool result = this->isLoop;
			ImGui::Checkbox("IsLoop", &result);
			
			ImGui::NewLine();

			// 全イベント情報表示
			for (int i = 0; i < Events.size(); i++) {
				// GUI名称設定
				std::string GUIName = "Event " + std::to_string(i + 1);
				Events[i].DebugGUI(GUIName);
				ImGui::NewLine();
			}
		}
	};
}