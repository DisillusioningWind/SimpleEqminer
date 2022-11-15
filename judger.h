#ifndef JUDGER_H
#define JUDGER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QVector>
#include <QStringList>
#include <QMap>
#include <QRegExp>
#include <QTime>
#include <QRandomGenerator>

class Judger : public QObject
{
    Q_OBJECT

    typedef enum class JudgeInfo
    {
        NoOutputInfo,
        InputDirPathDetected,
        InputDirPathNotExist,
        SubDirPathDetected,
        SubDirPathNotExist,
        FormatFilePathNotExist,
        FormatFileDataLoaded,
        RandomTestFileWrited,
        CompileFileCompleted,
        CompileFileFailed,
        TestFileRuntimeError,
        TestFileResultGenerated,
    } JInfo;

private:
    //does not change
    const QString OutputDirName = "output";
    const QString OutputEqualFileName = "equal.csv";
    const QString OutputInequalFileName = "inequal.csv";
    const QString FormatFileName = "stdin_format.txt";
    const QString RandomFileNamePrefix = "randomInput";
    QString InputDirPath;
    QString InputDirName;
    QString InputFileData;
    QStringList SubDirPath;
    int TestNum = 1;
    //change every sub dir
    QString CurSubDirPath;
    QString CurSubDirName;
    int CurSubDirIndex = 1;
    QString CurSubForPath;
    QStringList InputFormatStr;
    QStringList FileList;
    QMap<QString,QStringList> InequalFileList;
    //change every file
    QString CurCompileCmd;
    QString CurRunCmd;
public:
    explicit Judger(QObject *parent = nullptr);

signals:

public slots:
    void printJudgeInfo(JInfo inf);
    void runJudger(QString inputDirPath);
    QString getInputDirPath() const;
    QString readInputFileData();
    JInfo setInputDirPath(QString path);
    JInfo setSubDirPath();
    JInfo readFormatFileData(QString subDirPath);
    JInfo writeRandomTestFileData(QString subDirPathAbs);
    JInfo testSubDir(QString subDirPathAbs);
    JInfo generateTestFile(QString filePathAbs);
    JInfo runTestFile(QString filePathAbs, QString randomInputFilePathAbs);
    JInfo writeResult();
};

#endif // JUDGER_H
