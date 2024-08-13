#ifndef PLAYERRANKINGPAGE_H
#define PLAYERRANKINGPAGE_H
#include<QVBoxLayout>
#include<QLabel>
#include<QPushButton>
#include <QWidget>

// 定义一个简单的结构体来模拟 UserRankItem
struct UserRankItem
{
    QString username;
    QString avatarIcon;
    QString elapsedTime;

    UserRankItem(const QString &username, const QString &avatarIcon, const QString &elapsedTime)
        : username(username), avatarIcon(avatarIcon), elapsedTime(elapsedTime) {}
};

class PlayerRankingPage : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerRankingPage(QWidget *parent = nullptr);
    ~PlayerRankingPage();
    std::vector<UserRankItem>readRankingFromFile();
    void showNextPage();
    void showPrevPage();
    QString imagePath;
    void cleanrank();

signals:
    void returnToMainPage(); // 发出返回主页面信号

private slots:
    void on_ReturnButton_clicked();
    void showLevel1Ranking();
    void showLevel2Ranking();
    void showLevel3Ranking();

private:
    void updateRanking(const QString &level);
    std::vector<UserRankItem> readRankingFromFile(const QString &level);
    QVBoxLayout *mainLayout;
    QPushButton *returnButton;
    QLabel *rankTitle;
    QVBoxLayout *rankListLayout;
    int currentPage = 0;
    std::vector<UserRankItem> userRankList = readRankingFromFile();
};

#endif // PLAYERRANKINGPAGE_H
