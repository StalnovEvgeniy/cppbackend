mkdir build 
cd build

# Под Linux обязательно нужно указывать параметр
# -s compiler.libcxx=libstdc++11. Иначе собранная
# программа будет падать:
#conan install .. -s compiler.libcxx=libstdc++11 -s build_type=Release

# mkdir -p build-release 
# cd build-release
conan install .. --build=missing -s build_type=Release -s compiler.libcxx=libstdc++11
cmake .. -DCMAKE_BUILD_TYPE=Release
cd ..

# mkdir -p build-debug
# cd build-debug
# conan install .. --build=missing -s build_type=Debug -s compiler.libcxx=libstdc++11
# cmake .. -DCMAKE_BUILD_TYPE=Debug
# cd ..

# команда сборки и запуска
#cmake --build . 
#&& build/bin/game_server
