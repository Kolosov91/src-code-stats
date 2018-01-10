#ifndef DESCRIPTION_MACHINE_H
#define DESCRIPTION_MACHINE_H

#include <QObject>
#include <QThread>
#include <QTextStream>
#include <QList>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QFileInfoList>
#include "appid.h"
#include <math.h>
#include "constants.h"
#include "description_pattern.h"

/*ТАБЛИЦА ТОКЕНОВ*/
#define TC_TEXT 0
#define TC_ALG  1
#define TC_STEP 2
#define TC_ID   3
/*****/

struct Token
{
    int code;
    QString text;
//    QList<Token> tokens;
};

class Description_Machine  : public QThread //Машина комментирования
{
    Q_OBJECT
public:
    explicit Description_Machine(QObject *parent = 0);
    ~Description_Machine();

    void setPattern(Description_Pattern val) {pattern = val;}
    QString getResult() {return result;}

private:
    QString oneFileProcessing(QFileInfo file);              //Обработка одного файла
    QString oneDirProcessing(QFileInfo dir);                //Обработка одного каталога
    QString oneTokenProcessing(Token token);
    QList<Token> getTokensFromLine(QString line);
    QList<int> getTokensIndexes(QString line);

    QString result;
    Description_Pattern pattern;

signals:

public slots:

    // QThread interface
protected:
    void run();
};

#endif // DESCRIPTION_MACHINE_H
