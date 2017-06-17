//
// Created by Tiago on 6-Mar-17.
//

#include "RegularGrid.h"

using namespace MobileRT;

RegularGrid::RegularGrid(const float minX, const float maxX,
                         const float minY, const float maxY,
                         const float minZ, const float maxZ,
                         const unsigned int numberOfBlocks,
                        std::vector<Primitive *> )
{
    /*  loop boundingBoxes
     *      descobrir ponto min e max
     *      criar boundingBox
     *
     *  loop primitivas
     *      descobrir min e max dos eixos
     *      adicionar primitiva à boundingBox certa
     */

    Point3D *mins = new Point3D[3 * numberOfBlocks];
    Point3D *maxs = new Point3D[3 * numberOfBlocks];
    const float deviationX ((maxX - minX) / numberOfBlocks);
    const float deviationY ((maxY - minY) / numberOfBlocks);
    const float deviationZ ((maxZ - minZ) / numberOfBlocks);

    for (unsigned int i (0); i < numberOfBlocks; i++)
    {
        for (unsigned int j (0); j < 2; j++)
        {
            if (j == 0)
            {
                float x (deviationX * i);
                float y (deviationY * i);
                float z (deviationZ * i);
                mins[i] = Point3D(x, y, z);
            } else {
                float x (deviationX * i);
                float y (deviationY * i);
                float z (deviationZ * i);
                maxs[i] = Point3D(x, y, z);
            }
        }
    }

    //for (Point3D *point : mins)
    {

    }

}

RegularGrid::~RegularGrid(void)
{
}

bool RegularGrid::intersect(Intersection &, Ray &)
{
    return false;
}
