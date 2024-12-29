#include <rt/groups/simplegroup.h>

namespace rt
{

	BBox SimpleGroup::getBounds() const
	{
		BBox bbox;
		for (const auto &obj : primitives)
			bbox.extend(obj->getBounds());
		return bbox;
	}

	Intersection SimpleGroup::intersect(const Ray &ray, float tmin, float tmax) const
	{
		Intersection p_intersection;
		Intersection result_intersection = Intersection::failure();
		float min_distance = tmax; // tmax or tmin?
		for (auto &p : primitives)
		{
			p_intersection = p->intersect(ray, tmin, min_distance);
			if (p_intersection && p_intersection.distance < min_distance && p_intersection > tmin)
			{
				result_intersection = p_intersection;
				min_distance = p_intersection.distance;
			}
		}
		return result_intersection;
	}

	void SimpleGroup::rebuildIndex()
	{
		// do nothing
	}

	void SimpleGroup::add(Primitive *p)
	{
		primitives.push_back(p);
	}

	void SimpleGroup::setMaterial(Material *m)
	{
		for(auto& obj : primitives)
        	obj->setMaterial(m);
	}

	void SimpleGroup::setCoordMapper(CoordMapper *cm)
	{
		for(auto& prim: this->primitives)
		{
			prim->setCoordMapper(cm);
		}
	}

}
