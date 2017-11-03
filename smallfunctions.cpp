#include <mainwindow.h>
#include "ui_mainwindow.h"

void MainWindow::stringAdd(QString c)
{
    strList.append(c);
    str = strList.join("");
    ui->inputDisplayLabel->setText(str);
}

void MainWindow::stringDelete()
{
    strList.removeLast();
    str = strList.join("");
    ui->inputDisplayLabel->setText(str);
}

//函数changeCharacterChoice用于处理pageup和pagedown按钮
void MainWindow::changeCharacterChoice(int i)
{
    if(i == 1 && checkPage*5 <= chooseChineseStrList.length())  //i=1对应+和pageup
    {
        checkPage++;
        showChinese();
    }
    else if(i == 0 && checkPage != 1)                           //-和pagedown
    {
        checkPage--;
        showChinese();
    }
    else
        return;
}

void MainWindow::clearButtonContent(void)
{
    MainWindow::ui->wordLabel_1->clear();
    MainWindow::ui->wordLabel_2->clear();
    MainWindow::ui->wordLabel_3->clear();
    MainWindow::ui->wordLabel_4->clear();
    MainWindow::ui->wordLabel_5->clear();
}

//按下backspace后，删去应删去的内容
void MainWindow::backspace(void)
{
    searchModeSwitch = 0;
    QString deletePy = ui->inputDisplayLabel->text();
    int delete_mark_first = choosePYList.indexOf(deletePy);
    int delete_mark_last = choosePYList.lastIndexOf(deletePy);
    //qDebug()<<"deleting :"<<deletePy<<" first found in"<<delete_mark_first<<" last in"<<delete_mark_last;

    for(;delete_mark_last >= delete_mark_first; --delete_mark_last)
    {
        chooseChineseStrList.removeAt(delete_mark_first);
        choosePYList.removeAt(delete_mark_first);
        chooseFrequencyList.removeAt(delete_mark_first);
    }
}
