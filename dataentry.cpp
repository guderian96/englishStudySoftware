﻿#include "dataentry.h"
#include "ui_dataentry.h"
#include <QDir>

DataEntry::DataEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataEntry)
{
    ui->setupUi(this);
}

DataEntry::~DataEntry()
{
    delete ui;
}

void DataEntry::on_End_clicked()
{
    this->destroy();
}

void DataEntry::on_Entry_clicked()
{
    writeInText();
}

void DataEntry::writeInText()
{
    std::string fileName = path_.toStdString().c_str();
    FILE* fp;
    fp = fopen(fileName.c_str(), "a+");

    std::string english = ui->textEnglish->toPlainText().toStdString();
    std::string attr = ui->textAttribute->toPlainText().toStdString();
    std::string chinese = ui->textChinese->toPlainText().toStdString();
    if(!english.empty() && !attr.empty() && !chinese.empty()){
        std::string line = "\n" + english + "\t" + attr + "\t" + chinese + "\t|";
        fprintf(fp ,line.c_str());
    }
    fclose(fp);
}
