cmake_minimum_required(VERSION 3.16)

project(ClassScheduler VERSION 0.1 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};C:/PersonalDev/vcpkg/installed/x64-mingw-static")
set(CMAKE_TOOLCHAIN_FILE "C:/PersonalDev/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2 QuickLayouts Sql Core Charts Concurrent AxContainer)

find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

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
    SOURCES Managers/NetworkManager.h Managers/NetworkManager.cpp
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

target_link_libraries(appClassScheduler PRIVATE
    Qt6::Quick Qt6::QuickControls2 Qt6::QuickLayouts Qt6::Sql Qt6::Core Qt6::Charts Qt6::Concurrent Qt6::AxContainer
    CURL::libcurl
    nlohmann_json::nlohmann_json
    QXlsx::QXlsx
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
