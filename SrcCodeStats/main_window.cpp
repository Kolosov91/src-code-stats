#include "main_window.h"
#include "ui_main_window.h"

Main_Window::Main_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Main_Window)
{
    ui->setupUi(this);
    ui->treeWidget_underDir->setVisible(false);
    ui->treeWidget_underDir->setEnabled(true);
    ui->lineEdit_directoryProject->setReadOnly(true);
    /* Создание объектов*/
    machine = new Stat_Machine(this);
    pattern = new Search_Pattern(this);
    comment_pattern = new Comment_Pattern(this);
    comment_machine = new Comment_Machine(this);

    web = NULL;
    settings = new App_Settings(this);
    settings->load();//загрузка параметров приложения
    /* Соединение слотов-сигналов*/
    connect(comment_machine, SIGNAL(finished()), this, SLOT(on_finished_session_comment_machine()));
    connect(machine, SIGNAL(finished()), this, SLOT(on_finished_session_machine()));
    connect(machine, SIGNAL(sgnDirChanged(QString)), this, SLOT(on_dir_changed(QString)));
    connect(machine, SIGNAL(sgnFileChanged(QString)), this, SLOT(on_file_changed(QString)));
    connect(machine, SIGNAL(sgnCountAllStrChanged(int)), this, SLOT(on_count_all_str_changed(int)));
    connect(ui->pushButton_directoryProject, SIGNAL(clicked()), ui->action_chooseDir, SLOT(trigger()));

    //connect(ui->comboBox_codec, SIGNAL(currentTextChanged(QString)), comment_pattern, SLOT(setNameCodec(QString)));

    connect(comment_pattern, SIGNAL(updateOutput(QStringList)), this, SLOT(update_result_comment_caption(QStringList)));
    connect(ui->lineEdit_project, SIGNAL(textChanged(QString)), comment_pattern, SLOT(setProject(QString)));
    connect(ui->lineEdit_company, SIGNAL(textChanged(QString)), comment_pattern, SLOT(setCompany(QString)));
    connect(ui->lineEdit_author, SIGNAL(textChanged(QString)), comment_pattern, SLOT(setAuthor(QString)));
    connect(ui->spinBox_width_comment, SIGNAL(valueChanged(int)), comment_pattern, SLOT(setWidthComment(int)));
    connect(ui->spinBox_width_name_field, SIGNAL(valueChanged(int)), comment_pattern, SLOT(setWidthNameField(int)));
}

Main_Window::~Main_Window()
{
    delete ui;
}

void Main_Window::on_action_calcStat_triggered() //Старт анализа-поиска
{
    /*Сброс элементов интерфейса*/
    ui->listWidget->clear();
    pattern->clear();
    ui->label_total->setText(QString("Общее количество строк: X"));
    pattern->setSearchDir(ui->lineEdit_directoryProject->text());
    if (pattern->getSearchDir().isEmpty()) return; //Если не выбран каталог, то не выполняем поиск
    /*Настройка масок файлов*/
    QStringList list_files; //список масок
    if (ui->checkBox_c->isChecked())    list_files << FILE_C;
    if (ui->checkBox_h->isChecked())    list_files << FILE_H;
    if (ui->checkBox_cpp->isChecked())  list_files << FILE_CPP;
    if (ui->checkBox_cs->isChecked())   list_files << FILE_CS;
    pattern->appendFiles(list_files);   //добавить список масок в шаблон
    /*Настройка параметров анализа-поиска*/
    QList<int> list_stat_params; //список параметров
    list_stat_params << TOTAL_STAT;
    if (ui->radioButton_statParam_large->isChecked()) list_stat_params << LARGE_STAT;
    if (ui->checkBox_statParam_underDir->isChecked()) list_stat_params << UNDER_DIR;
    pattern->appendStatParams(list_stat_params); //добавить список параметров в шаблон

    QStringList listAllowedDirs;//Разрешённые для анализа подкаталоги
    if (ui->checkBox_statParam_underDir->isChecked())
    {
        int countItems = listPWidgetItem.count();//кол-во подкаталогов и их виджетов
        for(int ind=0; ind < countItems; ind++)
            if (listPWidgetItem.at(ind)->checkState(0) == Qt::Checked)
                listAllowedDirs.append(listAbsolutePath.at(ind));
    }
    listAllowedDirs.append(QFileInfo(pattern->getSearchDir()).absoluteFilePath());//добавление корневого каталога в разрешённые

    pattern->setAllowedDirs(listAllowedDirs);//Установка в шаблон разрешённых подкаталогов

    machine->setPattern(*pattern); //добавить шаблон в аналитическую машину
    machine->start(); //старт анализа-поиска
    this->setEnabled(false); //делаем интефейс недоступным до завершения анализа
}

void Main_Window::on_action_chooseDir_triggered() //Выбор каталога
{
    QFileDialog* dialog = new QFileDialog(this);
    QString dir = dialog->getExistingDirectory(this, QString("Открыть папку с проектом"),
                                 settings->lastOpenDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    settings->lastOpenDir = dir;
//    QString dir = ui->lineEdit_directoryProject->text();
    if (dir.isEmpty()) return;
    ui->lineEdit_directoryProject->setText(dir);
    pattern->setSearchDir(dir); //Установка полного имени каталого для анализа-поиска
    on_checkBox_statParam_underDir_toggled((ui->checkBox_statParam_underDir->checkState() == Qt::Checked)?true:false);
}

void Main_Window::on_finished_session_machine() //Окончание работы аналитической машины
{
    if (NULL != web) delete web;
    web = new QWebEngineView();//Создание компонента для отображения html-содержимого
    connect(web, SIGNAL(loadFinished(bool)), this, SLOT(loadHtmlFinished(bool)));
    web->setHtml(machine->getResultHtml());//Установить html-содержимое
}

void Main_Window::loadHtmlFinished(bool)
{
    web->show();
    this->setEnabled(true);//делаем интефейс доступным для использования
    ui->listWidget->addItem("Обработка завершена!");
}

void Main_Window::update_result_comment_caption(QStringList list)
{
    ui->textEdit_caption->clear();
    QTextCharFormat fmt = ui->textEdit_caption->currentCharFormat();
    fmt.setForeground(QBrush(Qt::darkGreen));
    ui->textEdit_caption->setCurrentCharFormat(fmt);
    foreach(QString line, list)
    {
        ui->textEdit_caption->append(line);
    }
}

void Main_Window::closeEvent(QCloseEvent *)//Событие закрытия приложения
{
    settings->save();//сохранение параметров приложения
}

void Main_Window::showEvent(QShowEvent *)
{
    comment_pattern->setProject("например");
    comment_pattern->setCompany("АО \"Авиаавтоматика\" им. В.В.Тарасова\"");
    comment_pattern->setAuthor("автор");
//    comment_pattern->setNameModule("пример названия");
//    comment_pattern->setNameFile("название.файл");
//    comment_pattern->setDescription("пример описания");
}

void Main_Window::on_dir_changed(QString dir_name) //Изменение текущего каталога аналитической машины во время анализа-поиска
{
    ui->listWidget->addItem(dir_name);
}

void Main_Window::on_file_changed(QString file_name) //Изменение текущего файла аналитической машины во время анализа-поиска
{
    ui->listWidget->addItem("     " + file_name);
}

void Main_Window::on_count_all_str_changed(int count) //Изменение кол-ва обработанных строк аналитической машины во время анализа-поиска
{
    ui->label_total->setText(QString("Общее количество строк: %1").arg(QString::number(count)));
}

void Main_Window::on_checkBox_statParam_underDir_toggled(bool checked)//Клик по "Учитывать подкаталоги"
{
    listPWidgetItem.clear();
    listAbsolutePath.clear();
    ui->treeWidget_underDir->clear();
    if (checked)
    {
        ui->treeWidget_underDir->setVisible(true);
        QDir curdir(pattern->getSearchDir());//корневой каталог (в данном случае текущий)
        QFileInfoList dirs = curdir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);//Список каталогов в текущем
        for(int ind = 0; ind < dirs.count(); ind++)//обход каталогов
        {
            QFileInfo infodir = dirs.at(ind);//информация о текущем подкаталоге
            QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget_underDir);// виджет подкаталоге
            listPWidgetItem.append(item);
            listAbsolutePath.append(infodir.absoluteFilePath());
            item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            item->setCheckState(0, Qt::Checked);
            item->setText(0, infodir.baseName());//имя подкаталога
            createTreeUnderdir(item, infodir);//углубляемся на уровень вниз
            ui->treeWidget_underDir->addTopLevelItem(item);//добавляем виджет подкаталоге на экран
        }
        ui->treeWidget_underDir->expandAll();
    }
    else
    {
        ui->treeWidget_underDir->setVisible(false);
    }
}

void Main_Window::createTreeUnderdir(QTreeWidgetItem *item, QFileInfo infodir)//Рекурсивная функция построения дерева подкаталогов
{
    QDir curdir(infodir.absoluteFilePath());//текущий каталог
    QFileInfoList dirs = curdir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);//Список каталогов в текущем
    for(int ind = 0; ind < dirs.count(); ind++)//обход каталогов
    {
        QFileInfo curinfodir = dirs.at(ind);//информация о текущем подкаталоге
        QTreeWidgetItem* curitem = new QTreeWidgetItem(item);// виджет подкаталоге
        listPWidgetItem.append(curitem);
        listAbsolutePath.append(curinfodir.absoluteFilePath());
        curitem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        curitem->setCheckState(0, Qt::Checked);
        curitem->setText(0, curinfodir.baseName());//имя подкаталога
        createTreeUnderdir(curitem, curinfodir);//углубляемся на уровень вниз
        item->addChild(curitem);//добавляем виджет подкаталоге на экран
    }
}

void Main_Window::on_action_commentStart_triggered()
{
    /*Сброс элементов интерфейса*/
    comment_pattern->clear();
    comment_pattern->setSearchDir(ui->lineEdit_directoryProject->text());
    if (comment_pattern->getSearchDir().isEmpty()) return; //Если не выбран каталог, то не выполняем поиск
    /*Настройка масок файлов*/
    QStringList list_files; //список масок
    if (ui->checkBox_c->isChecked())    list_files << FILE_C;
    if (ui->checkBox_h->isChecked())    list_files << FILE_H;
    if (ui->checkBox_cpp->isChecked())  list_files << FILE_CPP;
    if (ui->checkBox_cs->isChecked())   list_files << FILE_CS;
    comment_pattern->appendFiles(list_files);   //добавить список масок в шаблон
    /*Настройка параметров анализа-поиска*/
    QList<int> list_stat_params; //список параметров
    if (ui->checkBox_statParam_underDir->isChecked()) list_stat_params << UNDER_DIR;
    comment_pattern->appendStatParams(list_stat_params); //добавить список параметров в шаблон
    QStringList listAllowedDirs;//Разрешённые для анализа подкаталоги
    if (ui->checkBox_statParam_underDir->isChecked())
    {
        int countItems = listPWidgetItem.count();//кол-во подкаталогов и их виджетов
        for(int ind=0; ind < countItems; ind++)
            if (listPWidgetItem.at(ind)->checkState(0) == Qt::Checked)
                listAllowedDirs.append(listAbsolutePath.at(ind));
    }
    listAllowedDirs.append(QFileInfo(comment_pattern->getSearchDir()).absoluteFilePath());//добавление корневого каталога в разрешённые

    comment_pattern->setAllowedDirs(listAllowedDirs);//Установка в шаблон разрешённых подкаталогов

    switch (ui->comboBox_codec->currentIndex())
    {
        case 1: comment_pattern->setCodec(QTextCodec::codecForName("Windows-1251")); break;
        default :comment_pattern->setCodec(QTextCodec::codecForName("UTF-8")); break;
    }

    comment_machine->setPattern(*comment_pattern);//добавить шаблон
    comment_machine->start(); //старт
    this->setEnabled(false); //делаем интефейс недоступным до завершения анализа
}

void Main_Window::on_finished_session_comment_machine()
{
    this->setEnabled(true);
}
