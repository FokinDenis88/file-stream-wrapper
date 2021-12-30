#ifndef ERRORSHANDLERFILEOPERATIONS_H
#define ERRORSHANDLERFILEOPERATIONS_H

#include <exception>
#include <stdexcept>

namespace file {

	// Ошибка открытия файла
	class open_file_error : public std::runtime_error {
	public:
		open_file_error() : std::runtime_error("Error in opening file.") {}
	};

	// Ошибка закрытия файла
	class close_file_error : public std::runtime_error {
	public:
		close_file_error() : std::runtime_error("Error in closing file.") {}
	};

	// Ошибка записи информации в файл
	class write_file_error : public std::runtime_error {
	public:
		write_file_error() : std::runtime_error("Error in writing file content.") {}
	};

	// Ошибка чтения информации из файла
	class read_file_error : public std::runtime_error {
	public:
		read_file_error() : std::runtime_error("Error in reading file content.") {}
	};

	// Другие операции, работы с файлами
	class file_operation_error : public std::exception {
	public:
		file_operation_error() : std::exception("Error in operation with file.") {}
	};

	// Обработка ошибок

	void HandleOpenFileError();
	void HandleCloseFileError();
	void HandleWriteFileError();
	void HandleReadFileError();
	void HandleFileOperationError();
}

#endif // !ERRORSHANDLERFILEOPERATIONS_H