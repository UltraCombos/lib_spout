@REM ========================================
@REM 000_copy_files_from_SpoutSDK.bat
@REM 
@REM 2017/03/29 The78ester
@REM ========================================
@SETLOCAL ENABLEDELAYEDEXPANSION 

@PUSHD %~d0%~p0

@SET filecpy=XCOPY /Y /D /S
@SET INC_DIR=libs\SpoutSDK\include\
@SET LIB_DIR=libs\SpoutSDK\lib\

@IF EXIST "%INC_DIR%" (
	@RD /S /Q %INC_DIR%
)

@IF EXIST "%LIB_DIR%" (
	@RD /S /Q %LIB_DIR%
)

@%filecpy% %SPOUT_SDK_DIR%Binaries\*.lib %LIB_DIR%
@%filecpy% %SPOUT_SDK_DIR%Binaries\*.dll %LIB_DIR%

@%filecpy% %SPOUT_SDK_DIR%Include %INC_DIR%

@PAUSE
@POPD