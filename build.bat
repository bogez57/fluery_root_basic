@echo off

rem --- Unpack Arguments ------------------------------------------------------
for %%a in (%*) do set "%%a=1"

rem --- Settings --------------------------------------------------------------
set cl_flags=/Zi /nologo /FC /I..\code\
set console=/DBUILD_CONSOLE_OUTPUT=1

rem --- Prep Build Directory --------------------------------------------------
if not exist build mkdir build
xcopy /y /q /s /e data build

rem --- Build Metaprogram -----------------------------------------------------
pushd build
cl %cl_flags% ..\code\codegen\codegen.c
popd

rem --- Run Metaprogram -------------------------------------------------------
pushd code
..\build\codegen.exe os/ render/ render/d3d11/ ui/ g0/ draw/
popd

rem --- Build Everything ------------------------------------------------------
pushd build
if "%app_template_basic%"=="1" cl %cl_flags% ..\code\app_templates\app_template_basic_main.c ..\code\render\d3d11\render_d3d11_main.cpp ..\code\font_provider\dwrite\font_provider_dwrite_main.cpp /link /out:app_template_basic.exe
if "%g0%"=="1"                 cl %cl_flags% ..\code\g0\g0_main.c ..\code\render\d3d11\render_d3d11_main.cpp ..\code\font_provider\dwrite\font_provider_dwrite_main.cpp /link /out:g0.exe
if "%hmb_demo%"=="1"           cl %cl_flags% ..\code\hmb_demo\hmb_demo.c ..\code\render\d3d11\render_d3d11_main.cpp ..\code\font_provider\dwrite\font_provider_dwrite_main.cpp /link /out:hmb_demo.exe
if "%net_scratch%"=="1"        cl %cl_flags% ..\code\scratch\net_scratch.c /link /out:net_scratch.exe
if "%render_scratch%"=="1"     cl %cl_flags% ..\code\scratch\render_scratch.c ..\code\render\d3d11\render_d3d11_main.cpp ..\code\font_provider\dwrite\font_provider_dwrite_main.cpp /link /out:render_scratch.exe
if "%human_profiler%"=="1"     cl %cl_flags% /I..\code\human_profiler\thirdparty ..\code\human_profiler\human_profiler_main.c ..\code\render\d3d11\render_d3d11_main.cpp ..\code\font_provider\dwrite\font_provider_dwrite_main.cpp /link /out:human_profiler.exe
if "%workbench%"=="1"          cl %cl_flags% ..\code\workbench\workbench_main.c ..\code\render\d3d11\render_d3d11_main.cpp ..\code\font_provider\dwrite\font_provider_dwrite_main.cpp /link /out:workbench.exe
if "%subcheck%"=="1"           cl %cl_flags% %console% ..\code\subcheck\subcheck_main.c /link /out:subcheck.exe
if "%subgrant%"=="1"           go build ../code/subgrant/subgrant.go
popd

rem --- Unset Build Flags -----------------------------------------------------
for %%a in (%*) do set "%%a=0"
