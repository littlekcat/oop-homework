class Grid:
    def __init__(self, row_size=0, col_size=0):
        self.row_size = row_size
        self.col_size = col_size
        self.initialized = False
        self.cells = []
        if row_size and col_size:
            self.initialize()

    #初始化数组
    def initialize(self):
        self.cells = [[0 for _ in range(self.col_size)] for _ in range(self.row_size)]
        self.initialized = True

    #串行化
    def serialize(self):
        if not self.initialized:
            return None
        data = [self.row_size, self.col_size]
        for row in self.cells:
            data.extend(row)
        return data

    #反串行化
    def deserialize(self, data):
        it = iter(data)
        self.row_size = next(it)
        self.col_size = next(it)
        self.initialize()
        for i in range(self.row_size):
            for j in range(self.col_size):
                self.cells[i][j] = next(it)

    def get_num(self, row, col):
        if self.initialized:
            return self.cells[row][col]
        return None

    def set_num(self, row, col, num):
        if self.initialized:
            self.cells[row][col] = num

    def print(self):
        for row in self.cells:
            print(row)


class Sudoku(Grid):
    def __init__(self, size=0, data=""):
        super().__init__(size, size)
        self.size = size
        if data:
            self.parse(data)

    def parse(self, data):
        if len(data) != self.size * self.size:
            raise ValueError("Invalid input length for a Sudoku.")
        self.initialize()
        for i, char in enumerate(data):
            value = int(char)
            if not (0 <= value <= 9):
                raise ValueError("Only numbers 0-9 are allowed.")
            self.cells[i // self.size][i % self.size] = value

    def deserialize(self, data):
        super().deserialize(data)
        self.size = self.row_size

    def get_candidates(self, row, col):
        if row < 0 or row >= self.size:
            raise ValueError("Invalid row")
        if col < 0 or col >= self.size:
            raise ValueError("Invalid col")

        if self.cells[row][col] != 0:
            return []

        used = [False] * (self.size + 1)
        candidates = []

        side_len = self.size // 3

        #检查行和列
        for i in range(self.size):
            used[self.cells[row][i]] = True
            used[self.cells[i][col]] = True

        # Mark all used numbers in the subgrid
        box_row_start = (row // side_len) * side_len
        box_col_start = (col // side_len) * side_len

        for r in range(box_row_start, box_row_start + side_len):
            for c in range(box_col_start, box_col_start + side_len):
                used[self.cells[r][c]] = True

        #检查宫格
        for num in range(1, self.size + 1):
            if not used[num]:
                candidates.append(num)

        return candidates

    def get_all_candidates(self):
        ans = [[[self.get_candidates(i, j) for j in range(self.size) if self.cells[i][j] == 0] for i in range(self.size)]]
        return ans


def main():
    print("-------------------get_all_candidates test-------------------")
    input_data = "017903600000080000900000507072010430000402070064370250701000065000030000005601720"
    sudoku = Sudoku(9, input_data)
    sudoku.print()
    print(sudoku.get_all_candidates())

    print("\n\n-------------------serialize test-------------------")
    serialized_data = sudoku.serialize()
    print("Serialized:", serialized_data)

    print("\n\n-------------------deserialize test-------------------")
    new_sudoku = Sudoku()
    new_sudoku.deserialize(serialized_data)
    new_sudoku.print()

if __name__ == "__main__":
    main()