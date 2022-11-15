#include <QApplication>
#include "judgewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JudgeWidget::instance()->show();
    return a.exec();
}
