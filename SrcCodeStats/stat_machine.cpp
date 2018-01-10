#include "stat_machine.h"

#define M_TABLE_TAG(border, cellpadding, face, fontsize, align)  QString("<table border=\"%1\" cellpadding=\"%2\" cellspacing=\"0\" face =\"%3\" style=\"font-size:%4.0pt;\" align=\"%5\">\n").arg(border, cellpadding, face, fontsize, align)
#define M_PRGRF(text, align, bold) QString("<p align=\"%1\">").arg(align) + QString((bold)?"<b>":"") + QString("%1").arg(text) + QString((bold)?"</b>":"") + QString("</p>\n")

Stat_Machine::Stat_Machine(QObject *parent) : QThread(parent)
{
    fontName = "Arial";
    fontSize = 8;
}

Stat_Machine::~Stat_Machine()
{

}

/*Таблица - Состояния управляющего автомата анализа C-подобных файлов*/
#define ST_EMPTY_STR                        0       //пустая строка
#define ST_CMNT_END_CODE                    1       //предыдущаястрока - комментарий, текущая - конец многострочных комментарий и есть код
#define ST_CMNT_END_NO_CODE                 2       //предыдущаястрока - комментарий, текущая - конец многострочных комментарий и НЕТ кода
#define ST_CMNT_IN_ONLY                     3       //предыдущаястрока - комментарий, текущая - НЕ содержит окончание комментарий, т.е. является комментарием
#define ST_NO_CMNT_ONE_STR_CODE             4       //предыдущаястрока - НЕ комментарий, текущая строка содержит однострочный комментарий, кроме комментарий есть код
#define ST_NO_CMNT_ONE_STR_NO_CODE          5       //предыдущаястрока - НЕ комментарий, текущая строка содержит однострочный комментарий, кроме комментарий НЕТ кода
#define ST_NO_CMNT_MANY_STR_CODE            6       //предыдущаястрока - НЕ комментарий, текущая строка содержит начало и конец многострочных комментарий (однострочно), кроме многострочных комментариев есть код
#define ST_NO_CMNT_MANY_STR_NO_CODE         7       //предыдущаястрока - НЕ комментарий, текущая строка содержит начало и конец многострочных комментарий (однострочно), кроме начало многострочных комментариев НЕТ кода
#define ST_NO_CMNT_MANY_STR_BEG_CODE        8       //предыдущаястрока - НЕ комментарий, текущая строка содержит начало многострочных комментариев, кроме начало многострочных комментариев есть код
#define ST_NO_CMNT_MANY_STR_BEG_NO_CODE     9       //предыдущаястрока - НЕ комментарий, текущая строка содержит начало многострочных комментариев, кроме начало многострочных комментариев НЕТ кода
#define ST_CODE_ONLY                        10      //предыдущаястрока - НЕ комментарий, текущая строка содержит только код
/**/
/*Таблица - Токены-Символы*/
#define SYMB_ONE_COMM               QString("//")   //однострочный комментарий
#define SYMB_BEG_MANY_COMM          QString("/*")   //начало многострочного комментария
#define SYMB_END_MANY_COMM          QString("*/")   //конец многострочного комментария
/**/

int Stat_Machine::checkStateAnalizeAutomaton_c_file(QString line, bool isInComm)//Управляющий автомат анализа кода C-подобных файлов
{
    int state;//Состояние автомата
    if (line.isEmpty()) state = ST_EMPTY_STR;//текущая строка пустая
    else //текущая строка НЕ пустая
        if (isInComm)//текущая строка комментарий!
            if (line.contains(SYMB_END_MANY_COMM))//текущая строка содержит окончание комментариев
            {
               isInComm = false;
               int pose = line.indexOf(SYMB_END_MANY_COMM);
               QString tempLine  = line.remove(0, pose + SYMB_END_MANY_COMM.count()).trimmed();
               if (tempLine.count()>0)//кроме комментариев есть код
                   state = ST_CMNT_END_CODE;
               else//кроме комментариев НЕТ кода
                   state = ST_CMNT_END_NO_CODE;
            }
            else//текущая строка НЕ содержит окончание комментариев, т.е. является комментарием
                state = ST_CMNT_IN_ONLY;
        else //текущая строка НЕ комментарий!
            if (line.contains(SYMB_ONE_COMM))//текущая строка содержит однострочный комментарий
            {
                int pos = line.indexOf(SYMB_ONE_COMM);
                QString tempLine = line.remove(pos, line.count()).trimmed();
                if (tempLine.count()>0)//кроме комментариев есть код
                    state = ST_NO_CMNT_ONE_STR_CODE;
                else //кроме комментариев НЕТ кода
                    state = ST_NO_CMNT_ONE_STR_NO_CODE;
            }
            else //текущая строка НЕ содержит однострочный комментарий
                if (line.contains(SYMB_BEG_MANY_COMM) && line.contains(SYMB_END_MANY_COMM))//текущая строка содержит начало и конец многострочных комментарий (однострочно)
                {
                    int posb = line.indexOf(SYMB_BEG_MANY_COMM);
                    int pose = line.indexOf(SYMB_END_MANY_COMM);
                    QString tempLine = line.remove(posb, pose - posb + SYMB_END_MANY_COMM.count()).trimmed();
                    if (tempLine.count()>0)//кроме многострочных комментариев есть код
                        state = ST_NO_CMNT_MANY_STR_CODE;
                    else //кроме многострочных комментариев НЕТ кода
                        state = ST_NO_CMNT_MANY_STR_NO_CODE;
                }
                else //текущая строка НЕ содержит начало и конец многострочных комментарий (однострочно)
                    if (line.contains(SYMB_BEG_MANY_COMM))//текущая строка содержит начало многострочных комментариев
                    {
                        int posb = line.indexOf(SYMB_BEG_MANY_COMM);
                        QString tempLine = line.remove(posb, line.count()).trimmed();
                        if (tempLine.count()>0) //кроме начало многострочных комментариев есть код
                            state = ST_NO_CMNT_MANY_STR_BEG_CODE;
                        else //кроме начало многострочных комментариев НЕТ кода
                            state = ST_NO_CMNT_MANY_STR_BEG_NO_CODE;
                    }
                    else//текущая строка содержит только код
                        state = ST_CODE_ONLY;
    return state;
}


StatType Stat_Machine::analizeOne_c_file(QStringList lines) //Анализ содержимого одного файла
{
    #define ADD_TO_SRC_DATA(){ /*Формирование исходных данных для учёта статистических параметров*/ \
        curFileStat.useful_str++; \
        double length_str = lines.at(ind).count(); /*кол-во символов в текущей строке*/ \
        list_length_str->append(length_str); /*добавление кол-ва символов в список для анализа*/ \
        if (curFileStat.min_len_str > length_str) curFileStat.min_len_str = length_str; /*поиск минимального кол-ва*/ \
        if (curFileStat.max_len_str < length_str) curFileStat.max_len_str = length_str; /*поиск максимального кол-ва*/ \
    }
    StatType curFileStat;//статистика текущего файла
    int all_str = lines.count();//общее кол-во строк в файле
    bool isInComm = false;//признак комментария, т.е. текущая строчка является комментарием
    QList<double>* list_length_str = new QList<double>;//список длин строк
    for (int ind = 0; ind < all_str; ind++)//обход содержимого построчно
    {
        QString line = lines.at(ind).trimmed();//текущая строка
        int stateAnalizeAutomaton = checkStateAnalizeAutomaton_c_file(line, isInComm);//вызов шага управляющего автомата
        switch (stateAnalizeAutomaton)//проверка состояния автомата - алгоритм состояний
        {
            case ST_EMPTY_STR:                      continue;
            case ST_CMNT_END_CODE:                  ADD_TO_SRC_DATA(); isInComm = false; curFileStat.code_comment_str++;    break;
            case ST_CMNT_END_NO_CODE:               ADD_TO_SRC_DATA(); isInComm = false; curFileStat.comment_str++;         break;
            case ST_CMNT_IN_ONLY:                   ADD_TO_SRC_DATA(); curFileStat.comment_str++;                           break;
            case ST_NO_CMNT_ONE_STR_CODE:           ADD_TO_SRC_DATA(); curFileStat.code_comment_str++;                      break;
            case ST_NO_CMNT_ONE_STR_NO_CODE:        ADD_TO_SRC_DATA(); curFileStat.comment_str++;                           break;
            case ST_NO_CMNT_MANY_STR_CODE:          ADD_TO_SRC_DATA(); curFileStat.code_comment_str++;                      break;
            case ST_NO_CMNT_MANY_STR_NO_CODE:       ADD_TO_SRC_DATA(); curFileStat.comment_str++;                           break;
            case ST_NO_CMNT_MANY_STR_BEG_CODE:      ADD_TO_SRC_DATA(); isInComm = true; curFileStat.code_comment_str++;     break;
            case ST_NO_CMNT_MANY_STR_BEG_NO_CODE:   ADD_TO_SRC_DATA(); isInComm = true; curFileStat.comment_str++;          break;
            case ST_CODE_ONLY:                      ADD_TO_SRC_DATA(); curFileStat.code_str++;                              break;
        }
    }
    /*учёт статистических параметров*/
    curFileStat.expectation = expectation(list_length_str);//вычисление мат.ожидания
    curFileStat.variance = variance(list_length_str);//вычисление дисперсии
    curFileStat.deviation = deviation(list_length_str);//вычисление среднеквадратичного отклонения
    list_expectations->append(curFileStat.expectation);//добавление мат.ожидания для расчёта среднего мат.ожидания по проекту
    /*учёт общей статистики*/
    total.code_comment_str += curFileStat.code_comment_str;
    total.comment_str += curFileStat.comment_str;
    total.code_str += curFileStat.code_str;
    total.useful_str += curFileStat.useful_str;
    if (total.min_len_str > curFileStat.min_len_str) total.min_len_str = curFileStat.min_len_str;
    if (total.max_len_str < curFileStat.max_len_str) total.max_len_str = curFileStat.max_len_str;
    delete list_length_str;
    emit sgnCountAllStrChanged(total.useful_str);//изменение общего кол-ва обработанных строк
    return curFileStat;
}

double Stat_Machine::expectation(QList<double> *list)//математическое ожидание
{
    double sum = 0;
    for (int ind = list->count()-1; ind >= 0; ind--)
        sum += list->at(ind);
    return sum / (double)(list->count());
}

double Stat_Machine::variance(QList<double> *list)//дисперсия
{
    double sum = 0;
    double mean = expectation(list);
    for (int ind = list->count()-1; ind >= 0; ind--)
    {
        double temp = ((double)(list->count()) - mean);
        sum += temp * temp;
    }
    return sum / (double)(list->count());
}

double Stat_Machine::deviation(QList<double> *list)//среднеквадратичное отклонение
{
    double var = variance(list);
    return sqrt(var);
}

QString Stat_Machine::oneFileProcessing(QFileInfo file) //Обработка одного файла
{
    StatType curFileStat; //статистика для текущего обрабатываемого файла
    /**/
    emit sgnFileChanged(file.fileName()); //изменение имени текущего обрабатываемого файла
    QString fileName = file.absoluteFilePath();//полное имя обрабатываемого файла
    QFile txtFile(fileName);//обрабатываемый файл - текстовый файл
    if (!txtFile.open(QFile::ReadOnly | QFile::Text)) return "";//пытаемся его открыть
    {
        QTextStream stream(&txtFile);//поток для чтения текстового содержимого
        QStringList lines = stream.readAll().split('\n');//Разделяем содержимое на строки
        curFileStat = analizeOne_c_file(lines);//анализируем содержимое
    }
    txtFile.flush();
    txtFile.close();//закрываем файл
    /**/
    QString html;//результат обработки текущего файла в html-формате
    IF_LARGE_STAT(pattern)//Проверка параметра расширенной статистики в шаблоне
    {
        /*Создание таблицы со статистикой для текущего файла*/
        html += fileStatTable(file.fileName(), curFileStat);
    }
    return html;
}

QString Stat_Machine::oneDirProcessing(QFileInfo dir) //Обработка одного каталога - рекурсивная функция
{
    QString html; //результат обработки текущего каталога в html-формате
    QString absPath = dir.absoluteFilePath();//абсолютный путь к текущему каталогу
    QDir curDir(absPath);//текщий обрабатываемы каталог
    if (pattern.getAllowedDirs().contains(absPath))
    {
        emit sgnDirChanged(dir.absoluteFilePath());//изменение имени текущего обрабатываемого каталога
        IF_LARGE_STAT(pattern)//Проверка параметра расширенной статистики в шаблоне
        {
            /*Информация о текущем обрабатываемом каталоге*/
            html += QString("<font face=\"%1\" style=\"font-size:%2.0pt;\">\n").arg(fontName, fontSize);
            html += QString("<p align=\"center\"><b>Каталог: '%1'</b></p>\n").arg(curDir.dirName());
            html += QString("</font>\n");
        }
        QFileInfoList files = curDir.entryInfoList(pattern.getFiles(), QDir::Files);//список файлов в текущем каталоге с заданной маской фильтрации
        for(int ind = 0; ind < files.count(); ind++)//Обход списка файлов
            html += oneFileProcessing(files.at(ind));//обработка одного файла
    }
    IF_UNDER_DIR(pattern)//Проверка параметра обхода подкаталогов в шаблоне
    {
        QFileInfoList dirs = curDir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);//Список каталогов в текущем
        for(int ind = 0; ind < dirs.count(); ind++)//обход каталогов
            html += oneDirProcessing(dirs.at(ind));//Обработка одного каталога
    }
    return html;
}

/*Теги заголовка таблицы и строки таблицы*/

#define M_THEAD_TABLE(c1,c2,c3) { \
    html += QString("<thead><tr align=\"center\">\n"); \
    html += QString("<th>%1</th><th>%2</th><th>%3</th>\n").arg(c1, c2, c3); \
    html += QString("</tr></thead>\n"); \
}
#define M_ROW_TABLE(c1,c2,c3) { \
    html += QString("<tr align=\"center\">\n"); \
    html += QString("<td align=\"left\">%1</td><td>%2</td><td>%3</td>\n").arg(c1 , c2, QString::number(c3)); \
    html += QString("</tr>\n"); \
}

/**/

QString Stat_Machine::totalStatTable() //Генерация таблицы с общей статистикой
{
    QString html;//таблица общей статистики в html-формат
    html += M_TABLE_TAG("1","1", fontName, fontSize, "center");
    if (total.useful_str == 0) return html;
    html += QString("<caption><b>%1</b></caption>\n").arg("Общая статистика");
    /*Заголовок таблицы*/
    M_THEAD_TABLE("Параметр", "Идентификатор", "Значение");
    /*Содержимое таблицы*/
    /*Общее кол-во строк*/
    M_ROW_TABLE("Общее кол-во строк (шт)", "О", total.useful_str);
    M_ROW_TABLE("Комментированность кода (%)", "КМ", (total.code_comment_str/2 + total.comment_str)*100/total.useful_str);
    M_ROW_TABLE("Мин. кол-во символов в строке", "Мин", total.min_len_str);
    M_ROW_TABLE("Макс. кол-во символов в строке", "Макс", total.max_len_str);
    M_ROW_TABLE("Среднее кол-во символов в строке - мат. ожидание", "СрМатОж", total.expectation);
    html += QString("<table>");
    return html;
}

QString Stat_Machine::fileStatTable(QString caption, StatType stat) //Генерация одной таблицы с заданным содержимым
{
    QString html; //таблица в html-формат
    if (stat.useful_str == 0) return html;
    html += M_TABLE_TAG("1","1", fontName, fontSize, "center");
    html += QString("<caption><b>%1</b></caption>\n").arg(caption);
    /*Заголовок таблицы*/
    M_THEAD_TABLE("Параметр", "Идентификатор", "Значение");
    /*Содержимое таблицы*/
    M_ROW_TABLE("Общее кол-во строк (шт)", "О", stat.useful_str);
    M_ROW_TABLE("Комментированность кода (%)", "КМ", (stat.code_comment_str/2 + stat.comment_str)*100/stat.useful_str);
    M_ROW_TABLE("Мин. кол-во символов в строке", "Мин", stat.min_len_str);
    M_ROW_TABLE("Макс. кол-во символов в строке", "Макс", stat.max_len_str);
    M_ROW_TABLE("Кол-во символов в строке - мат. ожидание", "МатОж", stat.expectation);
    M_ROW_TABLE("Кол-во символов в строке - дисперсия", "Дисп", stat.variance);
    M_ROW_TABLE("Кол-во символов в строке - стреднеквадр. откл.", "СрдОткл", stat.deviation);
    html += QString("</table>\n");
    return html;
}

void Stat_Machine::clearTotalParams() //Сброс общей статистики анализа-поиска
{
    total.code_str = 0;
    total.useful_str = 0;
    total.comment_str = 0;
    total.code_comment_str = 0;
    total.expectation = 0;
    total.variance = 0;
    total.deviation = 0;
    total.max_len_str = INT_MIN;
    total.min_len_str = INT_MAX;
}

void Stat_Machine::run() //Основной алгоритм аналитической машины
{
    QString html; //результат в html-формате
    QFileInfo root(pattern.getSearchDir());//корневой каталог для анализа-поиска
    clearTotalParams();//Сброс общей статистики анализа-поиска
    /*HEAD*/
    html += QString("<html>\n");
    html += QString("<meta http-equiv 'Content-Type' content='text/html; charset=ANSI' />\n");
    html += QString("<head><title>%1</title></head>\n").arg(QString("name: %1 - version: %2 - author: %3").arg(NAME, VERSION, AUTHOR));//инфо о программе
    html += QString("<body>\n");
    /*BODY*/
    //заголовок документа
    html += QString("<font face=\"%1\" style=\"font-size:%2.0pt;\">\n").arg(fontName, fontSize);
    html += QString("<p align=\"center\">%1</p>\n").arg(QString("name: %1 - version: %2 - author: %3").arg(NAME, VERSION, AUTHOR));//инфо о программе
    html += QString("<hr/>\n");
    html += QString("<p align=\"center\"><b>%1</b></p>\n").arg(root.absoluteFilePath());
    html += QString("</font>\n");
    list_expectations = new QList<double>;
    html += oneDirProcessing(root);//обработка корневого каталога
    html += QString("<hr/>\n");
    /*ОБЩАЯ СТАТИСТИКА*/
    IF_TOTAL_STAT(pattern)//Проверка параметра общей статистики в шаблоне
    {
        total.expectation = expectation(list_expectations);
        html += totalStatTable();//генерация таблицы с общей статистикой в html-формате
    }
    delete list_expectations;
    html += QString("<hr/>\n");
    /*ПРИМЕЧАНИЕ*/
    html += M_PRGRF("Примечание:","left", false);
    html += M_PRGRF("О - все строки за исключением пустых;","left", false);
    html += M_PRGRF("КМ - содержание комментариев в %;","left", false);
    html += QString("</body>\n");
    html += QString("</html>\n");
    this->result_html = html;//копирование результата в соответствующее поле класса
}

