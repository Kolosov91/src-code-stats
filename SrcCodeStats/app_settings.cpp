#include "app_settings.h"

App_Settings::App_Settings(QObject *parent) : QObject(parent)
{

}

App_Settings::~App_Settings()
{

}

void App_Settings::load()
{
    QSettings* rdOpt = new QSettings("SrcCodeStats", QSettings::IniFormat);
    rdOpt->beginGroup("/Settings");
    lastOpenDir = rdOpt->value("/lastOpenDir", QDir::currentPath()).toString();
    rdOpt->endGroup();
    delete rdOpt;
}

void App_Settings::save()
{
    QSettings* wrOpt = new QSettings("SrcCodeStats", QSettings::IniFormat);
    wrOpt->beginGroup("/Settings");
    wrOpt->setValue("/lastOpenDir", lastOpenDir);
    wrOpt->endGroup();
    wrOpt->sync();
    delete wrOpt;
}

