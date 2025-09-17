Qtcreator build 参数：
```
-DCMAKE_TOOLCHAIN_FILE=C:/PersonalDev/vcpkg/scripts/buildsystems/vcpkg.cmake
-DVCPKG_TARGET_TRIPLET=x64-mingw-static
```
Custom process step1:
```
C:\Qt\6.9.1\mingw_64\bin\windeployqt.exe
--compiler-runtime --release --no-translations "%{buildDir}/appClassScheduler.exe"
%{buildDir}
```
Custom process step2:
```
C:\Program Files (x86)\Inno Setup 6\ISCC.exe
"%{buildDir}/../../package/installer_script.iss"
%{buildDir}
```