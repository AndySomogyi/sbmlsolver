//
// Created by Ciaran on 08/07/2021.
//

#ifndef ROADRUNNER_MATRIX3D_H
#define ROADRUNNER_MATRIX3D_H

#include "Matrix.h"

namespace rr {

    template<typename IndexType, typename DataType>
    class Matrix3D {
    public:

        Matrix3D() = default;

        Matrix3D(std::initializer_list <IndexType> idx, std::initializer_list <std::initializer_list<DataType>> data)
                :
                index_(idx.begin(), idx.end()),
                data_(data.begin(), data.end()) {}

        void insert(IndexType idx, Matrix<DataType> mat ){
            index_.push_back(idx);
            data_.push_back(mat);
        }



    public:
        std::vector <IndexType> index_;
        std::vector <DataType> data_;
    };
}

#endif //ROADRUNNER_MATRIX3D_H
