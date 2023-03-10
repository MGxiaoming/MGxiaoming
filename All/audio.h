#ifndef AUDIO_H
#define AUDIO_H

#include <QObject>

#include "speech.h"

#include <QAudioDeviceInfo> //查询音频设备
#include <QAudioInput> //音频输入
#include <QAudioFormat> //音频存储

#include <QFile>

#include <QMessageBox>
class audio : public QObject
{
    Q_OBJECT
public:
    explicit audio(QObject *parent = nullptr);

public slots:

public:

    void startAudio(QString fileName);//初始化

    QString startSpeech();//开始录音

    void stopAudio();//停止录音

private:
    QString AudioFileName;//录音后文件名

    QFile *AudioFile;//录音后文件

    QAudioInput *AudioDevice;//音频操作对象

    Speech *my_speech;
};

#endif // AUDIO_H
