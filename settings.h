#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

    std::string getJudgeMode() { return judgeMode_; }
    std::string getListMode() { return listMode_; }
    int getStep() { return step_; }
private slots:
    void on_applyButton_clicked();

    void on_chineseToEnglishMode_clicked();

    void on_ListenMode_clicked();

    void on_englishToChineseMode_clicked();

    void on_inputMode_clicked();

    void on_randomMode_clicked();

    void on_dictionaryMode_clicked();

    void on_textRandomBaseNumber_textChanged();

private:
    Ui::Settings *ui;
    std::string judgeMode_ = "englishToChineseMode";
    std::string listMode_ = "inputMode";
    int step_ = 17;

    void clearCheckBoxes();
    void updateNowState();
};

#endif // SETTINGS_H
