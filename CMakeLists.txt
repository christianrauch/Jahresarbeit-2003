cmake_minimum_required(VERSION 2.8)
project(ja2003)

find_package(SDL REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_image REQUIRED)

include_directories(${SDL_INCLUDE_DIR} ${SDL_TTF_INCLUDE_DIRS} ${SDL_IMAGE_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${SDL_LIBRARY} ${SDL_TTF_LIBRARIES} ${SDL_IMAGE_LIBRARIES})
