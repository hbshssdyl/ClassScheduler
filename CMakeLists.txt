cmake_minimum_required(VERSION 3.16)

project(ClassScheduler VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2 QuickLayouts)
find_package(Qt6 REQUIRED COMPONENTS Sql)
find_package(Qt6 REQUIRED COMPONENTS Core Charts)
find_package(Qt6 REQUIRED COMPONENTS Concurrent)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appClassScheduler
    main.cpp
)

qt_add_qml_module(appClassScheduler
    URI ClassScheduler
    VERSION 1.0

    QML_FILES Main.qml
    QML_FILES Qml/WelcomePage/WelcomePage.qml
    QML_FILES Qml/SearchClassInfo/SearchClassInfo.qml
    QML_FILES Qml/SearchClassInfo/SearchClassInfoDelegate.qml
    QML_FILES Qml/ScheduleClass/ScheduleClass.qml
    QML_FILES Qml/TeacherEvaluation/TeacherEvaluation.qml
    QML_FILES Qml/SearchStudentInfo/SearchStudentInfo.qml
    QML_FILES Qml/ActionItems/ActionItems.qml
    QML_FILES Qml/ActionItems/ActionItemDelegate.qml
    QML_FILES Qml/OperateArea/OperateArea.qml
    QML_FILES Qml/BasicComponent/MButton.qml
    QML_FILES Qml/LoginView/LoginView.qml
    QML_FILES Qml/FileView/FileView.qml
    QML_FILES Qml/FileView/FileUploadView.qml
    QML_FILES Qml/FileView/FileProcessingView.qml

    QML_FILES Qml/JSUtils/ColorUtils.js

    SOURCES Controller/Controller.h
    SOURCES Controller/Controller.cpp
    SOURCES Utils/ControllerUtils.h
    SOURCES Utils/ControllerUtils.cpp
    SOURCES Utils/DataUtils.h
    SOURCES Controller/SearchClassInfoController.cpp
    SOURCES Controller/SearchClassInfoController.h
    SOURCES Managers/DataManager.h Managers/DataManager.cpp
    QML_FILES Qml/BasicComponent/SearchBar.qml
    SOURCES Controller/SearchTeacherInfoController.cpp Controller/SearchTeacherInfoController.h
    QML_FILES Qml/SearchTeacherInfo/SearchTeacherInfo.qml Qml/SearchTeacherInfo/SearchTeacherInfoDelegate.qml
    QML_FILES Qml/JSUtils/MainUtils.js
    SOURCES Controller/ScheduleClassController.h Controller/ScheduleClassController.cpp
    SOURCES Controller/SearchStudentInfoController.cpp
    SOURCES Controller/SearchStudentInfoController.h
    QML_FILES Qml/SearchStudentInfo/SearchStudentInfoDelegate.qml

    RESOURCES Resource/Avatar-self.jpg
    RESOURCES Resource/Banner-sky.jpg
    RESOURCES Resource/Banner-mountain.jpg
    RESOURCES Resource/QA.svg
    RESOURCES Resource/folder.svg
    RESOURCES Resource/phone.svg
    RESOURCES Resource/study.svg
    RESOURCES Resource/tag.svg
    RESOURCES Resource/user.svg
    QML_FILES Qml/UserInfo/UserInfo.qml
    SOURCES Managers/UserManager.h Managers/UserManager.cpp
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
    PRIVATE Qt6::Quick Qt6::QuickControls2
)

target_link_libraries(appClassScheduler
    PRIVATE Qt6::Quick
)
target_link_libraries(appClassScheduler
    PRIVATE Qt6::Sql
)
target_link_libraries(appClassScheduler
    PRIVATE Qt6::Core Qt6::Charts
)
target_link_libraries(appClassScheduler
    PRIVATE Qt6::Concurrent
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







