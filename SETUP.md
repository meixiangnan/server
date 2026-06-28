# 服务端环境搭建说明

## 技术栈

| 组件 | 版本 |
|------|------|
| C++ 标准 | C++20 |
| Web 框架 | Drogon |
| 包管理 | Conan 2.x |
| 构建工具 | CMake 3.29+ |
| 数据库 | MySQL / MariaDB |
| 缓存 | Redis |
| 序列化 | Protobuf 5.27 |
| 脚本 | Lua 5.4 |
| JSON | jsoncpp 1.9.4 |

---

## 一、安装工具链

### 1. 安装 Visual Studio 2022

- 下载地址：https://visualstudio.microsoft.com/
- 安装时勾选「使用 C++ 的桌面开发」工作负载
- 确保 MSVC 编译器版本为 **194**（VS2022 默认）

### 2. 安装 CMake

- 下载地址：https://cmake.org/download/
- 版本要求：**3.29 或更高**
- 安装时勾选「Add CMake to the system PATH」

### 3. 安装 Python（Conan 依赖）

- 下载地址：https://www.python.org/downloads/
- 版本要求：3.8+
- 安装时勾选「Add Python to PATH」

### 4. 安装 Conan

```bat
pip install conan
```

验证安装：

```bat
conan --version
```

### 5. 配置 Conan Profile

```bat
conan profile detect --force
```

然后编辑生成的 profile 文件（路径通常在 `C:\Users\用户名\.conan2\profiles\default`），确认以下内容：

```ini
[settings]
os=Windows
compiler=msvc
compiler.version=194
compiler.cppstd=20
compiler.runtime=dynamic
build_type=Release
arch=x86_64
```

---

## 二、安装运行时依赖

### MySQL / MariaDB

- 下载地址：https://mariadb.org/download/
- 安装后记录好用户名、密码、端口（默认 3306）
- 创建对应的数据库和表（参考项目 SQL 脚本）

### Redis

- Windows 版下载：https://github.com/tporadowski/redis/releases
- 安装后默认端口 6379，无需密码（可按需配置）
- 启动服务：`redis-server`

---

## 三、克隆项目

```bash
git clone git@github.com:meixiangnan/server.git server_bzyx
cd server_bzyx
```

---

## 四、编译项目

### Release 版本

```bat
cd build

conan install .. -s compiler="msvc" -s compiler.version=194 -s compiler.cppstd=20 -s build_type=Release --output-folder . --build=missing

cmake .. --preset conan-default

cmake --build . --config Release --parallel
```

编译完成后，可执行文件输出到 `exe/bin/`。

### Debug 版本

```bat
cd build_debug

conan install .. -s compiler="msvc" -s compiler.version=194 -s compiler.cppstd=20 -s build_type=Debug --output-folder . --build=missing

cmake .. --preset conan-default

cmake --build . --config Debug --parallel
```

编译完成后，可执行文件输出到 `exe/bind/`。

---

## 五、配置服务

编译完成后在 `exe/bin/` 或 `exe/bind/` 目录下找到配置文件，修改以下内容：

- **MySQL 连接**：填写数据库地址、端口、用户名、密码、库名
- **Redis 连接**：填写 Redis 地址和端口
- **监听端口**：各服务默认端口按需修改

---

## 六、启动服务

项目包含三个独立服务，分别对应：

| 服务 | 目录 | 说明 |
|------|------|------|
| 游戏主服务 | `src/services/game/` | 处理游戏逻辑、WebSocket 连接 |
| 登录/版本服务 | `src/services/login_version/` | 处理登录、版本校验 |
| 监控服务 | `src/services/ds_moniter/` | 数据监控 WebSocket |

各服务编译后生成独立可执行文件，分别启动即可。

---

## 七、常见问题

**Conan 找不到包**

```bat
conan install .. --build=missing
```

加上 `--build=missing` 让 Conan 自动从源码编译缺失的依赖。

**CMake 找不到 drogon**

确认 `third_party/installed/drogon/win/release` 目录存在，该目录需要手动放置预编译的 drogon 库，或通过 Conan 安装。

**编译报 C++20 相关错误**

确认 Visual Studio 版本为 2022，MSVC 编译器版本 194，profile 中 `compiler.cppstd=20`。
