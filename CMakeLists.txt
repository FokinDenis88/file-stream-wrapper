# CMakeList.txt : CMake project for file-stream-wrapper, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

# Can be different with target
project("file-stream-wrapper" VERSION 1.0)

set(SOURCES
    src/file-stream-wrapper-project.cpp
    )

set(INCLUDES
    include
    external/include
    )

set(HEADER_FILES_FILTER
    include/get-file-lines.hpp
    include/read-binary-file.hpp
    include/read-binary-file-thread.hpp
    include/read-formatted-data-table-csv.hpp
    include/read-object-in-vec-byte.hpp
    include/read-serialized-data-file.hpp
    include/read-serialized-data-file-thread.hpp
    include/write-binary-file.hpp
    include/write-binary-file-thread.hpp
    include/write-data-table-csv.hpp
    include/write-serialized-data-file.hpp
    include/write-serialized-data-file-thread.hpp
    )
set(DOC_FILTER
    doc/to-do.txt
    doc/copy-past-helper.txt
    )
set(DEPRECATED_BAD_HEADER_FILTER
    include/read-data-file.hpp
    include/read-data-file-thread.hpp
    include/read-text-file.hpp
    include/read-text-file-big.hpp
    include/read-text-file-big-thread.hpp
    include/read-text-file-thread.hpp
    include/write-data-file.hpp
    include/write-data-file-thread.hpp
    include/write-text-file.hpp
    include/write-text-file-big.hpp
    include/write-text-file-big-thread.hpp
    include/write-text-file-thread.hpp
    )
set(SUPPORT_HEADER_FILTER
    include/bom.hpp
    include/data-table.hpp
    include/divide-char-vec-to-lines.hpp
    include/errors-handler-file-operations.hpp
    include/file-constants.hpp
    include/file-operations.hpp
    include/file-service.hpp
    include/iserializable.hpp
    include/read-stream-state.hpp
    include/serialization.hpp
)
set(SUPPORT_SOURCE_FILTER
    src/divide-char-vec-to-lines.cpp
    src/errors-handler-file-operations.cpp
    src/file-operations.cpp
    src/iserializable.cpp
    src/serialization.cpp
)


# Add source to this project's executable.
add_executable(file-stream-wrapper ${SOURCES}
                ${HEADER_FILES_FILTER} ${DOC_FILTER} ${DEPRECATED_BAD_HEADER_FILTER}
                ${SUPPORT_HEADER_FILTER} ${SUPPORT_SOURCE_FILTER}
              )
# Include Directories
target_include_directories(file-stream-wrapper PUBLIC ${INCLUDES})


# Filters for Grouping files in project
source_group("Header Files" FILES ${HEADER_FILES_FILTER})
source_group("Doc" FILES ${DOC_FILTER})
source_group("Deprecated - Bad/Header" FILES ${DEPRECATED_BAD_HEADER_FILTER})
source_group("Support/Header" FILES ${SUPPORT_HEADER_FILTER})
source_group("Support/Source" FILES ${SUPPORT_SOURCE_FILTER})


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET file-stream-wrapper PROPERTY CXX_STANDARD 20)
  set_property(TARGET file-stream-wrapper PROPERTY C_STANDARD 17)
endif()
set_property(TARGET file-stream-wrapper PROPERTY SUBSYSTEM WINDOWS)





# Help Information
# TODO: Add tests and install targets if needed.


# Добавляет директории для дальнейшей обработки. В них нужно добавлять еще одни CMakeLists.txt файлы
# add_subdirectory (Hello)

#add_library (Hello hello.cxx)
#target_include_directories (Hello PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#add_executable (helloDemo demo.cxx demo_b.cxx)

#target_link_libraries (helloDemo LINK_PUBLIC Hello)
# Add directories in which the linker will look for libraries.
#link_directories([AFTER|BEFORE] directory1 [directory2 ...])
#target_link_directories(<name> PUBLIC directory)

#set(SOURCE_EXE main.cpp)   set(SOURCE_LIB foo.cpp)
#add_library(foo STATIC ${SOURCE_LIB})	# Создание статической библиотеки с именем foo
#add_executable(main [WIN32] ${SOURCE_EXE})	# Создает исполняемый файл с именем main
#target_link_libraries(main foo)
#include_directories(foo)			# Расположение заголовочных файлов
#file(GLOB <variable> [LIST_DIRECTORIES true|false] [RELATIVE <path>] [CONFIGURE_DEPENDS] [<globbing-expressions>...])
#target_compile_options(first-test PRIVATE -fexceptions)

#find_package(OpenMP)
#if (OPENMP_FOUND)
    #set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    #set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif()

#set_property(TARGET file-stream-wrapper PROPERTY SUBSYSTEM WINDOWS)
#CMAKE_CXX_FLAGS    /SUBSYSTEM:CONSOLE

#source_group("Header Files" FILES ${INCLUDES} REGULAR_EXPRESSION *)

#OpenMP
#find_package(OpenMP REQUIRED)
#target_link_libraries(file-stream-wrapper PRIVATE OpenMP::OpenMP_CXX)