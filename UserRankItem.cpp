#include "UserRankItem.h"

UserRankItem::UserRankItem(const QString &username, int score, const QPixmap &avatar)
    : m_username(username), m_score(score), m_avatar(avatar)
{}

QString UserRankItem::rankInfo() const
{
    return QStringLiteral("%1 (Score: %2)").arg(m_username, QString::number(m_score));
}
