#include <iostream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <string>
#include <cstring>

class Grid {
protected:
    int** cells;
    int row_size;
    int col_size;
    bool initialized = false;

public:

    Grid(){}

    Grid(int _row_size, int _col_size) : row_size(_row_size), col_size(_col_size) {
        initialize();
    }

    Grid(const Grid& other) : row_size(other.row_size), col_size(other.col_size), cells(other.cells), initialized(other.initialized) {}

    //析构释放空间
    ~Grid() {
        for(int i = 0; i < row_size; i++) {
            delete[] cells[i];
        }
        cells = nullptr;
    }

    void initialize() {
        //分配空间
        cells = new int*[row_size];
    
        for(int i = 0; i < row_size; i++) {
            cells[i] = new int[col_size];
        }

        for(int i = 0; i < row_size; i++) {
            for(int j = 0; j < col_size; j++) {
                cells[i][j] = 0;
            }
        }
        initialized = true;
    }

    bool isInitialized() { return initialized; }

    Grid& operator=(const Grid& other) {
        //深拷贝
        if (this != &other) {
            
            if(initialized) {
                //释放原空间
                for(int i = 0; i < row_size; i++) {
                    delete[] cells[i];
                }
                cells = nullptr;
            }

            //初始化
            row_size = other.row_size;
            col_size = other.col_size;

            if(other.initialized) {
                initialize();

                //拷贝
                for(int i = 0; i < row_size; i++) {
                    for(int j = 0; j < col_size; j++) {
                        cells[i][j] = other.cells[i][j];
                    }
                }
            }
            else {
                initialized = other.initialized;
            }
        }
        return *this;
    }

    void check_row_col(const int row,  const int col) const {
        if(row < 0 || row >= row_size) throw std::invalid_argument("Invalid row");
        if(col < 0 || col >= col_size) throw std::invalid_argument("Invalid col");
    }

    int serialize(char* dst, int d_offset, int len) {
        int offset = d_offset;
        int int_size = sizeof(int);

        if(offset + (2+row_size*col_size)*int_size >= len && initialized) {
            return 0;
        }

        //将类属性串行化存入dst
        char* data = new char[int_size];
        
        *(int *)data = row_size;
        memcpy(dst + offset, data, int_size);
        offset += int_size;

        *(int *)data = col_size;
        memcpy(dst + offset, data, int_size);
        offset += int_size;

        for(int i = 0; i < row_size; i++) {
            for(int j = 0; j < col_size; j++) {
                *(int *)data = cells[i][j];
                memcpy(dst + offset, data, int_size);
                offset += int_size;
            }
        }
        return offset;
    }

    void deserialize(const char* dst, int d_offset) {

        //从dst中解析出类Grid
    
        int offset = d_offset;
        int int_size = sizeof(int);

        char* data = new char[int_size];

        //解析出row_size
        memcpy(data, dst + offset, int_size);
        row_size = *(int *)data;
        offset += int_size;

        //解析出col_size
        memcpy(data, dst + offset, int_size);
        col_size = *(int *)data;
        offset += int_size;

        //根据row和col初始化
        initialize();

        //解析出矩阵
        for(int i = 0; i < row_size; i++) {
            for(int j = 0; j < col_size; j++) {
                memcpy(data, dst + offset, int_size);
                cells[i][j] = *(int *)data;
                offset += int_size;
            }
        }
    }

    int get_row_size() {
        return initialized? row_size:0;
    }

    int get_col_size() {
        return initialized? col_size:0;
    }

    int get_num(int row, int col) const {
        if(initialized) {
            return cells[row][col];
        }
    }

    void set_num(int row, int col, int num) {
        if(initialized) {
            cells[row][col] = num;
        }
    }

    void print() const {
        for (int i = 0; i < row_size; i++) {
            std::cout << '[';
            for (int j = 0; j < col_size-1; j++) {
                std::cout << cells[i][j] << ", ";
            }
            std::cout << cells[i][col_size-1] << ']' << std::endl;
        }
    }
};

class Sudoku : public Grid {
    int size;

public:

    Sudoku(){}

    Sudoku(int _size) :size(_size), Grid(size, size) {}

    Sudoku(int _size, const std::string& data) :size(_size), Grid(_size, _size) {
        parse(data);
    }

    Sudoku(const Sudoku& other) : Grid(other) {}

    void parse(const std::string& data) {
        if (data.length() != size*size) throw std::invalid_argument("Invalid input length for a Sudoku.");

        for (int i = 0; i < size*size; i++) {
            int value = data[i] - '0';
            if (value < 0 || value > 9) throw std::invalid_argument("Only number 0-9 are allowed.");
            cells[i / size][i % size] = value;
        }
    }

    Sudoku& operator=(const Sudoku& other) {
        Grid::operator=(other);
        return *this;
    }

    void deserialize(const char* dst, int d_offset) {
        Grid::deserialize(dst, d_offset);
        size = row_size;
    }

    //指定行和列进行推理
    std::vector<int> get_candidates(int row, int col) const {

        check_row_col(row, col);

        if(row < 0 || row >= size) throw std::invalid_argument("Invalid row");
        if(col < 0 || col >=size) throw std::invalid_argument("Invalid col");

        if (cells[row][col] != 0)
            return {};

        std::vector<bool> used(size+1, false);
        std::vector<int> candidates;

        int side_len = size/3;

        for (int i = 0; i < size; i++) {
            used[cells[row][i]] = true;
            used[cells[i][col]] = true;
        }

        int boxRowStart = (row / side_len) * side_len;
        int boxColStart = (col / side_len) * side_len;

        for (int r = boxRowStart; r < boxRowStart + side_len; r++) {
            for (int c = boxColStart; c < boxColStart + side_len; c++) {
                used[cells[r][c]] = true;
            }
        }

        for (int num = 1; num <= size; num++) {
            if (!used[num]) {
                candidates.push_back(num);
            }
        }

        return candidates;
    }

    //对所有空位进行推理
    std::vector<std::vector<std::vector<int>>> get_all_candidates() {
        std::vector<std::vector<std::vector<int>>> ans(size);
        for(int i = 0; i < size; i++) {
            for(int j = 0; j < size; j++) {
                if(cells[i][j] == 0) {
                    ans[i].push_back(get_candidates(i,j));
                }
            }
        }
        return ans;
    }
};


int main() {
    std::string input = "017903600000080000900000507072010430000402070064370250701000065000030000005601720";

    std::cout << "--------------get_candidates test--------------\n";
    Sudoku s(9, input);
    s.print();
    s.get_candidates(0,0);
    s.get_candidates(0,4);
    std::vector<std::vector<std::vector<int>>> ans = s.get_all_candidates();

    for(int i = 0; i < 9; i++) {
        std::cout << '[';
        for(int j = 0; j < ans[i].size(); j++) {
            std::cout << '[';
            for(int k = 0; k < ans[i][j].size() - 1; k++) {
                std::cout << ans[i][j][k] << ' ';
            }
            std::cout << ans[i][j][ans[i][j].size() - 1] << "], ";
        }
        std::cout << "]\n";
    }

    std::cout << "\n\n---------------copy test---------------\n";
    Sudoku a(9, input);
    Sudoku b;
    b = a;
    b.print();

    std::cout << "\n\n---------------serialize test---------------\n";
    char* data = new char[1000];
    int offset = 0;
    offset = s.serialize(data, 0, 1000);
    s.print();


    std::cout << "\n\n---------------deserialize test---------------\n";
    offset = 0;
    Sudoku d;
    d.deserialize(data, offset);
    d.print();
    return 0;
};
