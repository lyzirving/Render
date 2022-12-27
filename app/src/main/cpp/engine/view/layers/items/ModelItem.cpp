#include "ModelItem.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ModelItem"

namespace view
{
    ModelItem::ModelItem(const char *path) : m_srcPath(path) {}

    ModelItem::~ModelItem() = default;

    void ModelItem::draw(const std::shared_ptr<ViewConv> &conv) {}
}

