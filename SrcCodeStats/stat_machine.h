#ifndef STAT_MACHINE_H
#define STAT_MACHINE_H

#include <QThread>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include "search_pattern.h"
#include <QDir>
#include <QFileInfoList>
#include "appid.h"
#include <math.h>
#include "constants.h"

struct StatType //Статистическая информация
{
    int useful_str = 0;         //Полезные строки (не пустые)
    int code_str = 0;           //Кол-во строк кода
    int code_comment_str = 0;   //из них - комментированный код
    int comment_str = 0;        //Кол-во строк с комментариями
    /**/
    double expectation = 0;     //Мат.ожидание
    double variance = 0;        //Дисперсия
    double deviation = 0;       //Среднеквадр.отклонение
    int max_len_str = INT_MIN;  //Макс. кол-во символов
    int min_len_str = INT_MAX;  //Мин. кол-во символов
};

class Stat_Machine : public QThread //Аналитическая машина
{
    Q_OBJECT
public:
    explicit Stat_Machine(QObject *parent = 0);
    ~Stat_Machine();

    void setPattern(Search_Pattern value)   /*Установить шаблон для анализа-поиска*/            {pattern = value;}
    QString getResultHtml()                 /*Получить результат анализа-поиска в html-виде*/   {return result_html;}

private:
    QString oneFileProcessing(QFileInfo file);              //Обработка одного файла
    QString oneDirProcessing(QFileInfo dir);                //Обработка одного каталога
    QString totalStatTable();                               //Генерация таблицы с общей статистикой
    QString fileStatTable(QString caption, StatType stat);   //Генерация одной таблицы с заданным содержимым
    StatType analizeOne_c_file(QStringList lines);             //Анализ содержимого одного файла
    int checkStateAnalizeAutomaton_c_file(QString line, bool isInComm);

    /*Статистические вычисления*/
    double expectation(QList<double>* list);    //математическое ожидание
    double variance(QList<double>* list);       //дисперсия
    double deviation(QList<double>* list);      //среднеквадратичное отклонение
    QList<double>* list_expectations;           //список мат.ожиданий длин строк по файлам - для вычисл. среднего
    /**/

    Search_Pattern pattern;     //Шаблон для анализа поиска
    QString result_html;        //Результат анализа-поиска в html-виде

    QString fontName;           //Название шрифта результата
    QString fontSize;           //Размер шрифта результата

    void clearTotalParams();    //Сброс общей статистики анализа-поиска
    StatType total;             //Общая статистика анализа-поиска

signals:
    void sgnDirChanged(QString dirName);        //Изменение текущего обрабатываемого каталога
    void sgnFileChanged(QString fileName);      //Изменение текущего обрабатываемого файла
    void sgnCountAllStrChanged(int count);      //Изменение кол-ва обработанных строк

    // QThread interface
protected:
    void run();     //Основной алгоритм аналитической машины
};

#endif // STAT_MACHINE_H
