#include "audio.h"

audio::audio(QObject *parent) : QObject(parent)
{
    my_speech = new Speech;
}

void audio::startAudio(QString fileName)
{
    if(fileName.isEmpty())
    {
        QMessageBox::warning(NULL,"警告(Audio)","无音频文件");
        return;
    }
    //检查音频设备
    QAudioDeviceInfo device = QAudioDeviceInfo::defaultInputDevice();
    if(device.isNull())
        QMessageBox::warning(NULL,"警告(Audio)","无音频设备");
    else{
        /*记录录音文件*/
        AudioFileName = fileName;//后续需要
        /*音频编码设置*/
        QAudioFormat m_format;
        /*设置采样率*/
        m_format.setSampleRate(16000);
        /*设置通道数*/
        m_format.setChannelCount(1);
        /*设置位深*/
        m_format.setSampleSize(16);
        /*设置编码格式*/
        m_format.setCodec("audio/pcm");//api传输要求格式

        /*判断设备是否支持该格式*/
        if(!device.isFormatSupported(m_format))
            m_format = device.nearestFormat(m_format);//寻找最接近的格式

        /*操作文件*/
        //打开文件
        AudioFile = new QFile;
        AudioFile->setFileName(fileName);
        AudioFile->open(QIODevice::WriteOnly);

        //创建录音对象
        AudioDevice = new QAudioInput(m_format,this);
        AudioDevice->start(AudioFile);
    }
}

QString audio::startSpeech()
{
    if(AudioFileName.isEmpty())
    {
        QMessageBox::warning(NULL,"警告(Audio)","文件不存在");
        return QString("");
    }
    //语音识别并返回识别后结果
    return my_speech->speechIdentify(AudioFileName);
}
void audio::stopAudio()
{
    //停止录音
    AudioDevice->stop();
    //关闭文件
    AudioFile->close();
    //删除文件对象指针并置空
    delete AudioFile;
    AudioFile = nullptr;
}
