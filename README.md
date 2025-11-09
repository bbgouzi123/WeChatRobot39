# WeChatRobot39

微信机器人工具，支持微信 3.9.5.81 版本。

## 📋 项目简介

WeChatRobot39 是一个基于 DLL 注入技术的微信自动化工具，可以实现消息收发、好友管理、群聊操作等功能。

> **注意**：本项目基于 [GuiShou（鬼手56）](https://github.com/TonyChen56) 的开源项目 WeChatRobot 进行适配，已适配到微信 3.9.5.81 版本。感谢原作者的开源贡献！

## ✨ 功能特性

- ✅ **消息收发**：支持文本、图片、文件等消息的发送和接收
- ✅ **好友管理**：获取好友列表、查看好友信息
- ✅ **群聊操作**：支持群聊消息、群成员管理
- ✅ **防撤回**：自动拦截撤回消息
- ✅ **自动回复**：支持关键词自动回复
- ✅ **多开支持**：支持微信多开
- ✅ **版本适配**：已适配微信 3.9.5.81 版本

## 🔧 系统要求

- **操作系统**：Windows 10/11
- **微信版本**：3.9.5.81（必须）
- **运行权限**：需要管理员权限
- **开发环境**：Visual Studio 2022（编译时）

## 📦 安装说明

### 1. 下载项目

```bash
git clone <repository-url>
cd WeChatRobot39
```

### 2. 编译项目

#### 前置要求

- Visual Studio 2022（Community 或更高版本）
- Windows SDK 10.0.19041.0 或更高版本
- MFC 库（v143 - x86 和 x64）

#### 编译步骤

1. **安装 MFC 库**（如果未安装）：
   - 打开 Visual Studio Installer
   - 修改 Visual Studio 2022
   - 切换到"单个组件"标签
   - 勾选"适用于 v143 生成工具的 C++ v14.44 (17.14) MFC (x86 和 x64)"
   - 点击"修改"安装

2. **编译项目**：
   ```powershell
   # 编译 WeChatHelper.dll
   cd WeChatRobot39\WeChatHelper
   msbuild WeChatHelper.vcxproj /p:Configuration=Release /p:Platform=x86
   
   # 编译 WeChatRobot.exe
   cd ..\WeChatRobot
   msbuild WeChatRobot.vcxproj /p:Configuration=Release /p:Platform=x86
   ```

3. **编译输出**：
   - `Release\WeChatHelper.dll` - 核心注入 DLL
   - `WeChatRobot\Release\WeChatRobot.exe` - 主程序

### 3. 文件准备

确保以下文件在同一目录下：
- `WeChatRobot.exe`
- `WeChatHelper.dll`

## 🚀 使用方法

### 1. 启动微信

确保微信 3.9.5.81 已安装并正在运行。

### 2. 运行程序

**重要**：必须以管理员权限运行！

1. **右键点击 `WeChatRobot.exe`**
2. **选择"以管理员身份运行"**

或者设置程序始终以管理员权限运行：
1. 右键点击 `WeChatRobot.exe` → 属性
2. 切换到"兼容性"标签
3. 勾选"以管理员身份运行此程序"
4. 点击"确定"

### 3. 使用功能

程序启动后会自动注入 DLL 到微信进程，然后可以使用以下功能：

- **好友列表**：查看和管理好友
- **聊天记录**：查看和发送消息
- **功能大全**：各种自动化功能

## ⚠️ 注意事项

### 权限要求

- **必须**以管理员权限运行，否则会出现"创建模块快照失败"错误
- 这是 Windows 的安全机制，访问其他进程需要管理员权限

### 版本要求

- **必须**使用微信 3.9.5.81 版本
- 其他版本可能无法正常工作或导致微信崩溃

### 使用限制

- 仅供学习和研究使用
- 请勿用于商业用途
- 使用本工具产生的任何后果由使用者自行承担

### 常见问题

#### 1. "创建模块快照失败"

**原因**：权限不足

**解决方案**：
- 以管理员权限运行 `WeChatRobot.exe`
- 或设置程序始终以管理员权限运行

#### 2. "dll已经注入，请勿重复注入"

**原因**：DLL 已经注入或权限不足导致误判

**解决方案**：
- 关闭所有 `WeChatRobot.exe` 进程
- 以管理员权限重新运行

#### 3. "当前微信版本不匹配"

**原因**：微信版本不是 3.9.5.81

**解决方案**：
- 安装微信 3.9.5.81 版本
- 或修改代码中的版本检查（不推荐）

## 🔨 技术说明

### 架构设计

- **WeChatHelper.dll**：注入到微信进程的 DLL，负责 Hook 微信函数
- **WeChatRobot.exe**：主程序 GUI，通过 `WM_COPYDATA` 与 DLL 通信

### 核心技术

- **DLL 注入**：使用 `CreateRemoteThread` 和 `LoadLibrary` 注入 DLL
- **函数 Hook**：使用内联 Hook 技术拦截微信函数
- **进程通信**：使用 `WM_COPYDATA` 消息进行进程间通信
- **内存操作**：直接读写微信进程内存

### 适配说明

本项目已适配微信 3.9.5.81 版本，主要修改包括：

- ✅ 版本检查更新
- ✅ 偏移地址更新（基于 wxhelper 项目）
- ✅ 登录检测逻辑更新（使用账户服务管理器）
- ✅ 项目配置更新（Windows SDK 和工具集）

## 📝 开发说明

### 项目结构

```
WeChatRobot39/
├── WeChatHelper/          # 注入 DLL 项目
│   ├── InitWeChat.cpp     # 初始化微信相关
│   ├── WndMsgLoop.cpp     # 窗口消息循环
│   ├── offset.h           # 偏移地址定义
│   └── ...
├── WeChatRobot/           # 主程序项目
│   ├── CInjectTools.cpp   # DLL 注入工具
│   ├── CMain.cpp          # 主窗口
│   └── ...
└── Release/               # 编译输出目录
```

### 编译配置

- **平台工具集**：v143 (Visual Studio 2022)
- **Windows SDK**：10.0.19041.0
- **字符集**：Unicode
- **MFC**：静态链接
- **ATL**：静态链接

## 🙏 致谢

### 原作者

本项目基于 **GuiShou（鬼手56）** 的开源项目 **WeChatRobot** 进行适配和修改。

**原作者信息**：
- **作者**：GuiShou（鬼手56）
- **GitHub**：https://github.com/TonyChen56
- **CSDN**：https://blog.csdn.net/qq_38474570
- **Email**：tonychen56@qq.com

感谢原作者的开源贡献，让更多人能够学习和研究微信自动化技术。

### 参考项目

- [wxhelper](https://github.com/...) - 参考项目（微信 3.9.5.81 偏移地址来源）

## 📄 许可证

本项目基于原作者 GuiShou 的开源项目 WeChatRobot 进行适配，仅供学习和研究使用。

## ⚠️ 免责声明

- 本工具仅供学习和研究使用
- 使用本工具产生的任何后果由使用者自行承担
- 请遵守相关法律法规，不得用于非法用途
- 原作者和适配者不对使用本工具造成的任何损失负责

## 📞 联系方式

如有问题或建议，请通过以下方式联系：

- GitHub Issues
- 项目讨论区

---

**最后更新**：2025-11-10

**适配版本**：微信 3.9.5.81
