cmake_minimum_required(VERSION 3.5)

project(CrackerJack VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui)

set(PROJECT_SOURCES
        ./source/AttackMethode.hpp
        ./source/AutoHuntConfigurator.cpp
        ./source/AutoHuntConfigurator.h
        ./source/AutoHuntConfigurator.ui
        ./source/AutoHuntData.cpp
        ./source/AutoHuntData.hpp
        ./source/AutoHunting.cpp
        ./source/AutoHunting.h
        ./source/AutoHuntingConfigurator.ui
        ./source/BattleList.hpp
        ./source/Calibrator.cpp
        ./source/Calibrator.h
        ./source/CJ_Image.cpp
        ./source/CJ_Image.h
        ./source/ClickDetector.hpp
        ./source/Equipment.cpp
        ./source/Equipment.h
        ./source/GameActivityChecker.cpp
        ./source/GameActivityChecker.h
        ./source/GameConnecter.cpp
        ./source/GameConnecter.h
        ./source/GameProcessData.hpp
        ./source/GameWindow.hpp
        ./source/Item.cpp
        ./source/Item.h
        ./source/JsonClass.hpp
        ./source/JsonParser.cpp
        ./source/JsonParser.h
        ./source/Key.h
        ./source/Logger.hpp
        ./source/main.cpp
        ./source/MainMenu.cpp
        ./source/MainMenu.h
        ./source/MainMenu.ui
        ./source/MinimapAnalyzer.cpp
        ./source/MinimapAnalyzer.h
        ./source/NewProfileConfiguartor.cpp
        ./source/NewProfileConfiguartor.h
        ./source/NewProfileConfiguartor.ui
        ./source/PathResource.h
        ./source/PlayerPosition.hpp
        ./source/Point3D.hpp
        ./source/Potion.hpp
        ./source/Profession.hpp
        ./source/Profile.cpp
        ./source/Profile.h
        ./source/RestorationMethode.hpp
        ./source/RGBstruct.cpp
        ./source/RGBstruct.h
        ./source/Route.cpp
        ./source/Route.h
        ./source/RouteCreator.cpp
        ./source/RouteCreator.h
        ./source/RouteCreator.ui
        ./source/RoutePoint.cpp
        ./source/RoutePoint.h
        ./source/ScreenAnalyzer.cpp
        ./source/ScreenAnalyzer.h
        ./source/ScreenSaver.cpp
        ./source/ScreenSaver.h
        ./source/SelectProfileWindow.cpp
        ./source/SelectProfileWindow.h
        ./source/SelectProfileWindow.ui
        ./source/SetNameWidnow.ui
        ./source/Settings.hpp
        ./source/Spell.h
        ./source/Timers.hpp
        ./source/Utilities.hpp
        ./source/VariablesClass.hpp
        ./source/VitalityAnalyzer.cpp
        ./source/VitalityAnalyzer.h
        ./source/Vitallity.hpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CrackerJack
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CrackerJack APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CrackerJack SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CrackerJack
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(CrackerJack PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Gui)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CrackerJack)
endif()
set_target_properties(CrackerJack PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CrackerJack
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CrackerJack)
endif()
