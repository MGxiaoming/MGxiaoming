#include "role.h"

role::role(QWidget *parent) : QWidget(parent)
{
    my_say = new QTextToSpeech;
    my_audio = new audio;//语音识别对象

    //ui->widget->setAcceptDrops(false);//禁用窗口控件的放下操作
    this->setAcceptDrops(true);//启用窗口控件的放下操作

    /*************************************窗口设置**********************************/
    deskRect = QApplication::desktop()->availableGeometry();//获取窗口大小，即屏幕宽高

    setAttribute(Qt::WA_TranslucentBackground) ; //窗体背景透明
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool);  //窗口置顶，无边框，在任务栏不显示图标
    this->setWindowTitle("小埋管家");//设置窗口标题
    this->setGeometry((deskRect.width()-105),(deskRect.height()-129),114,129);//窗口初始位置 //(deskRect.width()-105) == 1815，(deskRect.height()-129) == 901

    /*******************************图片初始设置***********************************/
    label = new QLabel(this);
    label->resize(114,129);
    label->setScaledContents(true);//设置图片自适应
    imag_path = "F:\\QTstudy\\QT\\my_desk_tool\\All\\1\\all\\xiaomai_static";//获取文件夹的位置
    dir.setPath(imag_path);//将文件夹的位置与dir绑定
    imag_length = dir.count();//获取文件夹的内容长度，会包括 . 和 .. 同linux
    dir.setNameFilters(imag_list);//将文件夹中的所有文件存于链表中
    imag_name = "F:\\QTstudy\\QT\\my_desk_tool\\All\\1\\all\\xiaomai_static\\" + dir[2];//获取文件绝对路径
    QImage imag(imag_name);
    label->setPixmap(QPixmap::fromImage(imag));

    /*******************************定时器切换图片***********************************/
    roletimer = new QTimer(this);
    connect(roletimer,SIGNAL(timeout()),this,SLOT(rolecut()));//定时器槽函数，定时切换图片
    roletimer->start(33);//每33毫秒秒调用一次，相当于1秒30帧左右

    /********************************系统托盘图标设置********************************/
    QApplication::setQuitOnLastWindowClosed(false);//当前窗口退出时，程序也结束
    myIcon = new QSystemTrayIcon(this);//定义系统托盘图标，并与widget窗口绑定
    myIcon->setIcon(QIcon(":/pic/xiaomai512.ico"));//系统托盘图标，非状态栏图标
    myIcon->setToolTip("小埋管家");//鼠标放在托盘图标上会显示名

    /******************************鼠标左击设置*************************************/
    connect(myIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
    myIcon->show();//鼠标左击后托盘功能显示

    /******************************托盘右键菜单*************************************/
    mMenu = new QMenu;
    createActions();
    createMenu();
    myIcon->setContextMenu(mMenu);//将菜单与托盘图标绑定
}
/************************************************************************************
 ***》》》                 自动切换帧图片
 * **********************************************************************************/
void role::rolecut()//定时切换图片
{
    static int flag = 3;
    if(flag > imag_length-1)
        flag = 2;
    imag_name = "F:\\QTstudy\\QT\\my_desk_tool\\All\\1\\all\\xiaomai_static\\" + dir[flag];
    QImage imag(imag_name);
    label->setPixmap(QPixmap::fromImage(imag));
    flag++;
}

/************************************************************************************
 ***》》》                 鼠标事件，人物位置操作
 * **********************************************************************************/
void role::mousePressEvent(QMouseEvent *event)//鼠标点击事件
{

    if(event->button() == Qt::LeftButton)//判断是否为鼠标左键
    {
        m_bDrag = true;
        //获得鼠标x坐标的初始位置
        mou_x = event->globalPos().x();//水平移动
        //        mouseStartPoint = event->globalPos();//任意方向移动
        //获得窗口x坐标的初始位置
        win_x = this->frameGeometry().topLeft().x();//水平移动
        //        windowLeftTop = this->frameGeometry().topLeft();
    }
}

void role::mouseMoveEvent(QMouseEvent *event)//鼠标移动事件
{

    if(m_bDrag)
    {
        //获得鼠标x坐标移动的距离
        int distan = event->globalPos().x() - mou_x;//获取水平坐标距离
        //        distence = event->globalPos() - mouseStartPoint;//获取交叉坐标点距离（即(x,y)）
        //改变x坐标窗口的位置
        this->move(win_x+distan,(deskRect.height()-129));
        //        this->move(windowLeftTop + distence);
    }
}

void role::mouseReleaseEvent(QMouseEvent *event)//鼠标松开事件
{

    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = false;
    }
    //鼠标键松开碰到屏幕边缘会复位
    if(this->frameGeometry().topLeft().x()>(deskRect.width()-102))   //deskRect.width()-102 == 1818
    {
        this->setGeometry((deskRect.width()-105),(deskRect.height()-129),300,300);//窗口跳到屏幕最右边位置   //deskRect.width()-105 == 1815
    }
    else if(this->frameGeometry().topLeft().x()<(deskRect.width()-1928))   //deskRect.width()-1930 == -10
    {
        this->setGeometry((deskRect.width()-1928),(deskRect.height()-129),300,300);//窗口跳到屏幕最左边位置   //deskRect.width()-1928 == -8
    }
    //        qDebug() << this->pos();
    //        qDebug() << deskRect.width();//可用窗口宽度
    //        qDebug() << deskRect.height();//可用窗口高度
}

void role::mouseDoubleClickEvent(QMouseEvent *event)//鼠标双击事件
{
    static bool flag = false;
    if(event->button() == Qt::LeftButton)
    {
        if(false == flag)
        {
            my_audio->startAudio("audiofile");
            QMessageBox::warning(NULL,"录音","双击人物结束录音");
            flag = true;
        }
        else{
            my_audio->stopAudio();
            retStr = my_audio->startSpeech();
            qDebug() << retStr;
            my_say->say(retStr);
            flag = false;
        }
    }
    if(event->button() == Qt::RightButton)
    {
        if(my_wall.nextWallpaper() == false)
            myIcon->showMessage("小埋壁纸","未加入壁纸哦",QSystemTrayIcon::Information,1000);
    }
}

/************************************************************************************
 ***》》》                         鼠标右击，系统托盘菜单设置
 ***********************************************************************************/
void role::createActions()
{       //鼠标右击功能
    mShowMainAction = new QAction("显示",this);
    mShowMainAction->setIcon(QIcon(":/pic/xiaomai00.ico"));
    connect(mShowMainAction,SIGNAL(triggered()),this,SLOT(on_showMainAction()));

    mHideAppAction = new QAction("隐藏",this);
    mHideAppAction->setIcon(QIcon(":/pic/xiaomai01.ico"));
    //connect(mHideAppAction,&QAction::triggered,[&](){this->hide();});
    connect(mHideAppAction,SIGNAL(triggered()),this,SLOT(on_hideMainAction()));

    mExitAppAction = new QAction("退出",this);
    mExitAppAction->setIcon(QIcon(":/pic/xiaomai02.ico"));
    connect(mExitAppAction,SIGNAL(triggered()), this, SLOT(on_exitAppAction()));

    mWearAppAction = new QAction("天气",this);
    mWearAppAction->setIcon(QIcon(":/pic/天气.ico"));
    connect(mWearAppAction,SIGNAL(triggered()),this,SLOT(weather_menu()));

    mWallAppAction = new QAction("壁纸",this);
    mWallAppAction->setIcon(QIcon(":/pic/壁纸.ico"));
    connect(mWallAppAction,SIGNAL(triggered()),this,SLOT(wallpaper_menu()));
}
void role::createMenu()
{
    mMenu = new QMenu(this);
    mMenu->addAction(mWallAppAction);//显示壁纸菜单
    mMenu->addAction(mWearAppAction);//显示天气菜单
    mMenu->addAction(mShowMainAction);//添加图标显示功能
    mMenu->addAction(mHideAppAction);//添加图标隐藏功能
    mMenu->addAction(mExitAppAction);//添加图标退出功能
}
void role::on_showMainAction()
{
    this->show();//显示主窗口
}

void role::on_hideMainAction()
{
    this->hide();//隐藏主窗口
}

void role::on_exitAppAction()
{
    exit(0);//退出主窗口
}

void role::weather_menu()
{
    my_wea.show();
}

void role::wallpaper_menu()
{
    my_wall.show();
}

/************************************************************************************
 ***》》》                                 鼠标左击菜单
 ***********************************************************************************/
void role::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::Trigger://鼠标左键单击
        this->show();
        break;
    case QSystemTrayIcon::DoubleClick://鼠标左键双击
        this->hide();
        //        myIcon->showMessage("小埋管家","欢迎哦",QSystemTrayIcon::Information, 1000);//windows提示
        break;
    default:
        break;
    }
}


//#include <QMessageBox>头文件
//void object::closeEvent(QCloseEvent *event)//关窗提示
//{//已被替代
//    QMessageBox *messg = new QMessageBox;
//    //QMessageBox::information(this, "小埋管家", "程序后台运行");
//    messg->setGeometry(0,0,0,0);
//    messg->information(this,"小埋管家", "程序后台运行");
//    setVisible(false);
//    event->ignore();
//}

void role::dragEnterEvent(QDragEnterEvent *event) //拖动文件到窗口，触发
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction(); //事件数据中存在路径，方向事件
    }
    else
    {
        event->ignore();
    }
}

void role::dropEvent(QDropEvent *event) //拖动文件到窗口释放文件，触发
{
    QString fileName;
    if(event->mimeData()->hasUrls())
    {
        QList<QUrl> urls = event->mimeData()->urls();
        fileName = urls.at(0).toLocalFile();
        DeleteFileOrFolder(fileName);
    }
}

bool role::DeleteFileOrFolder(const QString &strPath)//要删除的文件夹或文件的路径
{
    if (strPath.isEmpty() || !QDir().exists(strPath))//是否传入了空的路径||路径是否存在
        return false;

    QFileInfo FileInfo(strPath);

    if (FileInfo.isFile())//如果是文件
        QFile::remove(strPath);
    else if (FileInfo.isDir())//如果是文件夹
    {
        QDir qDir(strPath);
        qDir.removeRecursively();
    }
    return true;
}
