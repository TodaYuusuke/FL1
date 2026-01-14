#pragma once

#include <vector>
#include "CompositeNodeBase.h"

/// <summary>
/// 
/// </summary>
class RandomSelector : public CompositeNodeBase {
public:
	// コンストラクタ
	explicit RandomSelector(BlackBoard* black_board);
	// デストラクタ
	~RandomSelector();

	/// <summary>
	/// 更新
	/// </summary>
	void Tick() override;

private:
	/// <summary>
	/// 次のID取得
	/// </summary>
	/// <returns></returns>
	const int GetNextIndex() const override { return runningNodeIndex + 1; }
	/// <summary>
	/// 次のノードに進む
	/// </summary>
	void NodeIncrement() override {};
};