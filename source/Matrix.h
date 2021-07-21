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
        using ls::Matrix<T>::getArray;
        using ls::Matrix<T>::getValues;

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
        bool operator==(Matrix<T> &other) {
            // if both are empty, result is true
            if (empty() && other.empty()){
                return true;
            }
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
        bool operator!=( Matrix<T> &other) {
            return !(*this == other);
        }

        /**
         * @brief Element-wise compareison between this Matrix<double> with another @param other
         * Matrix<double>
         */
        bool almostEquals( Matrix<double> &other, const double &tolerance) {
            // when both matrices are empty, they are equal
            if (empty() && other.empty()){
                return true;
            }
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

        /**
         * @brief indicator method for empty Matrix.
         * @return true if matrix is empty, false otherwise
         */
        bool empty() {
            if (getArray() == nullptr)
                return true;
            return false;
        }
    };

}
#endif //ROADRUNNER_MATRIX_H
