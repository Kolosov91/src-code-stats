#ifndef CONSTANTS
#define CONSTANTS

/*Константы-значения файлов для обработки*/
#define FILE_C          QString("*.c")      //c-файл
#define FILE_H          QString("*.h")      //h-файл
#define FILE_CPP        QString("*.cpp")    //cpp-файл
#define FILE_CS         QString("*.cs")    //cs-файл
/**/
/*Константы-значения параметров обработки*/
#define TOTAL_STAT  1   //Общая статистика
#define LARGE_STAT  2   //Расширенная статистика (по каждому файлу)
#define UNDER_DIR   3   //Обход подкаталогов
/**/
#define IF_LARGE_STAT(pattern) if (pattern.getStatParams().contains(LARGE_STAT))
#define IF_TOTAL_STAT(pattern) if (pattern.getStatParams().contains(TOTAL_STAT))
#define IF_UNDER_DIR(pattern) if (pattern.getStatParams().contains(UNDER_DIR))

#endif // CONSTANTS

