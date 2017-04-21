set runpath=.\workspace\x64

set cv_ver=249
set cvpath=C:\pkg\opencv249\build
set cudapath=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v7.5
set path=%cvpath%\x64\vc11\bin\;%cudapath%\bin;%runpath%\debug;%runpath%\release;%path%

call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE\devenv.exe" ./workspace/navigation.sln

pause