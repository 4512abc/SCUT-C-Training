#ifndef USER_RANK_ITEM_H
#define USER_RANK_ITEM_H
#include "config.h"
#include <QString>
#include <QPixmap>

class UserRankItem
{
public:
    // 构造函数
    UserRankItem(const QString &username, int score, const QPixmap &avatar);
    // 获取用户名
    QString username() const { return m_username; }
    // 获取分数
    int score() const { return m_score; }
    // 获取头像图标
    QPixmap avatarIcon() const { return m_avatar; }
    // 获取包含排名详细信息的字符串（例如：用户名、分数等）
    QString rankInfo() const;

private:
    QString m_username;     // 用户名
    int m_score;           // 分数
    QPixmap m_avatar;       // 用户头像图标
};

#endif // USER_RANK_ITEM_H
