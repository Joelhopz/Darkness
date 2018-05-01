#include "tools/Octree.h"

using namespace std;

namespace engine
{
#ifdef DEBUG_OCTREE
    std::vector<engine::BoundingBox> usedBoundingBoxes;
    std::vector<engine::BoundingBox> searchBoundingBoxes;
    std::vector<engine::BoundingBox> initialSearchBoundingBoxes;
#endif
}
