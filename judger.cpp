#include "judger.h"

Judger::Judger(QObject *parent) : QObject(parent)
{

}

void Judger::runJudger(QString inputDirPath)
{
    JInfo inf;
    inf = setInputDirPath(inputDirPath);
    printJudgeInfo(inf);
    inf = setSubDirPath();
    printJudgeInfo(inf);
    for (CurSubDirIndex = 1; CurSubDirIndex < SubDirPath.size(); CurSubDirIndex++)
    {
        CurSubDirName = SubDirPath[CurSubDirIndex];
        inf = readFormatFileData(SubDirPath[CurSubDirIndex]);
        printJudgeInfo(inf);
        inf = writeRandomTestFileData(CurSubDirPath);
        printJudgeInfo(inf);
        inf = testSubDir(CurSubDirPath);
        printJudgeInfo(inf);
    }
}
void Judger::printJudgeInfo(JInfo inf)
{
    if(inf == JInfo::NoOutputInfo);
    else if(inf == JInfo::InputDirPathDetected)
        qInfo()<<"InputDirPath loaded:"<< InputDirPath;
    else if(inf == JInfo::InputDirPathNotExist)
        qInfo()<<"InputDirPath does not exist:"<< InputDirPath;
    else if(inf == JInfo::SubDirPathDetected)
        qInfo()<<"SubDirPath detected:"<< SubDirPath;
    else if(inf == JInfo::SubDirPathNotExist)
        qInfo()<<"SubDirPath does not exist:"<< SubDirPath;
    else if(inf == JInfo::FormatFileDataLoaded)
    {
        qInfo()<<"Now in subDir:"<<SubDirPath[CurSubDirIndex];
        qInfo()<<"FormatFileData loaded:"<< InputFormatStr;
    }
    else if(inf == JInfo::FormatFilePathNotExist)
        qInfo()<<"FormatFilePath does not exist:"<< CurSubForPath;
    else if(inf == JInfo::RandomTestFileWrited)
        qInfo()<<"Random Test File Writed";
    else if(inf == JInfo::CompileFileFailed)
        qWarning() << "Compile failed,command:"<< CurCompileCmd;
    else if(inf == JInfo::CompileFileCompleted)
        qInfo() << "Compile completed,command:"<< CurCompileCmd;
    else if(inf == JInfo::TestFileRuntimeError)
        qWarning() << "TestFile runtime error,command:"<< CurRunCmd;
    else if(inf == JInfo::TestFileResultGenerated)
        qInfo() << "TestFile result generated,command:"<< CurRunCmd;
}

Judger::JInfo Judger::setInputDirPath(QString path)
{
    InputDirPath = path;
    InputDirName = path.right(path.size() - path.lastIndexOf("/") - 1);
    return JInfo::InputDirPathDetected;
}
QString Judger::getInputDirPath() const
{
    return InputDirPath;
}
QString Judger::readInputFileData()
{
    QFile f(InputDirPath);
    if(f.open(QIODevice::Text | QIODevice::ReadOnly))
    {
        InputFileData = f.readAll();
        f.close();
        return InputFileData;
    }
    return "Open file failed!";
}

Judger::JInfo Judger::setSubDirPath()
{
    QDir d(InputDirPath);
    if(d.exists())
    {
        SubDirPath = d.entryList(QDir::Filter::NoDotAndDotDot | QDir::Filter::Dirs);
        return JInfo::SubDirPathDetected;
    }
    return JInfo::InputDirPathNotExist;
}
Judger::JInfo Judger::readFormatFileData(QString subDirPath)
{
    QDir d(InputDirPath);
    d.cd(subDirPath);
    CurSubDirPath = d.path();
    if(d.exists())
    {
        CurSubForPath = d.filePath(FormatFileName);
        QFile f(CurSubForPath);
        if(f.exists())
        {
            f.open(QIODevice::Text | QIODevice::ReadOnly);
            QTextStream in(&f);
            while(!in.atEnd())
            {
                QString line = in.readLine();
                InputFormatStr += line.split(" ");
            }
            f.close();
            return JInfo::FormatFileDataLoaded;
        }
        return JInfo::FormatFilePathNotExist;
    }
    return JInfo::SubDirPathNotExist;
}
Judger::JInfo Judger::writeRandomTestFileData(QString subDirPathAbs)
{
    QDir d(subDirPathAbs);
    for (int i = 0; i < TestNum; i++)
    {
        QString randomFileName = d.filePath(RandomFileNamePrefix + QString::number(i) + ".txt");
        QFile rf(randomFileName);
        rf.open(QIODevice::Text | QIODevice::WriteOnly);
        QTextStream out(&rf);
        //regex
        QRegExp rxInt("^int\\((\\d+),(\\d+)\\)$");
        QRegExp rxChar("^char$");
        QRegExp rxStr("^string\\((\\d+),(\\d+)\\)$");
        //for random generator
        const char randomArr[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        for (int j = 0; j < InputFormatStr.size(); j++)
        {
            //qsrand(QTime::currentTime().msec() + QTime::currentTime().second()*1000);
            if(rxInt.indexIn(InputFormatStr[j]) != -1)
            {
                int rInt = QRandomGenerator::global()->bounded(rxInt.cap(1).toInt(),rxInt.cap(2).toInt() + 1);
                out << rInt << " ";
            }
            if(rxChar.indexIn(InputFormatStr[j]) != -1)
            {
                int idx = QRandomGenerator::global()->bounded(1,53);
                char rChar = randomArr[idx];
                out << rChar << " ";
            }
            if(rxStr.indexIn(InputFormatStr[j]) != -1)
            {
                QString rStr;
                int len = QRandomGenerator::global()->bounded(rxStr.cap(1).toInt(),rxStr.cap(2).toInt() + 1);
                for(int k = 0; k < len; k++)
                {
                    rStr.append(randomArr[QRandomGenerator::global()->bounded(1,53)]);
                }
                out << rStr << " ";
            }
        }
    }
    return JInfo::RandomTestFileWrited;
}
Judger::JInfo Judger::testSubDir(QString subDirPathAbs)
{
    QDir d(subDirPathAbs);
    QStringList filter;
    filter.append("*.c");
    filter.append("*.cpp");
    d.setNameFilters(filter);
    QStringList fileList = d.entryList();
    FileList = fileList;

    for (int i = 0; i < fileList.size(); i++)
    {
        QString filePathAbs = d.filePath(fileList[i]);
        JInfo inf = generateTestFile(filePathAbs);
        printJudgeInfo(inf);
    }

    for (int i = 0; i < TestNum; i++)
    {
        QString randomInputFilePathAbs = d.filePath(RandomFileNamePrefix + QString::number(i) + ".txt");
        for (int j = 0; j < fileList.size(); j++)
        {
            QString testFilePathAbs = d.filePath(fileList[j]);
            JInfo inf = runTestFile(testFilePathAbs, randomInputFilePathAbs);
            printJudgeInfo(inf);
        }
        for (int a = 0; a < FileList.size() - 1; a++)
        {
            for (int b = a + 1; b < FileList.size(); b++)
            {
                QString output1 = FileList[a].left(FileList[a].lastIndexOf(".")) + ".txt";
                QString output2 = FileList[b].left(FileList[b].lastIndexOf(".")) + ".txt";
                QString path1 = d.filePath(output1);
                QString path2 = d.filePath(output2);
                QFile file1(path1);
                QFile file2(path2);
                if(file1.exists() && file2.exists())
                {
                    file1.open(QIODevice::Text | QIODevice::ReadOnly);
                    file2.open(QIODevice::Text | QIODevice::ReadOnly);
                    QString str1 = file1.readAll();
                    QString str2 = file2.readAll();
                    if(str1 != str2)
                    {
                        InequalFileList[FileList[a]].append(FileList[b]);
                        InequalFileList[FileList[b]].append(FileList[a]);
                    }
                    file1.close();
                    file2.close();
                }
            }
        }
    }

    JInfo inf = writeResult();
    printJudgeInfo(inf);
}
Judger::JInfo Judger::generateTestFile(QString filePathAbs)
{
    QString testFilePathAbs = filePathAbs.left(filePathAbs.lastIndexOf(".")) + ".out";
    //compile cmd str
    QString cmd;
    cmd += "g++ -O ";
    cmd += filePathAbs;
    cmd += " -o ";
    cmd += testFilePathAbs;
    CurCompileCmd = cmd;

    int ret = system(cmd.toStdString().c_str());
    if(ret != 0)
    {
        return JInfo::CompileFileFailed;
    }
    return JInfo::CompileFileCompleted;
}
Judger::JInfo Judger::runTestFile(QString filePathAbs, QString randomInputFilePathAbs)
{
    QString testRunFilePathAbs = filePathAbs.left(filePathAbs.lastIndexOf(".")) + ".out";
    QString testOutputFilePathAbs = filePathAbs.left(filePathAbs.lastIndexOf(".")) + ".txt";
    QString cmd;
    cmd += testRunFilePathAbs;
    cmd += " <";
    cmd += randomInputFilePathAbs;
    cmd += " >";
    cmd += testOutputFilePathAbs;
    cmd += " 2>&1";
    CurRunCmd = cmd;

    int ret = system(cmd.toStdString().c_str());
    if(ret != 0)
    {
        return JInfo::TestFileRuntimeError;
    }
    return JInfo::TestFileResultGenerated;
}
Judger::JInfo Judger::writeResult()
{
    QDir d;
    d.mkdir(OutputDirName);
    QString curPath = QDir::currentPath();
    QString prefix = InputDirName + "/" + CurSubDirName + "/";
    QString equalFilePath = curPath + "/" + OutputDirName + "/" + OutputEqualFileName;
    QString inEqualFilePath = curPath + "/" + OutputDirName + "/" + OutputInequalFileName;
    QFile eq(equalFilePath);
    QFile ineq(inEqualFilePath);
    eq.open(QIODevice::WriteOnly | QIODevice::Truncate);
    ineq.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream steq(&eq);
    QTextStream stineq(&ineq);
    steq<<"file1,file2\n";
    stineq<<"file1,file2\n";
    for (int a = 0; a < FileList.size() - 1; a++)
    {
        for (int b = a + 1; b < FileList.size(); b++)
        {
            QString file1 = FileList[a];
            QString file2 = FileList[b];
            if(InequalFileList[file1].contains(file2))
            {
                stineq<<prefix + file1<<","<<prefix + file2<<"\n";
            }
            else
            {
                steq<<prefix + file1<<","<<prefix + file2<<"\n";
            }
        }
    }
    eq.close();
    ineq.close();
    return JInfo::NoOutputInfo;
}
