#ifndef COMMENT_PATTERN_H
#define COMMENT_PATTERN_H

#include <QObject>
#include <QStringList>

class Comment_Pattern : public QObject
{
    Q_OBJECT
public:
    explicit Comment_Pattern(QObject *parent = 0);
    ~Comment_Pattern();

    Comment_Pattern(const Comment_Pattern &obj);//Конструктор копирования
    Comment_Pattern & operator=(const Comment_Pattern &obj);//Оператор присваивания

    QString getResult() { return result; }

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

    int width_comment;
    int width_name_field;

    QString externalLine;
    QString internalLine;

    QString rslt_project;
    QString project;
    const QString const_project = "ПРОЕКТ";
    void on_changed_project() { rslt_project = QString("%1 %2 %3 %4").arg(const_project, QString().fill(QChar::Space, width_name_field - const_project.count()), ":", project); on_changedResult();}

    QString rslt_company;
    QString company;
    const QString const_company = "ОРГАНИЗАЦИЯ";
    void on_changed_company() { rslt_company = QString("%1 %2 %3 %4").arg(const_company, QString().fill(QChar(' '), width_name_field - const_company.count()), ":", company); on_changedResult();}

    QString rslt_author;
    QString author;
    const QString const_author = "АВТОРЫ";
    void on_changed_author() { rslt_author = QString("%1 %2 %3 %4").arg(const_author, QString().fill(QChar(' '), width_name_field - const_author.count()), ":", author); on_changedResult();}

    QString rslt_name_module;
    QString name_module;
    const QString const_name_module = "НАЗВАНИЕ МОДУЛЯ";
    void on_changed_name_module() { rslt_name_module = QString("%1 %2 %3 %4").arg(const_name_module, QString().fill(QChar(' '), width_name_field - const_name_module.count()), ":", name_module); on_changedResult();}

    QString rslt_name_file;
    QString name_file;
    const QString const_name_file = "ИМЯ ФАЙЛА";
    void on_changed_name_file() { rslt_name_file = QString("%1 %2 %3 %4").arg(const_name_file, QString().fill(QChar(' '), width_name_field - const_name_file.count()), ":", name_file); on_changedResult();}

    QString rslt_description;
    QString description;
    const QString const_description = "ОПИСАНИЕ";
    void on_changed_description() { rslt_description = QString("%1 %2 %3 %4").arg(const_description, QString().fill(QChar(' '), width_name_field - const_description.count()), ":", description); on_changedResult();}

    QString result;

    void on_changedWidthComment();
    void on_changedWidthNameField();
    void on_changedResult();
    void on_changedResult_forWidget();

signals:
    void updateOutput(QStringList list);

public slots:
    void setWidthComment(int value) {width_comment = value; on_changedWidthComment();}
    void setWidthNameField(int value) {width_name_field = value; on_changedWidthNameField();}

    void setProject(QString value) {project = value; on_changed_project();}
    void setCompany(QString value) {company = value; on_changed_company();}
    void setAuthor(QString value) {author = value; on_changed_author();}
    void setNameModule(QString value) {name_module = value; on_changed_name_module();}
    void setNameFile(QString value) {name_file = value; on_changed_name_file();}
    void setDescription(QString value) {description = value; on_changed_description();}
};

#endif // COMMENT_PATTERN_H
