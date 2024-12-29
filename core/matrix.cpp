#include <core/matrix.h>
#include <core/assert.h>

#include <core/vector.h>
#include <core/point.h>

namespace rt
{

    Matrix::Matrix(const HomogeneousCoord &r1, const HomogeneousCoord &r2, const HomogeneousCoord &r3, const HomogeneousCoord &r4)
        : r1(r1), r2(r2), r3(r3), r4(r4) {}

    HomogeneousCoord &Matrix::operator[](int idx)
    {
        switch (idx)
        {
        case 0:
            return this->r1;
            break;
        case 1:
            return this->r2;
            break;
        case 2:
            return this->r3;
            break;
        case 3:
            return this->r4;
            break;
        default:
            return dflt; // a quick fix to warning
            break;
        }
    }

    HomogeneousCoord Matrix::operator[](int idx) const
    {
        switch (idx)
        {
        case 0:
            return this->r1;
            break;
        case 1:
            return this->r2;
            break;
        case 2:
            return this->r3;
            break;
        case 3:
            return this->r4;
            break;
        default:
            return this->dflt;
            break;
        }
    }

    Matrix Matrix::operator+(const Matrix &b) const
    {
        return Matrix(this->r1 + b.r1, this->r2 + b.r2, this->r3 + b.r3, this->r4 + b.r4);
    }

    Matrix Matrix::operator-(const Matrix &b) const
    {
        return Matrix(this->r1 - b.r1, this->r2 - b.r2, this->r3 - b.r3, this->r4 - b.r4);
    }

    Matrix Matrix::transpose() const
    {
        return Matrix(
            HomogeneousCoord(this->r1[0], this->r2[0], this->r3[0], this->r4[0]),
            HomogeneousCoord(this->r1[1], this->r2[1], this->r3[1], this->r4[1]),
            HomogeneousCoord(this->r1[2], this->r2[2], this->r3[2], this->r4[2]),
            HomogeneousCoord(this->r1[3], this->r2[3], this->r3[3], this->r4[3]));
    }

    Matrix Matrix::invert() const
    {
        Matrix result;
        const Matrix &m = *this;

        // Taken and modified from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
        result[0][0] = m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
        result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
        result[2][0] = m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
        result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

        float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
        if (det == 0)
            return Matrix::zero();

        result[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
        result[1][1] = m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
        result[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
        result[3][1] = m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
        result[0][2] = m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
        result[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
        result[2][2] = m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
        result[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
        result[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
        result[1][3] = m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
        result[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
        result[3][3] = m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

        result = result * (1.0f / det);
        return result;
    }

    bool Matrix::operator==(const Matrix &b) const
    {
        if (this->r1 == b.r1 && this->r2 == b.r2 && this->r3 == b.r3 && this->r4 == b.r4)
            return 1;
        else
            return 0;
    }

    bool Matrix::operator!=(const Matrix &b) const
    {
        if (this->r1 == b.r1 && this->r2 == b.r2 && this->r3 == b.r3 && this->r4 == b.r4)
            return 0;
        else
            return 1;
    }

    Matrix product(const Matrix &a, const Matrix &b)
    {
        Matrix b_t = b.transpose();
        return Matrix(a * b_t.r1, a * b_t.r2, a * b_t.r3, a * b_t.r4).transpose();
    }

    Matrix operator*(const Matrix &a, float scalar)
    {
        return Matrix(a.r1 * scalar, a.r2 * scalar, a.r3 * scalar, a.r4 * scalar);
    }

    Matrix operator*(float scalar, const Matrix &a)
    {
        return Matrix(scalar * a.r1, scalar * a.r2, scalar * a.r3, scalar * a.r4);
    }

    HomogeneousCoord Matrix::operator*(const HomogeneousCoord &b) const
    {
        return HomogeneousCoord(dot(this->r1, b), dot(this->r2, b), dot(this->r3, b), dot(this->r4, b));
    }

    Vector Matrix::operator*(const Vector &b) const
    {
        return Vector((*this) * HomogeneousCoord(b));
    }

    Point Matrix::operator*(const Point &b) const
    {
        return Point((*this) * HomogeneousCoord(b));
    }

    float Matrix::det() const
    {
        Matrix result;
        const Matrix &m = *this;
        float p00 = 0.0, p01 = 0.0, p02 = 0.0, p03 = 0.0;

        // outer: row=0, col: 0-->3 || inner: row~=0, col: 1-->3
        p00 = m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) - m[2][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[3][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]);
        p01 = -m[1][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[2][0] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) - m[3][0] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]);
        p02 = m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) - m[2][0] * (m[1][1] * m[3][3] + m[1][3] * m[3][1]) + m[3][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]);
        p03 = -m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[2][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) - m[3][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]);

        return m[0][0] * p00 + m[0][1] * p01 + m[0][2] * p02 + m[0][3] * p03;
    }

    Matrix Matrix::zero()
    {
        return Matrix(HomogeneousCoord::rep(0.0), HomogeneousCoord::rep(0.0), HomogeneousCoord::rep(0.0), HomogeneousCoord::rep(0.0));
    }

    Matrix Matrix::identity()
    {
        return Matrix(
            HomogeneousCoord(1.0, 0.0, 0.0, 0.0),
            HomogeneousCoord(0.0, 1.0, 0.0, 0.0),
            HomogeneousCoord(0.0, 0.0, 1.0, 0.0),
            HomogeneousCoord(0.0, 0.0, 0.0, 1.0));
    }

    Matrix Matrix::system(const Vector &e1, const Vector &e2, const Vector &e3)
    {
        return Matrix(HomogeneousCoord(e1), HomogeneousCoord(e2), HomogeneousCoord(e3), HomogeneousCoord(Point::rep(0.0))).transpose();
    }

}