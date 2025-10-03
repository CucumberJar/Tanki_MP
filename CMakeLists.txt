cmake_minimum_required(VERSION 3.22)  # 3.30 нет в релизе, лучше 3.22+
project(untitled5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/QT/6.8.1/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)
include_directories(
        src
        src/entities
        src/main
        src/main/ui
        src/MP
        src/map
)

add_executable(untitled5
        src/map/Base.cpp
        src/map/Stone.cpp
        src/main/ui/ConnectionWindow.cpp
        src/main/main.cpp
        src/tank/entities/Tank.cpp
        src/main/Game.cpp
        src/map/Wall.cpp
        src/tank/entities/Turret.cpp
        src/tank/entities/Bullet.cpp
        src/map/Tile.cpp
        src/main/TankClient.cpp
        src/main/GameLoad.cpp
        src/main/GameView.cpp
        src/main/GameScene.cpp

)
target_link_libraries(untitled5 Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)

