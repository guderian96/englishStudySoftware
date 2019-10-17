#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QDateTime>
#include <QMediaPlayer>
#include <QtTextToSpeech/QTextToSpeech>
#include <string>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initList();

    ui->textInput->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::vector<std::string> MainWindow::split(const std::string& src, const std::string& separator)
{
    std::vector<std::string> dest;
    std::string str = src;
    std::string substring;
    std::string::size_type start = 0, index;

    do
    {
        index = str.find_first_of(separator,start);
        if (index != std::string::npos)
        {
            substring = str.substr(start,index-start);
            dest.push_back(substring);
            start = str.find_first_not_of(separator,index);
            if (start == std::string::npos) return dest;
        }
    }while(index != std::string::npos);

    //the last token
    substring = str.substr(start);
    dest.push_back(substring);
    return dest;
}


void MainWindow::initList(){
    englishWordList_.clear();

    FILE* fp;
    fp = fopen("E:\\list.txt", "r");
    char in[100];
    while (fscanf(fp, "%s", &in) != EOF)
    {
        englishWord ew;
        std::string incs(in);
        ew.word = incs;
        char attr[100];
        fscanf(fp, "%s", &attr);
        std::string attrcs(attr);
        ew.attr = attrcs;
        char mean[100];
        while(fscanf(fp, "%s", &mean)){
            std::string meancs(mean);
            if(meancs == "|")
                break;
            std::string lastStr = meancs.substr(meancs.size() - 1);
            if(lastStr == ";" || lastStr == "," || lastStr == "，"){
                meancs = meancs.substr(0 ,meancs.size() - 1);
            }
            auto subMeans = split(meancs, ",");
            for(auto subMean : subMeans){
                 ew.means.push_back((subMean));
            }
        }
        englishWordList_.push_back(ew);
    }
    fclose(fp);
    std::string number = std::to_string(englishWordList_.size());
    ui->textDebugInfo->setText(number.c_str());
}

void MainWindow::on_pushButtonInit_clicked()
{
    initList();
}

void MainWindow::clearEditTexts(){
    ui->textChinese->setText("");
    ui->textWordAttr->setText("");
    ui->textEnglish->setText("");
    ui->textInput->setText("");
    ui->textJudeResult->setText("");
}

void MainWindow::showChineseMeans(){
    std::string means;
    for(auto& mean : englishWordList_[currentIndex_].means){
        means += mean;
        means += "      ";
    }
    QString chinese = QString::fromStdString(means);
    ui->textChinese->setText(chinese);
}

void MainWindow::showAllInfo(){
    showChineseMeans();
    ui->textEnglish->setText(englishWordList_[currentIndex_].word.c_str());
    ui->textWordAttr->setText(englishWordList_[currentIndex_].attr.c_str());
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event){
    if (obj == ui->textInput) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (event->type() == QEvent::KeyPress) {
            if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter){
                on_pushButtonJudge_clicked();
                return true;
            }
            else if(keyEvent->key() == Qt::Key_Down){
                on_pushButtonNext_clicked();
                return true;
            }
        }
        else {
            return false;
        }
    } else {
        // pass the event on to the parent class
        return QMainWindow::eventFilter(obj, event);
    }
}

void MainWindow::on_pushButtonNext_clicked()
{
    std::string modeState = settings_.getJudgeMode();
    clearEditTexts();
    int stepping = 1;
    std::string listModeState = settings_.getListMode();
    int stepBaseNUmber = settings_.getStep();
    if(listModeState == "randomMode")
        stepping = std::rand() % stepBaseNUmber + 1;
    currentIndex_ = (currentIndex_ + stepping) % englishWordList_.size();

    if(modeState == "listenMode"){
       checkLoaclFile();
    }
    else if(modeState == "chineseToEnglishMode"){
        ui->textWordAttr->setText(englishWordList_[currentIndex_].attr.c_str());
        showChineseMeans();
    }
    else{
        ui->textEnglish->setText(englishWordList_[currentIndex_].word.c_str());
        ui->textWordAttr->setText(englishWordList_[currentIndex_].attr.c_str());
    }
    ui->textInput->setFocus();
}

void MainWindow::on_pushButtonJudge_clicked()
{
    QString input = ui->textInput->toPlainText();
    std::string inputStr = input.toStdString();
    bool found = false;
    //std::string output = englishWordList_[currentIndex_].word + " " + inputStr;
    //ui->textDebugInfo->setText(output.c_str());
    std::string modeState = settings_.getJudgeMode();

    if(modeState == "listenMode" || modeState == "chineseToEnglishMode"){
        QString qW(englishWordList_[currentIndex_].word.c_str());
        found = (input == qW);//用string判断得不到正确结果？？？？？
    }
    else{
        for(auto& mean : englishWordList_[currentIndex_].means){
            if(mean == inputStr){
                found = true;
                break;
            }
        }
    }
    std::string judgeResult = found ? "正确" : "错误";
    ui->textJudeResult->setText(judgeResult.c_str());
    showAllInfo();
}

void MainWindow::downLoadUtils(){
    if(currentIndex_ < 0)
        currentIndex_ = 0;
    std::string url = "http://tts.baidu.com/text2audio?lan=en&ie=UTF-8&spd=2&text=";
    std::string word = englishWordList_[currentIndex_].word;
    url += word;

    m_url = url.c_str();
    //ui->textDebugInfo->setText(m_url);

    if (m_url.isEmpty())
    {
        QMessageBox::information(this, QString::fromLocal8Bit("错误"),
          QString::fromLocal8Bit("请指定需要下载的URL"), NULL);
        return;
    }

    QUrl newUrl = QUrl::fromUserInput(m_url);
    if (!newUrl.isValid())
    {
        QMessageBox::information(this, QString::fromLocal8Bit("错误"),
          QString("无效URL: %1 \n 错误信息: %2").arg(m_url, newUrl.errorString()));
        return;
    }

    showFileSize();

    std::string loaclPath = "E:\\words\\";

    QString tempDir(loaclPath.c_str()); //临时目录
    if (tempDir.isEmpty())
    {
        //QMessageBox::information(this, tr("错误"), "请设置下载目录");
        return;
    }

    QString fullFileName =tempDir + newUrl.fileName();

    if (QFile::exists(fullFileName))
        QFile::remove(fullFileName);

    downloadedFile =new QFile(fullFileName);  //创建临时文件
    if (!downloadedFile->open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("错误"),"临时文件打开错误");
        return;
    }

    //记录当前的时间秒数
    //m_StartTimestamp = QDateTime::currentDateTime().toMSecsSinceEpoch(); //毫秒级
    //m_StartSec = QDateTime::currentDateTime().toTime_t(); //秒级

    //m_timer->start(1000);   //下载速度定时器，以秒为单位

    //ui->btnDownload->setEnabled(false);

    reply = networkManager.get(QNetworkRequest(newUrl));

    //下载完响应finished信号
    connect(reply, SIGNAL(finished()), this, SLOT(on_finished()));

    //缓冲区有新的下载数据时，响应readyRead信号
    connect(reply, SIGNAL(readyRead()), this, SLOT(on_readyRead()));

    //下载进度控制
    //connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
     //       this, SLOT(on_downloadProgress(qint64,qint64)));
}

void MainWindow::checkLoaclFile()
{
    QFileInfo fileInfo;
    std::string finalName = "E:\\words\\" + englishWordList_[currentIndex_].word + ".mp3";
    fileInfo.setFile(finalName.c_str());
    if(fileInfo.exists()){
        playMP3(finalName);
        //QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
    }
    else{
        downLoadUtils();
    }
}

void MainWindow::playMP3(std::string fileName)
{
    QMediaPlayer * player = new QMediaPlayer;

    player->setMedia(QUrl::fromLocalFile(fileName.c_str()));
    player->setVolume(30);
    player->play();
}

void MainWindow::on_pushButtonRead_clicked()
{
    checkLoaclFile();
}

//网络响应结束
void MainWindow::on_finished()
{
    QFileInfo fileInfo;
    std::string finalName = "E:\\words\\" + englishWordList_[currentIndex_].word + ".mp3";
    fileInfo.setFile(finalName.c_str());
    downloadedFile->copy(finalName.c_str());

    downloadedFile->close();
    delete downloadedFile;
    downloadedFile = Q_NULLPTR;

    reply->deleteLater();
    reply = Q_NULLPTR;

    //打开下载的文件
    //if (ui->checkOpen->isChecked())
    //{
    playMP3(finalName);
    //QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
    //}

   // m_timer->stop();

    //ui->btnDownload->setEnabled(true);
}

void MainWindow::on_readyRead()
{
    downloadedFile->write(reply->readAll());
}

/**
 * @brief MainWindow::getFileTotalSize
 * @param url      下载文件的url
 * @param tryTimes 请求的次数，因为可能某种原因会导致请求失败（比如当前网络不是很好，或者文件服务器繁忙），所以做了三次尝试，尽可能保证请求成功。
 * @return 文件大小，单位 byte, 1k= 1024b
 */
qint64 MainWindow::getFileTotalSize(QString url, int tryTimes)
{
    qint64 size = -1;

    if (tryTimes <= 0)
    {
        tryTimes = 1;
    }

    do
    {
        QNetworkAccessManager manager;
        // 事件循环，等待请求文件头信息结束;
        QEventLoop loop;
        // 超时，结束事件循环;
        QTimer timer;

        //发出请求，获取文件地址的头部信息;
        QNetworkReply *reply = manager.head(QNetworkRequest(url));
        if (!reply)
            continue;

        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));

        timer.start(2000);  //超过2s结束事件循环，重新请求
        loop.exec();

        if (reply->error() != QNetworkReply::NoError)
        {
            qDebug() << reply->errorString();
            continue;
        }
        if (reply->error() != QNetworkReply::NoError)
        {
            // 请求发生错误;
            qDebug() << reply->errorString();
            continue;
        }
        else if (!timer.isActive())
        {
            // 请求超时超时,未获取到文件信息;
            qDebug() << "Request Timeout";
            continue;
        }
        timer.stop();

        QVariant var = reply->header(QNetworkRequest::ContentLengthHeader);
        size = var.toLongLong();
        reply->deleteLater();
        break;
    } while (tryTimes--);

    return size;
}

void MainWindow::showFileSize()
{
    qint64 size = getFileTotalSize(m_url, 3);
    //ui->textDebugInfo->clear();
    //ui->textDebugInfo->setText(std::to_string(size).c_str());
}

void MainWindow::on_dataEntry_clicked()
{
    dataEntry_.show();
}

void MainWindow::on_pushButton_clicked()
{
    settings_.show();
}
