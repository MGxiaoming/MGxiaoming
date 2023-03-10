#ifndef WEATHER_H
#define WEATHER_H

#include <QWidget>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonObject>

#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QDateTime>

#include <QString>
#include <QDateTime>
#include <QByteArray>
#include <QDebug>
#include <QImage>
namespace Ui {
class weather;
}

class weather : public QWidget
{
    Q_OBJECT

public:
    explicit weather(QWidget *parent = 0);
    ~weather();

    void Init();
    void aqi_result(int aqi,int i);
    void type_result(QString type, int i);
    void background_main(QString weatherpic);

protected slots:
    void run();
private slots:
    void read_data(QNetworkReply *reply);
    void get_weather();
    void on_comboBox_activated(const QString &arg1);

private:
    Ui::weather *ui;

    QNetworkAccessManager *manager;
    QString weatherpic;
    QString week[5];
    QLabel *rangewendu[5];
    QLabel *wtpye[5];
    QLabel *windlb[5];
    QLabel *aqilb[5];
    QLabel *datalb[5];
    QLabel *typepiclb[5];
    QTimer *mtimer;
    int num;

};

#endif // WEATHER_H
