#ifndef ERRORSHANDLERFILEOPERATIONS_H
#define ERRORSHANDLERFILEOPERATIONS_H

#include <exception>
#include <stdexcept>

namespace file {
	constexpr char error_msg_open_file[]		{ "Error in opening file." };
	constexpr char error_msg_�lose_file[]		{ "Error in closing file." };
	constexpr char error_msg_write_file[]		{ "Error in writing file content." };
	constexpr char error_msg_read_file[]		{ "Error in reading file content." };
	constexpr char error_msg_file_operation[]	{ "Error in operation with file." };
	constexpr char error_msg_stream_is_closed[]	{ "File stream is not opened." };

	//* ������ �������� �����
	struct ErrorOpenFile : public std::runtime_error {
		ErrorOpenFile() : std::runtime_error(error_msg_open_file) {}
	};

	// ������ �������� �����
	struct ErrorCloseFile : public std::runtime_error {
		ErrorCloseFile() : std::runtime_error(error_msg_�lose_file) {}
	};

	// ������ ������ ���������� � ����
	struct ErrorWriteFile : public std::runtime_error {
		ErrorWriteFile() : std::runtime_error(error_msg_write_file) {}
	};

	// ������ ������ ���������� �� �����
	struct ErrorReadFile : public std::runtime_error {
		ErrorReadFile() : std::runtime_error(error_msg_read_file) {}
	};

	// ������ ��������, ������ � �������
	struct ErrorFileOperation : public std::exception {
		ErrorFileOperation() : std::exception("Error in operation with file.") {}
	};
	
	/** Stream is not opened for operation. */
	struct ErrorFileStreamIsClosed : public std::runtime_error {
		ErrorFileStreamIsClosed() : std::runtime_error(error_msg_stream_is_closed) {}
	};



	// ��������� ������

	void HandleOpenFileError();
	void HandleCloseFileError();
	void HandleWriteFileError();
	void HandleReadFileError();
	void HandleFileOperationError();
}

#endif // !ERRORSHANDLERFILEOPERATIONS_H