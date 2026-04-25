# Aerozen 代码风格与格式化约定

文档索引见 [docs/README.md](README.md)。

本文档归纳本仓库在 `aerozen/` 下已对齐的 C++ 风格（参考 `topic_statistics`、`topic_storage`、`wait_helpers` 等文件的写法），供新增与修改代码时保持一致。

---

## 1. 命名空间与大括号

- 命名空间左大括号与名称同一行：`namespace aerozen {`。
- 文件末尾闭合时加尾注释：`}  // namespace aerozen`。

## 2. 头文件与 Include

- **Include guard**：宏名与头文件对应，且 `#ifndef`、`#define`、`#endif` 注释中的名称一致，例如 `AEROZEN_WAITHELPERS_HPP_`。
- **Include 顺序**：用空行分组，常见顺序为：
  - C / POSIX 头（如 `<unistd.h>`）
  - C++ 标准库（`<chrono>`、`<string>` 等）
  - 第三方库
  - 本项目头文件（`"aerozen/..."`）
- 同组内可按字母或依赖关系排序，保持文件内一致即可。

## 3. 注释（Doxygen 风格）

对外 API、类与重要成员优先使用块注释，标签统一为 **`@`** 前缀（与 `topic_storage.hpp`、`topic_statistics.hpp` 一致）：

```cpp
/**
 * @brief 一句话说明用途。
 * 可选的补充说明段落。
 * @param[in] _name 参数说明。
 * @return 返回值说明。
 * @note 可选注意事项。
 */
```

- 避免在公开接口上混用 `/// \brief` 与 `/** @brief */` 两套风格。
- 文件头版权与许可证块保持仓库现有 Apache 2.0 模板；块注释结尾行使用 ` */`（与现有文件一致）。

## 4. 类型与引用 / 指针写法

- `const` 引用：**`const T& _name`**，`&` 紧贴类型名，不要写成 `const T &_name`。

## 5. 命名约定

- **`namespace aerozen` 中的对外 API**：采用 **PascalCase**，例如 `WaitForShutdown`、`WaitUntil`、`WaitForService`、`WaitForTopic`。
- **内部实现细节**（如匿名命名空间内的静态函数）：可使用小写加下划线，例如 `signal_handler`，与对外 API 区分。
- **Gazebo / `gz::transport`** 的符号与命名遵循上游库，不要与 `aerozen::` 混用或强行改成同一套命名。

## 6. 声明、调用与 Lambda 排版

- 参数较多或默认实参较长时：**函数名独占一行，参数每行一项，逗号在行尾**，缩进对齐。
- Lambda 体与 `[]` 之间留空格：`[] { return true; }`，`[&]() { ... }` 等与项目现有测试、实现保持一致。

## 7. 源文件分段（可选）

在 `.cpp` 中可用一行分隔符区分逻辑块（与 `topic_statistics.cpp` 等一致），例如：

```text
//////////////////////////////////////////////////
```

## 8. 与第三方库的边界

- **`aerozen::`**：本仓库提供的 API 与类型。
- **`gz::transport::`** 等：外部依赖；调用处保持其原有命名（如 `gz::transport::waitForShutdown`），除非项目层面有明确的适配层再统一封装。

---

## 修订说明

若本文档与某段存量代码不一致，以**最近整理过、且与上述规则一致**的模块为准；后续修改存量文件时，可顺带向本约定靠拢，但避免无关的大范围重排（单次 PR 尽量聚焦功能或明确风格迁移）。
