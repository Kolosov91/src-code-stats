#include "search_pattern.h"

Search_Pattern::Search_Pattern(QObject *parent) : QObject(parent) //Конструктор
{
    this->listFiles.clear();
    this->listStatParams.clear();
    this->searchDir.clear();
    this->listAllowedDirs.clear();
}

Search_Pattern::~Search_Pattern()
{

}

Search_Pattern::Search_Pattern(const Search_Pattern &obj) : QObject() //Конструктор копирования
{
    this->listFiles = obj.listFiles;
    this->listStatParams = obj.listStatParams;
    this->searchDir = obj.searchDir;
    this->listAllowedDirs = obj.listAllowedDirs;
}

Search_Pattern &Search_Pattern::operator=(const Search_Pattern &obj) //Оператор присваивания
{
    if (this != &obj)//проверка на ссылку на самого себя
    {
        this->listFiles = obj.listFiles;
        this->listStatParams = obj.listStatParams;
        this->searchDir = obj.searchDir;
        this->listAllowedDirs = obj.listAllowedDirs;
    }
    return *this;
}

