@echo off

if not exist build mkdir build

xcopy /y /q /s /e data\ build\data\

pushd build
cl /Zi /nologo /FC /I..\code\ ..\code\codegen\codegen.c
popd

pushd code
..\build\codegen.exe os/ render/ render/d3d11/ ui/ g0/ draw/
popd

pushd build
cl /Zi /nologo /FC /I..\code\ /DBUILD_CORE_LINK_MODE=BUILD_LINK_MODE_SOURCE /DBUILD_CONSOLE_OUTPUT=0 ..\code\app_templates\app_template_basic_main.c ..\code\render\d3d11\render_d3d11.cpp ..\code\font_provider\dwrite\font_provider_dwrite.cpp /link user32.lib gdi32.lib dwmapi.lib UxTheme.lib winmm.lib Advapi32.lib shell32.lib ole32.lib /out:app_template_basic.exe
popd
