#include "judgewidget.h"

JudgeWidget::JudgeWidget(QWidget *parent) : QWidget(parent)
{
    iniUI();
    iniStyleSheet();
    iniSignalSlots();
}
JudgeWidget::~JudgeWidget()
{}

JudgeWidget *JudgeWidget::instance()
{
    static JudgeWidget ins;
    return &ins;
}
void JudgeWidget::logOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString log;
    switch(type)
    {
    case QtInfoMsg:
        log.append("Info: ");
        break;
    case QtDebugMsg:
        log.append("Debug: ");
        break;
    case QtWarningMsg:
        log.append("Warning: ");
        break;
    case QtCriticalMsg:
        log.append("Critical: ");
        break;
    case QtFatalMsg:
        log.append("Fatal: ");
        break;
    }
    log.append(msg);
    logWrite(log);
}
void JudgeWidget::logWrite(QString log)
{
       instance()->brInput->append(log);
}

void JudgeWidget::iniUI()
{
    //window setting
    qInstallMessageHandler(logOutput);
    setAcceptDrops(true);
    this->resize(1200,700);
    //widget
    QString testInputPath = "/home/whr/Projects/input";
    edInput = new QTextEdit();
    brInput = new QTextBrowser();
    btnInput = new QPushButton("Start judging");
    //widget setting
    edInput->setAcceptDrops(false);
    //layout
    layInput = new QGridLayout(this);
    layInput->setMargin(20);
    layout()->setSpacing(10);
    //spacer
    QSpacerItem* spa = new QSpacerItem(400,400,QSizePolicy::Maximum,QSizePolicy::Expanding);

    layInput->addWidget(edInput,1,1);
    layInput->addWidget(btnInput,2,1);
    layInput->addItem(spa,3,1);
    layInput->addWidget(brInput,1,2,3,1);
}
void JudgeWidget::iniStyleSheet()
{

}
void JudgeWidget::iniSignalSlots()
{
    connect(btnInput,SIGNAL(clicked()),this,SLOT(onBtnInputClick()));
}

void JudgeWidget::dragEnterEvent(QDragEnterEvent *e)
{
    if(e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
}
void JudgeWidget::dropEvent(QDropEvent *e)
{
    foreach (QUrl url, e->mimeData()->urls()) {
        QString dirPath = url.toLocalFile();
        qInfo()<< "dirPath:"<< dirPath;
        edInput->setText(dirPath);
    }
}

void JudgeWidget::onBtnInputClick()
{
    judger.runJudger(edInput->toPlainText());
}
