#ifndef COMMENT_MACHINE_H
#define COMMENT_MACHINE_H

#include <QObject>
#include <QThread>
#include "comment_pattern.h"
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QFileInfoList>
#include "constants.h"
#include <QTextCodec>

class Comment_Machine : public QThread //Машина комментирования
{
    Q_OBJECT
public:
    explicit Comment_Machine(QObject *parent = 0);
    ~Comment_Machine();

    void setPattern(Comment_Pattern val) {pattern = val;}
    QStringList getListProcedFiles() { return list_proced_files;}

private:
    Comment_Pattern pattern;
    QStringList list_proced_files;

    QString oneFileProcessing(QFileInfo file);              //Обработка одного файла
    QString oneDirProcessing(QFileInfo dir);                //Обработка одного каталога

signals:

public slots:

    // QThread interface
protected:
    void run();
};

#endif // COMMENT_MACHINE_H
