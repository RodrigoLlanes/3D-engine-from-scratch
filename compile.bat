conan install . --output-folder=build --build=missing
cd build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"
cmake --build . --config Release
.\Release\engine.exe
del .\Release\engine.exe