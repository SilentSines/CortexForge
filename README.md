# CortexForge

基于 CMake 的 Cortex-M 多平台嵌入式工程框架，支持 ARMCLANG 和 GCC 双工具链。换芯片只需建目录、写 `sources.cmake`，换编译器只需换 preset，不改任何现有代码。

## 目录结构

```
CortexForge/
├── CMakeLists.txt                 # 顶层：组装可执行文件、flash target
├── CMakePresets.json              # 预设配置（每个芯片x编译器一个 preset）
├── cmake/
│   ├── toolchain-armclang.cmake   # ARMCLANG 工具链定义
│   └── toolchain-gcc.cmake        # GCC 工具链定义
├── .vscode/
│   ├── launch.json.in             # 调试配置模板（CMake 自动生成 launch.json）
│   ├── tasks.json.in              # 任务配置模板（CMake 自动生成 tasks.json）
│   └── settings.json.in           # OpenOCD 路径设置模板（CMake 自动生成 settings.json）
├── scripts/
│   ├── build-armclang.bat         # Windows / ARMCLANG 一键编译
│   ├── build-gcc.bat              # Windows / GCC 一键编译
│   └── build-gcc.sh               # Linux / GCC 一键编译
├── main.c                         # 应用入口
│
├── platform/                      # 平台层（按芯片隔离）
│   ├── cmsis/                     # ARM CMSIS 核心头文件（纯头文件，所有平台共享）
│   └── at32f403a/                 # AT32F403A 平台
│       ├── CMakeLists.txt         # MCU_FLAGS / LINK_FLAGS / PROJECT_DEFINES（按编译器分支）
│       ├── AT32F403Axx.svd        # SVD 文件
│       ├── startup/               # 启动代码 + 中断服务 + 系统时钟
│       │   ├── sources.cmake      # 按编译器选启动文件
│       │   ├── ARM/               # armasm 语法启动文件（ARMCLANG 用）
│       │   │   └── startup_at32f403a_407.s
│       │   ├── GCC/               # GNU 语法启动文件 + syscalls + sysmem（GCC 用）
│       │   │   ├── startup_at32f403a_407.s
│       │   │   ├── syscalls.c
│       │   │   └── sysmem.c
│       │   ├── system_at32f403a_407.c   # 共用
│       │   └── at32f403a_407_int.c      # 共用
│       ├── vendor/                # 芯片厂商标准库
│       │   └── sources.cmake
│       ├── hal/                   # HAL 封装（gpio/uart/spi/dma/...）
│       │   └── sources.cmake
│       ├── linker/
│       │   ├── armclang.sct      # ARMCLANG 链接散列文件
│       │   └── gcc.ld            # GCC 链接脚本
│       └── usb_lib/               # USB 驱动库（可选，目录存在才编译 USB）
│           └── sources.cmake
│
├── bsp/                           # 板级支持（led/btn/oled/tft/...）
│   └── sources.cmake
│
├── usb/                           # 外部 USB class（可选，需平台有 usb_lib 才编译）
│   └── sources.cmake
│
└── utils/                         # 轻量框架（平台无关）
    ├── task_manager.c/h           # 协作式任务调度
    ├── msg_manager.c/h            # 发布-订阅消息总线
    ├── page_manager.c/h           # 页面栈状态机
    ├── rand.c/h
    └── sources.cmake
```

## 环境要求

| 工具 | 说明 |
|------|------|
| Keil MDK / ARMCLANG 6 | 商业编译器（armclang / armasm / armlink / fromelf），仅 Windows |
| GNU Arm Embedded Toolchain | 开源编译器（arm-none-eabi-gcc），Windows / Linux 均可 |
| CMake >= 3.21 | 构建系统 |
| Ninja | 构建后端（CMakePresets 指定） |
| OpenOCD | 烧录与调试 |

### 跨平台支持

| 操作系统 | ARMCLANG | GCC | 说明 |
|----------|----------|-----|------|
| Windows | 可用（需 Keil 安装） | 可用 | 工具链需指定绝对路径 |
| Linux | 不可用 | 可用 | 工具链通过 PATH 查找，无需绝对路径 |

GCC 工具链在 Linux 下不需要设置 `GCC_BIN`，只要 `arm-none-eabi-gcc` 在 PATH 中即可。CMake 会自动查找。

## 双工具链支持

框架自动检测编译器类型并适配：

| 编译器 | preset 名 | 输出目录 | 输出后缀 | 全局宏 |
|--------|-----------|----------|----------|--------|
| ARMCLANG | `at32f403a-armclang` | `.build/at32f403a-armclang` | `.axf` | `ARM` |
| GCC | `at32f403a-gcc` | `.build/at32f403a-gcc` | `.elf` | `GCC` |

代码中可通过 `#ifdef ARM` / `#ifdef GCC` 区分编译器。

### 编译器差异处理

| 项目 | ARMCLANG | GCC |
|------|----------|-----|
| MCU 标志 | `--target=arm-arm-none-eabi` | 无需 target |
| 链接器 | armlink + scatter file (.sct) | gcc + linker script (.ld) |
| 汇编器 | armasm.exe（armasm 语法） | arm-none-eabi-gcc（GNU 语法） |
| 启动文件 | `startup/ARM/startup_at32f403a_407.s` | `startup/GCC/startup_at32f403a_407.s` |
| 系统调用 | 不需要 | `syscalls.c` + `sysmem.c` |
| microlib | `--library_type=microlib` | `--specs=nano.specs` |
| 镜像转换 | `fromelf --i32combined` | `objcopy -Oihex` |
| 链接器选项 | `--cpu` / `--scatter` / `--entry` | `-T` / `-Wl,--gc-sections` / `-Wl,-Map` |

## 快速开始

### 命令行

```powershell
# ARMCLANG
cmake --preset at32f403a-armclang
cmake --build .build/at32f403a-armclang

# GCC
cmake --preset at32f403a-gcc
cmake --build .build/at32f403a-gcc

# 烧录（自动先编译再烧录）
cmake --build .build/at32f403a-armclang --target flash
```

> **注意**：切换编译器时必须删除对应的 build 目录，CMake cache 不会自动刷新工具链设置。

输出文件在 `.build/at32f403a-armclang/output/` 或 `.build/at32f403a-gcc/output/`：

| 文件 | 说明 |
|------|------|
| `Project.axf` / `Project.elf` | ELF 调试镜像 |
| `Project.hex` | Intel HEX 烧录文件 |
| `Project.bin` | 二进制烧录文件 |
| `Project.map` | 链接映射表 |

### VS Code

需安装插件：**Cortex-Debug**（提供 ARM Cortex-M 的 GDB 调试支持）。

| 快捷键 | 动作 | 说明 |
|--------|------|------|
| `Ctrl+Shift+B` | 编译 | 调用 `cmake --build` |
| `F5` | 仿真 | 只启动调试，**不自动烧录** |
| `F6` | 烧录 | 先编译再 OpenOCD 烧录（需手动配置 keybinding，见下方） |

> F6 不是 VS Code 默认快捷键，需在 `keybindings.json` 中手动绑定：
> ```json
> { "key": "f6", "command": "workbench.action.tasks.runTask", "args": "flash_openocd" }
> ```

> **重要**：F5 仿真前必须先按 F6 烧录一次。为了兼容 ARMCLANG 的 `.axf` 格式，`launch.json` 中 `loadFiles` 为空、`symbolFiles` 指向 ELF/AXF 文件，Cortex-Debug 不会自动下载 hex 到芯片。正确流程是：`F6 烧录` → `F5 仿真`。

`launch.json`、`tasks.json`、`settings.json` 由 CMake 在配置阶段通过 `configure_file` 自动生成，路径和 preset 名自动适配当前编译器。

### 命令行烧录

不使用 VS Code 也可以直接命令行烧录（自动先编译）：

```powershell
# Windows
cmake --build .build/at32f403a-armclang --target flash

# Linux
cmake --build .build/at32f403a-gcc --target flash
```

## 配置选项

| 选项 | 默认值 | 说明 |
|------|--------|------|
| `PLATFORM` | （无，必填） | 目标平台，对应 `platform/` 下的目录名 |
| `USE_MICROLIB` | `ON` | ARMCLANG: microlib; GCC: nano.specs |
| `PROJECT_OPT_LEVEL` | `0` | 优化等级（0-3） |
| `ARMCLANG_BIN` | `D:/Keil_v5/ARM/ARMCLANG/Bin`（仅 Win） | ARMCLANG 可执行文件目录 |
| `GCC_BIN` | `D:/Arm-None-EABI/bin`（仅 Win） | GCC 可执行文件目录，Linux 走 PATH |
| `OPENOCD_SCRIPTS_DIR` | `D:/OpenOCD/scripts`（Win）/ 空（Linux） | OpenOCD 脚本目录，Linux 用内置路径 |
| `OPENOCD_EXECUTABLE` | Win: 绝对路径 / Linux: `openocd` | OpenOCD 可执行文件 |

覆盖示例：

```powershell
cmake --preset at32f403a-armclang -DUSE_MICROLIB=OFF -DPROJECT_OPT_LEVEL=2
```

## 构建脚本

`scripts/` 目录提供一键编译脚本，通过芯片型号传参：

| 脚本 | 平台 / 编译器 | 用法 |
|------|---------------|------|
| `build-armclang.bat` | Windows / ARMCLANG | `scripts\build-armclang.bat at32f403a` |
| `build-gcc.bat` | Windows / GCC | `scripts\build-gcc.bat at32f403a` |
| `build-gcc.sh` | Linux / GCC | `./scripts/build-gcc.sh at32f403a` |

脚本逻辑：有 build 目录直接编译，没有则先 `cmake --preset` 配置再编译。

## 添加新平台

以 STM32F103 为例：

### 1. 创建平台目录

```
platform/stm32f103/
├── CMakeLists.txt         # 从 at32f403a/CMakeLists.txt 复制，改 MCU 参数
├── STM32F103xx.svd
├── startup/
│   ├── sources.cmake      # 按编译器选启动文件
│   ├── ARM/               # armasm 语法启动文件
│   └── GCC/               # GNU 语法启动文件 + syscalls.c + sysmem.c
├── vendor/
│   ├── sources.cmake      # 列标准库文件
│   └── ...
├── hal/
│   └── sources.cmake
└── linker/
    ├── armclang.sct       # ARMCLANG 链接散列文件
    └── gcc.ld             # GCC 链接脚本
```

### 2. 修改 platform CMakeLists.txt

按内核查表改对应的值即可。

**编译标志 (MCU_FLAGS)：**

| 内核 | `-mcpu` | `-mfpu` | `-mfloat-abi` |
|------|---------|---------|---------------|
| M4 (带 FPU) | `cortex-m4` | `fpv4-sp-d16` | `hard` |
| M4 (无 FPU) | `cortex-m4` | - | - |
| M3 | `cortex-m3` | - | - |
| M0/M0+ | `cortex-m0` / `cortex-m0plus` | - | - |

> GCC 直接用上面的标志。ARMCLANG 需额外加 `--target=arm-arm-none-eabi`。

**ARMCLANG 链接 `--cpu`：**

| 内核 | `--cpu` |
|------|---------|
| M4 (带 FPU) | `Cortex-M4.fp.sp` |
| M4 (无 FPU) | `Cortex-M4` |
| M3 | `Cortex-M3` |
| M0/M0+ | `Cortex-M0` |

**ARMCLANG 汇编 `--cpu`：**

| 内核 | `--cpu` |
|------|---------|
| M4 (带 FPU) | `Cortex-M4.fp` |
| M4 (无 FPU) | `Cortex-M4` |
| M3 | `Cortex-M3` |
| M0/M0+ | `Cortex-M0` |

> GCC 链接和汇编都用 `MCU_FLAGS`，不需要 `--cpu`。

**芯片相关配置：**

| 配置项 | AT32F403A (示例) | STM32F103 (示例) |
|--------|-------------------|------------------|
| `PROJECT_DEFINES` | `AT32F403ACGT7 USE_STDPERIPH_DRIVER AT_START_F403A_V1` | `STM32F103C8 USE_STDPERIPH_DRIVER` |
| `OPENOCD_TARGET` | `target/at32f403axx.cfg` | `target/stm32f1x.cfg` |
| `SVD_FILE` | `platform/at32f403a/AT32F403Axx.svd` | `platform/stm32f103/STM32F103xx.svd` |

### 3. 添加 CMakePresets 条目

```json
{
    "name": "stm32f103-armclang",
    "displayName": "STM32F103 (ARMCLANG)",
    "generator": "Ninja",
    "binaryDir": "${sourceDir}/.build/stm32f103-armclang",
    "toolchainFile": "${sourceDir}/cmake/toolchain-armclang.cmake",
    "cacheVariables": { "PLATFORM": "stm32f103" }
},
{
    "name": "stm32f103-gcc",
    "displayName": "STM32F103 (GCC)",
    "generator": "Ninja",
    "binaryDir": "${sourceDir}/.build/stm32f103-gcc",
    "toolchainFile": "${sourceDir}/cmake/toolchain-gcc.cmake",
    "cacheVariables": { "PLATFORM": "stm32f103" }
}
```

### 4. 编译

```powershell
cmake --preset stm32f103-armclang
cmake --build .build/stm32f103-armclang
```

无需改顶层 CMakeLists.txt、bsp/、utils/ 或 .vscode/ 模板。

## USB 模块

USB 采用两级条件编译：

```
platform/at32f403a/usb_lib/ 目录存在？
├─ 是 → HAS_PLATFORM_USB=TRUE
│      遍历两个目录的 sources.cmake：
│      1. platform/at32f403a/usb_lib/sources.cmake → 定义 USB_LIB_SOURCE → 编译 usb_lib
│      2. usb/sources.cmake                        → 定义 USB_SOURCE   → 编译 usb
│      有 sources.cmake 就 include，没有就跳过
│
└─ 否 → 整个 USB 链路跳过
```

`usb_lib/` 放芯片厂商 USB 驱动，`usb/` 放外部 USB class（CDC/HID/Audio 等）。H/D 的区分在 `usb/` 内部自行组织目录结构，CMake 只认 `sources.cmake` 里列的文件。

`sources.cmake` 格式：

```cmake
# platform/at32f403a/usb_lib/sources.cmake
set(USB_LIB_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/src/usbd_core.c
    ${CMAKE_CURRENT_LIST_DIR}/src/usbd_int.c
)
```

```cmake
# usb/sources.cmake
set(USB_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/cdc/cdc_class.c
    ${CMAKE_CURRENT_LIST_DIR}/hid/hid_class.c
)
```

## sources.cmake 约定

所有源文件列表通过 `sources.cmake` 管理，不使用 GLOB。每个模块一个文件，CMakeLists.txt 通过 `include()` 引入：

| sources.cmake 位置 | 变量名 | 编译为 |
|---------------------|--------|--------|
| `platform/xxx/vendor/sources.cmake` | `VENDOR_SOURCE` | `vendor` OBJECT 库 |
| `platform/xxx/startup/sources.cmake` | `STARTUP_SOURCE` + `STARTUP_ASM` | `startup` OBJECT 库 |
| `platform/xxx/hal/sources.cmake` | `HAL_SOURCE` | `hal` OBJECT 库 |
| `platform/xxx/usb_lib/sources.cmake` | `USB_LIB_SOURCE` | `usb_lib` OBJECT 库（条件） |
| `bsp/sources.cmake` | `BSP_SOURCE` | `bsp` OBJECT 库 |
| `usb/sources.cmake` | `USB_SOURCE` | `usb` OBJECT 库（条件） |
| `utils/sources.cmake` | `UTILS_SOURCE` | `utils` OBJECT 库 |

格式模板：

```cmake
set(XXX_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/module/module.c
    ${CMAKE_CURRENT_LIST_DIR}/other/other.c
)
```

`startup/sources.cmake` 特殊处理：按编译器选择启动文件，C 文件共用。

```cmake
set(STARTUP_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/system_at32f403a_407.c
    ${CMAKE_CURRENT_LIST_DIR}/at32f403a_407_int.c
)
if(COMPILER_TYPE STREQUAL "GCC")
    set(STARTUP_ASM ${CMAKE_CURRENT_LIST_DIR}/GCC/startup_at32f403a_407.s)
    list(APPEND STARTUP_SOURCE
        ${CMAKE_CURRENT_LIST_DIR}/GCC/syscalls.c
        ${CMAKE_CURRENT_LIST_DIR}/GCC/sysmem.c
    )
else()
    set(STARTUP_ASM ${CMAKE_CURRENT_LIST_DIR}/ARM/startup_at32f403a_407.s)
endif()
```

## Include 路径

顶层 CMakeLists.txt 的 `include_directories()` 对所有子目录生效：

| 路径 | 覆盖头文件 |
|------|-----------|
| `platform/cmsis/` | ARM CMSIS 核心头（core_cm4.h 等） |
| `platform/xxx/startup/` | 设备头（at32f403a_407.h 等） |
| `platform/xxx/vendor/inc/` | 厂商标准库头 |
| `platform/xxx/hal/` | HAL 封装头 |
| `bsp/` | BSP 头（bsp.h / led.h 等） |
| `utils/` | 框架头（task_manager.h 等） |
| `usb/`（条件） | 外部 USB class 头 |
| `platform/xxx/usb_lib/inc/`（条件） | USB 驱动头 |

## 工具链

### ARMCLANG (`cmake/toolchain-armclang.cmake`)

| 变量 | 工具 | 用途 |
|------|------|------|
| `CMAKE_C_COMPILER` | armclang.exe | C 编译 |
| `CMAKE_ASM_COMPILER` | armasm.exe | 汇编编译 |
| `CMAKE_LINKER` | armlink.exe | 链接 |
| `CMAKE_OBJCOPY` | fromelf.exe | 镜像转换（hex/bin 生成） |

### GCC (`cmake/toolchain-gcc.cmake`)

| 变量 | 工具 | 用途 |
|------|------|------|
| `CMAKE_C_COMPILER` | arm-none-eabi-gcc.exe | C 编译 |
| `CMAKE_ASM_COMPILER` | arm-none-eabi-gcc.exe | 汇编编译 |
| `CMAKE_LINKER` | arm-none-eabi-gcc.exe | 链接 |
| `CMAKE_OBJCOPY` | arm-none-eabi-objcopy.exe | 镜像转换 |
| `CMAKE_SIZE` | arm-none-eabi-size.exe | 尺寸报告 |
