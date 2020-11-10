#pragma once

#include "core_global.h"

namespace Core {
namespace Internal {
class IntensityImagePrivate;
}

class CORE_EXPORT IntensityImage
{
public:
    IntensityImage();
    IntensityImage(size_t width, size_t height, double value = 0.0);
    IntensityImage(size_t width, size_t height, const double *data);
    IntensityImage(size_t width, size_t height, double *data);
    IntensityImage(const IntensityImage &rhs);
    IntensityImage(IntensityImage &&rhs);
    ~IntensityImage();

    IntensityImage& operator = (const IntensityImage &rhs);
    IntensityImage& operator = (IntensityImage &&rhs);
    double& operator () (size_t row, size_t col);
    const double& operator () (size_t row, size_t col) const;

    bool empty() const;
    size_t width() const;
    size_t height() const;
    size_t rows() const;
    size_t cols() const;
    size_t elems() const;

    // elements stored in column-major ordering (ie. column by column)
    double getValueAt(size_t row, size_t col) const;
    void setValueAt(size_t row, size_t col, double value);
    const double* data() const;
    double* data();

    // statistical information
    double sum() const;
    double mean() const;
    double variance() const;
    void distribution(double &meanValue, double &variance) const;
    double stddev() const;
    double max() const;
    double min() const;

    static void swap(IntensityImage &lhs, IntensityImage &rhs);

private:
    Internal::IntensityImagePrivate *d;
};

} // namespace Core
