#ifndef RENDER_BVHNODE_H
#define RENDER_BVHNODE_H

#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class aiScene;
class aiNode;
class aiMesh;

namespace gfx
{
    class GfxMesh;

    struct RrtTriangle;

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

    class BVHBuilder
    {
    public:
        BVHBuilder(const char *name, bool debug = false);

        virtual ~BVHBuilder();

        std::shared_ptr<BVHNode> build();

    protected:
        void load();
        void processNode(aiNode *node, const aiScene *scene);
        void processMesh(aiMesh *mesh, const aiScene *scene);

        std::string m_name;
        std::string m_srcPath;
        std::vector<RrtTriangle> m_triangles;
        bool m_debugFlag;

    private:
        static std::shared_ptr<BVHNode> buildWithSAH(std::vector<RrtTriangle> &triangles,
                                                     uint32_t l, uint32_t r, uint32_t limit);
        static bool cmpX(const RrtTriangle &lhs, const RrtTriangle &rhs);
        static bool cmpY(const RrtTriangle &lhs, const RrtTriangle &rhs);
        static bool cmpZ(const RrtTriangle &lhs, const RrtTriangle &rhs);

        void dealDebugMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<std::shared_ptr<GfxMesh>> m_debugMesh;
        glm::vec3 m_debugMax, m_debugMin;
        uint32_t m_debugMeshId;
    };
}

#endif //RENDER_BVHNODE_H
