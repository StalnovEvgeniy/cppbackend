FROM gcc:11.3

RUN apt update && \
    apt install -y \
      python3-pip \
      cmake \
    && \
    pip3 install conan

# копируем conanfile.txt в контейнер и запускаем conan install
COPY conanfile.txt /app/
RUN mkdir /app/build && cd /app/build && \
    conan install .. --build=missing

# только после этого копируем остальные иходники
COPY ./src /app/src
COPY ./data /app/data
COPY CMakeLists.txt /app/

RUN cd /app/build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . 
