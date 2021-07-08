//
// Created by Ciaran on 08/07/2021.
//

#ifndef ROADRUNNER_MATRIX_H
#define ROADRUNNER_MATRIX_H

#include "rr-libstruct/lsMatrix.h"

namespace rr {

    template<typename T>
    class Matrix : public ls::Matrix<T> {
    public:
        using ls::Matrix<T>::Matrix;
        using ls::Matrix<T>::numCols;
        using ls::Matrix<T>::numRows;
        using ls::Matrix<T>::operator();
        using ls::Matrix<T>::operator[];
        using ls::Matrix<T>::resize;

        /**
         * @brief Constructor for creating a Matrix<T> from a
         * ls::Matrix<T>
         */
         explicit Matrix<T>( ls::Matrix<T>& matrix){
             resize(matrix.numRows(), matrix.numCols());
             for (int i=0; i< numRows(); i++){
                 for (int j=0; j<numCols(); j++){
                     operator()(i, j) = matrix(i, j);
                 }
             }
         }
         explicit Matrix<T>( ls::Matrix<T>* matrix)
                 : Matrix<T>(*matrix){}

        /**
         * @brief Element-wise equality operator to compare a Matrix<T> with
         * another Matrix<T>
         * @note to compare Matrix<double> use Matrix<double>::almostEqual
         */
        bool operator==(const Matrix<T> &other) {
            if (numRows() != other.numRows()) {
                return false;
            }
            if (numCols() != other.numCols()) {
                return false;
            }
            bool equals = true;
            for (int i = 0; i < numRows(); i++) {
                for (int j = 0; j < numCols(); j++) {
                    if (this->operator()(i, j) != other(i, j)) {
                        equals = false;
                        break;
                    }
                }
            }
            return equals;
        }

        /**
         * @brief inequality operators
         */
        bool operator!=(const Matrix<T> &other) {
            return !(*this == other);
        }

        /**
         * @brief Element-wise compareison between this Matrix<double> with another @param other
         * Matrix<double>
         */
        bool almostEquals(const Matrix<double> &other, const double &tolerance) {
            if (numRows() != other.numRows()) {
                return false;
            }
            if (numCols() != other.numCols()) {
                return false;
            }
            bool equals = true;
            for (int i = 0; i < numRows(); i++) {
                for (int j = 0; j < numCols(); j++) {
                    if ((this->operator()(i, j) - other(i, j)) > tolerance) {
                        equals = false;
                        break;
                    }
                }
            }
            return equals;
        }

        Matrix<T> mult(Matrix<T> matrix){
            ls::Matrix<T> mat = ls::mult(*this, matrix);
            return Matrix<T>(mat);
        }
    };

}
#endif //ROADRUNNER_MATRIX_H
