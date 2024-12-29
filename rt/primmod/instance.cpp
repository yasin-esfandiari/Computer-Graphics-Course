#include <rt/primmod/instance.h>

namespace rt
{

    Instance::Instance(Primitive *content)
        : group(content), transformation_matrix(Matrix::identity())
    {
        this->inv_trnsfrm = Matrix::identity(); // transformation matrix is initialized with I and inverse of I is I
    }

    Primitive *Instance::content()
    {
        return this->group;
    }

    void Instance::reset()
    {
        this->transformation_matrix = Matrix::identity();
        this->inv_trnsfrm = Matrix::identity(); // transformation matrix is reset I and inverse of I is I
    }

    void Instance::translate(const Vector &t)
    {
        Matrix translatation_matrix = Matrix::identity();

        // last column only
        translatation_matrix[0][3] = t.x;
        translatation_matrix[1][3] = t.y;
        translatation_matrix[2][3] = t.z;

        this->transformation_matrix = product(translatation_matrix, this->transformation_matrix);
        this->inv_trnsfrm = transformation_matrix.invert(); // update its invert everytime it's changed.
    }

    void Instance::rotate(const Vector &nnaxis, float angle)
    {
        Vector r = nnaxis.normalize();
        Vector s = basis(r).normalize();
        Vector t = cross(r, s).normalize();
        Matrix ration_matrix = Matrix(HomogeneousCoord(r), HomogeneousCoord(s), HomogeneousCoord(t), HomogeneousCoord(Point::rep(0.0)));

        this->transformation_matrix = product(ration_matrix, this->transformation_matrix);
        Matrix ration_matrix_x = Matrix::identity();
        ration_matrix_x[1][1] = cosf(angle);
        ration_matrix_x[1][2] = -sinf(angle);
        ration_matrix_x[2][1] = sinf(angle);
        ration_matrix_x[2][2] = cosf(angle);
        this->transformation_matrix = product(ration_matrix_x, this->transformation_matrix);
        this->transformation_matrix = product(ration_matrix.transpose(), this->transformation_matrix);

        this->inv_trnsfrm = transformation_matrix.invert(); // update its invert everytime it's changed.
    }

    void Instance::scale(float f)
    {
        Matrix scale_matrix = Matrix::identity();

        // change the diagonal elements
        scale_matrix[0][0] = f;
        scale_matrix[1][1] = f;
        scale_matrix[2][2] = f;
        this->transformation_matrix = product(scale_matrix, this->transformation_matrix);
        this->inv_trnsfrm = transformation_matrix.invert(); // update its invert everytime it's changed.
    }

    void Instance::scale(const Vector &s)
    {
        Matrix scale_matrix = Matrix::identity();

        // change the diagonal elements
        scale_matrix[0][0] = s.x;
        scale_matrix[1][1] = s.y;
        scale_matrix[2][2] = s.z;
        this->transformation_matrix = product(scale_matrix, this->transformation_matrix);
        this->inv_trnsfrm = transformation_matrix.invert(); // update its invert everytime it's changed.
    }

    void Instance::setMaterial(Material *m)
    {
        /* TODO */ NOT_IMPLEMENTED;
    }

    void Instance::setCoordMapper(CoordMapper *cm)
    {
        /* TODO */ NOT_IMPLEMENTED;
    }

    Intersection Instance::intersect(const Ray &ray, float tmin, float tmax) const
    {
        Matrix inverse_transformation_t = this->inv_trnsfrm.transpose();
        for (int i = 0; i < 4; i++)
        {
            inverse_transformation_t.r4[i] = 0;
        }
        Vector inv_trnsfrm_mul_dir = this->inv_trnsfrm * ray.d;
        float inv_trnsfrm_mul_dir_lngth = inv_trnsfrm_mul_dir.length();

        Ray transformed_ray = Ray(this->inv_trnsfrm * ray.o, inv_trnsfrm_mul_dir.normalize());
        tmax = tmax * inv_trnsfrm_mul_dir_lngth; // CHANGED
        tmin = tmin * inv_trnsfrm_mul_dir_lngth; // tmin should also scale
        Intersection response = this->group->intersect(transformed_ray, tmin, tmax);

        if (response)
        {
            float orig_distance = (response.distance * 1.0) / inv_trnsfrm_mul_dir_lngth;
            Point hitpoint = ray.getPoint(orig_distance);
            Vector norm = (inverse_transformation_t * response.normal()).normalize();

            return Intersection(orig_distance, ray, response.solid, norm, hitpoint);
        }
        else
            return Intersection::failure();
    }

    BBox Instance::getBounds() const
    {
        BBox bounds = this->group->getBounds();
        Point min_bbox = bounds.min;
        Point max_bbox = bounds.max;
        min_bbox = this->transformation_matrix * min_bbox;
        max_bbox = this->transformation_matrix * max_bbox;

        bounds.min = min_bbox;
        bounds.max = max_bbox;

        return bounds;
    }

    Vector Instance::basis(const Vector &vec)
    {
        if (fabs(vec.x) <= fabs(vec.y) && fabs(vec.x) <= fabs(vec.z))
            return Vector(0.0, -vec.z, vec.y);
        else if (fabs(vec.y) <= fabs(vec.z))
            return Vector(-vec.z, 0.0, vec.x);
        else
            return Vector(-vec.y, vec.x, 0.0);
    }

}