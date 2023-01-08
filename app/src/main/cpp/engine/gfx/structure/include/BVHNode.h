#ifndef RENDER_BVHNODE_H
#define RENDER_BVHNODE_H

#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class aiScene;
class aiNode;
class aiMesh;

namespace view
{
    class ViewConv;
}

namespace gfx
{
    class GfxMesh;
    struct RrtTriangle;
    struct RrtBVHNode;

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
        BVHBuilder(const char *name, bool adj = false, bool debug = false);

        virtual ~BVHBuilder();

        std::shared_ptr<BVHNode> build();
        void buildEncoded(std::vector<RrtBVHNode> &encodeNodes);
        void drawDebug(const std::shared_ptr<view::ViewConv> &conv);
        void getTriangles(std::vector<RrtTriangle> &out);

    protected:
        void load();
        void processNode(aiNode *node, const aiScene *scene);
        void processMesh(aiMesh *mesh, const aiScene *scene);

        std::string m_name;
        std::string m_srcPath;
        std::vector<RrtTriangle> m_triangles;
        bool m_adjFlag, m_debugFlag;
        glm::vec3 m_max, m_min;

    private:
        static std::shared_ptr<BVHNode> buildWithSAH(std::vector<RrtTriangle> &triangles, int l, int r, int limit);
        static int buildWithSAH(std::vector<RrtTriangle> &triangles, std::vector<RrtBVHNode> &outNodes,
                                int l, int r, int limit);
        static bool cmpX(const RrtTriangle &lhs, const RrtTriangle &rhs);
        static bool cmpY(const RrtTriangle &lhs, const RrtTriangle &rhs);
        static bool cmpZ(const RrtTriangle &lhs, const RrtTriangle &rhs);
        static void lowestCostSAH(std::vector<RrtTriangle> &triangles, int l, int r, float &lowCost,
                                  int &lowAxis, int &lowSplit);

        void adjust();
        void dealDebugMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<std::shared_ptr<GfxMesh>> m_debugMesh;
        glm::mat4 m_debugModelMt;
        uint32_t m_debugMeshId;
    };
}

#endif //RENDER_BVHNODE_H
