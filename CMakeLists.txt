cmake_minimum_required(VERSION 3.10)

# Название проекта
project(laba1)

# Устанавливаем флаги для покрытия (если нужно)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

# Указываем исходные файлы проекта
set(SOURCES
    src/main.cpp
    src/DynamicArray.cpp
    src/FBT.cpp
    src/ForwardList.cpp
    src/hash.cpp
    src/library.h
    src/LinkedList.cpp
    src/Queue.cpp
    src/Stack.cpp
)

# Создаем исполнимый файл для проекта
add_executable(laba1 ${SOURCES})

# Указываем исходные файлы для тестов
set(TEST_SOURCES
    tests/test_DynamicArray.cpp
    tests/test_ForwardList.cpp
    tests/test_LinkedList.cpp
    tests/test_Queue.cpp
    tests/test_Stack.cpp
    tests/test_FBT.cpp
    tests/test_hash.cpp
)

# Создаем исполнимый файл для тестов
add_executable(test_all ${TEST_SOURCES})

# Линковка с GoogleTest
find_package(GTest REQUIRED)
target_link_libraries(test_all GTest::GTest GTest::Main)

# Регистрируем тесты с CTest
enable_testing()
add_test(NAME run_all_tests COMMAND test_all)

# Запуск тестов после сборки
add_custom_command(TARGET test_all POST_BUILD
    COMMAND ctest
    COMMENT "Запуск тестов после сборки"
)

# Команды для сбора покрытия
add_custom_target(coverage
    COMMAND lcov --directory . --capture --output-file coverage.info
    COMMAND lcov --remove coverage.info '/usr/*' --output-file coverage_cleaned.info
    COMMAND genhtml coverage_cleaned.info --output-directory out
    COMMENT "Генерация отчета покрытия"
)