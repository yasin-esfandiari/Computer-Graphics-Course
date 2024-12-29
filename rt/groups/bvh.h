#ifndef CG1RAYTRACER_GROUPS_BVH_HEADER
#define CG1RAYTRACER_GROUPS_BVH_HEADER

#include <rt/groups/group.h>
#include <rt/bbox.h>

namespace rt
{

    class BVH : public Group
    {
    public:
        BVH();

        virtual BBox getBounds() const;
        virtual Intersection intersect(const Ray &ray, float tmin = 0, float tmax = FLT_MAX) const;
        virtual void rebuildIndex();
        virtual void add(Primitive *p);
        virtual void setMaterial(Material *m);
        virtual void setCoordMapper(CoordMapper *cm);
        virtual void buildBVHTree(int parent_index, std::vector<int> parentPrimitiveIndex);

        // new
        virtual BBox bboxExtender(std::vector<int> target_indexes);
        virtual void sortPrimitives(std::vector<int> target_indexes);
        virtual void sort(std::vector<int> &a, int left, int right, int dim);
        virtual void buildBVHTree_SAH(int parent_index, std::vector<int> parentPrimitiveIndex);
        virtual int sortPartition(std::vector<int> &a, int left, int right, int dim);
        std::vector<int> getSortedSubset(std::vector<int> target_indexes, int axis);
        virtual BBox getCustomBounds(std::vector<int> target_indexes);

        typedef struct Node
        {
            BBox box;
            bool is_empty;
            std::vector<int> leaf_primitives;
            int left_child, right_child;
            Node() {}
        } Node;

        std::vector<BBox> primitive_bbox;
        std::vector<Point> primitive_center;
        std::vector<Node> BVHtree;
        bool sah_active;
        std::vector<int> sorted_primitives_x;
        std::vector<int> sorted_primitives_y;
        std::vector<int> sorted_primitives_z;

        

        // Do not use this structure as your node layout:
        // It is inefficient and has a large memory footprint.
        struct SerializedNode
        {
            bool isLeaf;
            BBox bbox;

            // For leaves only
            std::vector<Primitive *> primitives;

            // For internal nodes only
            size_t leftChildId;
            size_t rightChildId;
        };

        // Implement this function if you want to take part in the BVH speed competition
        struct Output
        {
            virtual void setNodeCount(size_t nodeCount) = 0;
            virtual void setRootId(size_t rootId) = 0;
            virtual void writeNode(size_t nodeId, const SerializedNode &node) = 0;
        };
        void serialize(Output &output);

        // Implementing this is optional and not needed for the BVH speed competition,
        // but it will allow you to run the benchmark utility and test your BVH exporter.
        struct Input
        {
            virtual size_t getNodeCount() = 0;
            virtual size_t getRootId() = 0;
            virtual const SerializedNode &readNode(size_t nodeId) = 0;
        };
        void deserialize(Input &input);
    };

}

#endif