#include "intensityimage.h"

#include <stdexcept>
#include <math.h>

namespace Core {
namespace Internal {

class IntensityImagePrivate
{
public:
    IntensityImagePrivate()
        : width(0), height(0), data(nullptr)
    {

    }

    ~IntensityImagePrivate()
    {
        delete data;
        data = nullptr;
    }

    void copy(size_t w, size_t h, double value)
    {
        if (w == 0 || h == 0)
        {
            width = height = 0;
            data = nullptr;
            return;
        }

        size_t numOfElem = w * h;
        if (data != nullptr && width == w && height == h){
            for (size_t i = 0; i < numOfElem; ++i) {
                data[i] = value;
            }
            return;
        }

        if (data != nullptr)
            delete data;
        data = new double[numOfElem];
        for (size_t i = 0; i < numOfElem; ++i) {
            data[i] = value;
        }

        width = w;
        height = h;

    }

    void copy(size_t w, size_t h, const double *d)
    {
        if (w == 0 || h == 0 || d == nullptr)
        {
            width = height = 0;
            data = nullptr;
            return;
        }

        size_t numOfElem = w * h;
        if (data != nullptr && width == w && height == h){
            for (size_t i = 0; i < numOfElem; ++i) {
                data[i] = d[i];
            }
            return;
        }

        if (data != nullptr)
            delete data;
        data = new double[numOfElem];
        for (size_t i = 0; i < numOfElem; ++i) {
            data[i] = d[i];
        }

        width = w;
        height = h;
    }

    size_t width;
    size_t height;
    double *data;
};

} // namespace Internal
} // namespace Core

using namespace Core;

IntensityImage::IntensityImage()
    : d(new Internal::IntensityImagePrivate)
{

}

IntensityImage::IntensityImage(size_t width, size_t height, double value)
    : d(new Internal::IntensityImagePrivate)
{
    d->copy(width, height, value);
}

IntensityImage::IntensityImage(size_t width, size_t height, const double *data)
    : d(new Internal::IntensityImagePrivate)
{
    d->copy(width, height, data);
}

IntensityImage::IntensityImage(size_t width, size_t height, double *data)
    : d(new Internal::IntensityImagePrivate)
{
    if (width == 0 || height == 0 || data == nullptr)
        return;
    d->width = width;
    d->height = height;
    d->data = data;
}

IntensityImage::IntensityImage(const IntensityImage &rhs)
    : d(new Internal::IntensityImagePrivate)
{
    d->copy(rhs.width(), rhs.height(), rhs.data());
}

IntensityImage::IntensityImage(IntensityImage &&rhs)
    : d(new Internal::IntensityImagePrivate)
{
    swap(*this, rhs);
}

IntensityImage::~IntensityImage()
{
    delete d;
    d = nullptr;
}

IntensityImage &IntensityImage::operator =(const IntensityImage &rhs)
{
    d->copy(rhs.width(), rhs.height(), rhs.data());
    return *this;
}

IntensityImage &IntensityImage::operator =(IntensityImage &&rhs)
{
    swap(*this, rhs);
    return *this;
}

double &IntensityImage::operator ()(size_t row, size_t col)
{
    if (row >= d->height || col >= d->width)
        throw std::out_of_range("Row index or col index out of range");

    size_t index = row + col * d->height;
    return d->data[index];
}

const double &IntensityImage::operator ()(size_t row, size_t col) const
{
    if (row >= d->height || col >= d->width)
        throw std::out_of_range("Row index or col index out of range");

    size_t index = row + col * d->height;
    return d->data[index];
}

bool IntensityImage::empty() const
{
    return (d->data == nullptr);
}

size_t IntensityImage::width() const
{
    return d->width;
}

size_t IntensityImage::height() const
{
    return d->height;
}

size_t IntensityImage::rows() const
{
    return d->height;
}

size_t IntensityImage::cols() const
{
    return d->width;
}

size_t IntensityImage::elems() const
{
    return d->width * d->height;
}

double IntensityImage::getValueAt(size_t row, size_t col) const
{
    return (*this)(row, col);
}

void IntensityImage::setValueAt(size_t row, size_t col, double value)
{
    if (row >= d->height || col >= d->width)
        throw std::out_of_range("Row index or col index out of range");

    size_t index = row + col * d->height;
    d->data[index] = value;
}

const double *IntensityImage::data() const
{
    return d->data;
}

double *IntensityImage::data()
{
    return d->data;
}

double IntensityImage::sum() const
{
    size_t numOfElem = d->width * d->height;
    double result = 0.0;
    for (size_t i = 0; i < numOfElem; ++i) {
        result += d->data[i];
    }
    return result;
}

double IntensityImage::mean() const
{
    size_t numOfElems = elems();
    if (numOfElems == 0)
        return 0.0;

    double sumOfElems = sum();
    double result = sumOfElems / static_cast<double>(numOfElems);
    return result;
}

double IntensityImage::variance() const
{
    double meanTemp = 0.0, varTemp = 0.0;
    distribution(meanTemp, varTemp);
    return varTemp;
}

void IntensityImage::distribution(double &meanValue, double &variance) const
{
    size_t numOfElems = elems();
    if (numOfElems == 0)
    {
        meanValue = variance = 0.0;
        return;
    }

    double meanTemp = mean();
    double varTemp = 0.0;
    for (size_t i = 0; i < numOfElems; ++i) {
        varTemp += std::pow(d->data[i] - meanTemp, 2);
    }
    varTemp = varTemp / static_cast<double>(numOfElems);

    meanValue = meanTemp;
    variance = varTemp;
}

double IntensityImage::stddev() const
{
    double result = std::sqrt(variance());
    return result;
}

double IntensityImage::max() const
{
    size_t numOfElems = elems();
    if (numOfElems == 0)
        return 0.0;

    double result = d->data[0];
    for (size_t i = 1; i < numOfElems; ++i) {
        if (d->data[i] > result){
            result = d->data[i];
        }
    }
    return result;
}

double IntensityImage::min() const
{
    size_t numOfElems = elems();
    if (numOfElems == 0)
        return 0.0;

    double result = d->data[0];
    for (size_t i = 1; i < numOfElems; ++i) {
        if (d->data[i] < result){
            result = d->data[i];
        }
    }
    return result;
}

void IntensityImage::swap(IntensityImage &lhs, IntensityImage &rhs)
{
    Internal::IntensityImagePrivate *temp = lhs.d;
    lhs.d = rhs.d;
    rhs.d = temp;
}
