#include <rt/groups/bvh.h>

namespace rt
{

    BVH::BVH(){
        this->sah_active = false;
    }

    void BVH::rebuildIndex()
    {
        // compute BBox primitives
        BBox p;
        for (auto &obj : this->primitives)
        {
            p = obj->getBounds();
            this->primitive_bbox.push_back(p);
            this->primitive_center.push_back((p.min + p.max) * 0.5f);
        }

        Node root;
        this->BVHtree.push_back(root);

        std::vector<int> parent_primitive_index;
        auto size = this->primitives.size();
        for (decltype(size) i = 0; i < size; i++)
            parent_primitive_index.push_back(i);

        if (this->sah_active)
        {
            sortPrimitives(parent_primitive_index);
            buildBVHTree_SAH(0, parent_primitive_index);
            return;
        }

        buildBVHTree(0, parent_primitive_index);
    }

    BBox BVH::getBounds() const
    {
        return this->BVHtree.at(0).box;
    }

    Intersection BVH::intersect(const Ray &ray, float tmin, float tmax) const
    {
        std::vector<int> stack;
        stack.push_back(0); // parent
        Intersection response = Intersection::failure();
        Intersection temp = response;
        int current_node;
        float best_distance = tmax; // instead of previousbestdistance
        while (!stack.empty())
        {
            current_node = stack.back();
            stack.pop_back();
            Node node = BVHtree[current_node];
            if (node.is_empty)
            {
                std::pair<float, float> t = this->BVHtree[current_node].box.intersect(ray);
                if (t.first > t.second)
                    continue;
                // iterate over leaf primitives
                std::vector<int> lp = node.leaf_primitives;
                for (auto &p : lp)
                {
                    temp = this->primitives[p]->intersect(ray, tmin, best_distance); // added tmin to argument
                    if (temp)
                    {
                        best_distance = temp.distance;
                        response = temp;
                    }
                }
            }
            else
            {
                std::pair<float, float> t = this->BVHtree[current_node].box.intersect(ray);
                if (t.first > best_distance)
                    continue;
                Node node = this->BVHtree[current_node];
                int leftIndex = node.left_child;
                int rightIndex = node.right_child;
                std::pair<float, float> t1 = this->BVHtree[leftIndex].box.intersect(ray);
                std::pair<float, float> t2 = this->BVHtree[rightIndex].box.intersect(ray);
                bool left_intersect = t1.second > t1.first;
                bool right_intersect = t2.second > t2.first;
                if (left_intersect && right_intersect)
                {
                    if (t1.first < t2.first)
                    {
                        stack.push_back(rightIndex);
                        stack.push_back(leftIndex);
                    }
                    else
                    {
                        stack.push_back(leftIndex);
                        stack.push_back(rightIndex);
                    }
                }
                else if (left_intersect)
                    stack.push_back(leftIndex);
                else if (right_intersect)
                    stack.push_back(rightIndex);
                else
                    continue;
            }
        }
        return response;
    }

    void BVH::add(Primitive *p)
    {
        this->primitives.push_back(p);
    }

    void BVH::setMaterial(Material *m)
    {
        for(auto& obj : primitives)
            obj->setMaterial(m);
    }

    void BVH::setCoordMapper(CoordMapper *cm)
    {
        /* TODO */ NOT_IMPLEMENTED;
    }

    void BVH::serialize(BVH::Output &output)
    {
        // To implement this function:
        // - Call output.setNodeCount() with the number of nodes in the BVH
        /* TODO */
        // - Set the root node index using output.setRootId()
        /* TODO */
        // - Write each and every one of the BVH nodes to the output using output.writeNode()
        /* TODO */ NOT_IMPLEMENTED;
    }
    void BVH::deserialize(BVH::Input &input)
    {
        // To implement this function:
        // - Allocate and initialize input.getNodeCount() nodes
        /* TODO */
        // - Fill your nodes with input.readNode(index)
        /* TODO */
        // - Use the node at index input.getRootId() as the root node
        /* TODO */ NOT_IMPLEMENTED;
    }

    void BVH::buildBVHTree(int parent_index, std::vector<int> parent_primitive_index)
    {
        std::vector<int> left_primitive_index, right_primitive_index;
        int left_child_index, right_child_index;

        // exit condition in recursion --> leaf node [less than 3 primitives]
        if (parent_primitive_index.size() <= 2)
        {
            this->BVHtree[parent_index].is_empty = true;
            for (auto &i : parent_primitive_index)
            {
                this->BVHtree[parent_index].leaf_primitives.push_back(i);
            }
            this->BVHtree[parent_index].box = bboxExtender(parent_primitive_index);
            parent_primitive_index.clear();
            return;
        }

        // ~leaf node.
        this->BVHtree[parent_index].is_empty = false;
        this->BVHtree[parent_index].box = bboxExtender(parent_primitive_index);

        // Copmuting the split plane
        std::pair<int, float> split_info;
        BBox centre = BBox();
        for (auto &i : parent_primitive_index)
            centre.extend(this->primitive_center[i]);
        Vector edge = centre.diagonal();
        // split index
        if (fabs(edge.x) >= fabs(edge.y) && fabs(edge.x) >= fabs(edge.z))
            split_info.first = 0;
        else if (fabs(edge.y) >= fabs(edge.z))
            split_info.first = 1;
        else
            split_info.first = 2;
        // average over the split-axis
        split_info.second = (centre.max[split_info.first] + centre.min[split_info.first]) * 0.5;

        for (auto &p : parent_primitive_index)
        {
            Point prim_center = this->primitive_center[p];
            if (prim_center[split_info.first] <= split_info.second)
                left_primitive_index.push_back(p);
            else
                right_primitive_index.push_back(p);
        }
        parent_primitive_index.clear();

        Node left_child;
        this->BVHtree.push_back(left_child);
        left_child_index = this->BVHtree.size() - 1;
        this->BVHtree[parent_index].left_child = left_child_index;
        buildBVHTree(left_child_index, left_primitive_index);
        left_primitive_index.clear();

        Node right_child;
        this->BVHtree.push_back(right_child);
        right_child_index = this->BVHtree.size() - 1;
        this->BVHtree[parent_index].right_child = right_child_index;
        buildBVHTree(right_child_index, right_primitive_index);
        right_primitive_index.clear();

        return;
    }

    BBox BVH::bboxExtender(std::vector<int> indices)
    {
        BBox result;
        for (auto &i : indices)
            result.extend(this->primitive_bbox[i]);
        return result;
    }

    void BVH::sortPrimitives(std::vector<int> target_indexes)
    {
        std::vector<int> temp = target_indexes;
        sort(temp, 0, temp.size() - 1, 0);
        this->sorted_primitives_x = temp;
        sort(temp, 0, temp.size() - 1, 1);
        this->sorted_primitives_y = temp;
        sort(temp, 0, temp.size() - 1, 2);
        this->sorted_primitives_z = temp;
    }
    
    void BVH::sort(std::vector<int> &a, int left, int right, int dim)
    {
        if (left < right)
        {
            int pivotIndex = sortPartition(a, left, right, dim);
            sort(a, left, pivotIndex - 1, dim);
            sort(a, pivotIndex, right, dim);
        }
    }

    int BVH::sortPartition(std::vector<int> &a, int left, int right, int dim)
    {
        int pivotIndex = left + (right - left) / 2;
        int pivotValue = a[pivotIndex];
        int i = left, j = right;
        while (i <= j)
        {
            while (this->primitive_center.at(a.at(i))[dim] < this->primitive_center.at(pivotValue)[dim])
            {
                i++;
            }
            while (this->primitive_center.at(a.at(j))[dim] > this->primitive_center.at(pivotValue)[dim])
            {
                j--;
            }
            if (i <= j)
            {
                std::swap(a[i], a[j]);
                i++;
                j--;
            }
        }
        return i;
    }

    std::vector<int> BVH::getSortedSubset(std::vector<int> target_indexes, int axis)
    {
        std::vector<int> global_sorted;
        std::vector<int> response;

        if (axis == 0)
            global_sorted = this->sorted_primitives_x;
        else if (axis == 1)
            global_sorted = this->sorted_primitives_y;
        else
        {
            global_sorted = this->sorted_primitives_z;
        }

        for (auto &i : global_sorted)
        {
            if (std::find(target_indexes.begin(), target_indexes.end(), i) != target_indexes.end())
                response.push_back(i);
        }
        return response;
    }

    BBox BVH::getCustomBounds(std::vector<int> target_indexes)
    {
        BBox response;
        for (auto &i : target_indexes)
        {
            response.extend(this->primitive_bbox.at(i));
        }
        return response;
    }

    void BVH::buildBVHTree_SAH(int parentIndex, std::vector<int> parentPrimitiveIndex)
    {
        // std::vector<int>    leftPrimitiveIndex, rightPrimitiveIndex;
        int leftChildIndex, rightChildIndex;

        if (parentPrimitiveIndex.size() <= 2) // This is a leaf node
        {
            this->BVHtree.at(parentIndex).is_empty = true; // make it a leaf node
            for (auto &i : parentPrimitiveIndex)
            {
                this->BVHtree.at(parentIndex).leaf_primitives.push_back(i);
            }
            this->BVHtree.at(parentIndex).box = getCustomBounds(parentPrimitiveIndex);
            parentPrimitiveIndex.clear();
            return;
        }
        // This is not a leaf node.

        this->BVHtree.at(parentIndex).is_empty = false;
        this->BVHtree.at(parentIndex).box = getCustomBounds(parentPrimitiveIndex);

        // std::pair<int, float> split_info = compute_split(parentPrimitiveIndex);
        std::vector<int> final_sorted;
        float score_global = FLT_MAX;
        int index_global = 0;
        for (int axis_counter = 0; axis_counter < 3; axis_counter++) // iterating over three dimensions
        {
            // sort(parentPrimitiveIndex, 0, parentPrimitiveIndex.size() - 1, axis_counter); // sorting the projection of centroid on particular axis
            std::vector<int> sorted_indexes;
            std::vector<float> right_scores, left_scores;
            std::vector<int> temp;

            sorted_indexes = getSortedSubset(parentPrimitiveIndex, axis_counter);

            for (auto it = sorted_indexes.rbegin(); it != sorted_indexes.rend(); ++it) // Calculating partial cost from right to left
            {
                temp.push_back(*it);
                right_scores.insert(right_scores.begin(), (getCustomBounds(temp).area() * temp.size()));
            }
            temp.clear();
            right_scores.erase(right_scores.begin());
            for (auto &it : sorted_indexes) // Calculating partial cost from left to right
            {
                temp.push_back(it);
                left_scores.push_back((getCustomBounds(temp).area() * temp.size()));
            }
            left_scores.pop_back();
            temp.clear();
            int index_local = 0;
            float score_local = FLT_MAX;

            auto size = right_scores.size();
            for (decltype(size) i = 0; i < size; i++)
            {
                float SAH = (right_scores.at(i) + left_scores.at(i));
                if (score_local > SAH)
                {
                    index_local = i;
                    score_local = SAH;
                }
            }
            if (score_local < score_global)
            {
                score_global = score_local;
                index_global = index_local;
                final_sorted = parentPrimitiveIndex;
            }
            right_scores.clear();
            left_scores.clear();
        }
        score_global = score_global / getCustomBounds(parentPrimitiveIndex).area();

        if (score_global > parentPrimitiveIndex.size()) // Checking if the net cost of splitting is more than not splitting the node
        {
            this->BVHtree.at(parentIndex).is_empty = true;
            for (auto &i : parentPrimitiveIndex)
            {
                this->BVHtree.at(parentIndex).leaf_primitives.push_back(i);
                this->BVHtree.at(parentIndex).box.extend(this->primitives.at(i)->getBounds());
            }
            parentPrimitiveIndex.clear();

            return;
        }

        std::vector<int> leftPrimitiveIndex(final_sorted.begin(), final_sorted.begin() + index_global + 1);
        std::vector<int> rightPrimitiveIndex(final_sorted.begin() + index_global + 1, final_sorted.end());
        parentPrimitiveIndex.clear();

        Node leftChild;
        this->BVHtree.push_back(leftChild);
        leftChildIndex = this->BVHtree.size() - 1;
        this->BVHtree.at(parentIndex).left_child = leftChildIndex;
        buildBVHTree(leftChildIndex, leftPrimitiveIndex);

        leftPrimitiveIndex.clear();

        Node rightChild;
        this->BVHtree.push_back(rightChild);
        rightChildIndex = this->BVHtree.size() - 1;
        this->BVHtree.at(parentIndex).right_child = rightChildIndex;
        buildBVHTree(rightChildIndex, rightPrimitiveIndex);
        rightPrimitiveIndex.clear();

        return;
    }

}