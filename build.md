
安装依赖环境

drogon 编译Cmake

官方
```
cmake .. -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake" -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_INSTALL_PREFIX="D:"
cmake --build . --parallel --target install

```

```
cmake .. -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake" -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_CXX_FLAGS="-fcoroutines" -DCMAKE_CXX_FLAGS="-std=c++20" -DCMAKE_INSTALL_PREFIX="installed"
```

```bat
conan install .. -s compiler="msvc" -s compiler.version=194 -s compiler.cppstd=20 -s build_type=Release --output-folder . --build=missing
```

```bat
conan install .. -s compiler="msvc" -s compiler.version=194 -s compiler.cppstd=20 -s build_type=Debug --output-folder . --build=missing
```

生成工程文件
```
cmake .. --preset conan-default
```