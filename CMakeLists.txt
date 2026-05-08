cmake_minimum_required(VERSION 3.16)
project(unknownMusicPlayer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
#pkg_check_modules(PC_CHAFA REQUIRED chafa)
pkg_check_modules(PC_GLIB REQUIRED glib-2.0)
pkg_check_modules(PC_TAGLIB REQUIRED taglib)

add_executable(ump
    src/main.cpp
    src/ui/common.cpp
    src/ui/render.cpp
    src/keyboard/keyboard.cpp
    src/songlist/song.cpp
    src/songlist/list.cpp
    src/event/events.cpp
    src/event/globalVar.cpp
    src/logger/log.cpp
    src/cover/loadCover.cpp
    src/cover/showCover.cpp
)

target_include_directories(ump PRIVATE
    include
#    ${PC_CHAFA_INCLUDE_DIRS}
    ${PC_GLIB_INCLUDE_DIRS}
    ${PC_TAGLIB_INCLUDE_DIRS}
)

target_compile_options(ump PRIVATE -Wall)

target_link_directories(ump PRIVATE
#    ${PC_CHAFA_LIBRARY_DIRS}
    ${PC_GLIB_LIBRARY_DIRS}
    ${PC_TAGLIB_LIBRARY_DIRS}
)

target_link_libraries(ump PRIVATE
#    ${PC_CHAFA_LIBRARIES}
    ${PC_GLIB_LIBRARIES}
    ${PC_TAGLIB_LIBRARIES}
    dl
    pthread
    m
    tag
)
