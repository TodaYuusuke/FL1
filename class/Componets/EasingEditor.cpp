#include "EasingEditor.h"
#include <imgui/imgui_internal.h>

using namespace LWP;
using namespace LWP::Math;

EasingEditor::EasingEditor() {
	values_.push_back(Vector2{ 0.0f,0.2f });
	values_.push_back(Vector2{ 0.0f,0.4f });
	values_.push_back(Vector2{ 0.0f,0.3f });
	values_.push_back(Vector2{ 0.0f,0.6f });
	values_.push_back(Vector2{ 0.0f,0.8f });
	values_.push_back(Vector2{ 0.0f,0.5f });

	for (int i = 0; i < values_.size(); i++) {
		values_[i].x = i * (1.0f / static_cast<float>((int)values_.size() - 1));
	}

	json_.Init(kDirectoryPath + "Easing.json");
	for (int i = 0; i < values_.size(); i++) {
		std::string name = "point" + std::to_string(i);
		json_.AddValue<Vector2>(name, &values_[i]);
	}
	json_.CheckJsonFile();
}

void EasingEditor::Init() {

}

void EasingEditor::Update() {
	ImGui::Begin("Editable Graph");

	// キャンバス座標
	ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
	// キャンバスサイズ
	ImVec2 canvas_size = ImVec2(400, 200);
	// 線描画
	ImDrawList* draw = ImGui::GetWindowDrawList();

	// 入力領域確保
	ImGui::InvisibleButton("Graph", canvas_size);
	bool hovered = ImGui::IsItemHovered();
	//bool active = ImGui::IsItemActive();

	ImVec2 mouse = ImGui::GetIO().MousePos;

	// 背景
	draw->AddRectFilled(
		canvas_pos,
		ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y),
		IM_COL32(40, 40, 40, 255)
	);
	// 枠
	draw->AddRect(
		canvas_pos,
		ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y),
		IM_COL32(255, 255, 255, 255)
	);

	// 点の描画 & 編集
	for (int i = 0; i < values_.size(); i++) {
		Vector4 color = { 255, 100, 100, 255 };
		float x = canvas_pos.x + (1.0f - values_[i].x) * canvas_size.x;
		float y = canvas_pos.y + (1.0f - values_[i].y) * canvas_size.y;
		// 点の座標
		ImVec2 p(x, y);

		// ドラッグ処理
		if (hovered && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
			ImVec2 len;
			len.x = mouse.x - p.x;
			len.y = mouse.y - p.y;
			// 範囲内の点を移動
			if (ImLengthSqr(len) < 400.0f) {
				Vector2 newValue = {
					1.0f - (mouse.x - canvas_pos.x) / canvas_size.x,
					1.0f - (mouse.y - canvas_pos.y) / canvas_size.y
				};
				if (i != 0 && i != (int)values_.size() - 1) values_[i].x = std::clamp<float>(newValue.x, 0.0f, 1.0f);
				values_[i].y = std::clamp<float>(newValue.y, 0.0f, 1.0f);

				// 色変更
				color = { 100, 100, 255, 255 };

			}
		}

		// 点
		draw->AddCircleFilled(p, 5.0f, IM_COL32((int)color.x, (int)color.y, (int)color.z, (int)color.w));

		// 線
		if (i > 0) {
			//float px = canvas_pos.x + (1.0f - values_[i - 1].x) * canvas_size.x;
			//float py = canvas_pos.y + (1.0f - values_[i - 1].y) * canvas_size.y;
			//draw->AddLine(ImVec2(px, py), p, IM_COL32(0, 200, 255, 255), 2.0f);
		}
	}

	// 線の始点終点
	std::vector<LWP::Math::Vector2> linePos;
	for (int i = 0; i < segment_; i++) {
		float t = 1.0f / (segment_ - 1) * i;
		if (t >= 1.0f) {
			t = 1.0f;
		}
		Vector2 pos = CatmullRomSpline(values_, t);
		linePos.push_back(pos);
	}
	for (int i = 1; i < segment_; i++) {
		ImVec2 start = {
			canvas_pos.x + (1.0f - linePos[i - 1].x) * canvas_size.x,
			canvas_pos.y + (1.0f - linePos[i - 1].y) * canvas_size.y
		};
		ImVec2 end = {
			canvas_pos.x + (1.0f - linePos[i].x) * canvas_size.x,
			canvas_pos.y + (1.0f - linePos[i].y) * canvas_size.y
		};
		draw->AddLine(start, end, IM_COL32(0, 200, 255, 255), 2.0f);
	}

	if (ImGui::Button("Save")) {
		json_.Save();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load")) {
		json_.Load();
	}

	ImGui::End();
}

Vector2 EasingEditor::CatmullRomSpline(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3, float t) {
	const float s = 0.5f;
	float t2 = t * t;
	float t3 = t2 * t;

	Vector2 e3 = (p0 * -1.0f) + (p1 * 3.0f) - (p2 * 3.0f) + p3;
	Vector2 e2 = (p0 * 2.0f) - (p1 * 5.0f) + (p2 * 4.0f) - p3;
	Vector2 e1 = (p0 * -1.0f) + p2;
	Vector2 e0 = p1 * 2.0f;

	Vector2 result = ((e3 * t3) + (e2 * t2) + (e1 * t) + e0) * s;
	const float EPS = 1e-6f;
	if (std::fabsf(result.x) < EPS) {
		result.x = 0.0f;
	}
	if (std::fabsf(result.y) < EPS) {
		result.y = 0.0f;
	}
	return result;
}
Vector2 EasingEditor::CatmullRomSpline(const std::vector<Vector2>& controlPoints, float t) {
	assert(controlPoints.size() >= 4 && "制御点は4点未満です");

	// 区画数を制御点の数-1
	size_t division = controlPoints.size() - 1;
	// 1区間の長さ(全体を1)
	float areaWidth = 1.0f / division;

	// 区間内の始点を0 終点を1の時の現在地
	float t_2 = std::fmod(t, areaWidth) * division;
	// 0~1を出ないようにする
	t_2 = std::clamp<float>(t_2, 0.0f, 1.0f);

	// 区間番号
	size_t index = static_cast<size_t>(t / areaWidth);
	// 区間番号が上限を超えないようにする
	index = std::clamp<size_t>(index, 0, controlPoints.size() - 1);

	// 4線分のインデックス
	std::array<size_t, 4> indeces = {
		indeces[0] = index - 1,
		indeces[1] = index,
		indeces[2] = index + 1,
		indeces[3] = index + 2
	};
	// 制御点のサイズを超えないようにする
	indeces[0] = std::clamp<size_t>(indeces[0], 0, controlPoints.size() - 1);
	indeces[1] = std::clamp<size_t>(indeces[1], 0, controlPoints.size() - 1);
	indeces[2] = std::clamp<size_t>(indeces[2], 0, controlPoints.size() - 1);
	indeces[3] = std::clamp<size_t>(indeces[3], 0, controlPoints.size() - 1);

	// 最初の区間のp0はp1を重複使用
	if (index == 0) {
		indeces[0] = indeces[1];
	}
	// 最後の区間のp3はp2を重複使用
	if (indeces[3] >= controlPoints.size()) {
		indeces[2] -= 1;
		indeces[3] = indeces[2];
	}
	// 4点の座標
	std::array<Vector2, 4> points = {
		points[0] = controlPoints[indeces[0]],
		points[1] = controlPoints[indeces[1]],
		points[2] = controlPoints[indeces[2]],
		points[3] = controlPoints[indeces[3]]
	};

	return CatmullRomSpline(points[0], points[1], points[2], points[3], t_2);
}