cmake_minimum_required(VERSION 3.1)
project(spacecharts)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK2 REQUIRED gtk+-2.0)

include_directories (${GTK2_INCLUDE_DIRS})
link_directories (${GTK2_LIBRARY_DIRS})
add_definitions (${GTK2_CFLAGS_OTHER})

file(GLOB source_files
        "include/*.*"
        "src/*.*"
)

add_executable( spacecharts src/main.c ${source_files} )

install(TARGETS spacecharts RUNTIME DESTINATION bin)

