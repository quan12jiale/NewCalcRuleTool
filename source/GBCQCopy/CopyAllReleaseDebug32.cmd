
set QtPath=%1
set QtPath=%QtPath:\=/%
set GMPPath=%2
set GMPPath=%GMPPath:\=/%
set GTJPath=%3
set GTJPath=%GTJPath:\=/%
set DestPath=%4
set DestPath=%DestPath:\=/%
set Dest_LIB_DIR=%5
set Dest_LIB_DIR=%Dest_LIB_DIR:\=/%

md "%DestPath%"

echo "***********NewCalcRuleTool运行依赖库***********"
echo "--------------------GMP Copy--------------------"
Copy /Y "%GMPPath%\GMP\Externals\GGDB\bin\Win32\ReleaseDebug\Dll7zip.dll" "%DestPath%\"

Copy /Y "%GMPPath%\GlodonResource\Glodon\bin\Release\X86\GSPEngine.dll" "%DestPath%\"
Copy /Y "%GMPPath%\GlodonResource\Glodon\bin\Release\X86\GEPEngine.dll" "%DestPath%\"
Copy /Y "%GMPPath%\GlodonResource\Glodon\bin\Release\X86\GLDZip.dll" "%DestPath%\"
Copy /Y "%GMPPath%\GlodonResource\Glodon\bin\Release\X86\GSP.dll" "%DestPath%\"
Copy /Y "%GMPPath%\GlodonResource\Glodon\bin\Release\X86\GLDWidget.dll" "%DestPath%\"
Copy /Y "%GMPPath%\GlodonResource\Glodon\bin\Release\X86\GLDXLS.dll" "%DestPath%\"
Copy /Y "%GMPPath%\GlodonResource\Glodon\bin\Release\X86\GLDCommon.dll" "%DestPath%\"
Copy /Y "%GMPPath%\GlodonResource\Glodon\bin\Release\X86\GLDTableView.dll" "%DestPath%\"
Copy /Y "%GMPPath%\GlodonResource\Glodon\bin\Release\X86\GLDPrivateCalledPlatform.dll" "%DestPath%\"
Copy /Y "%GMPPath%\GlodonResource\Glodon\bin\Release\X86\GLDXML.dll" "%DestPath%\"

Copy /Y "%GMPPath%\GlodonResource\ThirdPart\VMProtect\bin\Win32\VMProtectSDK32.dll" "%DestPath%\"
Copy /Y "%GMPPath%\GlodonResource\ThirdPart\libxl\bin\libxl.dll" "%DestPath%\"
Copy /Y "%GMPPath%\GlodonResource\ThirdPart\lzma\lib\Release\X86\LZMA.dll" "%DestPath%\"

Copy /Y "%GMPPath%\GMP\Externals\GGDB\lib\Win32\ReleaseDebug\Dll7zip.lib" "%Dest_LIB_DIR%\"
Copy /Y "%GMPPath%\GlodonResource\Glodon\bin\Release\X86\GSPEngine.lib" "%Dest_LIB_DIR%\"
Copy /Y "%GMPPath%\GlodonResource\Glodon\bin\Release\X86\GEPEngine.lib" "%Dest_LIB_DIR%\"
Copy /Y "%GMPPath%\GlodonResource\Glodon\bin\Release\X86\GLDZip.lib" "%Dest_LIB_DIR%\"
Copy /Y "%GMPPath%\GlodonResource\Glodon\bin\Release\X86\GSP.lib" "%Dest_LIB_DIR%\"
Copy /Y "%GMPPath%\GlodonResource\Glodon\bin\Release\X86\GSPKeyLib.lib" "%Dest_LIB_DIR%\"
Copy /Y "%GMPPath%\GlodonResource\ThirdPart\VMProtect\lib\Win32\VMProtectSDK32.lib" "%Dest_LIB_DIR%\"

echo "--------------------GTJ Copy--------------------"
Copy /Y "%GTJPath%\api-ms-win-crt-string-l1-1-0.dll" "%DestPath%\"
Copy /Y "%GTJPath%\MSVCP140.dll" "%DestPath%\"
Copy /Y "%GTJPath%\api-ms-win-crt-convert-l1-1-0.dll" "%DestPath%\"
Copy /Y "%GTJPath%\api-ms-win-crt-filesystem-l1-1-0.dll" "%DestPath%\"
Copy /Y "%GTJPath%\api-ms-win-crt-stdio-l1-1-0.dll" "%DestPath%\"
Copy /Y "%GTJPath%\api-ms-win-crt-environment-l1-1-0.dll" "%DestPath%\"
Copy /Y "%GTJPath%\VCRUNTIME140.dll" "%DestPath%\"
Copy /Y "%GTJPath%\api-ms-win-crt-runtime-l1-1-0.dll" "%DestPath%\"
Copy /Y "%GTJPath%\api-ms-win-crt-time-l1-1-0.dll" "%DestPath%\"
Copy /Y "%GTJPath%\api-ms-win-crt-heap-l1-1-0.dll" "%DestPath%\"
Copy /Y "%GTJPath%\api-ms-win-crt-multibyte-l1-1-0.dll" "%DestPath%\"
Copy /Y "%GTJPath%\api-ms-win-crt-math-l1-1-0.dll" "%DestPath%\"
Copy /Y "%GTJPath%\api-ms-win-crt-utility-l1-1-0.dll" "%DestPath%\"
Copy /Y "%GTJPath%\api-ms-win-crt-locale-l1-1-0.dll" "%DestPath%\"

echo "--------------------Qt Copy--------------------"
Copy /Y "%QtPath%\Qt5Xml.dll" "%DestPath%\"
Copy /Y "%QtPath%\Qt5PrintSupport.dll" "%DestPath%\"
Copy /Y "%QtPath%\Qt5Gui.dll" "%DestPath%\"
Copy /Y "%QtPath%\Qt5Core.dll" "%DestPath%\"
Copy /Y "%QtPath%\Qt5Widgets.dll" "%DestPath%\"
echo "***********Copy NewCalcRuleTool依赖库结束***********"

echo "---------------所有 Copy 结束---------------"
