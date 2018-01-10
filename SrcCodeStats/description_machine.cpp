#include "description_machine.h"

Description_Machine::Description_Machine(QObject *parent) : QThread(parent)
{

}

Description_Machine::~Description_Machine()
{

}

#define SLASH_3 "///"
#define SLASH_1 "/"

QString Description_Machine::oneFileProcessing(QFileInfo file)
{
    QString fileName = file.absoluteFilePath();//полное имя обрабатываемого файла
    QFile txtFile(fileName);//обрабатываемый файл - текстовый файл
    QStringList lines;
    if (!txtFile.open(QFile::ReadOnly)) return "";//пытаемся его открыть
    {
        QTextStream stream(&txtFile);//поток для чтения текстового содержимого
        stream.setCodec(pattern.getCodec());
        lines = stream.readAll().split('\n');
    }
    txtFile.flush();
    txtFile.close();//закрываем файл
    QStringList procLines;
    for(int i = 0; i < lines.count(); i++)//удаление лишних строк
        if (lines.at(i).contains(SLASH_3))
        {
            int ind_splash = lines.at(i).indexOf(SLASH_3);
            QString newLine = lines[i].remove(0, lines.at(i).count() - ind_splash);
            procLines.append(newLine);
        }
    lines.clear();
    QList<Token> list_alg;
    for(int i = 0; i < procLines.count(); i++)//Построение списка токенов
    {
        QList<Token> temp_list = getTokensFromLine(procLines.at(i));
        list_alg.append(temp_list);
    }
    /*Обработка списка токенов*/
    QString result;

    return result;
}

//QString Description_Machine::oneTokenProcessing(Token token)
//{
////    switch(token.code)
////    {

////    }
//}

QList<Token> Description_Machine::getTokensFromLine(QString line)
{
    QList<int> token_indexes = getTokensIndexes(line);
    QList<Token> tokens;
    for(int i = 0; i < token_indexes.count(); i++)
    {
        QString text;
        if (i != token_indexes.count()-1)
        {
            text = line.mid(token_indexes.at(i), token_indexes.at(i+1) - token_indexes.at(i));
        }
        else
        {
            text = line.mid(token_indexes.at(i), line.count() - token_indexes.at(i));
        }
        if (text.trimmed().count() == 0) continue;
        Token item;
        if (text.contains(SLASH_3))
        {
            text = text.replace("/","").trimmed();
            if ((text.contains("alg")) && (0 == text.indexOf("alg")))
            {
                text = text.remove(0, 3).trimmed();
                item.code = TC_ALG;
                item.text = text;
            }
            else if ((text.contains("step")) && (0 == text.indexOf("step")))
            {
                text = text.remove(0, 4).trimmed();
                item.code = TC_STEP;
                item.text = text;
            }
            else if ((text.contains("id")) && (0 == text.indexOf("id")))
            {
                text = text.remove(0, 2).trimmed();
                item.code = TC_ID;
                item.text = text;
            }
        }
        else
        {
            text = text.replace("/","").trimmed();
            if (text.isEmpty()) continue;
            item.code = TC_TEXT;
            item.text = text;
        }
        tokens.append(item);
    }
    return tokens;
}

QList<int> Description_Machine::getTokensIndexes(QString line)
{
    QList<int> token_indexes;
    int curIndex = 0;
    while (curIndex < line.count()-1)
        if (line.contains(SLASH_3))
        {
            curIndex = line.indexOf(SLASH_3);
            token_indexes.append(curIndex);
            curIndex = line.indexOf(SLASH_1, curIndex + 3);
            token_indexes.append(curIndex);
        }
        else
            break;
    return token_indexes;
}

QString Description_Machine::oneDirProcessing(QFileInfo dir)
{
    QString list_files_in_dir; //результат обработки текущего каталога
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

void Description_Machine::run()
{
    QFileInfo root(pattern.getSearchDir());//корневой каталог для анализа-поиска
    result = oneDirProcessing(root);//обработка корневого каталога
}

