#ifndef ERRORSHANDLERFILEOPERATIONS_H
#define ERRORSHANDLERFILEOPERATIONS_H

#include <exception>
#include <stdexcept>

namespace file {

	// ������ �������� �����
	class open_file_error : public std::runtime_error {
	public:
		open_file_error() : std::runtime_error("Error in opening file.") {}
	};

	// ������ �������� �����
	class close_file_error : public std::runtime_error {
	public:
		close_file_error() : std::runtime_error("Error in closing file.") {}
	};

	// ������ ������ ���������� � ����
	class write_file_error : public std::runtime_error {
	public:
		write_file_error() : std::runtime_error("Error in writing file content.") {}
	};

	// ������ ������ ���������� �� �����
	class read_file_error : public std::runtime_error {
	public:
		read_file_error() : std::runtime_error("Error in reading file content.") {}
	};

	// ������ ��������, ������ � �������
	class file_operation_error : public std::exception {
	public:
		file_operation_error() : std::exception("Error in operation with file.") {}
	};

	// ��������� ������

	void HandleOpenFileError();
	void HandleCloseFileError();
	void HandleWriteFileError();
	void HandleReadFileError();
	void HandleFileOperationError();
}

#endif // !ERRORSHANDLERFILEOPERATIONS_H