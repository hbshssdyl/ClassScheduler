cmake_minimum_required(VERSION 3.16)
project(ClassScheduler
    VERSION 0.1
    LANGUAGES CXX
)

#============================================
# 基本配置
#============================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#============================================
# 依赖配置
#============================================
find_package(Qt6 REQUIRED COMPONENTS
    Quick QuickControls2 QuickLayouts Sql Core Charts Concurrent AxContainer)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)

# 调试信息
message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL libraries: ${OPENSSL_LIBRARIES}")
message(STATUS "CURL include dir: ${CURL_INCLUDE_DIRS}")
message(STATUS "CURL libraries: ${CURL_LIBRARIES}")

include_directories(
    ${CURL_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
)

qt_standard_project_setup(REQUIRES 6.8)

#============================================
# QXlsx处理
#============================================
add_subdirectory(QXlsx)

#============================================
# 可执行文件配置
#============================================
qt_add_executable(appClassScheduler
    main.cpp
)

qt_add_qml_module(appClassScheduler
    URI ClassScheduler
    VERSION 1.0
    # QML文件
    QML_FILES Main.qml
    QML_FILES Qml/WelcomePage/WelcomePage.qml
    QML_FILES Qml/SearchClassInfo/SearchClassInfo.qml
    QML_FILES Qml/SearchClassInfo/SearchClassInfoDelegate.qml
    QML_FILES Qml/ScheduleClass/ScheduleClass.qml
    QML_FILES Qml/TeacherEvaluation/TeacherEvaluation.qml
    QML_FILES Qml/SearchStudentInfo/SearchStudentInfo.qml
    QML_FILES Qml/SearchStudentInfo/SearchStudentInfoDelegate.qml
    QML_FILES Qml/ActionItems/ActionItems.qml
    QML_FILES Qml/ActionItems/ActionItemDelegate.qml
    QML_FILES Qml/OperateArea/OperateArea.qml
    QML_FILES Qml/LoginView/LoginView.qml
    QML_FILES Qml/FileView/FileView.qml
    QML_FILES Qml/FileView/FileUploadView.qml
    QML_FILES Qml/FileView/FileProcessingView.qml
    QML_FILES Qml/JSUtils/ColorUtils.js
    QML_FILES Qml/JSUtils/MainUtils.js
    QML_FILES Qml/BasicComponent/SearchBar.qml
    QML_FILES Qml/BasicComponent/DateSelector.qml
    QML_FILES Qml/BasicComponent/MButton.qml
    QML_FILES Qml/BasicComponent/MCheckButton.qml
    QML_FILES Qml/SearchTeacherInfo/SearchTeacherInfo.qml
    QML_FILES Qml/SearchTeacherInfo/SearchTeacherInfoDelegate.qml
    QML_FILES Qml/UserInfo/UserInfo.qml
    QML_FILES Qml/TaskManagerView/TaskManagerView.qml
    QML_FILES Qml/TaskAssistantView/TaskAssistantView.qml
    QML_FILES Qml/FeedbackApp/FeedbackApp.qml
    QML_FILES Qml/FeedbackCompany/FeedbackCompany.qml
    QML_FILES Qml/AccountView/AccountView.qml

    # 源文件
    SOURCES Utils/DataUtils.h
    SOURCES Utils/ControllerUtils.h
    SOURCES Utils/ControllerUtils.cpp
    SOURCES Utils/ManagerUtils.h
    SOURCES Utils/ManagerUtils.cpp

    SOURCES Controller/Controller.h
    SOURCES Controller/Controller.cpp
    SOURCES Controller/SearchClassInfoController.cpp
    SOURCES Controller/SearchClassInfoController.h
    SOURCES Controller/SearchTeacherInfoController.cpp
    SOURCES Controller/SearchTeacherInfoController.h
    SOURCES Controller/ScheduleClassController.h
    SOURCES Controller/ScheduleClassController.cpp
    SOURCES Controller/SearchStudentInfoController.cpp
    SOURCES Controller/SearchStudentInfoController.h
    SOURCES Controller/TaskController.h
    SOURCES Controller/TaskController.cpp
    SOURCES Controller/DatabaseController.cpp
    SOURCES Controller/DatabaseController.h
    SOURCES Controller/FeedbackController.h
    SOURCES Controller/FeedbackController.cpp
    SOURCES Controller/AccountViewController.h
    SOURCES Controller/AccountViewController.cpp

    SOURCES Managers/CoreFramework.h
    SOURCES Managers/CoreFramework.cpp
    SOURCES Managers/UserManager.h
    SOURCES Managers/UserManager.cpp
    SOURCES Managers/NetworkManager.h
    SOURCES Managers/NetworkManager.cpp
    SOURCES Managers/DataManager.h
    SOURCES Managers/DataManager.cpp
    SOURCES Managers/TaskManager.h
    SOURCES Managers/TaskManager.cpp
    SOURCES Managers/FeedbackManager.h
    SOURCES Managers/FeedbackManager.cpp
    SOURCES Managers/AccountManager.h
    SOURCES Managers/AccountManager.cpp
    SOURCES Managers/Logger.cpp
    SOURCES Managers/Logger.h

    # 资源文件
    RESOURCES Resource/Avatar-self.jpg
    RESOURCES Resource/Banner-sky.jpg
    RESOURCES Resource/Banner-mountain.jpg
    RESOURCES Resource/QA.svg
    RESOURCES Resource/folder.svg
    RESOURCES Resource/phone.svg
    RESOURCES Resource/study.svg
    RESOURCES Resource/tag.svg
    RESOURCES Resource/user.svg
    RESOURCES Resource/database.svg
    RESOURCES Resource/welcome.svg
    RESOURCES Resource/welcome.jpg
    RESOURCES Resource/tianming.jpg
    RESOURCES Resource/ok.svg
    RESOURCES Resource/notok.svg
    RESOURCES Resource/notification.svg
    RESOURCES Resource/task.svg
    RESOURCES Resource/manage.svg
    RESOURCES Resource/question.svg
    RESOURCES Resource/dianzan.svg
    RESOURCES Resource/phone.svg
    RESOURCES Resource/close.svg
    RESOURCES Resource/update.svg
)

#============================================
# 链接配置
#============================================
target_link_libraries(appClassScheduler PRIVATE
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::QuickLayouts
    Qt6::Sql
    Qt6::Core
    Qt6::Charts
    Qt6::Concurrent
    Qt6::AxContainer
    CURL::libcurl
    OpenSSL::SSL
    OpenSSL::Crypto
    ZLIB::ZLIB
    nlohmann_json::nlohmann_json
    QXlsx::QXlsx
    -lws2_32
    -static
)

#============================================
# 安装配置
#============================================
include(GNUInstallDirs)
install(TARGETS appClassScheduler
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
