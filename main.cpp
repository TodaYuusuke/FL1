#include <Adapter.h>
#include "class/scene/Title.h"
#include "class/scene/TestScene.h"
#include "class/scene/GameScene.h"
#include "class/scene/TutorialScene.h"
#include "class/scene/ResultScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LWP::System::Run<TutorialScene>(L"4206_多腕戦機バンディット");
	return 0;
}