cmake_minimum_required(VERSION 3.13)

project(cnf-solver
    LANGUAGES CXX)


add_compile_definitions(UNICODE _UNICODE WIN32_LEAN_AND_MEAN)

if(MSVC)
    add_compile_options(/utf-8 /W4)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

add_executable(${PROJECT_NAME} WIN32 src/main.cpp)

add_library(${PROJECT_NAME}_objs OBJECT
    src/Algorithms/GeneticAlgorithm.cpp
    src/Algorithms/SimulatedAnnealing.cpp
    src/Algorithms/BeeHive.cpp
    
    src/Core/CNF.cpp
    src/Core/Candidate.cpp
    src/Core/Candidates.cpp

    src/Utils/Utils.cpp
)

add_library(${PROJECT_NAME}_wnd_objs OBJECT
    src/UI/MainWindow.cpp
    src/UI/GAWindow.cpp
    src/UI/SAWindow.cpp
    src/UI/GraphWindow.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME}_objs PRIVATE include)
target_include_directories(${PROJECT_NAME}_wnd_objs PRIVATE include)

if(WIN32)
    set(WIN32_LIBS
        kernel32.lib
        user32.lib
        gdi32.lib
        winspool.lib
        shell32.lib
        ole32.lib
        oleaut32.lib
        uuid.lib
        comdlg32.lib
        advapi32.lib
        gdiplus.lib
        comctl32.lib
    )

    target_link_libraries(${PROJECT_NAME}_wnd_objs PRIVATE ${WIN32_LIBS})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${PROJECT_NAME}_objs
    ${PROJECT_NAME}_wnd_objs
)
