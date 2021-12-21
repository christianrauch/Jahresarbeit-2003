cmake_minimum_required(VERSION 3.10)
project(Jahresarbeit-2003)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(ASAN "build with AddressSanitizer")

if (ASAN)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} \
        -fsanitize=address \
        -fsanitize=bool \
        -fsanitize=bounds \
        -fsanitize=enum \
        -fsanitize=float-cast-overflow \
        -fsanitize=float-divide-by-zero \
        -fsanitize=nonnull-attribute \
        -fsanitize=returns-nonnull-attribute \
        -fsanitize=signed-integer-overflow \
        -fsanitize=undefined \
        -fsanitize=vla-bound \
        -fno-sanitize=alignment \
        -fsanitize=leak \
        -fsanitize=object-size \
    ")
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

add_executable(ja2003 main.c)
target_link_libraries(ja2003 ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} m)

if((CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT OR NOT CMAKE_INSTALL_PREFIX) AND UNIX)
    set(CMAKE_INSTALL_PREFIX "/opt/${PROJECT_NAME}")
endif()

install(TARGETS ja2003 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY grafik fonts DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES hilfe.txt highscore.txt DESTINATION ${CMAKE_INSTALL_PREFIX})
