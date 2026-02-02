#include <Adapter.h>
#include "class/scene/Title.h"
#include "class/scene/TestScene.h"
#include "class/scene/GameScene.h"
#include "class/scene/TutorialScene.h"
#include "class/scene/ResultScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LWP::System::Run<ResultScene>(L"4206_タイトル未定");
	return 0;
}