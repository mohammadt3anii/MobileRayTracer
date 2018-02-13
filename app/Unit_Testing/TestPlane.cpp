#include "MobileRT/Shapes/Plane.hpp"
#include <gtest/gtest.h>

using ::MobileRT::AABB;
using ::MobileRT::Plane;
using ::MobileRT::Point3D;
using ::MobileRT::Vector3D;

class TestPlane : public testing::Test {
protected:
	Plane *plane;

	virtual void SetUp() {
		plane = new Plane(Point3D(-1,0,0), Vector3D(1,0,0));
	}

	virtual void TearDown() {
		delete plane;
	}
};

TEST_F(TestPlane, IntersectBoxOutsideX) {
	AABB box (Point3D(1, 0, 0), Point3D(2, 1, 1));
	const bool result (plane->intersect(box));
	ASSERT_EQ(false, result);
}

TEST_F(TestPlane, IntersectBoxInsideX) {
	AABB box (Point3D(-1.5f, 0, 0), Point3D(0.5f, 1, 1));
	const bool result (plane->intersect(box));
	ASSERT_EQ(true, result);
}

TEST_F(TestPlane, IntersectBoxOutsideY) {
	Plane plane (Point3D(0,0,0), Vector3D(0,1,0));
	AABB box (Point3D(-1, 0.5, -1), Point3D(0, 1.5, 0));
	const bool result (plane.intersect(box));
	ASSERT_EQ(false, result);
}

TEST_F(TestPlane, IntersectBoxInsideY) {
	Plane plane (Point3D(0,0,0), Vector3D(0,1,0));
	AABB box (Point3D(0, -0.5f, 0), Point3D(0, 0.5, 0));
	const bool result (plane.intersect(box));
	ASSERT_EQ(true, result);
}

TEST_F(TestPlane, IntersectBoxOutsideZ) {
	Plane plane (Point3D(0,0,0), Vector3D(0,0,-1));
	AABB box (Point3D(-1, 0, 0.5), Point3D(0, 1, 1.5));
	const bool result (plane.intersect(box));
	ASSERT_EQ(false, result);
}

TEST_F(TestPlane, IntersectBoxInsideZ) {
	Plane plane (Point3D(0,0,0), Vector3D(0,0,-1));
	AABB box (Point3D(0, 0, -1.5f), Point3D(0, 1, 2.5));
	const bool result (plane.intersect(box));
	ASSERT_EQ(true, result);
}

TEST_F(TestPlane, IntersectBoxInsideZ2) {
	Plane plane (Point3D(0,0,0), Vector3D(0,0,1));
	AABB box (Point3D(0, 0, -1.5f), Point3D(0, 1, 2.5));
	const bool result (plane.intersect(box));
	ASSERT_EQ(true, result);
}

TEST_F(TestPlane, DistancePoint) {
	Plane plane (Point3D(0,0,0), Vector3D(0,0,1));
	Point3D p(0, 0, -1.5f);
	const float result (plane.distance(p));
	const float expected (-1.5f);
	ASSERT_LE(expected, result);
}

TEST_F(TestPlane, IntersectBoxBorderX) {
	AABB box (Point3D(-1, 0, 0), Point3D(0, 1, 1));
	const bool result (plane->intersect(box));
	ASSERT_EQ(true, result);
}
