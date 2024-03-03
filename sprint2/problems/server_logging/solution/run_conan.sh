mkdir build 
cd build

# Под Linux обязательно нужно указывать параметр
# -s compiler.libcxx=libstdc++11. Иначе собранная
# программа будет падать:
conan install .. -s compiler.libcxx=libstdc++11 -s build_type=Release

cmake .. -DCMAKE_BUILD_TYPE=Release

# команда сборки и запуска
cmake --build . 
#&& build/bin/game_server
