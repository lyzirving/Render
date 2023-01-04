#ifndef RENDER_BVHNODE_H
#define RENDER_BVHNODE_H

#include <memory>
#include <string>
#include <glm/glm.hpp>

class aiScene;
class aiNode;
class aiMesh;

namespace gfx
{
    class BVHNode
    {
    public:
        BVHNode();

        virtual ~BVHNode();

        std::shared_ptr<BVHNode> m_left, m_right;
        /**
         * @brief only valid in leaf node. records how many items this leaf contains
         */
        int32_t m_itemCnt;
        /**
         * @brief only valid in leaf node. records the start index of items in the container
         */
        int32_t m_startInd;
        /**
         * @brief bounding box of current node, m_AA contains min value, m_BB contains max value
         */
        glm::vec3 m_AA, m_BB;
    };

    class BVNBuilder
    {
    public:
        BVNBuilder(const char *path);

        virtual ~BVNBuilder();

    protected:
        void load();
        void processNode(aiNode *node, const aiScene *scene);

        std::string m_srcPath, m_srcDirectory;
        std::string m_name;
    };
}

#endif //RENDER_BVHNODE_H
