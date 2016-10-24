REM this is for windows 7 + VS2013
REM replace the 12.0 with something else if you use a different version of VS
REM no idea if this works on win8/10

set INCLUDE=C:\Program Files\Microsoft Visual Studio 12.0\VC\include;C:\Program Files\Windows Kits\8.1\Include\shared;C:\Program Files\Windows Kits\8.1\Include\um;C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include;C:\Program Files (x86)\Windows Kits\8.1\Include\shared;C:\Program Files (x86)\Windows Kits\8.1\Include\um
set LIB=C:\Program Files\Microsoft Visual Studio 12.0\VC\LIB;C:\Program Files\Microsoft Visual Studio 12.0\VC\ATLMFC\LIB;C:\Program Files\Windows Kits\8.1\lib\winv6.3\um\x86;C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\LIB;C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\ATLMFC\LIB;C:\Program Files (x86)\Windows Kits\8.1\lib\winv6.3\um\x86
set PATH=C:\Program Files\Microsoft Visual Studio 12.0\VC\bin;C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin;%PATH%

cl ray1.cc -Feray1.exe
cl ray2.cc -Feray2.exe
cl ray3.cc -Feray3.exe
cl ray4.cc -Feray4.exe
cl ray5.cc -Feray5.exe
