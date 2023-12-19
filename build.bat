@echo off

rem --- Unpack Arguments ------------------------------------------------------
for %%a in (%*) do set "%%a=1"

rem --- Settings --------------------------------------------------------------
set compiler=cl
set cl_flags=/Zi /nologo /FC /I..\code\ /I..\local\
set gfx=/DOS_FEATURE_GFX=1
set net=/DOS_FEATURE_NET=1
set console=/DBUILD_CONSOLE_OUTPUT=1
set telemetry=/DBUILD_TELEMETRY=1

rem --- Prep Build Directory --------------------------------------------------
if not exist build mkdir build
xcopy /y /q /s /e /i data build

rem --- Prep Local Directory --------------------------------------------------
if not exist local mkdir local

rem --- Build & Run Metaprogram -----------------------------------------------
pushd build
%compiler% %cl_flags% %console% ..\code\metagen\metagen_main.c /link /out:metagen.exe
metagen.exe
popd

rem --- Build Everything ------------------------------------------------------
pushd build
if "%app_template_basic%"=="1"  %compiler% %cl_flags% %gfx% %net%                               ..\code\app_templates\app_template_basic_main.c ..\code\render\d3d11\render_d3d11_main.cpp ..\code\font_provider\dwrite\font_provider_dwrite_main.cpp                    /link /out:app_template_basic.exe
if "%g0%"=="1"                  %compiler% %cl_flags% %gfx% %telemetry%                         ..\code\g0\g0_main.c ..\code\render\d3d11\render_d3d11_main.cpp ..\code\font_provider\dwrite\font_provider_dwrite_main.cpp                                               /link /out:g0.exe
if "%hmb_demo%"=="1"            %compiler% %cl_flags% %gfx%                                     ..\code\hmb_demo\hmb_demo.c ..\code\render\d3d11\render_d3d11_main.cpp ..\code\font_provider\dwrite\font_provider_dwrite_main.cpp                                        /link /out:hmb_demo.exe
if "%mdesk_scratch%"=="1"       %compiler% %cl_flags%                                           ..\code\scratch\mdesk_scratch.c                                                                                                                                          /link /out:mdesk_scratch.exe
if "%net_scratch%"=="1"         %compiler% %cl_flags% %console% %net%                           ..\code\scratch\net_scratch.c                                                                                                                                            /link /out:net_scratch.exe
if "%render_scratch%"=="1"      %compiler% %cl_flags% %gfx%                                     ..\code\scratch\render_scratch.c ..\code\render\d3d11\render_d3d11_main.cpp ..\code\font_provider\dwrite\font_provider_dwrite_main.cpp                                   /link /out:render_scratch.exe
if "%human_profiler%"=="1"      %compiler% %cl_flags% %gfx% /I..\code\human_profiler\thirdparty ..\code\human_profiler\human_profiler_main.c ..\code\render\d3d11\render_d3d11_main.cpp ..\code\font_provider\dwrite\font_provider_dwrite_main.cpp                       /link /out:human_profiler.exe
if "%workbench%"=="1"           %compiler% %cl_flags% %gfx% %net%                               ..\code\workbench\workbench_main.c ..\code\render\d3d11\render_d3d11_main.cpp ..\code\font_provider\dwrite\font_provider_dwrite_main.cpp                                 /link /out:workbench.exe
if "%subcheck%"=="1"            %compiler% %cl_flags% %console%                                 ..\code\subcheck\subcheck_main.c                                                                                                                                         /link /out:subcheck.exe
if "%subgrant%"=="1"            go build                                                        ../code/subgrant/subgrant.go
if "%tests%"=="1"               %compiler% %cl_flags% %console%                                 ..\code\tests\tests_main.c                                                                                                                                               /link /out:tests.exe
popd

rem --- Unset Build Flags -----------------------------------------------------
for %%a in (%*) do set "%%a=0"
