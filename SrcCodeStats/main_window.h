#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QWebEngineView>
#include <QTreeWidget>
#include <QFileInfo>
#include "search_pattern.h"
#include "stat_machine.h"
#include "app_settings.h"
#include "comment_pattern.h"
#include "comment_machine.h"

namespace Ui {
class Main_Window;
}

class Main_Window : public QMainWindow //Главное окно программы
{
    Q_OBJECT

public:
    explicit Main_Window(QWidget *parent = 0);
    ~Main_Window();

private slots:
    void on_action_calcStat_triggered();        //Старт анализа-поиска
    void on_action_chooseDir_triggered();       //Выбор каталога
    void on_finished_session_machine();         //Окончание работы аналитической машины
    void on_finished_session_comment_machine();         //Окончание работы комментируеющей машины

    void on_dir_changed(QString dir_name);      //Изменение текущего каталога аналитической машины во время анализа-поиска
    void on_file_changed(QString file_name);    //Изменение текущего файла аналитической машины во время анализа-поиска
    void on_count_all_str_changed(int count);   //Изменение кол-ва обработанных строк аналитической машины во время анализа-поиска

    void on_checkBox_statParam_underDir_toggled(bool checked);              //Клик по "Учитывать подкаталоги"
    void createTreeUnderdir(QTreeWidgetItem * item, QFileInfo infodir);   //Рекурсивная функция построения дерева подкаталогов

    void loadHtmlFinished(bool);//результат в html загружен на форму
    void update_result_comment_caption(QStringList list);
    void on_action_commentStart_triggered();

private:
    Ui::Main_Window *ui;        //Содержимое окна - создаётся в редакторе форм
    /*****/
    Stat_Machine* machine;      //Аналитическая машина
    Search_Pattern* pattern;    //Поисковый шаблон (параметры анализа-поиска)

    QWebEngineView* web;//компонент для отображения результата в html
    App_Settings* settings;//параметры приложения

    Comment_Machine* comment_machine;
    Comment_Pattern* comment_pattern;

    /*Соответствие списков - по индексу элемента*/
    QList<QTreeWidgetItem *> listPWidgetItem;   //Список указателей на виджеты подкаталогов
    QList<QString> listAbsolutePath;            //Список абсолютных путей подкаталогов
    /**/

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *);//Событие закрытия приложения

    // QWidget interface
protected:
    void showEvent(QShowEvent *);
};

#endif // MAIN_WINDOW_H
