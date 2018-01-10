#ifndef SEARCH_PATTERN_H
#define SEARCH_PATTERN_H

#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>
#include "constants.h"

class Search_Pattern : public QObject //Шаблон анализа-поиска
{
    Q_OBJECT
public:
    explicit Search_Pattern(QObject *parent = 0);//Конструктор
    ~Search_Pattern();

    Search_Pattern(const Search_Pattern &obj);//Конструктор копирования
    Search_Pattern & operator=(const Search_Pattern &obj);//Оператор присваивания

    void clear() {clearFiles(); clearStatParams(); clearSearchDir(); clearAllowedDirs();}

    void clearFiles()                       /*Сброс списка масок-файлов*/               {listFiles.clear();}
    void appendFile(QString value)          /*Добавить маску*/                          {if (!listFiles.contains(value)) listFiles.append(value);}
    void appendFiles(QStringList list)      /*Добавить список масок*/                   {foreach (QString item, list) {appendFile(item);}}
    QStringList getFiles()                  /*Получить список масок*/                   {return listFiles;}

    void clearStatParams()                  /*Сброс списка параметров*/                 {listStatParams.clear();}
    void appendStatParam(int value)         /*Добавить параметр*/                       {if (!listStatParams.contains(value)) listStatParams.append(value);}
    void appendStatParams(QList<int> list)  /*Добавить список параметров*/              {foreach (int item, list) {appendStatParam(item);}}
    QList<int> getStatParams()              /*Получить список параметров*/              {return listStatParams;}

    void clearSearchDir()                   /*Сброс каталога для анализа-поиска*/       {searchDir.clear();}
    void setSearchDir(QString dir)          /*Установить каталог для анализа-поиска*/   {searchDir = dir;}
    QString getSearchDir()                  /*Получить каталог для анализа-поиска*/     {return searchDir;}

    void clearAllowedDirs()                 /*Сброс Разрешённые для анализа-поиска каталоги*/       {listAllowedDirs.clear();}
    void setAllowedDirs(QStringList list)   /*Установить Разрешённые для анализа-поиска каталоги*/  {listAllowedDirs = list;}
    QStringList getAllowedDirs()            /*Получить Разрешённые для анализа-поиска каталоги*/    {return listAllowedDirs;}
private:
    QStringList listFiles;          //Список масок-файлов для обработки шаблона
    QList<int> listStatParams;      //Список параметров обработки шаблона
    QString searchDir;              //Каталог для анализа-поиска
    QStringList listAllowedDirs;    //Разрешённые для анализа-поиска каталоги
};

#endif // SEARCH_PATTERN_H
