#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QKeyEvent>
#include <QFile>
#include <QUrl>
#include <QTimer>
#include <QLabel>
#include "global.h"
#include "dataentry.h"
#include "settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ui::MainWindow *ui;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    std::vector<englishWord> englishWordList_;
    global g_;
    DataEntry dataEntry_;
    Settings settings_;
    int currentIndex_ = -1;

    qint64 getFileTotalSize(QString url, int tryTimes);  //获取文件大小
    void showFileSize();
    void downLoadUtils();
    void checkLoaclFile();
    std::vector<std::string> split(const std::string& src, const std::string& separator);
private slots:
    void on_readyRead();
    void on_finished();

    void on_pushButtonInit_clicked();

    void on_pushButtonNext_clicked();

    void on_pushButtonJudge_clicked();

    void on_pushButtonRead_clicked();

    void on_dataEntry_clicked();

    void on_pushButton_clicked();
protected:
    bool eventFilter(QObject *obj, QEvent *ev);
private:
    void playMP3(std::string fileName);
    void initList();
    void showChineseMeans();
    void clearEditTexts();
    void showAllInfo();

private:
    QNetworkAccessManager networkManager;  //网络管理
    QNetworkReply *reply;   //网络响应
    QFile *downloadedFile;  //保存下载的临时文件

    QString m_url;
    QTimer  *m_timer;   //定时计算下载速度
    qint64 m_StartTimestamp;
    qint64 m_StartSec;

    qint64 m_hadLoadSize;   //已下载的文件大小
    qint64 m_tmpAllsize;
};

#endif // MAINWINDOW_H
