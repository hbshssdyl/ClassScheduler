; -- 应用基本信息（需要修改）--
#define MyAppName "天明书院"
#define MyAppVersion "v1.0.1"
#define MyAppPublisher "Dylandu"
#define MyAppURL "https://www.example.com"
#define MyAppExeName "appClassScheduler.exe" ; 必须与你的CMake目标名一致！
#define MyAppIconName "app_icon.ico"

; -- 自动路径计算（无需修改）--
#define ProjectRoot ExtractFileDir(ExtractFileDir(SourcePath))
#define BuildDir ProjectRoot + "\build"
#define OutputDir ProjectRoot + "\package\output"

; -- 智能检测构建目录 --
; 直接使用条件逻辑检测构建目录
#if DirExists(BuildDir + "\Desktop_Qt_6_9_1_MinGW_64_bit-Release")
  #define BuildPath BuildDir + "\Desktop_Qt_6_9_1_MinGW_64_bit-Release"
  #define FoundBuildDir true
#elif DirExists(BuildDir + "\Desktop_Qt_6_9_1_MinGW_64_bit-Debug")
  #define BuildPath BuildDir + "\Desktop_Qt_6_9_1_MinGW_64_bit-Debug"
  #define FoundBuildDir true
#elif DirExists(BuildDir + "\Desktop_Qt_6_9_1_MSVC2019_64bit-Release")
  #define BuildPath BuildDir + "\Desktop_Qt_6_9_1_MSVC2019_64bit-Release"
  #define FoundBuildDir true
#elif DirExists(BuildDir + "\Desktop_Qt_6_9_1_MSVC2019_64bit-Debug")
  #define BuildPath BuildDir + "\Desktop_Qt_6_9_1_MSVC2019_64bit-Debug"
  #define FoundBuildDir true
#elif DirExists(BuildDir + "\Release")
  #define BuildPath BuildDir + "\Release"
  #define FoundBuildDir true
#elif DirExists(BuildDir + "\Debug")
  #define BuildPath BuildDir + "\Debug"
  #define FoundBuildDir true
#elif DirExists(BuildDir + "\MinGW_64-bit-Release")
  #define BuildPath BuildDir + "\MinGW_64-bit-Release"
  #define FoundBuildDir true
#elif DirExists(BuildDir + "\MinGW_64-bit-Debug")
  #define BuildPath BuildDir + "\MinGW_64-bit-Debug"
  #define FoundBuildDir true
#elif DirExists(BuildDir)
  #define BuildPath BuildDir
  #define FoundBuildDir true
#else
  #pragma error "未找到构建目录！请先在Qt Creator中编译项目。"
#endif

; -- 资源文件路径（可选）--
#define AssetsDir ProjectRoot + "\assets"
#define LicenseFile AssetsDir + "\license.txt"
#define AppIcon AssetsDir + "\icons\app_icon.ico"

; -- 脚本开始 --
[Setup]
AppId={{5A3C9F1B-8D2E-4A7C-B6F3-9E8D7C6B5A4F}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir={#OutputDir}
OutputBaseFileName={#MyAppName}_Setup_{#MyAppVersion}
Compression=lzma
SolidCompression=yes
SetupIconFile={#AppIcon}
PrivilegesRequired=admin
CloseApplications=yes

[Languages]
Name: "chinesesimplified"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
; 默认勾选桌面图标
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]

; ----------------------------
; 主程序
; ----------------------------
Source: "{#BuildPath}\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion

; ----------------------------
; 所有依赖DLL
; ----------------------------
Source: "{#BuildPath}\*.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs

; ----------------------------
; 平台插件（Qt6必需）
; ----------------------------
Source: "{#BuildPath}\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs

; ----------------------------
; Qt模块插件（可选，按需添加）
; ----------------------------
#if DirExists(BuildPath + "\styles")
Source: "{#BuildPath}\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs
#endif

#if DirExists(BuildPath + "\imageformats")  
Source: "{#BuildPath}\styles\*"; DestDir: "{app}\styles"; Flags: ignoreversion recursesubdirs
#endif

#if DirExists(BuildPath + "\translations")
Source: "{#BuildPath}\translations\*"; DestDir: "{app}\translations"; Flags: ignoreversion recursesubdirs
#endif

; ----------------------------
; QML模块（关键）
; ----------------------------

; ----------------------------
; ClassScheduler 模块的 qmldir 文件（保持目录结构，如果存在的话）
; ----------------------------
Source: "{#BuildPath}\ClassScheduler\qmldir"; DestDir: "{app}\ClassScheduler"; Flags: ignoreversion skipifsourcedoesntexist
Source: "{#BuildPath}\ClassScheduler\*\qmldir"; DestDir: "{app}\ClassScheduler"; Flags: ignoreversion recursesubdirs createallsubdirs skipifsourcedoesntexist
Source: "{#BuildPath}\ClassScheduler\*\*\qmldir"; DestDir: "{app}\ClassScheduler"; Flags: ignoreversion recursesubdirs createallsubdirs skipifsourcedoesntexist
Source: "{#BuildPath}\ClassScheduler\*\*\*\qmldir"; DestDir: "{app}\ClassScheduler"; Flags: ignoreversion recursesubdirs createallsubdirs skipifsourcedoesntexist

; ----------------------------
; windeployqt 部署的 QML（非源码，Qt 自带库的 QML 文件） 
; ----------------------------
Source: "{#BuildPath}\qml\*"; DestDir: "{app}\qml"; Flags: ignoreversion recursesubdirs


; 程序图标文件
Source: "{#AssetsDir}\icons\app_icon.ico"; DestDir: "{app}"; Flags: ignoreversion

; ----------------------------
; License
; ----------------------------
Source: "{#LicenseFile}"; DestDir: "{app}"; Flags: ignoreversion; Check: FileExists('{#LicenseFile}')

[Icons]
; 开始菜单
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{app}\{#MyAppIconName}"
; 桌面快捷方式（默认勾选）
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon; IconFilename: "{app}\{#MyAppIconName}"
; 卸载
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
