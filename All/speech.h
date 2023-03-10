#ifndef SPEECH_H
#define SPEECH_H

#include <QObject>

#include "http.h"

#include <QFile>

#include <QMap>

#include <QByteArray>
#include <QMessageBox>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QHostInfo>

#include <QDebug>

#include <QMessageBox>

const QString BaiduSpeechUrl = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2"; //获取token请求的Url
const QString BaiduSpeechClientID = "v0jaeF9X3Z7e0n5TsHaD5keZ";                                 //AK
const QString BaiduSpeechClientSecret = "Sm3pQ1ERXLu11RlRddnlpRBsHBdUNvVn";     //SK

//语音识别的Url
const QString BaiduSpeechSatrtUrl = "https://vop.baidu.com/pro_api?dev_pid=80001&cuid=%1&token=%2";
//80001识别普通话  %1:本机唯一标识(本机标识)  %2:获取的token

class Speech : public QObject
{
    Q_OBJECT
public:
    explicit Speech(QObject *parent = nullptr);

signals:

public slots:

public:
    QString speechIdentify(QString audioFile);

private:
    QString getJsonValue(QByteArray &data, QString &key);

private:
    QString accessToken;

};

#endif // SPEECH_H
