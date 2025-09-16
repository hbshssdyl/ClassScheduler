; -- 应用基本信息（需要修改）--
#define MyAppName "天明书院"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "Dylandu"
#define MyAppURL "https://www.baidu.com"
#define MyAppExeName "appClassScheduler.exe" ; 必须与你的CMake目标名一致！

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
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "chinesesimplified"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; 主执行文件
Source: "{#BuildPath}\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
; 递归包含构建目录中的所有文件和子目录
Source: "{#BuildPath}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; 许可文件（可选）
Source: "{#LicenseFile}"; DestDir: "{app}"; Flags: ignoreversion; Check: FileExists('{#LicenseFile}')

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Code]
// 安装前检查
function InitializeSetup(): Boolean;
var
  BuildPath: string;
  Msg: string;
begin
  BuildPath := ExpandConstant('{#BuildPath}');
  Msg := '构建目录信息:' + #13#10 +
         '检测到的路径: ' + BuildPath + #13#10 +
         '项目根目录: ' + ExpandConstant('{#ProjectRoot}') + #13#10#13#10;

  if not DirExists(BuildPath) then
  begin
    Msg := Msg + '❌ 错误：构建目录不存在！' + #13#10 +
           '请先编译项目并运行windeployqt。';
    MsgBox(Msg, mbError, MB_OK);
    Result := False;
    Exit;
  end;

  if not FileExists(BuildPath + '\{#MyAppExeName}') then
  begin
    Msg := Msg + '❌ 错误：未找到主程序文件！' + #13#10 +
           '请确认：' + #13#10 +
           '1. 项目已成功编译' + #13#10 +
           '2. 已运行windeployqt收集依赖' + #13#10 +
           '3. MyAppExeName变量设置正确';
    MsgBox(Msg, mbError, MB_OK);
    Result := False;
    Exit;
  end;

  Msg := Msg + '✅ 构建目录检测成功！';
  MsgBox(Msg, mbInformation, MB_OK);
  Result := True;
end;