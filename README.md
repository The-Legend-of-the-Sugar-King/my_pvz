# 🌱 Terminal Plants VS Zombies

一个基于 **C++17 + FTXUI** 开发的终端版植物大战僵尸游戏。

---

# 📖 项目简介

本项目使用：

* C++17
* FTXUI
* CMake
* 多线程
* 面向对象设计
* Git/GitHub

实现了一个运行在 Linux / WSL 终端中的 Plants VS Zombies。

项目目标：

* 学习现代 C++ 项目结构
* 练习面向对象设计
* 熟悉 FTXUI 图形界面库
* 掌握 CMake 工程构建
* 练习 Git/GitHub 协作开发
* 尝试 AI 辅助开发流程

---

# ✨ 已实现功能

## 🎮 游戏系统

* 主菜单系统
* 暂停系统
* 游戏结束界面
* 光标移动系统

## 🌱 植物系统

当前已实现：

| 植物      | 功能     |
| ------- | ------ |
| 🌱 豌豆射手 | 发射子弹攻击 |
| 🌻 向日葵  | 生产阳光   |
| 💣 土豆地雷 | 接触爆炸   |
| 🍒 樱桃炸弹 | 范围爆炸   |
| 🧱 坚果墙  | 高生命防御  |

---

## 🧟 僵尸系统

* 自动生成僵尸
* 自动寻路前进
* 攻击植物
* 动态难度提升
* 随时间增强血量

---

## 💥 战斗系统

* 子弹碰撞检测
* 范围爆炸
* 实时血量计算
* 实时对象清理

---

## 🖥 UI系统

基于 FTXUI 实现：

* 地图渲染
* 信息栏
* 状态栏
* 菜单界面
* 游戏结束界面

---

# 🧠 AI辅助开发

本项目在开发过程中使用 ChatGPT 辅助完成：

* FTXUI 图形界面设计
* CMake 构建系统
* 项目结构拆分
* 部分游戏逻辑设计
* Bug 调试与修复
* GitHub 仓库管理

开发过程中参考了：

* FTXUI 官方文档
* 部分开源项目设计思路

但所有代码均由本人完成修改、整合与实现。

---

# 📂 项目结构

```text
my_pvz/
│
├── main.cpp                # 程序入口
│
├── tool.hpp                # 核心声明
│
├── world/
│   ├── world.cpp           # 游戏核心逻辑
│   ├── zombie.cpp          # 僵尸逻辑
│   ├── plant.cpp           # 植物逻辑
│   └── shot.cpp            # 子弹逻辑
│
├── ui/
│   ├── draw.cpp            # 地图绘制
│   ├── input.cpp           # 输入处理
│   └── menu.cpp            # 菜单系统
│
├── CMakeLists.txt
├── README.md
└── .gitignore
```

---

# ⚙️ 编译运行

## Linux / WSL

### 1. 克隆项目

```bash
git clone https://github.com/The-Legend-of-the-Sugar-King/my_pvz.git
```

---

### 2. 创建 build 目录

```bash
mkdir build
cd build
```

---

### 3. 生成 Makefile

```bash
cmake ..
```

---

### 4. 编译项目

```bash
make
```

---

### 5. 启动游戏

```bash
./world
```

---

# 🎹 操作说明

| 按键      | 功能     |
| ------- | ------ |
| W A S D | 移动光标   |
| 1       | 选择豌豆射手 |
| 2       | 选择向日葵  |
| 3       | 选择土豆地雷 |
| 4       | 选择樱桃炸弹 |
| 5       | 选择坚果墙  |
| Space   | 种植植物   |
| X       | 铲除植物   |
| Enter   | 查看属性   |
| ESC     | 暂停游戏   |
| Q       | 退出游戏   |

---

# 📈 游戏机制

## 🌞 阳光系统

* 初始阳光：500
* 向日葵自动生产阳光

---

## 🧟 动态难度系统

随着时间增加：

* 僵尸生成速度提高
* 僵尸血量提高
* 游戏难度不断增加

---

# 🛠 使用技术

## C++

* 类与继承
* 多态
* 指针管理
* STL 容器
* 多线程

---

## FTXUI

* 组件系统
* 终端图形界面
* 事件系统
* 实时渲染

---

## 工程化

* CMake
* Git
* GitHub
* 模块化拆分

---

# 🚀 后续计划

未来准备加入：

* 更多植物
* 更多僵尸
* 关卡系统
* 存档系统
* 音乐系统
* 动画效果
* Boss僵尸
* 无尽模式

---

# 📷 游戏截图

## 主菜单

<img width="523" height="1265" alt="image" src="https://github.com/user-attachments/assets/6da17109-5293-4ddd-9882-71ce86a81470" />


---

## 游戏界面

<img width="1017" height="1359" alt="image" src="https://github.com/user-attachments/assets/1804f0d0-1d01-4587-9537-af9f91a9b40d" />


---

## 游戏结束

<img width="1255" height="1364" alt="image" src="https://github.com/user-attachments/assets/45debc01-3440-424a-ad9a-c16164be0aef" />


---

# 👨‍💻 作者

GitHub：

[The-Legend-of-the-Sugar-King](https://github.com/The-Legend-of-the-Sugar-King?utm_source=chatgpt.com)

---

# ⭐ Star

如果你喜欢这个项目，欢迎点一个 Star ⭐
