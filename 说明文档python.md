# **Grid与Sudoku说明（python）**

### Grid 类

#### 属性

- `row_size`: 行数
- `col_size`: 列数
- `initialized`: 是否已初始化
- `cells`: 表格的数据存储，初始化为二维数组

#### 方法

- `__init__(self, row_size=0, col_size=0)`: 构造函数，可指定行和列，自动调用初始化。
- `initialize(self)`: 初始化`cells`为指定大小的零矩阵。
- `serialize(self)`: 将表格数据序列化为列表，包含行、列和表格数据。
- `deserialize(self, data)`: 从序列化数据恢复表格状态。
- `copy(self, grid)`: 类的深拷贝
- `get_row_size(self)`: 返回网格的行数。
- `get_col_size(self)`: 返回网格的列数。
- `get_num(self, row, col)`: 获取指定单元格的数值。
- `set_num(self, row, col, num)`: 设置指定单元格的数值。
- `print(self)`: 打印当前表格。



### Sudoku 类

继承于`Grid`类，用于实现并管理数独特有的功能和数据。

#### 属性

- `size`: 数独边长

#### 方法

- `__init__(self, size=0, data="")`: 构造函数，可以初始化数独大小以及直接通过字符串解析初始数独状态。
- `parse(self, data)`: 解析字符串格式数据填充数独。
- `deserialize(self, data)`: 覆盖基类方法，反序列化数独数据，同时更新数独大小。
- `get_candidates(self, row, col)`: 获取指定单元格可用的数字候选。
- `get_all_candidates(self)`: 获取所有单元格的数字候选。



### 主函数详解

在主函数中，测试了数独的功能，包括：

- 初始化数独
- 打印数独格局
- 深拷贝测试
- 获取特定格子和所有空格的候选数字列表
- 序列化和反序列化功能测试



## 注意事项

- 在解析数独时，输入字符串的长度必须严格等于数独大小的平方。
- 数独的序列化数据包含行数、列数和数独数据，反序列化时需确保数据的完整性以正确还原数独状态。