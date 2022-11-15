#ifndef JUDGEWIDGET_H
#define JUDGEWIDGET_H

#include "classinstance.h"

#include <QWidget>
#include <QDebug>
#include <QLayout>
#include <QSpacerItem>
#include <QTextEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>

class JudgeWidget : public QWidget
{
    Q_OBJECT

private:
    //widgets
    QTextEdit* edInput;
    QTextBrowser* brInput;
    QPushButton* btnInput;
    QGridLayout* layInput;

    //initialize
    void iniUI();
    void iniStyleSheet();
    void iniSignalSlots();

    //events
    void dragEnterEvent(QDragEnterEvent* e);
    void dropEvent(QDropEvent* e);
public:
    JudgeWidget(QWidget *parent = nullptr);
    ~JudgeWidget();

    //instance
    static JudgeWidget* instance();
    static void logOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg);
    static void logWrite(QString log);
public slots:
    void onBtnInputClick();
};

#endif // JUDGEWIDGET_H
