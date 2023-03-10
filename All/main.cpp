#include "role.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    role r;
    r.show();
    return a.exec();
}
