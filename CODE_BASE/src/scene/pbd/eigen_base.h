#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <iterator>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/LU>
#include <Eigen-unsupported/Eigen/MatrixFunctions>

//#define PBD

/// NAMESPACES
using namespace std;
using namespace Eigen;

/// RENDERING CONSTANTS
constexpr int numFrames = 1000;
constexpr int dim = 3;
constexpr bool testing = true;

/// TYPES
typedef double T;
typedef Matrix<T, dim, 1> V; // vertical
typedef Matrix<T, 1, dim> V_horizontal;
typedef Matrix<T, Dynamic, Dynamic> DM;

/// CONSTANTS
constexpr T EPSILON = 1e-6;
constexpr T PI = 3.141592653589793238462643383279;

/// TYPE METHODS
T dot_horizontal(V_horizontal a, V_horizontal b) {
    return (a * b.transpose())(0, 0);
}
T dot_vertical(V a, V b) {
    return (a.transpose() * b)(0, 0);
}
T clamp(T a, T minVal, T maxVal) {
    return max(minVal, min(maxVal, a));
}