#ifndef APP_SETTINGS_H
#define APP_SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QDir>

class App_Settings : public QObject //Сохранение/загрузка параметров приложения
{
    Q_OBJECT
public:
    explicit App_Settings(QObject *parent = 0);
    ~App_Settings();

    void load();//загрузка параметров
    void save();//сохранение параметров

    /*параметры приложения*/
    QString lastOpenDir;//последняя открываемая папка
};

#endif // APP_SETTINGS_H
