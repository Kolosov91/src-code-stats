#include "comment_machine.h"

Comment_Machine::Comment_Machine(QObject *parent)  : QThread(parent)
{

}

Comment_Machine::~Comment_Machine()
{

}

QString Comment_Machine::oneFileProcessing(QFileInfo file) //Обработка одного файла
{
    QString fileName = file.absoluteFilePath();//полное имя обрабатываемого файла
    QFile txtFile(fileName);//обрабатываемый файл - текстовый файл
    QString file_value;
    if (!txtFile.open(QFile::ReadOnly)) return "";//пытаемся его открыть
    {
        QTextStream stream(&txtFile);//поток для чтения текстового содержимого
        stream.setCodec(pattern.getCodec());
        file_value = stream.readAll();
    }
    txtFile.flush();
    txtFile.close();//закрываем файл
    if (!txtFile.open(QFile::WriteOnly )) return "";//пытаемся его открыть
    {
        QTextStream stream(&txtFile);//поток для чтения текстового содержимого
        stream.setCodec(pattern.getCodec());
        pattern.setNameFile(QFileInfo(fileName).fileName());
        stream << pattern.getResult();
        stream << file_value;
    }
    txtFile.flush();
    txtFile.close();//закрываем файл
    /**/
    return fileName + QString("\n");
}

QString Comment_Machine::oneDirProcessing(QFileInfo dir) //Обработка одного каталога - рекурсивная функция
{
    QString list_files_in_dir; //результат обработки текущего каталога в html-формате
    QString absPath = dir.absoluteFilePath();//абсолютный путь к текущему каталогу
    QDir curDir(absPath);//текщий обрабатываемы каталог
    if (pattern.getAllowedDirs().contains(absPath))
    {
        QFileInfoList files = curDir.entryInfoList(pattern.getFiles(), QDir::Files);//список файлов в текущем каталоге с заданной маской фильтрации
        for(int ind = 0; ind < files.count(); ind++)//Обход списка файлов
            list_files_in_dir += oneFileProcessing(files.at(ind));//обработка одного файла
    }
    IF_UNDER_DIR(pattern)//Проверка параметра обхода подкаталогов в шаблоне
    {
        QFileInfoList dirs = curDir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);//Список каталогов в текущем
        for(int ind = 0; ind < dirs.count(); ind++)//обход каталогов
            list_files_in_dir += oneDirProcessing(dirs.at(ind));//Обработка одного каталога
    }
    return list_files_in_dir;
}

void Comment_Machine::run()
{
    QFileInfo root(pattern.getSearchDir());//корневой каталог для анализа-поиска
    QString list_files = oneDirProcessing(root);//обработка корневого каталога
    list_proced_files = list_files.split('\n');
}

