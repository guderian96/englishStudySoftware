#ifndef DATAENTRY_H
#define DATAENTRY_H

#include <QWidget>

namespace Ui {
class DataEntry;
}

class DataEntry : public QWidget
{
    Q_OBJECT

public:
    explicit DataEntry(QWidget *parent = 0);
    ~DataEntry();

    void setCurrentTextPath(QString path){ path_ = path; }
private slots:
    void on_End_clicked();

    void on_Entry_clicked();

private:
    Ui::DataEntry *ui;
    void writeInText();
    QString path_;
};

#endif // DATAENTRY_H
