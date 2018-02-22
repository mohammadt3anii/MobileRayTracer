#include "MobileRT/Shapes/Plane.hpp"
#include <gtest/gtest.h>

using ::MobileRT::AABB;
using ::MobileRT::Plane;
using glm::vec3;

class TestPlane : public testing::Test {
protected:
	Plane *plane;

	virtual void SetUp() {
		plane = new Plane(glm::vec3(-1,0,0), glm::vec3(1,0,0));
	}

	virtual void TearDown() {
		delete plane;
	}
};

TEST_F(TestPlane, IntersectBoxOutsideX) {
	AABB box (glm::vec3(1, 0, 0), glm::vec3(2, 1, 1));
	const bool result (plane->intersect(box));
	ASSERT_EQ(false, result);
}

TEST_F(TestPlane, IntersectBoxInsideX) {
	AABB box (glm::vec3(-1.5f, 0, 0), glm::vec3(0.5f, 1, 1));
	const bool result (plane->intersect(box));
	ASSERT_EQ(true, result);
}

TEST_F(TestPlane, IntersectBoxOutsideY) {
	Plane plane (glm::vec3(0,0,0), glm::vec3(0,1,0));
	AABB box (glm::vec3(-1, 0.5, -1), glm::vec3(0, 1.5, 0));
	const bool result (plane.intersect(box));
	ASSERT_EQ(false, result);
}

TEST_F(TestPlane, IntersectBoxInsideY) {
	Plane plane (glm::vec3(0,0,0), glm::vec3(0,1,0));
	AABB box (glm::vec3(0, -0.5f, 0), glm::vec3(0, 0.5, 0));
	const bool result (plane.intersect(box));
	ASSERT_EQ(true, result);
}

TEST_F(TestPlane, IntersectBoxOutsideZ) {
	Plane plane (glm::vec3(0,0,0), glm::vec3(0,0,-1));
	AABB box (glm::vec3(-1, 0, 0.5), glm::vec3(0, 1, 1.5));
	const bool result (plane.intersect(box));
	ASSERT_EQ(false, result);
}

TEST_F(TestPlane, IntersectBoxInsideZ) {
	Plane plane (glm::vec3(0,0,0), glm::vec3(0,0,-1));
	AABB box (glm::vec3(0, 0, -1.5f), glm::vec3(0, 1, 2.5));
	const bool result (plane.intersect(box));
	ASSERT_EQ(true, result);
}

TEST_F(TestPlane, IntersectBoxInsideZ2) {
	Plane plane (glm::vec3(0,0,0), glm::vec3(0,0,1));
	AABB box (glm::vec3(0, 0, -1.5f), glm::vec3(0, 1, 2.5));
	const bool result (plane.intersect(box));
	ASSERT_EQ(true, result);
}

TEST_F(TestPlane, DistancePoint) {
	Plane plane (glm::vec3(0,0,0), glm::vec3(0,0,1));
	glm::vec3 p(0, 0, -1.5f);
	const float result (plane.distance(p));
	const float expected (-1.5f);
	ASSERT_LE(expected, result);
}

TEST_F(TestPlane, IntersectBoxBorderX) {
	AABB box (glm::vec3(-1, 0, 0), glm::vec3(0, 1, 1));
	const bool result (plane->intersect(box));
	ASSERT_EQ(true, result);
}
