#ifndef DESCRIPTION_PATTERN_H
#define DESCRIPTION_PATTERN_H

#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>
#include "constants.h"

class Description_Pattern : public QObject
{
    Q_OBJECT
public:
    explicit Description_Pattern(QObject *parent = 0);
    ~Description_Pattern();

    Description_Pattern(const Description_Pattern &obj);//Конструктор копирования
    Description_Pattern & operator=(const Description_Pattern &obj);//Оператор присваивания

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

    QTextCodec* getCodec() { return textCodec;}
    void setCodec(QTextCodec* cdc) {textCodec = cdc;}
private:
    QTextCodec* textCodec;

    QStringList listFiles;          //Список масок-файлов для обработки шаблона
    QList<int> listStatParams;      //Список параметров обработки шаблона
    QString searchDir;              //Каталог для анализа-поиска
    QStringList listAllowedDirs;    //Разрешённые для анализа-поиска каталоги

signals:

public slots:
};

#endif // DESCRIPTION_PATTERN_H
