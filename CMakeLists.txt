cmake_minimum_required(VERSION 3.5)

project(untitled9 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        widget.cpp
        widget.h
        widget.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(untitled9
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        chatroomwindow.h
        clientwidget.h
        CompetitionPage.h
        config.h
        dragpushbutton.h
        FailureDialog.h
        FreeModePage.h
        HelpWindow.h
        InputModePage.h
        LevelOnepa.h
        logindialog.h
        musicplayer.h
        PlayerRankingPage.h
        PracticePage.h
        serverwidget.h
        SettingsPage.h
        SuccessDialog.h
        sudokugame.h
        UserRankItem.h
        chatroomwindow.cpp
        clientwidget.cpp
        CompetitionPage.cpp
        dragpushbutton.cpp
        FailureDialog.cpp
        HelpWindow.cpp
        InputModePage.cpp
        LevelOnepa.cpp
        logindialog.cpp
        musicplayer.cpp
        PlayerRankingPage.cpp
        PracticePage.cpp
        serverwidget.cpp
        SettingsPage.cpp
        SuccessDialog.cpp
        sudokugame.cpp
        UserRankItem.cpp
        FreeModePage.cpp
        images.qrc
        ExitDialog.h
        ExitDialog.cpp
        CustomInputDialog.h
        CustomInputDialog.cpp
        CustomHintDialog.h
        CustomHintDialog.cpp
        MsgBox.h
        MsgBox.cpp
        inputdialog.h
        inputdialog.cpp
        custommessagebox.h
        custommessagebox.cpp
        UsernameFind.h
        UsernameFind.cpp
        PasswordFind.h
        PasswordFind.cpp
        playerdatawindow.h
        playerdatawindow.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET untitled9 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(untitled9 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(untitled9
            ${PROJECT_SOURCES}
        )
    endif()
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Multimedia)

find_package(Qt6 COMPONENTS Multimedia REQUIRED)

find_package(Qt6 COMPONENTS Network REQUIRED)


target_link_libraries(untitled9 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.untitled9)
endif()
set_target_properties(untitled9 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS untitled9
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(untitled9)
endif()



target_link_libraries(untitled9 PRIVATE Qt${QT_VERSION_MAJOR}::Network)


target_link_libraries(untitled9 PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia)
