#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QKeyEvent>
#include <QTextCursor>
#include <QDebug>
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include "trie.h"

enum letterCase{UpperCase, lowerCase};  //大小写字母
enum mode{Chinese, English};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    mode Mode = Chinese;
    letterCase LetterCase = lowerCase;
    void stringAdd(QString);
    void stringDelete(void);
    void IMRefresh(void);    //读取数据库，更新备选词列表
    void showChinese(void);  //更新备选字词显示
    void changeCharacterChoice(int i); //当用户按下+-page+-时，调整备选字词选项的函数
    void clearButtonContent(void);
    void inputChinese(int i);   //用户按下1~5键选择字词后，将字词输出到文本框中
    void backspace(void);    //专门用于处理backSpace

private:
    Ui::MainWindow *ui;
    QString str;
    QStringList strList;     //这二者用于实现拼音回显，储存目前的拼音，供inputDisaplayLabel回显
    QStringList chooseChineseStrList;
    QStringList choosePYList;
    QList<int> chooseFrequencyList;
    //QList<int>orderNumList;
    QTextCursor cursor;
    QSqlQuery query;
    QSqlDatabase db;
    trie trieTree;
    int checkPage;               //check用于处理字词显示时的分页，代表目前处在第几页
    //10.11
    int searchModeSwitch;       //当此开关被打开（1），进行词典树匹配查表
    //10.28
    int inputingNotFinishedSwitch;  //当此开关被打开(1），说明可能需要自定义词库
    QString lastTimeInputChinese;
    QString lastTimeInputPY;
    QStringList personalizeChineseList;
    QStringList personalizePYList;
    QList<int> personalizeAppearenceList;

protected:
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // MAINWINDOW_H
