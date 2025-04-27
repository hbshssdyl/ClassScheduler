cmake_minimum_required(VERSION 3.16)

project(ClassScheduler VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Sql)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appClassScheduler
    main.cpp
)

qt_add_qml_module(appClassScheduler
    URI ClassScheduler
    VERSION 1.0

    QML_FILES Main.qml
    QML_FILES Qml/WelcomePage/WelcomePage.qml
    QML_FILES Qml/SearchTeacherInfo/SearchTeacherInfo.qml
    QML_FILES Qml/ScheduleClass/ScheduleClass.qml
    QML_FILES Qml/CalcOneToOneMoney/CalcOneToOneMoney.qml
    QML_FILES Qml/CalcClassMoney/CalcClassMoney.qml
    QML_FILES Qml/ActionItems/ActionItems.qml
    QML_FILES Qml/ActionItems/ActionItemDelegate.qml
    QML_FILES Qml/OperateArea/OperateArea.qml
    QML_FILES Qml/BasicComponent/MButton.qml
    QML_FILES Qml/JSUtils/ColorUtils.js
    QML_FILES Qml/SearchTeacherInfo/SearchTeacherInfoDelegate.qml
    QML_FILES Qml/LoginView/LoginView.qml

    SOURCES Controller/Controller.h
    SOURCES Controller/Controller.cpp
    SOURCES Utils/ControllerUtils.h
    SOURCES Utils/ControllerUtils.cpp
    SOURCES Utils/DataUtils.h
    SOURCES Controller/SearchTeacherInfoController.cpp
    SOURCES Controller/SearchTeacherInfoController.h
    SOURCES Managers/DBManager.h Managers/DBManager.cpp
    QML_FILES Qml/FileUploadView/FileProcessingView.qml Qml/FileUploadView/FileUploadView.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appClassScheduler PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appClassScheduler
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appClassScheduler
    PRIVATE Qt6::Quick
)
target_link_libraries(appClassScheduler
    PRIVATE Qt6::Sql
)

include(GNUInstallDirs)
install(TARGETS appClassScheduler
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# QXlsx
add_subdirectory(QXlsx)
target_link_libraries(appClassScheduler PRIVATE QXlsx::QXlsx)

# # OpenXLSX
# add_subdirectory(OpenXLSX)
# target_link_libraries(appClassScheduler PRIVATE OpenXLSX::OpenXLSX)







