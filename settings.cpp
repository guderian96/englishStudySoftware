#include "settings.h"
#include "ui_settings.h"
#include <QDir>

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    QString volumeQStr(std::to_string(volume_).c_str());
    ui->textEditVolume->setText(volumeQStr);
    ui->horizontalSliderVolume->setValue(volume_);

    updateNowState();
    initWordLists();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::initWordLists()
{
    QString path = QDir::currentPath() + "\\lexicon";
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "*.txt";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    bool found = false;
    for(auto& file :files){
        if(file == "list.txt"){
            found = true;
        }
        ui->comboBoxWordLists->addItem(file);
    }
    if(found)
        ui->comboBoxWordLists->setCurrentText("list.txt");
}

void Settings::on_applyButton_clicked()
{
    this->destroy();
}

void Settings::on_chineseToEnglishMode_clicked()
{
    judgeMode_ = "chineseToEnglishMode";
    updateNowState();
}

void Settings::on_ListenMode_clicked()
{
    judgeMode_ = "listenMode";
    updateNowState();
}

void Settings::on_englishToChineseMode_clicked()
{
    judgeMode_ = "englishToChineseMode";
    updateNowState();
}

void Settings::clearCheckBoxes()
{
    ui->chineseToEnglishMode->setChecked(false);
    ui->englishToChineseMode->setChecked(false);
    ui->ListenMode->setChecked(false);
    ui->inputMode->setChecked(false);
    ui->randomMode->setChecked(false);
    ui->dictionaryMode->setChecked(false);
}

void Settings::updateNowState()
{
    clearCheckBoxes();

    QString nowStateStr;
    if(judgeMode_ == "englishToChineseMode"){
        ui->englishToChineseMode->setChecked(true);
        nowStateStr = ui->englishToChineseMode->text();
    }
    else if(judgeMode_ == "chineseToEnglishMode"){
        ui->chineseToEnglishMode->setChecked(true);
        nowStateStr = ui->chineseToEnglishMode->text();
    }
    else if(judgeMode_ == "listenMode"){
        ui->ListenMode->setChecked(true);
        nowStateStr = ui->ListenMode->text();
    }
    ui->NowMode->setText(nowStateStr);

    QString nowListStateStr;
    if(listMode_ == "inputMode"){
        ui->inputMode->setChecked(true);
        nowListStateStr = ui->inputMode->text();
    }
    else if(listMode_ == "randomMode"){
        ui->randomMode->setChecked(true);
        nowListStateStr = ui->randomMode->text();
    }
    else if(listMode_ == "dictionaryMode"){
        ui->dictionaryMode->setChecked(true);
        nowListStateStr = ui->dictionaryMode->text();
    }
    ui->listMode->setText(nowListStateStr);
}

void Settings::on_inputMode_clicked()
{
    listMode_ = "inputMode";
    updateNowState();
}

void Settings::on_randomMode_clicked()
{
    listMode_ = "randomMode";
    updateNowState();
}

void Settings::on_dictionaryMode_clicked()
{
    listMode_ = "dictionaryMode";
    updateNowState();
}

void Settings::on_textRandomBaseNumber_textChanged()
{
    std::string randomBaseNumber = ui->textRandomBaseNumber->toPlainText().toStdString();
    step_ = std::stoi(randomBaseNumber);
}

void Settings::on_comboBoxWordLists_currentTextChanged(const QString &arg1)
{
    currentList_ = arg1;
}

void Settings::on_horizontalSliderVolume_valueChanged(int value)
{
    volume_ = value;
    QString volumeQStr(std::to_string(volume_).c_str());
    ui->textEditVolume->setText(volumeQStr);
}
