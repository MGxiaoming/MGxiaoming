#include "weather.h"
#include "ui_weather.h"

weather::weather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::weather)
{
    ui->setupUi(this);
    this->setWindowTitle("小埋天气");

    Init();//初始化

    this->setFixedSize(this->width(),this->height());//固定窗口大小
}

weather::~weather()
{
    delete ui;
}

void weather::Init()
{

    num = 0;

    this->rangewendu[0] = ui->range_wendu1;
    this->rangewendu[1] = ui->range_wendu2;
    this->rangewendu[2] = ui->range_wendu3;
    this->rangewendu[3] = ui->range_wendu4;
    this->rangewendu[4] = ui->range_wendu5;

    wtpye[0] = ui->typelb1;
    wtpye[1] = ui->typelb2;
    wtpye[2] = ui->typelb3;
    wtpye[3] = ui->typelb4;
    wtpye[4] = ui->typelb5;

    windlb[0] = ui->windlb1;
    windlb[1] = ui->windlb2;
    windlb[2] = ui->windlb3;
    windlb[3] = ui->windlb4;
    windlb[4] = ui->windlb5;

    aqilb[0] = ui->aqilb1;
    aqilb[1] = ui->aqilb2;
    aqilb[2] = ui->aqilb3;
    aqilb[3] = ui->aqilb4;
    aqilb[4] = ui->aqilb5;

    datalb[1] = ui->datalb2;
    datalb[2] = ui->datalb3;
    datalb[3] = ui->datalb4;
    datalb[4] = ui->datalb5;

    typepiclb[0] = ui->typepiclb1;
    typepiclb[1] = ui->typepiclb2;
    typepiclb[2] = ui->typepiclb3;
    typepiclb[3] = ui->typepiclb4;
    typepiclb[4] = ui->typepiclb5;

    //天气url
//    QUrl url("http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=贵阳&needMoreData=true&pageNo=1&pageSize=7");
    QUrl url("https://restapi.amap.com/v3/weather/weatherInfo?city=110101&key=006f7d6f8c1b7892048479873920a14b");
    manager = new QNetworkAccessManager(this);    //创建一个请求对象
    mtimer = new QTimer(this);
    QNetworkRequest request(url); //发送天气请求
    manager->get(request);//获取通过http回传的天气信息

    //关联finished信号
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(read_data(QNetworkReply*)));//信号与槽：如果回传信息成功，则记录获取的json信息
    //获取时间以及刷新天气页面
    connect(mtimer,SIGNAL(timeout()),this,SLOT(run()));
    mtimer->start(1000);// 1秒计时
}

//判断空气指数是哪个级别
void weather::aqi_result(int aqi,int i)
{
    QString aqi_rank;

    if(aqi<=50) {aqi_rank ="优";aqilb[i]->setStyleSheet("background-color:#55ff7f");}
    else if(aqi<=100) {aqi_rank ="良"; aqilb[i]->setStyleSheet("background-color:#ffff00");}
    else if(aqi<=150) {aqi_rank ="轻度污染";aqilb[i]->setStyleSheet("background-color:#ffaa00");}
    else if(aqi<=200) {aqi_rank ="中度污染";aqilb[i]->setStyleSheet("background-color:#ff5500");}
    else if(aqi<=300) {aqi_rank ="重污染";aqilb[i]->setStyleSheet("background-color:#b83d00"); }
    else {aqi_rank ="严重污染";aqilb[i]->setStyleSheet("background-color:#686868"); }
    qDebug()<<aqi_rank;
    aqilb[i]->setText(" "+QString::number(aqi)+" "+aqi_rank);
}

//判断天气
void weather::type_result(QString type,int i)
{
    QString path;
    if(type == "晴"){path = ":/pic/晴天.png";}
    else if(type=="多云" || type == "晴转多云"){path = ":/pic/多云.png";}
    else if(type=="阴"){path = ":/pic/阴天.png";}
    else if(type=="阵雨"){path = ":/pic/阵雨.png";}
    else if(type=="雷阵雨"){path = ":/pic/雷阵雨.png";}
    else if(type=="小雨"){path = ":/pic/小雨.png";}
    else if(type=="中雨"){path = ":/pic/中雨.png";}
    else if(type=="大雨"){path = ":/pic/大雨.png";}
    else if(type=="暴雨"){path = ":/pic/暴雨.png";}
    else if(type =="大暴雨"||type =="大到暴雨"){path = ":/pic/特大暴雨.png";}
    else {path = ":/pic/扬沙.png";}
    typepiclb[i]->setPixmap(QPixmap(path));
}
void weather::background_main(QString weatherpic)//界面背景（无法更改）
{
//    if(weatherpic=="晴"||weatherpic == "多云" ||weatherpic == "晴转多云"||weatherpic=="阴")
//    {
//       this->setStyleSheet("border-image:url(:/picture/background800.png)");
//    }
//    else  this->setStyleSheet("border-image:url(:/picture/background800.png)");
}
void weather::read_data(QNetworkReply *reply)//解析获取的json信息  -------------------------------------json解析
{
    QByteArray array = reply->readAll();//读出所有的json信息到特定的数组中
    QJsonParseError error;//创建错误对象，判断是否成功获取到相关的json信息
    QJsonDocument doc = QJsonDocument::fromJson(array,&error);//创建一个json文档，并将特定数组中的json信息转换成json文档储存
    qDebug() << reply->readAll();

    if(error.error !=QJsonParseError::NoError)
    {
        qDebug("数据错误");//判断json内容是否存在
        return ;
    }
    QJsonObject obj =  doc.object();//将json文档转换成一个大整体的json对象
    //提示操作成功
    qDebug() << obj.value("msg").toString();
    qDebug()<<"开始===================================开始";

    //时间
    QString time = obj.value("data").toObject().value("list").toArray().at(0).toObject().value("lastUpdateTime").toString();
    time = time.mid(5,5);
    QDateTime datetime = QDateTime::currentDateTime();
    QString date_week = datetime.toString("ddd");

    /*******************************************json解析原理*****************************************
1--- {"code":0,    注：开头数字并非json信息中的，而是我自己标识的序号，方便分类各个json片段
    "msg":"操作成功",
    "data":{------2
        "total":7,
        "sourceName":"墨迹天气",
        "list":[------3
            {-------4
                "city":"贵阳",
                "lastUpdateTime":"2022-10-29 11:55:08",
                "date":"2022-10-29",
                "weather":"阴",
                "temp":9,
                "humidity":"94%",
                "wind":"南风2级",
                "pm25":16,
                "pm10":18,
                "low":9,
                "high":15,
                "airData":"18",
                "airQuality":"优",
                "dateLong":1666972800000,
                "weatherType":2,
                "windLevel":2,
                "province":"贵州",
                "moreData":{------5
                    "sunrise":"2022-10-29 07:01:00",
                    "sunset":"2022-10-29 18:13:00",
                    "precipitation":"0",
                    "alert":null
                                  }------5对象
              }------4对象
                     ]------3数组
                }------2对象
对象1--}
    1.首先在网络上通过在线json信息标准化显示工具将一长串json信息以上面的形式显示，方便后续分类json中的信息对象、数组、值等
    2.如上标准的json信息显示，其只是整个json信息中的其中一个json对象
    3.故需要先将整个json信息先转换成json文档，再将一整个json文档拆分成一个个小对象，再取出json对象中单独的json值，即可使用
    4.如果一个json值又是一个json对象（即值的后面有一个花括号{}，其就是一个json对象），那就还需要将此json值转换成json对象再取出其中的值
    5.如果一个json值是一个json数组（即值的后面是一个方括号[]，其就是一个json数组），那也要将其json值转换成json对象再取出其中的值
            由上可知：json文档  >  json对象  >  json数组  >  json值
//不确定：     但只有单个的json值可以被直接取出使用，而且json值只能从json对象中取出，即json值的母亲是json对象，其他都是父亲，不能直接取
    *******************************************json解析原理*****************************************/

    //地区
    QString city = obj.value("data").toObject().value("list").toArray().at(0).toObject().value("city").toString();

    //实时温度
    int temp = obj.value("data").toObject().value("list").toArray().at(0).toObject().value("temp").toInt();
    QString wendu = QString::number(temp);
    ui->wendu->setText(wendu);

    qDebug()<<time<<"  "<<city<<"  "<<"温度："<<wendu;
    qDebug()<<"===================================";

    //近一周的天气
    QJsonArray weather_array = obj.value("data").toObject().value("list").toArray();
    for(int i =0;i<5;i++)
    {
        QJsonObject weatherobj = weather_array.at(i).toObject();
        //最高温度
        QString high =QString::number(weatherobj.value("high").toInt());
        //high = high.mid(3);//返回字符串的前n个字符？？？？？？？？？？？
        //最低温度
        QString low = QString::number(weatherobj.value("low").toInt());
        //low = low.mid(3,2);//返回字符串的前n个字符？？？？？？？？？？？
        qDebug()<<"最低气温："<<low<<"最高气温："<<high;
        //温度范围
        QString range_wendu = low+" ~ "+high+" ℃";
        qDebug()<<"温度范围："<<range_wendu;
        rangewendu[i]->setText(range_wendu);

        //日期
        QString ymd = weatherobj.value("date").toString();
        ymd = ymd.mid(5);
        qDebug()<<ymd;
        qDebug()<<"===================================";
        if(i!=0){
            datalb[i]->setText(ymd);
        }

        //天气
        QString type = weatherobj.value("weather").toString();
        if(i == 0) weatherpic = type;
        wtpye[i]->setText(type);
        type_result(type,i);

        //风
        QString fx = weatherobj.value("wind").toString();
        //QString fl = weatherobj.value("fl").toString();
        QString wind = fx;
        windlb[i]->setText(wind);

        //空气质量
        QString aqi_str = weatherobj.value("airData").toString();
        int aqi = aqi_str.toInt();
        aqi_result(aqi,i);

        qDebug()<<"===================================";
        qDebug()<<type<<"  "<<wind<<"  "<<aqi;
    }
    //日月
    ui->datalb->setText(" "+time);
    //星期
    ui->weeklab->setText(date_week);

    //设置QMainWindow的背景，没卵用
    background_main(weatherpic);

    //实时天气
    ui->currweatherlb->setText(weatherpic+" (实时) ");

}

void weather::get_weather()//获取城市天气的API
{
    QString ip;

    //通过获取城市输入框的当前值来选择哪个城市的URL
    QString city = ui->comboBox->currentText();
    if(city =="贵阳市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=贵阳市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="北京市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=北京市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="广州市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=广州市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="合肥市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=合肥市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="襄阳市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=襄阳市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="上海市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=上海市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="成都市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=成都市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="武汉市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=武汉市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="西安市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=西安市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="北京市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=北京市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="海南市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=海南市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="济南市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=济南市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="石家庄"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=石家庄&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="哈尔滨"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=哈尔滨&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="沈阳市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=沈阳市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="呼和浩特"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=呼和浩特&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="乌鲁木齐"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=乌鲁木齐&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="兰州市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=兰州市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="银川市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=银川市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="郑州市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=郑州市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="杭州市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=杭州市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="南昌市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=南昌市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="南宁市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=南宁市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="长沙市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=长沙市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="昆明市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=昆明市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="拉萨市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=拉萨市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="西宁市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=西宁市&needMoreData=true&pageNo=1&pageSize=7";}
    else if(city=="福州市"){ip = "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=福州市&needMoreData=true&pageNo=1&pageSize=7";}

    //天气url
    QUrl url(ip);
    //创建一个请求对象
    QNetworkRequest request(url);//发送请求
    manager->get(request);//获取回传数据

}

void weather::run()
{
    QString time =  QTime::currentTime().toString("hh:mm:ss");
    ui->timelb->setText(time);
    num++;
    //隔五分钟刷新一次
    if(num>=5*60){get_weather();num = 0;}
    //get_weather();
}

void weather::on_comboBox_activated(const QString &addr_str)
{
    ui->addr->setText(addr_str);
    get_weather();
}
