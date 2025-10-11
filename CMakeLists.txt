cmake_minimum_required(VERSION 3.10)
project(Jahresarbeit-2003)

option(ASAN "build with AddressSanitizer")

if (ASAN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
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

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)

add_executable(ja2003 main.c)
target_link_libraries(ja2003 PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image)

if((CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT OR NOT CMAKE_INSTALL_PREFIX) AND UNIX)
    set(CMAKE_INSTALL_PREFIX "/opt/${PROJECT_NAME}")
endif()

install(TARGETS ja2003 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY grafik fonts DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES hilfe.txt highscore.txt DESTINATION ${CMAKE_INSTALL_PREFIX})
