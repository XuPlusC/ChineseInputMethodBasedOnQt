#include "mainwindow.h"
#include "ui_mainwindow.h"

//本文件包含所有核心功能的具体实现，包括主窗口的构造函数、按键事件过滤器、查表更新等功能
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusLabel->setAlignment(Qt::AlignHCenter);
    ui->capsLock->setAlignment(Qt::AlignCenter);
    ui->textEdit->installEventFilter(this);
    ui->wordLabel_1->adjustSize();
    ui->wordLabel_2->adjustSize();
    ui->wordLabel_3->adjustSize();
    ui->wordLabel_4->adjustSize();
    ui->wordLabel_5->adjustSize();

    searchModeSwitch = 0;
    inputingNotFinishedSwitch = 0;
    //数据库初始化部分
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:\\workspace\\Qt\\fk\\my1.db");//db.setDatabaseName("DatabaseTrial.db");
    bool ok = db.open();
    if(ok)
    {
        query=QSqlQuery(db);
        qDebug()<<"hi, database";
    }
    else
        qDebug()<<"openDb failed";

    //词典树初始化部分
    if(trieTree.initialTrie())
        qDebug()<<"Trie initial ok!";
    else
        qDebug()<<"Trie Failed!";
}

MainWindow::~MainWindow()
{
    delete ui;
}

//主事件过滤器。对用户的按键做出反应
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->textEdit)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *newKeyEvent = static_cast<QKeyEvent *>(event);
            switch(newKeyEvent->key())
            {
            case Qt::Key_Q: if(Mode == Chinese){stringAdd("q");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_W: if(Mode == Chinese){stringAdd("w");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_E: if(Mode == Chinese){stringAdd("e");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_R: if(Mode == Chinese){stringAdd("r");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_T: if(Mode == Chinese){stringAdd("t");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_Y: if(Mode == Chinese){stringAdd("y");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_U: if(Mode == Chinese){stringAdd("u");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_I: if(Mode == Chinese){stringAdd("i");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_O: if(Mode == Chinese){stringAdd("o");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_P: if(Mode == Chinese){stringAdd("p");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_A: if(Mode == Chinese){stringAdd("a");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_S: if(Mode == Chinese){stringAdd("s");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_D: if(Mode == Chinese){stringAdd("d");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_F: if(Mode == Chinese){stringAdd("f");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_G: if(Mode == Chinese){stringAdd("g");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_H: if(Mode == Chinese){stringAdd("h");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_J: if(Mode == Chinese){stringAdd("j");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_K: if(Mode == Chinese){stringAdd("k");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_L: if(Mode == Chinese){stringAdd("l");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_Z: if(Mode == Chinese){stringAdd("z");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_X: if(Mode == Chinese){stringAdd("x");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_C: if(Mode == Chinese){stringAdd("c");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_V: if(Mode == Chinese){stringAdd("v");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_B: if(Mode == Chinese){stringAdd("b");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_N: if(Mode == Chinese){stringAdd("n");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_M: if(Mode == Chinese){stringAdd("m");      searchModeSwitch = 0;      IMRefresh(); return true;} else return false;
            case Qt::Key_Shift:
                if(Mode == Chinese)
                {
                    Mode = English;
                    ui->inputDisplayLabel->setText("");
                    //!!!!!!!!!!!!
                    str.clear();
                    strList.clear();
                    clearButtonContent();
                    ui->statusLabel->setText("EN");
                }
                else
                {
                    Mode = Chinese;
                    ui->statusLabel->setText("CN");
                }
                return true;
            case Qt::Key_Backspace:     //糟糕的代码。第一部分可以returnfalse
                if(ui->inputDisplayLabel->text().isEmpty() == true )
                {
                    cursor = ui->textEdit->textCursor();
                    if(cursor.hasSelection())
                       cursor.clearSelection();//取消当前被选中文本
                    cursor.deletePreviousChar();//删除光标前的单个字符
                }
                else
                {     
                    backspace();        //这部分操作较为繁琐且涉及到对private部分的操作，故单独列为函数
                    stringDelete();     //移除待查拼音的末尾字符
                    IMRefresh();
                    showChinese();
                }              

                if(ui->inputDisplayLabel->text().isEmpty() == true)
                    clearButtonContent();                              
                return true;
            case Qt::Key_CapsLock:
                if(Mode == English)
                {
                    if(LetterCase == lowerCase)
                    {
                        LetterCase = UpperCase;
                        ui->capsLock->setText("Caps On");
                        return false;
                    }
                    else
                    {
                        LetterCase = lowerCase;
                        ui->capsLock->setText("Caps Off");
                        return false;
                    }
                    return true;
                }
                else if(Mode == Chinese)
                {
                    Mode = English;
                    ui->capsLock->setText("Caps On");
                    ui->inputDisplayLabel->setText("");
                    //!!!!!!!!!!!!
                    str.clear();
                    strList.clear();
                    clearButtonContent();
                    ui->statusLabel->setText("EN");
                    return false;
                }
            case Qt::Key_PageUp:
            {
                if(Mode==Chinese)
                {
                    changeCharacterChoice(0);
                    return true;
                }
                else
                    return false;
            }
            case Qt::Key_PageDown:
            {
                if(Mode==Chinese)
                {
                    changeCharacterChoice(1);
                    return true;
                }
                else
                    return false;
            }
            case Qt::Key_1:
            {
                //this part can be set as a function!
                if(ui->inputDisplayLabel->text().isEmpty() == false && !ui->wordLabel_1->text().isEmpty())
                {
                    inputChinese(5);
                    return true;
                }
                else if(ui->wordLabel_1->text().isEmpty() && !ui->inputDisplayLabel->text().isEmpty())
                    return true;
                else
                    return false;
            }
            case Qt::Key_2:
            {
                if(ui->inputDisplayLabel->text().isEmpty() == false && !ui->wordLabel_2->text().isEmpty())
                {                    
                    inputChinese(4);
                    return true;
                }
                else if(ui->wordLabel_2->text().isEmpty() && !ui->inputDisplayLabel->text().isEmpty())
                    return true;
                else
                    return false;
            }
            case Qt::Key_3:
            {
                if(ui->inputDisplayLabel->text().isEmpty() == false && !ui->wordLabel_3->text().isEmpty())
                {
                    inputChinese(3);
                    return true;
                }
                else if(ui->wordLabel_3->text().isEmpty() && !ui->inputDisplayLabel->text().isEmpty())
                    return true;
                else
                    return false;
            }
            case Qt::Key_4:
            {
                if(ui->inputDisplayLabel->text().isEmpty() == false && !ui->wordLabel_4->text().isEmpty())
                {
                    inputChinese(2);
                    return true;
                }
                else if(ui->wordLabel_4->text().isEmpty() && !ui->inputDisplayLabel->text().isEmpty())
                    return true;
                else
                    return false;
            }
            case Qt::Key_5:
            {
                if(ui->inputDisplayLabel->text().isEmpty() == false && !ui->wordLabel_5->text().isEmpty())
                {
                    inputChinese(1);
                    return true;
                }
                else if(ui->wordLabel_5->text().isEmpty() && !ui->inputDisplayLabel->text().isEmpty())
                    return true;
                else
                    return false;
            }
            case Qt::Key_Space:
            {
                if(ui->inputDisplayLabel->text().isEmpty() == false && !ui->wordLabel_1->text().isEmpty())
                {
                    inputChinese(5);
                    return true;
                }
                else if(ui->wordLabel_1->text().isEmpty() && !ui->inputDisplayLabel->text().isEmpty())
                    return true;
                else
                    return false;
            }
            default:
                return false;
            }
        }
        else
            return false;
    }
    else
        return false;
}

//IMRefresh根据目前的待搜索全拼更新备选字词列表。
//任何修改了待搜索全拼内容的行为发生之后（如按下字母键，backspace..)都必须调用此函数
void MainWindow::IMRefresh(void)
{
    QString temp1, temp2;       //temp1用来储存每一次从数据库中读入的字词
                                //temp2用来存储对应的拼音
    QString searchContent = ui->inputDisplayLabel->text();      //代表用于查找的拼音串
    //MainWindow::searchContent = ui->inputDisplayLabel->text();
    QString search;         //search是用于sqlite执行的sql语句
    checkPage = 1;          //调用本函数代表输入了新的拼音，需要将分页归于初始值
    int i = 0;              //i是循环计数器。
    int found_mark = 0;     //debug用的标记，代表查全拼是否查到了内容
    int tempFrequency;

    //如果待搜索全拼为空，直接返回
    if(ui->inputDisplayLabel->text().isEmpty())
    {
        clearButtonContent();
        chooseChineseStrList.clear();
        choosePYList.clear();
        return;
    }
    //search = "SELECT * FROM PYList WHERE pinyin = '" + searchContent +"' ORDER BY freq DESC;";
    //根据首字母决定执行的sql语句。建立数据库时按拼音首字母做成了26个table，故需要手动指向对应的table
    switch (searchContent.at(0).unicode()) {
    case 'q':
        qDebug()<<"q!";
        search = "SELECT * FROM PYlistq WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'w':
        search = "SELECT * FROM PYlistw WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'e':
        search = "SELECT * FROM PYliste WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'r':
        search = "SELECT * FROM PYlistr WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 't':
        search = "SELECT * FROM PYlistt WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'y':
        search = "SELECT * FROM PYlisty WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'u':
        search = "SELECT * FROM PYlistu WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'i':
        search = "SELECT * FROM PYlisti WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'o':
        search = "SELECT * FROM PYlisto WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'p':
        search = "SELECT * FROM PYlistp WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'a':
        search = "SELECT * FROM PYlista WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 's':
        search = "SELECT * FROM PYlists WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'd':
        search = "SELECT * FROM PYlistd WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'f':
        search = "SELECT * FROM PYlistf WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'g':
        search = "SELECT * FROM PYlistg WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'h':
        search = "SELECT * FROM PYlisth WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'j':
        search = "SELECT * FROM PYlistj WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'k':
        search = "SELECT * FROM PYlistk WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'l':
        search = "SELECT * FROM PYlistl WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'z':
        search = "SELECT * FROM PYlistz WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'x':
        search = "SELECT * FROM PYlistx WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'c':
        search = "SELECT * FROM PYlistc WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'v':
        search = "SELECT * FROM PYlistv WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'b':
        search = "SELECT * FROM PYlistb WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'n':
        search = "SELECT * FROM PYlistn WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    case 'm':
        search = "SELECT * FROM PYlistm WHERE py = '" + searchContent +"' ORDER BY level DESC;";
        break;
    default:
        break;
    }
    query.exec(search);
    //先查找全拼对应字词
    while(query.next())
    {
        found_mark = 1;
        temp1 = query.value(0).toString();
        temp2 = query.value(1).toString();
        tempFrequency = query.value(2).toInt();
        if(chooseChineseStrList.contains(temp1) == false)       //如果没有出现重复字词，直接往备选字词表和备选拼音表中加入数据
        {           
            chooseChineseStrList.insert(i, temp1);
            choosePYList.insert(i, temp2);
            chooseFrequencyList.insert(i, tempFrequency);
            i++;
        }
        else     //如果出现了重复字词，也就是说需要更新这个词的拼音并同时修改这个词在备选字词表和备选拼音表的位置
        {
            int remove = chooseChineseStrList.indexOf(temp1,0);//查找所需要删除的旧位置
            chooseChineseStrList.removeAt(remove);
            choosePYList.removeAt(remove);
            chooseFrequencyList.removeAt(remove);
            chooseChineseStrList.insert(i, temp1);
            choosePYList.insert(i, temp2);
            chooseFrequencyList.insert(i, tempFrequency);
            i++;
        }
    }
    if(!found_mark)
        qDebug()<<"nothing found!into secondary mode"<<"listRemians:"<<chooseChineseStrList.length()<<"\nswitch status:"<<searchModeSwitch;
    /*********10.27**********/
    if(searchModeSwitch  && ui->inputDisplayLabel->text().length()>1)
    {
        QString singleWord = ui->inputDisplayLabel->text();
        while(singleWord.length() > 0)
        {
            if(trieTree.search(trieTree.root, singleWord.toStdString()))
            {
                qDebug()<<"singleWord::"<<singleWord;
                switch (searchContent.at(0).unicode())
                {
                case 'q':
                    search = "SELECT * FROM PYlistq WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'w':
                    search = "SELECT * FROM PYlistw WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'e':
                    search = "SELECT * FROM PYliste WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'r':
                    search = "SELECT * FROM PYlistr WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 't':
                    search = "SELECT * FROM PYlistt WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'y':
                    search = "SELECT * FROM PYlisty WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'u':
                    search = "SELECT * FROM PYlistu WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'i':
                    search = "SELECT * FROM PYlisti WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'o':
                    search = "SELECT * FROM PYlisto WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'p':
                    search = "SELECT * FROM PYlistp WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'a':
                    search = "SELECT * FROM PYlista WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 's':
                    search = "SELECT * FROM PYlists WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'd':
                    search = "SELECT * FROM PYlistd WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'f':
                    search = "SELECT * FROM PYlistf WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'g':
                    search = "SELECT * FROM PYlistg WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'h':
                    search = "SELECT * FROM PYlisth WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'j':
                    search = "SELECT * FROM PYlistj WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'k':
                    search = "SELECT * FROM PYlistk WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'l':
                    search = "SELECT * FROM PYlistl WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'z':
                    search = "SELECT * FROM PYlistz WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'x':
                    search = "SELECT * FROM PYlistx WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'c':
                    search = "SELECT * FROM PYlistc WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'v':
                    search = "SELECT * FROM PYlistv WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'b':
                    search = "SELECT * FROM PYlistb WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'n':
                    search = "SELECT * FROM PYlistn WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                case 'm':
                    search = "SELECT * FROM PYlistm WHERE py = '" + singleWord +"' ORDER BY level DESC;";
                    break;
                default:
                    break;
                }
                query.exec(search);
                while(query.next())
                {
                    found_mark = 1;
                    temp1 = query.value(0).toString();
                    if(chooseChineseStrList.contains(temp1) == false)       //如果没有出现重复字词，直接往备选字词表和备选拼音表中加入数据
                    {
                        temp2 = query.value(1).toString();
                        chooseChineseStrList.insert(i, temp1);
                        choosePYList.insert(i, temp2);
                        chooseFrequencyList.insert(i, tempFrequency);
                        i++;
                    }
                    else     //如果出现了重复字词，也就是说需要更新这个词的拼音并同时修改这个词在备选字词表和备选拼音表的位置
                    {
                        int remove = chooseChineseStrList.indexOf(temp1,0);
                        //qDebug()<<"Now remove is"<<remove;
                        chooseChineseStrList.removeAt(remove);
                        choosePYList.removeAt(remove);
                        chooseFrequencyList.removeAt(remove);
                        temp2 = query.value(1).toString();
                        chooseChineseStrList.insert(i, temp1);
                        choosePYList.insert(i, temp2);
                        chooseFrequencyList.insert(i, tempFrequency);
                        i++;
                    }
                }
            }
            singleWord.remove(singleWord.length() - 1, 1);
        }
    }
    showChinese();
    qDebug()<<chooseFrequencyList;
}

void MainWindow::showChinese(void)
{
    clearButtonContent();
    if(checkPage*5 >= chooseChineseStrList.length())
    {
        //使用switch语句，更新特定数量的被选词列表
        switch (checkPage*5 - chooseChineseStrList.length())
        {
        case 0:
            ui->wordLabel_5->setText(chooseChineseStrList.at((checkPage-1)*5 + 4));
        case 1:
            ui->wordLabel_4->setText(chooseChineseStrList.at((checkPage-1)*5 + 3));
        case 2:
            ui->wordLabel_3->setText(chooseChineseStrList.at((checkPage-1)*5 + 2));
        case 3:
            ui->wordLabel_2->setText(chooseChineseStrList.at((checkPage-1)*5 + 1));
        case 4:
            ui->wordLabel_1->setText(chooseChineseStrList.at((checkPage-1)*5 + 0));
        default:
            break;
        }
    }
    else
    {
        ui->wordLabel_1->setText(chooseChineseStrList.at((checkPage-1)*5 + 0));
        ui->wordLabel_2->setText(chooseChineseStrList.at((checkPage-1)*5 + 1));
        ui->wordLabel_3->setText(chooseChineseStrList.at((checkPage-1)*5 + 2));
        ui->wordLabel_4->setText(chooseChineseStrList.at((checkPage-1)*5 + 3));
        ui->wordLabel_5->setText(chooseChineseStrList.at((checkPage-1)*5 + 4));
    }
    ui->wordLabel_1->adjustSize();
    ui->wordLabel_2->adjustSize();
    ui->wordLabel_3->adjustSize();
    ui->wordLabel_4->adjustSize();
    ui->wordLabel_5->adjustSize();
}

//inputChinese函数：用户按下1~5键选择字词后，将字词输出到文本框中
void MainWindow::inputChinese(int i)
{
    QString tempPY = ui->inputDisplayLabel->text();
    QString tempChinese;
    searchModeSwitch = 1;    
    int j = 1;
    cursor = ui->textEdit->textCursor();
    switch (i) {
    case 1:      
        cursor.insertText(ui->wordLabel_5->text());
        tempChinese = ui->wordLabel_5->text();
        break;
    case 2:
        cursor.insertText(ui->wordLabel_4->text());
        tempChinese = ui->wordLabel_4->text();
        break;
    case 3:
        cursor.insertText(ui->wordLabel_3->text());
        tempChinese = ui->wordLabel_3->text();
        break;
    case 4:
        cursor.insertText(ui->wordLabel_2->text());
        tempChinese = ui->wordLabel_2->text();
        break;
    case 5:
        cursor.insertText(ui->wordLabel_1->text());
        tempChinese = ui->wordLabel_1->text();
        break;
    default:
        break;
    }

    while(j++ <= choosePYList.at(checkPage*5 - i).length())//从待查全拼strList删去已选择的拼音
        strList.removeFirst();
    str = strList.join("");

    /***********10.28新增部分，修改词频计数*********************/
    int tempFrequency = chooseFrequencyList.at(checkPage*5 - i) + 1;
    QString search_level = QString::number(tempFrequency, 10);
    QString search_chinese = chooseChineseStrList.at(checkPage*5 - i);
    QString search_pinyin = choosePYList.at(checkPage*5 - i);
    QString search;

    switch (search_pinyin.at(0).unicode())
    {
    case 'q':
        search = "UPDATE PYlistq SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'w':
        search = "UPDATE PYlistw SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'e':
        search = "UPDATE PYliste SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'r':
        search = "UPDATE PYlistr SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 't':
        search = "UPDATE PYlistt SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'y':
        search = "UPDATE PYlisty SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'u':
        search = "UPDATE PYlistu SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'i':
        search = "UPDATE PYlisti SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'o':
        search = "UPDATE PYlisto SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'p':
        search = "UPDATE PYlistp SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'a':
        search = "UPDATE PYlista SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 's':
        search = "UPDATE PYlists SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'd':
        search = "UPDATE PYlistd SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'f':
        search = "UPDATE PYlistf SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'g':
        search = "UPDATE PYlistg SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'h':
        search = "UPDATE PYlisth SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'j':
        search = "UPDATE PYlistj SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'k':
        search = "UPDATE PYlistk SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'l':
        search = "UPDATE PYlistl SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'z':
        search = "UPDATE PYlistz SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'x':
        search = "UPDATE PYlistx SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'c':
        search = "UPDATE PYlistc SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'v':
        search = "UPDATE PYlistv SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'b':
        search = "UPDATE PYlistb SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'n':
        search = "UPDATE PYlistn SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    case 'm':
        search = "UPDATE PYlistm SET level = " + search_level + " where hanzi = '"+ (QString)search_chinese + "' and py = '" + (QString)search_pinyin + "';";
        break;
    default:
        break;
    }
    qDebug()<<search;
    /************************************************/


    /*******************10.12************************/
    //以下部分在用户按下1~5键选择了汉字后唤起，用于从备选拼音和备选汉字列表中删去应该剔除的元素
    QString deletePy = choosePYList.at(checkPage*5 - i);
    qDebug()<<"removing : "<<deletePy;      //这是用户选择的汉字的全拼，实际删除中还需要删除该全拼的各个子音节所对应的汉字
    QString deletePy_firstTwoCharacter;     //由于本字库除了元音字（如啊）之外至少要两个字母才能出字，故删去拼音时还要考虑其首二字母

    deletePy_firstTwoCharacter.append(deletePy.at(0));
    if(deletePy.length() >= 2)
        deletePy_firstTwoCharacter.append(deletePy.at(1));
    if(deletePy_firstTwoCharacter.at(0) == 'm' || deletePy_firstTwoCharacter.at(0) == 'n')
        deletePy_firstTwoCharacter.remove(1,1); //解决m和n的问题。由于字库中唯二的非元音却可以出字的字母是mn，故需要单独处理

    qDebug()<<"first two char is "<<deletePy_firstTwoCharacter;

    int remove_last= (choosePYList.lastIndexOf(deletePy) >= choosePYList.lastIndexOf(deletePy_firstTwoCharacter))? choosePYList.lastIndexOf(deletePy):choosePYList.lastIndexOf(deletePy_firstTwoCharacter);
    qDebug()<<"remove limit: "<<remove_last <<"as total is "<<choosePYList.length();

    //以上部分，程序寻找用户输入汉字的全拼或首二字母的最后索引，取二者最大值m作为删除的上限，即删除备选拼音、汉字列表的第1~m个。
    for(;remove_last >= 0; remove_last--)
    {
        //qDebug()<<"removed: "<<chooseChineseStrList.at(0);
        choosePYList.removeFirst();
        chooseChineseStrList.removeFirst();
        chooseFrequencyList.removeFirst();
        //qDebug()<<"Py remian length:"<<choosePYList.length()<<"Chinese remain length:"<<chooseChineseStrList.length()<<" and mark is "<<remove_last;
    }
    //执行删除操作
    //qDebug()<<"now remains:"<<chooseChineseStrList;
    checkPage = 1;
    showChinese();
    ui->inputDisplayLabel->setText(str);
    /**********************11.3**********************
    if(!str.isEmpty())          //这一次拼音输入了词库中没有的词，可能需要添加新词，记录下拼音和汉字
    {
        inputingNotFinishedSwitch = 1;
        lastTimeInputPY = tempPY;       //记录新词的全拼。当第一次判定为可能需要加新词时，此时tempPY保存的拼音即新词的拼音。
    }
    else
    {
        if(inputingNotFinishedSwitch = 1)//备选拼音列表不再有词且开关被打开过，即此时已完成新词的汉语录入。将新词存入新词链表
        {
            int index = personalizeChineseList.indexOf(lastTimeInputChinese);
            if(index == -1)
            {
                personalizeChineseList.append(lastTimeInputChinese);
                personalizePYList.append(lastTimeInputPY);
                personalizeAppearenceList.append(1);
            }
            else
            {
                qDebug()<<"previous appr is"<<personalizeAppearenceList.at(index);
                personalizeAppearenceList.at(index)++;
                qDebug()<<"now appr is"<<personalizeAppearenceList.at(index);
            }
            inputingNotFinishedSwitch = 0;
        }
        else    //一次到位，输入的词是词库有的词
        {

        }
    }
    if(inputingNotFinishedSwitch)
    {
        lastTimeInputChinese.append(tempChinese);
    }
    /*************************************************/
    IMRefresh();
}
