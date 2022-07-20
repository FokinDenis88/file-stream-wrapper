#ifndef FILECONSTANTS_H
#define FILECONSTANTS_H

#include <fstream>

// write, read files, another service operations based on fstream
namespace file {
    // Запись в конец файла - режим открытия файла
    constexpr std::ios_base::openmode OpenModeWriteAppend = std::ios_base::out | std::ios_base::app;

    // Перезапись файла. Внимание все данные в файле удаляются, потом записывается новая информация. - Режим открытия файла
    constexpr std::ios_base::openmode OpenModeWriteRewrite = std::ios_base::out | std::ios_base::trunc;

    // Чтение файла с начала - режима открытия файла.
    constexpr std::ios_base::openmode OpenModeReadFromBegin = std::ios_base::in;

    // Запись объекта в бинарный файл. Добавление данных в конец файла
    constexpr std::ios_base::openmode OpenModeWriteBinaryAppend = std::ios_base::out | std::ios_base::binary | std::ios_base::app;

    // Запись объекта в бинарный файл. Добавление данных в конец файла
    constexpr std::ios_base::openmode OpenModeWriteBinaryRewrite = std::ios_base::out | std::ios_base::binary | std::ios_base::trunc;

    // Чтение файла с начала - режима открытия файла.
    constexpr std::ios_base::openmode OpenModeReadBinary = std::ios_base::in | std::ios_base::binary;

    // Чтение и запись файла в бинарном режиме
    constexpr std::ios_base::openmode OpenModeReadWriteBinary = std::ios_base::in | std::ios_base::out | std::ios_base::binary;

}

#endif // !FILECONSTANTS_H