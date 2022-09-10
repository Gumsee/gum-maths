#pragma once
#include "vec.h"
#include "GumMathsClass.h"
#include <iostream>
#include <string>

template<typename T, unsigned int N, unsigned int M>
struct mat
{
    T v[M][N];
    
    mat()
    {
        for(unsigned int i = 0; i < M; i++)
            for(unsigned int j = 0; j < N; j++)
                v[i][j] = i == j ? T(1) : T(0);
    }

    mat(T const& f)
    {
        for(unsigned int i = 0; i < M; i++)
            for(unsigned int j = 0; j < N; j++)
                v[i][j] = f;
    }

    /*template<typename TT, unsigned int NN, unsigned int MM>
    mat(mat<TT, NN, MM> const& m)
    {
        for(unsigned int i = 0; i < (N > NN ? NN : N); i++)
            for(unsigned int j = 0; j < (M > MM ? MM : M); j++)
                v[i][j] = m[i][j];
    }*/

    template <typename... Args, typename = typename std::enable_if<sizeof...(Args) == N * M>::type>
    explicit mat(Args&&... values) 
    {
        //static_assert(is_all_same<T, values...>::value, "Arguments must be int.");
        T args[] = { static_cast<T>(values)... };
        memcpy(&v[0], &args[0], M * N * sizeof(T));
        this->operator=(transpose(*this));
    }

    /**
     * multiplies matrix 2x4 with a matrix 4x2
     *                   |  1  2 |   
     * |  1  2  3  4 | x |  5  6 | = |  90 100 |
     * |  5  6  7  8 |   |  9 10 |   | 202 228 |
     *                   | 13 14 |  
     *       NxM           NNxMM   =     NxMM
     * @param mat
     * @return matrix product
     */
    template<typename TT, unsigned int NN, unsigned int MM>
    mat<T, N, MM> operator*(mat<TT, NN, MM> const& m)
    {
        static_assert(M == NN, "Matrices can't be multiplied!");
        mat<T, N, MM> tmpMat(T(0));
        for(unsigned int i = 0; i < MM; i++)
            for(unsigned int j = 0; j < N; j++)
                for(unsigned int k = 0; k < M; k++)
                    tmpMat[i][j] = tmpMat[i][j] + v[k][j] * m[i][k];
        return tmpMat;
    }
    template<typename TT, unsigned int NN, unsigned int MM>
    void operator*=(mat<TT, NN, MM> const& m)
    {
        static_assert(M == NN, "Matrices can't be multiplied!");
        mat<T, N, MM> tmpMat(T(0));
        for(unsigned int i = 0; i < MM; i++)
            for(unsigned int j = 0; j < N; j++)
                for(unsigned int k = 0; k < M; k++)
                    tmpMat[i][j] = tmpMat[i][j] + v[k][j] * m[i][k];
        
        this->operator=(tmpMat);
    }

    /**
     * multiplies matrix with a floating point value
     * | 1 0 0 0 |       | 5 0 0 0 |
     * | 0 1 0 0 | x 5 = | 0 5 0 0 |
     * | 0 0 1 0 |       | 0 0 5 0 |
     * | 0 0 0 1 |       | 0 0 0 5 |
     * @param val
     */
    template<typename TT>
    mat<T, N, M> operator*(TT const& f)
    {
        mat<T, N, M> retmat(0);
        for(unsigned int i = 0; i < M; i++)
            for(unsigned int j = 0; j < N; j++)
                retmat[i][j] = v[i][j] * f;
        return retmat;
    }
    template<typename TT>
    void operator*=(TT const& f)
    {
        for(unsigned int i = 0; i < M; i++)
            for(unsigned int j = 0; j < N; j++)
                v[i][j] *= f;
    }

    /**
     * multiplies matrix (NxM) with a vector (Mx1)
     * | 1 0 0 0 |   | x |   |  x |
     * | 0 2 0 0 | x | y | = | 2y |
     * | 0 0 3 0 |   | z |   | 3z |
     * | 0 0 0 4 |   | w |   | 4w |
     * @param val
     * @return product vector (Nx1)
     */
    template<typename TT>
    tvec<TT, N> operator*(tvec<TT, M> const& vvec)
    {
        tvec<TT, N> retvec;
        for(unsigned int i = 0; i < N; i++)
            for(unsigned int j = 0; j < M; j++)
                retvec[i] += vvec[j] * v[j][i];
        
        return retvec;
    }

    template<typename TT, unsigned int NN, unsigned int MM>
    void operator=(mat<TT, NN, MM> const& m)
    {
        for(unsigned int i = 0; i < (N < NN ? N : NN); i++)
            for(unsigned int j = 0; j < (M < MM ? M : MM); j++)
                v[i][j] = (T)m[i][j];
    }

    T* operator[](unsigned int index)          { return v[index]; }
    const T* operator[](unsigned int index) const  { return v[index]; }


    mat<T, N, M> cofactors(const unsigned int& p, const unsigned int& q)
    {
        mat<T, N, M> retmat(0);
        int i = 0, j = 0;
        if(N != M)
            std::cerr << "GumMaths: Matrix cofactors failed: wrong dimensions" << std::endl;
    
        // Looping for each element of the matrix
        for (unsigned int row = 0; row < N; row++)
        {
            for (unsigned int col = 0; col < N; col++)
            {
                //  Copying into temporary matrix only those element
                //  which are not in given row and column
                if (row != p && col != q)
                {
                    retmat[i][j++] = v[row][col];
    
                    // Row is filled, so increase row index and
                    // reset col index
                    if (j == N - 1)
                    {
                        j = 0;
                        i++;
                    }
                }
            }
        }
    }

    T determinant()
    {
        T det = 0;
        if(N == M)
        {
            for(unsigned int i = 0; i < N; i++)
            {
                T addRow = 1;
                T subRow = 1;
                for(unsigned int j = 0; j < N; j++)
                {
                    int k = j + i;
                    k = (k >= (int)N ? k - N : k);
                    addRow *= v[k][j];
                }
                det += addRow;

                for(unsigned int j = N - 1; j >= 0; j--)
                {
                    int k = (N - j) + i;
                    k = (k >= (int)N ? k - N : k);
                    subRow *= v[k][j];
                }
                det -= subRow;
            }
        }
        return det;
    }


    /**
     * inverts the axis' of a 4x4 matrix
     *                           | 1 1 |
     * transpose (| 1 2 3 4 |) = | 2 2 |
     *            | 1 2 3 4 |    | 3 3 |
     *                           | 4 4 |
     * @return itself
     */
    template<typename TT, unsigned int NN, unsigned int MM>
    static mat<TT, MM, NN> transpose(mat<TT, NN, MM> const& m)
    {
        mat<TT, MM, NN> tmpMat;
        for(unsigned int i = 0; i < MM; i++)
            for(unsigned int j = 0; j < NN; j++)
                tmpMat[j][i] = m[i][j];
        return tmpMat;
    }

    template<typename TT, unsigned int NN, unsigned int MM>
    static mat<TT, NN, MM> inverse(mat<TT, NN, MM> const& m)
    {
        static_assert(MM == NN, "Matrix has no inverse!");
        if(NN == 4 && MM == 4)
        {
            mat<TT, NN, MM> retmat(0), inv(0);

            inv[0][0] = m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[2][1] * m[1][2] * m[2][2] - m[2][1] * m[1][2] * m[2][1];
            inv[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
            inv[2][0] = m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
            inv[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];
            inv[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
            inv[1][1] = m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
            inv[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
            inv[3][1] = m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
            inv[0][2] = m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
            inv[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
            inv[2][2] = m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
            inv[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
            inv[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
            inv[1][3] = m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
            inv[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
            inv[3][3] = m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

            float det = m[0][0] * inv[0][0] + m[0][1] * inv[1][0] + m[0][2] * inv[2][0] + m[0][3] * inv[3][0];

            if (det == 0)
                return m; //There's no inverse

            det = 1.0 / det;

            for (unsigned int i = 0; i < MM; i++)
                for (unsigned int j = 0; j < NN; j++)
                    retmat[i][j] = inv[i][j] * det;

            return retmat;
        }
        else
        {
            /*TT determinant = m.determinant();
            if(determinant == 0)
                return m;
            
            mat<TT, NN, MM> retmat(0);
            for(int i = 0; i < NN; i++)
                for(int j = 0; j < NN; j++)
                    retmat[i][j] = m[i][j] / determinant;
                    
            return retmat;*/

            //TODO
            return m;
        }
    }

    std::string toString(const bool& oneline = true, const std::string& prefix = "mat"+std::to_string(N)+"x"+std::to_string(M)+"(", const std::string& suffix = ")", const std::string& delimiter = ", ")
    {
        std::string output = prefix;
        if(!oneline)
            output += "\n";
        for(unsigned int i = 0; i < N; i++)
        {
            if(!oneline)
                output += "    ";
            for(unsigned int j = 0; j < M; j++)
            {
                if(j + i * M < M * N - 1)
                    output += std::to_string(v[j][i]) + delimiter;
                else
                    output += std::to_string(v[j][i]);
            }
            if(!oneline)
                output += "\n";
        }
        output += suffix;
        return output;
    }

    unsigned int width() 
    {
        return M;
    }

    unsigned int height() 
    {
        return N;
    }
};

typedef mat<float,  2, 2>  mat2;
typedef mat<int,    2, 2> imat2;
typedef mat<double, 2, 2> dmat2;
typedef mat<float,  3, 3>  mat3;
typedef mat<int,    3, 3> imat3;
typedef mat<double, 3, 3> dmat3;
typedef mat<float,  4, 4>  mat4;
typedef mat<int,    4, 4> imat4;
typedef mat<double, 4, 4> dmat4;