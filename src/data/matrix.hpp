#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <initializer_list>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <thread>
#include <future>
#include <fstream>
#include <cstring>
#include <vector>
#include <iostream>

namespace s21 {

#define DEFAULT_COLS_VAL 3

enum DegubType {
    AsArray,
    AsMatrix
};

template <typename T = double, typename Alloc = std::allocator<T>>
class Matrix {
public:

    Matrix(size_t rows = 1, size_t cols = 1)
        : data_(nullptr)
        , size_(0)
        , capacity_(rows * cols)
        , cols_val_(cols)
    {
        AllocateMatrix();
        FillWithZeros();
    }

    Matrix(std::initializer_list<T> const &items)
        : data_(nullptr)
        , size_(0)
        , capacity_(items.size())
        , cols_val_(DEFAULT_COLS_VAL)
    {
        AllocateMatrix();
        for(auto it : items) Push(it);
    }

    Matrix(std::vector<T> arr)
        : data_(nullptr)
        , size_(0)
        , capacity_(arr.capacity())
        , cols_val_(DEFAULT_COLS_VAL)
    {
        AllocateMatrix();
        for(auto it : arr) Push(it);
    }

    Matrix(const Matrix& other) 
        : data_(nullptr)
        , size_(other.size_)
        , capacity_(other.capacity_)
        , cols_val_(other.cols_val_)
    {
        CopyMartix(other);
    }

    Matrix(Matrix &&other) noexcept
        : data_(other.data_)
        , size_(other.size_)
        , capacity_(other.capacity_)
        , cols_val_(other.cols_val_)
        , alloc_(other.alloc_)
    {
        other.data_ = nullptr;
        other.size_ = other.capacity_ = other.cols_val_ = 0;
    }

    Matrix& operator=(const Matrix& other) {
        if(this != &other) {
            CopyMartix(other);
        }
        return *this;
    }

    Matrix& operator=(Matrix&& other) noexcept {
        if(this != &other) {
            Clear();
            Matrix nw(std::move(other));
            Swap(nw);
        }
        return *this;
    }

    ~Matrix() { DeallocateMatrix(); }

    void Push(T val) {
        if(capacity_ == size_) capacity_ == 0 ? Reserve(2) : Reserve(capacity_ << 1);
        std::allocator_traits<Alloc>::construct(alloc_, data_ + size_++, std::move_if_noexcept(val));
    }

    inline void PushVector(std::vector<T> vec) { for(auto it : vec) Push(it); }

    void Erase(size_t indx) {
        if(indx < size_) {
            T* nw = std::allocator_traits<Alloc>::allocate(alloc_, capacity_);
            size_t i = 0, z = 0;
            try
            {
                for(; i < size_; ++i) {
                    if(i != indx)
                        std::allocator_traits<Alloc>::construct(alloc_, nw + z++, std::move_if_noexcept(data_[i]));
                }
            }
            catch(...)
            {
                for(z = 0; z < i; ++z) {
                    std::allocator_traits<Alloc>::destroy(alloc_, nw + z);
                }
                std::allocator_traits<Alloc>::deallocate(alloc_, nw, capacity_);
                DeallocateMatrix();
                throw;
            }
            DeallocateMatrix();
            --size_;
            data_ = nw;
            nw = nullptr;
        }
    }

    void Insert(T val, size_t indx) {
        if(indx < size_) {
            T* nw = std::allocator_traits<Alloc>::allocate(alloc_, capacity_ + 1);
            size_t i = 0, z = 0;
            try {
                for(; i < size_; ++i) {
                    if(i == indx)
                        std::allocator_traits<Alloc>::construct(alloc_, nw + z++, val);
                    std::allocator_traits<Alloc>::construct(alloc_, nw + z++, std::move_if_noexcept(data_[i]));
                }
            } catch(...) {
                for(; z > 0; --z) {
                    std::allocator_traits<Alloc>::destroy(alloc_, nw + z);
                }
                std::allocator_traits<Alloc>::destroy(alloc_, nw);
                std::allocator_traits<Alloc>::deallocate(alloc_, nw, capacity_ + 1);
                DeallocateMatrix();
                throw;
            }
            DeallocateMatrix();
            ++size_;
            ++capacity_;
            data_ = nw;
            nw = nullptr;
        }
    }

    void Reserve(size_t size) {
        if(size > capacity_) {
            T* nw = std::allocator_traits<Alloc>::allocate(alloc_, size);
            size_t i = 0;
            try
            {
                for(; i < size_; ++i) {
                    std::allocator_traits<Alloc>::construct(alloc_, nw + i, std::move_if_noexcept(data_[i]));
                }
            }
            catch(...)
            {
                for(size_t z = 0; z < i; ++z) {
                    std::allocator_traits<Alloc>::destroy(alloc_, nw + z);
                }
                std::allocator_traits<Alloc>::deallocate(alloc_, nw, size);
                DeallocateMatrix();
                throw;
            }
            DeallocateMatrix();
            capacity_ = size;
            data_ = nw;
            nw = nullptr;
        }
    }

    void LoadVectorData(std::vector<T> vec) {
        Clear();
        Reserve(vec.capacity());
        for(auto it : vec) Push(it);
    }

    void FillWithRandomNumbers(size_t minval = 0, size_t maxval = 100) {
        srand(clock());
        for(size_t i = 0; i < size_; ++i)
            std::allocator_traits<Alloc>::destroy(alloc_, data_ + i);
        for(size_t i = 0; i < capacity_; ++i) {
            std::allocator_traits<Alloc>::construct(alloc_, data_ + i, T(rand() % (maxval + 1) + minval));
        }
        size_ = capacity_;
    }

    void ResetValues() {
        for(size_t i = 0; i < size_; ++i)
            std::allocator_traits<Alloc>::destroy(alloc_, data_ + i);
        for(size_t i = 0; i < size_; ++i)
            std::allocator_traits<Alloc>::construct(alloc_, data_ + i, T());
    }

    void FillWithZeros() {
        for(size_t i = 0; i < size_; ++i)
            std::allocator_traits<Alloc>::destroy(alloc_, data_ + i);
        for(size_t i = 0; i < capacity_; ++i)
            std::allocator_traits<Alloc>::construct(alloc_, data_ + i, T());
        size_ = capacity_;
    }

    void Clear() {
        DeallocateMatrix();
        size_ = capacity_ = 0;
    }

    void Swap(Matrix& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(cols_val_, other.cols_val_);
        std::swap(alloc_, other.alloc_);
    }

    inline size_t Rows() const noexcept { return cols_val_ == 0 ? 0 : size_ / cols_val_; }

    inline size_t Cols() const noexcept { return cols_val_; }

    inline size_t Size() const noexcept { return size_; }

    inline size_t Capacity() const noexcept { return capacity_; }

    void SetColsSize(size_t size) noexcept { if(size > 0) cols_val_ = size; }

    void Debug(DegubType type = AsMatrix) {
        if(type == AsMatrix) {
            printf("\n-------------------------------------------------------------\n");
            for(size_t i = 0; i < size_; ++i) {
                if(i % cols_val_ == 0 && i != 0) printf("\n");
                std::cout << data_[i] << " ";
            }
            printf("\n-------------------------------------------------------------\n");
        } else {
            printf("\n-------------------------------------------------------------\n");
            for(size_t i = 0; i < size_; ++i) {
                std::cout << data_[i] << " ";
            }
            printf("\n-------------------------------------------------------------\n");
        }
        
    }

    void SumMatrix(const Matrix& other) {
        if(SimilarMatrix(other)) {
            for(size_t i = 0; i < size_; ++i) {
                data_[i] += other.data_[i];
            }
        }
    }

    void SubMatrix(const Matrix& other) {
        if(SimilarMatrix(other)) {
            for(size_t i = 0; i < size_; ++i) {
                data_[i] -= other.data_[i];
            }
        }
    }

    void MulMatrix(Matrix& other) {
        if(Cols() == other.Rows()) {
            Size() >= 100 ? LargeMatrixMult(other) : SmallMatrixMult(other);
        }
    }

    void MulNumber(const T& val) {
        for(size_t i = 0; i < size_; ++i) data_[i] *= val;
    }

    bool EqMatrix(const Matrix& other) const noexcept {
        if(SimilarMatrix(other)) {
            for(size_t i = 0; i < size_; ++i) {
                if(std::fabs(data_[i] - other.data_[i]) > 1E-7) return false;
            }
            return true;
        }
        return false;
    }

    s21::Matrix<T> Transpose() {
        s21::Matrix<> nw(Cols(), Rows());
        for(size_t i = 0; i < Cols(); ++i) {
            for(size_t z = 0; z < Rows(); ++z) {
                nw.Push((*this)(z, i));
            }
        }
        return nw;
    }

    void SaveMatrix(std::string path) {
        std::ofstream fout;
        fout.open(path);
        if(fout.is_open()) {
            fout << "cols:" << Cols() << "\n";
            for(size_t i = 0; i < Rows(); ++i) {
                for(size_t z = 0; z < Cols(); ++z) {
                    fout << (*this)(i, z) << " ";
                }
                fout << "\n";
            }

            fout.close();
        }
    }

    s21::Matrix<T> LoadMatrix(std::string path) {
        std::ifstream fin;
        std::vector<T> vec;
        s21::Matrix<T> m;
        fin.open(path);
        if(fin.is_open()) {
            std::string str;
            while(std::getline(fin, str)) {
                if(!std::isdigit(str.data()[0])) m.SetColsSize(ParseColsVal(str));
                else ParseString(str, &vec);
            }
            m.LoadVectorData(vec);
        }
        return m;
    }

    inline bool IsEmpty() const noexcept { return size_ == 0; }

    // Array representation
    inline T& operator[](size_t indx) {
        return indx > size_ ? data_[0] : data_[indx];
    }

    // Matrix representation
    T& operator()(size_t i, size_t z) {
        size_t pos = i * cols_val_ + z;
        return pos > size_ ? data_[0] : data_[pos];
    }

    inline bool operator==(const Matrix& other) const noexcept { return EqMatrix(other); }

    inline bool operator!=(const Matrix& other) const noexcept { return !EqMatrix(other); }

    Matrix operator+(const Matrix& other) {
        Matrix<> nw(*this);
        nw.SumMatrix(other);
        return nw;
    }

    Matrix& operator+=(const Matrix& other) {
        SumMatrix(other);
        return *this;
    }

    Matrix operator-(const Matrix& other) {
        Matrix<> nw(*this);
        nw.SubMatrix(other);
        return nw;
    }

    Matrix& operator-=(const Matrix& other) {
        SubMatrix(other);
        return *this;
    }

    Matrix operator*(Matrix& other) {
        Matrix<> nw(*this);
        nw.MulMatrix(other);
        return nw;
    }

    Matrix operator*(const T& val) {
        Matrix<> nw(*this);
        nw.MulNumber(val);
        return nw;
    }

    friend Matrix operator*(T num, Matrix& matrix) {
        matrix.MulNumber(num);
        return matrix;
    }

    Matrix& operator*=(Matrix& other) {
        MulMatrix(other);
        return *this;
    }

    Matrix& operator*=(const T& val) {
        MulNumber(val);
        return *this;
    }

    friend Matrix& operator*=(T num, Matrix& matrix) {
        matrix.MulNumber(num);
        return matrix;
    }

private:
    T* data_;
    size_t size_;
    size_t capacity_;
    size_t cols_val_;
    Alloc alloc_;

    inline void AllocateMatrix() { data_ = std::allocator_traits<Alloc>::allocate(alloc_, capacity_); }
    
    void DeallocateMatrix() {
        if(data_) {
            for(size_t i = 0; i < size_; ++i) {
                std::allocator_traits<Alloc>::destroy(alloc_, data_ + i);
            }
            std::allocator_traits<Alloc>::deallocate(alloc_, data_, capacity_);
            data_ = nullptr;
        }
    }

    void CopyMartix(const Matrix& other) {
        DeallocateMatrix();
        capacity_ = other.capacity_;
        size_ = other.size_;
        cols_val_ = other.cols_val_;
        AllocateMatrix();
        std::uninitialized_copy(other.data_, other.data_ + other.size_, data_);
    }

    inline bool SimilarMatrix(const Matrix& other) const noexcept { return size_ == other.size_ && cols_val_ == other.cols_val_; }

    void SmallMatrixMult(Matrix& other) {
        s21::Matrix<T> res(Rows(), other.Cols());
        for(size_t i = 0; i < cols_val_; ++i) {
            for(size_t z = 0; z < cols_val_; ++z) {
                for(size_t y = 0; y < cols_val_; ++y) {
                    res(i, y) += (*this)(i, z) * other(z, y);
                }
            }
        }
        *this = res;
    }

    void LargeMatrixMult(Matrix& other) {
        s21::Matrix<T> res(Rows(), other.Cols());
        size_t nthreads = std::min(cols_val_, static_cast<size_t>(std::thread::hardware_concurrency()));
        size_t rows_per_thread = cols_val_ / nthreads;
        std::vector<std::future<void>> tasks;
        for(size_t t = 0; t < nthreads; ++t) {
            tasks.emplace_back(std::async(std::launch::async, [&](size_t from, size_t to) {
                for(size_t i = from; i < to; ++i) {
                    for(size_t z = 0; z < cols_val_; ++z) {
                        for(size_t y = 0; y < cols_val_; ++y) {
                            res(i, y) += (*this)(i, z) * other(z, y);
                        }
                    }
                }
            }, t * rows_per_thread, (t == nthreads - 1 ? cols_val_ % nthreads : 0) + (t + 1) * rows_per_thread));
        }
        for(auto &it : tasks) it.wait();
        *this = res;
    }

    void ParseString(std::string str, std::vector<T>* vec) {
        char *lex = std::strtok(str.data(), " ");
        while(lex != NULL) {
            vec->push_back(T(std::atof(lex)));
            lex = std::strtok(NULL, " ");
        }
    }

    size_t ParseColsVal(std::string str) {
        std::string temp;
        for(size_t i = 0; i < str.length(); ++i) {
            if(std::isdigit(str[i])) temp.push_back(str[i]);
        }
        return size_t(std::atoi(temp.data()));
    }

}; // Matrix

}; // namespace s21

#endif // MATRIX_HPP