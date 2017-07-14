//
// Created by Tiago on 6-Mar-17.
//

#include "RegularGrid.hpp"

using MobileRT::RegularGrid;

RegularGrid::RegularGrid(const float , const float ,
                         const float , const float ,
                         const float , const float ,
                         const unsigned int ,
                         std::vector<Primitive *>) {
    /*  loop boundingBoxes
     *      descobrir ponto min e max
     *      criar boundingBox
     *
     *  loop primitivas
     *      descobrir min e max dos eixos
     *      adicionar primitiva à boundingBox certa
     */

    /*Point3D *mins = new Point3D[3 * numberOfBlocks];
    Point3D *maxs = new Point3D[3 * numberOfBlocks];
    const float deviationX((maxX - minX) / numberOfBlocks);
    const float deviationY((maxY - minY) / numberOfBlocks);
    const float deviationZ((maxZ - minZ) / numberOfBlocks);

    for (unsigned int i(0u); i < numberOfBlocks; i++) {
        for (unsigned int j(0u); j < 2u; j++) {
            if (j == 0u) {
                float x(deviationX * i);
                float y(deviationY * i);
                float z(deviationZ * i);
                mins[i] = Point3D(x, y, z);
            } else {
                float x(deviationX * i);
                float y(deviationY * i);
                float z(deviationZ * i);
                maxs[i] = Point3D(x, y, z);
            }
        }
    }

    //for (Point3D *point : mins)
    {

    }*/

}

RegularGrid::~RegularGrid() {
}

bool RegularGrid::intersect(Intersection &, Ray &) {
    return false;
}

/*void RegularGrid::divideBoundingBox(AxisAlignedBoundingBox &bb) {
    const Point3D pointMid((bb.pointMax_ - bb.pointMin_) / 2.0f);

    //4 cubes below
    const Point3D min1(bb.pointMin_.x_, bb.pointMin_.y_, bb.pointMin_.z_);
    const Point3D max1(pointMid.x_, pointMid.y_, pointMid.z_);

    const Point3D min2(bb.pointMax_.x_, bb.pointMin_.y_, bb.pointMin_.z_);
    const Point3D max2(pointMid.x_, pointMid.y_, pointMid.z_);

    const Point3D min3(pointMid.x_, bb.pointMin_.y_, pointMid.z_);
    const Point3D max3(bb.pointMin_.x_, pointMid.y_, bb.pointMax_.z_);

    const Point3D min4(pointMid.x_, bb.pointMin_.y_, pointMid.z_);
    const Point3D max4(bb.pointMax_.x_, pointMid.y_, bb.pointMax_.z_);

    //4 cubes above
    const Point3D min5(bb.pointMin_.x_, pointMid.y_, bb.pointMin_.z_);
    const Point3D max5(pointMid.x_, bb.pointMax_.y_, pointMid.z_);

    const Point3D min6(bb.pointMax_.x_, pointMid.y_, bb.pointMin_.z_);
    const Point3D max6(pointMid.x_, bb.pointMax_.y_, pointMid.z_);

    const Point3D min7(pointMid.x_, pointMid.y_, pointMid.z_);
    const Point3D max7(bb.pointMin_.x_, bb.pointMax_.y_, bb.pointMax_.z_);

    const Point3D min8(pointMid.x_, pointMid.y_, pointMid.z_);
    const Point3D max8(bb.pointMax_.x_, bb.pointMax_.y_, bb.pointMax_.z_);
}*/
