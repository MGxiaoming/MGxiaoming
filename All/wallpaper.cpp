#include "wallpaper.h"
#include <QDebug>

#define TIME 900000

wallpaper::wallpaper(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("小埋壁纸");

    bt = new QPushButton("选择壁纸",this);
    rd = new QRadioButton("循环切换",this);

    QVBoxLayout *vbox = new QVBoxLayout(this);

    vbox->setAlignment(Qt::AlignCenter);
    vbox->addWidget(bt);
    vbox->addWidget(rd);
    this->setLayout(vbox);

    timer = new QTimer(this);

    connect(bt,SIGNAL(clicked(bool)),this,SLOT(change_wall()));
    connect(rd,SIGNAL(clicked(bool)),this,SLOT(auto_wall(bool)));
    connect(timer,SIGNAL(timeout()),this,SLOT(time_start()));
}

void wallpaper::setDesktopImage(QString path)
{
//    通过修改桌面壁纸的注册表值达到修改桌面图片的功效

    //壁纸注册表表
    QSettings wallPaper("HKEY_CURRENT_USER\\Control Panel\\Desktop",
                        QSettings::NativeFormat);

    //新的桌面图片路径
//    QString path("G:\\MG\\OneDrive\\图片\\1080_1920\\R-C (1).jfif");

    //给壁纸注册表设置新的值（新的图片路径）
    wallPaper.setValue("Wallpaper",path);
    QByteArray byte = path.toLocal8Bit();

    //调用windowsAPI
    SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, byte.data(), SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);

}

bool wallpaper::nextWallpaper()
{
    if(pixs.length()-1 == index++)
        index = 0;
    if(pixs.length())
    {
        setDesktopImage(pixs.at(index));
        return true;
    }
    else
        return false;
}

void wallpaper::change_wall()
{
    pixs = QFileDialog::getOpenFileNames(this,"请选择图片","G:\\MG\\OneDrive\\图片\\1080_1920","Images (*.png *.xpm *.jpg *.jfif)");
    index = 0;
    if(pixs.length())
        setDesktopImage(pixs.at(index));
}

void wallpaper::time_start()
{
    if(pixs.length())
    {
        if(pixs.length()-1 == index++)
            index = 0;
        setDesktopImage(pixs.at(index));
    }
}

void wallpaper::auto_wall(bool flag)
{
    if(flag == true)
        timer->start(TIME);
    else
        timer->stop();
}
