//http://akrzemi1.wordpress.com/2012/01/02/type-inference-for-temporaries
//http://talesofcpp.fusionfenix.com/post-8/true-story-i-will-always-find-you
//
//
#include <utility>
#include <memory>

namespace matrix {

typedef unsigned int size_t;

typedef std::pair<size_t, size_t> Point;

template <typename T>
class Matrix
{
    std::unique_ptr<T[]> data_;

public:
    explicit Matrix(size_t clms, size_t rows):data_(new T[clms*rows]) {
    }

    explicit Matrix(size_t sz) : Matrix(sz, sz) {}

    //const T& operator[](Point p) const;
};

template <typename T, size_t M, size_t N>
class MatrixT //: public Matrix<T>
{
    T data_[M * N];

public:
    const T& operator[](Point p) const {
        // assert (p.first < M && p.second < N)
        return data_[p.first * N + p.second];
    }
};


template <typename T, size_t N>
// requires: T is Addable, Regular
T trace( MatrixT<T, N, N> const& mtrx )
{
    T tr{}; // value-initialization
    for (size_t i = 0; i != N; ++i) {
        //ans += mtrx[{i, i}];
        tr += mtrx[Point{i, i}];
    }
    return tr;
}

} // matrix

int main()
{
    matrix::MatrixT<int, 3, 3> mtrx;

    auto t = matrix::trace(mtrx);

    auto m1 = matrix::Matrix<int>{4,4};

    matrix::Matrix<int> m{4,4};

    return 0;
}
