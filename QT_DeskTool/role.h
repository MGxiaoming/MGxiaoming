#ifndef ROLE_H
#define ROLE_H

#include <QWidget>

//语音识别
#include "audio.h"

//天气
#include "weather.h"

//更换壁纸
#include "wallpaper.h"

#include <QTextToSpeech>
#include <QVoice>

//拖动删除文件
#include <QFile>//文件
#include <QDir>//文件夹
#include <QUrl>//文件路径

#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>


//人物
#include <QLabel>//用于展示人物图片
#include <QTimer>//定时器，用于定时切换图片

#include <QDebug>
#include<QMouseEvent>//鼠标事件
#include <QPoint>//坐标点
#include <QApplication>//窗口退出确认

#include <QAction>
#include <QMenu>//托盘菜单
#include <QSystemTrayIcon>//系统托盘
#include<QCloseEvent>//关闭窗口事件

#include <QDesktopWidget>

class role : public QWidget
{
    Q_OBJECT
public:
    explicit role(QWidget *parent = nullptr);

public slots:
    void rolecut();//切换照片

    void on_showMainAction();//显示
    void on_hideMainAction();//隐藏
    void on_exitAppAction();//退出
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);//托盘鼠标左击
    void weather_menu();//天气
    void wallpaper_menu();//壁纸

public:
    void mousePressEvent(QMouseEvent *event);//鼠标键按下事件
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event);//鼠标键松开事件
    void mouseDoubleClickEvent(QMouseEvent *event);//鼠标双击事件

    //    void closeEvent(QCloseEvent *event);
    void createActions();
    void createMenu();

    //拖动文件、删除
    void dragEnterEvent(QDragEnterEvent*event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    bool DeleteFileOrFolder(const QString &strPath);

public:
    QLabel *label;
    QTimer      *roletimer;

    QDir dir;
    QString imag_path;
    QStringList imag_list;
    QString imag_name;
    int imag_length;


    QPoint      mouseStartPoint;//鼠标起始点
    QPoint      windowLeftTop;//窗口左上角点
    QPoint      distence;//两者距离
    int mou_x;//鼠标的x坐标
    int win_x;//窗口的x坐标
    bool        m_bDrag;//用于判断鼠标键


    QSystemTrayIcon* myIcon;
    QMenu *mMenu;
    QAction *mShowMainAction;
    QAction *mExitAppAction;
    QAction *mHideAppAction;
    QAction *mWearAppAction;
    QAction *mWallAppAction;

    weather my_wea;//天气

    wallpaper my_wall;//壁纸

    //获取可用桌面大小
    QRect deskRect;

    audio *my_audio;

    QTextToSpeech *my_say;
    QString retStr;

};

#endif // ROLE_H
