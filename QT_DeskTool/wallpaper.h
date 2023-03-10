#ifndef WALLPAPER_H
#define WALLPAPER_H

#include <QWidget>

#include "windows.h"
#include <QSettings>

#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QTimer>

#include <QFileDialog>

#include <QSystemTrayIcon>

class wallpaper : public QWidget
{
    Q_OBJECT
public:
    explicit wallpaper(QWidget *parent = nullptr);

public:
    QPushButton *bt;
    QRadioButton *rd;

    QTimer *timer;

    QStringList pixs;
    int index;

public:
        void setDesktopImage(QString);
        bool nextWallpaper();

public slots:
    void change_wall();
    void time_start();
    void auto_wall(bool);

};

#endif // WALLPAPER_H
