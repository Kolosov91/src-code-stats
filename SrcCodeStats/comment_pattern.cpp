#include "comment_pattern.h"

Comment_Pattern::Comment_Pattern(QObject *parent) : QObject(parent)
{
    width_comment = 60;
    width_name_field = 16;
    on_changedWidthNameField();
    on_changedWidthComment();
}

Comment_Pattern::~Comment_Pattern()
{

}

Comment_Pattern::Comment_Pattern(const Comment_Pattern &obj) : QObject()
{
    textCodec = obj.textCodec;
    listFiles = obj.listFiles;          //Список масок-файлов для обработки шаблона
    listStatParams = obj.listStatParams;      //Список параметров обработки шаблона
    searchDir = obj.searchDir;              //Каталог для анализа-поиска
    listAllowedDirs = obj.listAllowedDirs;    //Разрешённые для анализа-поиска каталоги

    width_comment = obj.width_comment;
    width_name_field = obj.width_name_field;

    externalLine = obj.externalLine;
    internalLine = obj.internalLine;

    rslt_project = obj.rslt_project;
    project = obj.project;

    rslt_company = obj.rslt_company;
    company = obj.company;

    rslt_author =obj.rslt_author ;
    author = obj.author;

    rslt_name_module = obj.rslt_name_module;
    name_module = obj.name_module;

    rslt_name_file = obj.rslt_name_file;
    name_file = obj.name_file;

    rslt_description = obj.rslt_description;
    description = obj.description;

    result = obj.result;
}

Comment_Pattern &Comment_Pattern::operator=(const Comment_Pattern &obj)
{
    if (this != &obj)//проверка на ссылку на самого себя
    {
        textCodec = obj.textCodec;
        listFiles = obj.listFiles;          //Список масок-файлов для обработки шаблона
        listStatParams = obj.listStatParams;      //Список параметров обработки шаблона
        searchDir = obj.searchDir;              //Каталог для анализа-поиска
        listAllowedDirs = obj.listAllowedDirs;    //Разрешённые для анализа-поиска каталоги

        width_comment = obj.width_comment;
        width_name_field = obj.width_name_field;

        externalLine = obj.externalLine;
        internalLine = obj.internalLine;

        rslt_project = obj.rslt_project;
        project = obj.project;

        rslt_company = obj.rslt_company;
        company = obj.company;

        rslt_author =obj.rslt_author ;
        author = obj.author;

        rslt_name_module = obj.rslt_name_module;
        name_module = obj.name_module;

        rslt_name_file = obj.rslt_name_file;
        name_file = obj.name_file;

        rslt_description = obj.rslt_description;
        description = obj.description;

        result = obj.result;
    }
    return *this;
}

void Comment_Pattern::on_changedWidthComment()
{
    externalLine.clear();
    externalLine = externalLine.fill(QChar('='), width_comment);
    internalLine.clear();
    internalLine = internalLine.fill(QChar('-'), width_comment);
    on_changedResult();
}

void Comment_Pattern::on_changedWidthNameField()
{
    on_changed_project();
    on_changed_company();
    on_changed_author();
    on_changed_name_module();
    on_changed_name_file();
    on_changed_description();
}

void Comment_Pattern::on_changedResult()
{
    result.clear();
    result += QString("/*\n");
    result += (QString(" * %1\n").arg(externalLine));
    result += (QString(" * %1\n").arg(rslt_project));
    result += (QString(" * %1\n").arg(rslt_company));
    result += (QString(" * %1\n").arg(rslt_author));
    result += (QString(" * %1\n").arg(internalLine));
    result += (QString(" * %1\n").arg(rslt_name_module));
    result += (QString(" * %1\n").arg(rslt_name_file));
    result += (QString(" * %1\n").arg(internalLine));
    result += (QString(" * %1\n").arg(rslt_description));
    result += (QString(" * %1\n").arg(externalLine));
    result += QString(" */\n");
    on_changedResult_forWidget();
}

void Comment_Pattern::on_changedResult_forWidget()
{
    QStringList widget_result;
    widget_result = result.split('\n');
    emit updateOutput(widget_result);
}

