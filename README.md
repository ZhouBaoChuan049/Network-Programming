# Network Programming

> Linux 网络编程学习与实践仓库  
> 基于 C/C++，从 Socket 基础通信逐步深入到 I/O 多路复用、Reactor、Epoll ET 以及自定义应用层协议。

本仓库记录我在学习 **Linux / C++ 网络编程**过程中编写的全部实验代码与实践项目。

相比单纯记录知识点，我更倾向于通过**手写代码、拆分模块、调试运行和逐步迭代**的方式理解网络编程中的核心机制，并尝试将零散的系统调用逐渐组织成完整的网络服务程序。

---

## 1. 学习路线

整个仓库按照从基础到深入的思路逐步演进：

```text
Socket 基础
    │
    ├── TCP
    │   ├── Echo
    │   ├── Dictionary
    │   └── Remote Shell
    │
    ├── UDP
    │   ├── Echo
    │   ├── Dictionary
    │   └── Chat Room
    │
    ▼
I/O 多路复用
    │
    ├── select
    ├── poll
    └── epoll
    │
    ▼
HTTP
    │
    └── Mini HTTP Server
    │
    ▼
应用层协议设计
    │
    └── Custom Protocol Calculator
    │
    ▼
Reactor
    │
    └── Epoll ET Reactor Server
```

---

## 2. 技术栈

### Programming Language

- C++
- C

### Operating System

- Linux

### Network Programming

- TCP
- UDP
- Socket API
- IPv4
- Client / Server 网络模型
- 阻塞 I/O
- 非阻塞 I/O

### I/O Multiplexing

- `select`
- `poll`
- `epoll`
- `epoll ET（Edge Triggered）`

### Server Architecture

- Reactor
- Event-driven Programming
- `Channel`
- `Connection`
- `Listener`
- Callback

### Application Layer

- HTTP
- 自定义应用层协议
- TCP 长连接
- 请求 / 响应模型
- 网络字节流处理

### C++ / Linux

- C++11 / C++17
- STL
- Linux System Call
- Makefile
- GDB
- Git

---

## 3. 项目目录

| 项目 | 内容 | 主要知识点 |
|---|---|---|
| `Internet_TCP_EchoDemo` | TCP Echo 服务 | TCP、Socket、客户端/服务器 |
| `Internet_TCP_Dictionary` | TCP 字典服务 | TCP 通信、请求处理 |
| `Internet_TCP_RemoteShell` | TCP 远程 Shell | TCP、进程创建、命令执行 |
| `Internet_UDP_EchoDemo` | UDP Echo 服务 | UDP、无连接通信 |
| `Internet_UDP_Dictionary` | UDP 字典服务 | UDP、数据报通信 |
| `Internet_UDP_ChatRoom` | UDP 聊天室 | UDP、多客户端通信 |
| `Internet_SelectAndPollAndEpoll_Demo` | I/O 多路复用实验 | select / poll / epoll |
| `Internet_MiniHTTPServer` | 简易 HTTP Server | HTTP、请求解析、Socket |
| `Internet_CustomProtocolCalculator` | 自定义协议计算器 | 应用层协议、序列化/反序列化 |
| `Internet_Reactor_EpollETServer` | Reactor 网络服务器 | Reactor、epoll ET、事件驱动 |
| `TestSpdlog` | spdlog 使用测试 | 日志系统、第三方库 |

---

## 4. 核心实践

### 4.1 TCP / UDP Socket 编程

从最基础的 Socket API 开始，实现客户端与服务器之间的数据通信。

涉及：

```text
socket
bind
listen
accept
connect
send
recv
sendto
recvfrom
close
```

通过 TCP 与 UDP 两种不同传输方式的实践，理解：

- 面向连接与无连接通信
- 字节流与数据报
- TCP Client / Server 模型
- UDP Client / Server 模型
- 网络通信中的请求与响应

---

### 4.2 I/O 多路复用

实现并对比：

```text
select
  ↓
poll
  ↓
epoll
```

重点理解 Linux 下服务器处理多个连接时的 I/O 事件管理方式。

进一步实践：

- 文件描述符集合
- I/O 就绪事件
- 阻塞与非阻塞
- `epoll_create`
- `epoll_ctl`
- `epoll_wait`
- LT / ET
- 非阻塞 Socket + Epoll

---

### 4.3 HTTP Server

在 Socket 编程的基础上实现简易 HTTP Server。

核心目标不是实现完整 HTTP 协议，而是通过手写 Server 理解：

```text
TCP
 ↓
HTTP Request
 ↓
Request Parsing
 ↓
Business Processing
 ↓
HTTP Response
 ↓
TCP
```

从而建立：

> **应用层协议最终仍然建立在传输层通信之上**

这一层抽象的认识。

---

### 4.4 自定义应用层协议

实现一个基于 TCP 的计算器服务，通过自定义协议完成客户端与服务器之间的数据交换。

核心流程：

```text
Client
  │
  │  Custom Protocol
  ▼
Server
  │
  ├── Parse Request
  │
  ├── Execute Calculation
  │
  └── Build Response
  │
  ▼
Client
```

重点实践：

- 应用层协议设计
- 请求 / 响应结构
- 网络字节流解析
- 消息边界
- 序列化 / 反序列化
- TCP 粘包 / 半包问题

---

### 4.5 Reactor + Epoll ET

这是当前仓库中相对完整的一部分。

在前面的 Socket、非阻塞 I/O 和 Epoll 基础上，进一步尝试将网络服务器抽象为 Reactor 模型。

整体结构：

```text
                 ┌──────────────┐
                 │    Reactor   │
                 └──────┬───────┘
                        │
                 epoll_wait()
                        │
          ┌─────────────┼─────────────┐
          ▼             ▼             ▼
      Listener       Channel       Connection
          │             │             │
        Accept        Event         Read/Write
          │             │             │
          └─────────────┴─────────────┘
                        │
                     Callback
```

核心实践包括：

- `epoll`
- 非阻塞 Socket
- ET 模式
- Reactor
- Listener
- Channel
- Connection
- Callback
- 事件驱动
- 网络连接生命周期管理

这一部分也是后续进一步学习 **高性能网络服务器、RPC、网络框架以及中间件**的基础。

---

## 5. 从 Socket 到 Reactor

这个仓库最重要的不是某一个单独的 Demo，而是整个代码的演进过程：

```text
单连接 TCP Server
        ↓
多种 TCP / UDP 实验
        ↓
多个客户端
        ↓
select / poll
        ↓
epoll
        ↓
非阻塞 I/O
        ↓
Epoll ET
        ↓
Reactor
        ↓
事件驱动网络服务器
```

这个过程也是我理解 Linux 网络编程的主要方式：

> **先理解系统调用，再理解 I/O 模型，最后尝试进行架构抽象。**

---

## 6. 学习目标

通过这个仓库，希望逐步掌握：

### Linux 网络编程

- Socket 编程模型
- TCP / UDP
- 网络字节流
- I/O 多路复用
- 非阻塞 I/O
- Epoll
- LT / ET
- Reactor

### 网络服务器

- Client / Server 架构
- 多连接管理
- 事件驱动模型
- 网络连接生命周期
- 请求 / 响应模型
- 应用层协议设计

### C++ 工程实践

- C++ 面向对象设计
- 模块化
- 回调机制
- STL
- Makefile
- GDB
- Git
- 第三方库使用

---

## 7. 后续计划

当前仓库主要完成 Linux 网络编程基础以及 Reactor + Epoll ET 的实践。

后续将继续向更完整的 C++ 网络服务器方向深入：

```text
Reactor
   ↓
线程池
   ↓
定时器
   ↓
Buffer
   ↓
HTTP Server
   ↓
RPC
   ↓
高性能网络服务器
   ↓
分布式 / 微服务
```

同时会进一步关注：

- 网络库设计
- 多线程并发
- TCP 连接管理
- 定时器
- Buffer 设计
- HTTP 协议
- RPC
- 序列化
- 日志系统
- MySQL
- Redis
- C++ 后端工程实践

---

## 8. Repository Philosophy

这个仓库不是某一个完整商业项目，而是一个**持续演进的学习型工程仓库**。

我更关注：

```text
为什么这样设计？
        ↓
底层系统调用做了什么？
        ↓
多个模块如何协作？
        ↓
如何从 Demo 演进成 Server？
        ↓
如何进一步抽象成可复用的网络组件？
```

因此，仓库中的代码会随着学习深入不断重构和完善。

相比直接使用成熟网络框架，我更希望在学习阶段尽可能自己实现核心机制，从而建立对 Linux 网络 I/O、事件驱动和服务器架构的底层理解。

---

## 9. Environment

推荐环境：

```text
Linux
GCC / G++
C++11 / C++17
Make
GDB
Git
```

部分项目会根据具体实现使用额外的第三方库。

---

## 10. Build

进入对应项目目录后，根据项目中的 `Makefile` 进行编译。

例如：

```bash
cd Internet_Reactor_EpollETServer
make
```

运行对应的 Server / Client：

```bash
./Server
./Client
```

具体编译方式以各子项目中的 `Makefile` 为准。

---

## 11. Project Status

| 模块 | 状态 |
|---|:---:|
| TCP Socket | ✅ |
| UDP Socket | ✅ |
| TCP Echo | ✅ |
| UDP Echo | ✅ |
| TCP Dictionary | ✅ |
| UDP Dictionary | ✅ |
| UDP Chat Room | ✅ |
| TCP Remote Shell | ✅ |
| Select | ✅ |
| Poll | ✅ |
| Epoll | ✅ |
| Mini HTTP Server | ✅ |
| Custom Protocol | ✅ |
| Reactor | ✅ |
| Epoll ET | ✅ |
| Thread Pool | 🚧 |
| Timer | 🚧 |
| Buffer | 🚧 |
| RPC | 📋 |
| Distributed System | 📋 |

---

## 12. Author

**ZhouBaoChuan**

C/C++ / Linux / Network Programming

GitHub：

https://github.com/ZhouBaoChuan049

---

## 13. About This Repository

> **Learn from the system call, build from the socket, and evolve toward the server.**

这个仓库会持续记录我在 Linux / C++ 网络编程方向上的学习、实践与代码演进。
