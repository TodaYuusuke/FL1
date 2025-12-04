#pragma once

class BulletBase;
/// <summary>
/// 弾の移動方式基底クラス
/// </summary>
class IMovement {
public:
    // コンストラクタ
    IMovement() = default;
    // デストラクタ
    virtual ~IMovement() = default;

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="bullet"></param>
    virtual void Update(BulletBase* bullet) = 0;

public:// アクセサ
    /// <summary>
    /// 移動方式の種類を取得
    /// </summary>
    /// <returns></returns>
    int GetType() { return type_; }

protected:
    int type_;
};