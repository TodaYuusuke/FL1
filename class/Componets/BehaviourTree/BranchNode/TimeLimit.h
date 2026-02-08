#pragma once
#include "BranchNodeBase.h"

class TimeLimit : public BranchNodeBase {
public:
    TimeLimit(BlackBoard* blackboard, INode* true_node, INode* false_node, float limit);
    // デストラクタ
    ~TimeLimit();

    /// <summary>
    /// 初期化
    /// </summary>
    void Init() override;
    /// <summary>
    /// 解放処理
    /// </summary>
    //void Finalize() override;

    /// <summary>
    /// 更新
    /// </summary>
    void Tick() override;

private:
    /// <summary>
    /// 条件
    /// </summary>
    /// <returns></returns>
    const bool IsCondition() override;

private:
    float elapsed_;
    float limit_;
};
