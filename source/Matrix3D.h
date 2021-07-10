//
// Created by Ciaran on 08/07/2021.
//

#ifndef ROADRUNNER_MATRIX3D_H
#define ROADRUNNER_MATRIX3D_H

#include "Matrix.h"

namespace rr {

    template<typename IndexType, typename DataType>
    class Matrix3D {
        using Matrix3DInitializer = std::initializer_list<std::initializer_list<std::initializer_list<DataType>>>;
    public:

        /**
         * @brief default construct a 3D matrix
         */
        Matrix3D() = default;

        /**
         * @brief construct an empty 3D matrix with numRows x numCols x numZ dimensions.
         */
        Matrix3D(int numRows, int numCols, int numZ)
                : index_(std::vector<IndexType>(numZ)),
                  data_(std::vector<Matrix<DataType>>(numZ)) {
            for (int i = 0; i < numZ; i++) {
                data_[i].resize(numRows, numCols);
            }
        }

        Matrix3D(std::initializer_list<IndexType> idx, Matrix3DInitializer data)
                : index_(idx.begin(), idx.end()), data_(data.begin(), data.end()) {
            if (index_.size() != data.size()) {
                throw std::logic_error("The size of index != size of 3D data in Matrix3D initializer list. ");
            }
        }

        void insert(IndexType idx, Matrix<DataType> mat) {
            // enforce unique indexes
            if (std::find(index_.begin(), index_.end(), idx) != index_.end()) {
                std::ostringstream err;
                err << "requested insertion of element with index \"" << idx;
                err << "\" into Matrix3D but an element with this index already exists." << std::endl;
                throw std::invalid_argument(err.str());
            }
            if (!index_.empty()) {
                // All sizes are compared to index 0, so when initializing
                // from empty, we do not check integrity of user input
                // need to check that all Z have dimensions x and y
                if (mat.numRows() != numRows()) {
                    std::ostringstream err;
                    err << "Cannot insert a matrix with " << mat.numRows();
                    err << " rows into a Matrix3D that has " << numRows() << "rows";
                    throw std::invalid_argument(err.str());
                }
                if (mat.numCols() != numCols()) {
                    std::ostringstream err;
                    err << "Cannot insert a matrix with " << mat.numCols();
                    err << " rows into a Matrix3D that has " << numCols() << "rows";
                    throw std::invalid_argument(err.str());
                }
            }
            index_.push_back(idx);
            data_.push_back(mat);
        }

        /**
         * @brief Indexer to slice a Matrix3D and return a 2D matrix of type Matrix<DataType>.
         */
        Matrix<DataType> &operator[](IndexType idx) {
            // first check if idx in index
            if (std::find(index_.begin(), index_.end(), idx) == index_.end()) {
                std::ostringstream err;
                err << "Index \"" << idx << "\" has been requested but is not present in this Matrix3D.";
                throw std::invalid_argument(err.str());
            }
            // get index of idx
            int pos = std::distance(index_.begin(), std::find(index_.begin(), index_.end(), idx));
            return data_[pos];
        }

        /**
         * @brief get number of rows in this 3D matrix
         * @details x is rows, y is columns, z is depth.
         */
        int numRows() {
            return data_[0].numRows();
        }

        /**
         * @brief get number of columns in this 3D matrix
         * @details if x is rows, y is columns, z is depth.
         */
        int numCols() {
            return data_[0].numCols();
        }

        /**
         * @brief get number of matrices in this 3D matrix
         * @details if x is rows, y is columns, z is depth.
         */
        int numZ() {
            return index_.size();
        }


    private:
        std::vector<IndexType> index_;
        std::vector<Matrix<DataType>> data_;
    };
}

#endif //ROADRUNNER_MATRIX3D_H
