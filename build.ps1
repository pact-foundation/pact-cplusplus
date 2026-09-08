# Before:
# D:\dev\fusion\3P\7zip\25.01\win64\bin\7za.exe e -y -so .\pact_ffi-windows-x86_64.dll.gz > pact_ffi-windows-x86_64.dll

md build -Force
cmake -S consumer -B build -DCMAKE_BUILD_TYPE=Release -DBoost_ROOT="D:\dev\fusion\3P\BOOST\1.1850.1\win64_0002" -DPACT_FFI_ROOT="D:\dev\pact_ffi-v0.5.6" -DPactUseConan=OFF -DPactBuildTests=OFF -DBUILD_SHARED_LIBS=OFF
cmake --build build --config Release
cmake --install build --prefix .\build\install\pact-cpp

# After:
# cd build/install/pact-cpp
# D:\dev\fusion\3P\7zip\25.01\win64\bin\7za.exe -mm=lzma -mx9 -r a ../WIN64.zip *
# cd ../../../