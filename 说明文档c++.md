# Grid与Sudoku说明（C++）

## Grid 类

`Grid` 类作为基类，用于表示由整数值组成的二维网格。这个类提供了基本功能，如初始化、深拷贝、赋值、以及网格数据的序列化和反序列化。

### 构造函数和析构函数

- **Grid()**: 默认构造函数。
- **Grid(int _row_size, int _col_size)**: 用指定的行和列大小初始化一个网格。
- **Grid(const Grid& other)**: 拷贝构造函数，复制一个现有的 `Grid` 实例。
- **~Grid()**: 析构函数，清理动态分配的网格内存。

### 方法

- **void initialize()**: 分配空间并初始化网格单元为 0。
- **bool isInitialized()**: 返回网格是否已正确初始化。
- **void check_row_col(int row, int col) const**: 如果指定的行或列索引超出网格的界限，则抛出异常。
- **int serialize(char* dst, int d_offset, int len)**: 将整个网格序列化为字节数组。
- **void deserialize(const char* dst, int d_offset)**: 从序列化的字节数组中填充网格。
- **int get_row_size()**: 返回网格的行数。
- **int get_col_size()**: 返回网格的列数。
- **int get_num(int row, int col) const**: 访问指定位置的数字。
- **void set_num(int row, int col, int num)**: 在指定位置设置数字。
- **void print() const**: 以人类可读格式输出网格。

### 运算符重载

- **Grid& operator=(const Grid& other)**: 赋值运算符重载，用于从另一个实例复制网格数据，进行深拷贝。

## Sudoku 类

`Sudoku` 类从 `Grid` 派生，专门用于数独游戏。它继承了 `Grid` 的所有功能，并添加了通过字符串解析和推理数独所需的特定方法。

### 构造函数

- **Sudoku()**: 默认构造函数。
- **Sudoku(int _size)**: 初始化一个大小为 `_size` 的数独网格。
- **Sudoku(int _size, const std::string& data)**: 初始化一个数独网格并使用提供的字符串填充它。
- **Sudoku(const Sudoku& other)**: 拷贝构造函数。

### 方法

- **void parse(const std::string& data)**: 从字符串解析数独的数据，确保字符串长度正确并且只含数字0-9。
- **Sudoku& operator=(const Sudoku& other)**: 赋值运算符重载。
- **void deserialize(const char* dst, int d_offset)**: 从序列化数据反序列化数独网格，并更新大小。
- **std::vector<int> get_candidates(int row, int col) const**: 获取给定格子的候选数字列表。
- **std::vector<vector<vector<int>>> get_all_candidates()**: 对所有空格进行推理，获取整个数独的候选数字列表。

### 主函数详解

在主函数中，测试了数独的功能，包括：

- 初始化数独
- 打印数独格局
- 获取特定格子和所有空格的候选数字列表
- 深拷贝测试
- 序列化和反序列化功能测试

## 注意事项

- 在解析数独时，输入字符串的长度必须严格等于数独大小的平方。
- 数独的序列化数据包含行数、列数和数独数据，反序列化时需确保数据的完整性以正确还原数独状态。