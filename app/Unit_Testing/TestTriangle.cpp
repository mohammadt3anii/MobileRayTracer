#include "MobileRT/Shapes/Triangle.hpp"
#include <gtest/gtest.h>

using ::MobileRT::AABB;
using ::MobileRT::Point3D;
using ::MobileRT::Triangle;

class TestTriangle : public testing::Test {
protected:
	Triangle *triangle;

	virtual void SetUp() {
		triangle = new Triangle(Point3D(0,0,0),
														Point3D(0,1,0),
														Point3D(0,0,1));
	}

	virtual void TearDown() {
		delete triangle;
	}
};

TEST_F(TestTriangle, ConstructorVALUES) {
	ASSERT_EQ(0, triangle->pointA_.x_());
	ASSERT_EQ(0, triangle->pointA_.y_());
	ASSERT_EQ(0, triangle->pointA_.z_());

	ASSERT_EQ(0, triangle->pointB_.x_());
	ASSERT_EQ(1, triangle->pointB_.y_());
	ASSERT_EQ(0, triangle->pointB_.z_());

	ASSERT_EQ(0, triangle->pointC_.x_());
	ASSERT_EQ(0, triangle->pointC_.y_());
	ASSERT_EQ(1, triangle->pointC_.z_());

	ASSERT_EQ(0, triangle->AC_.x_());
	ASSERT_EQ(0, triangle->AC_.y_());
	ASSERT_EQ(1, triangle->AC_.z_());

	ASSERT_EQ(0, triangle->AB_.x_());
	ASSERT_EQ(1, triangle->AB_.y_());
	ASSERT_EQ(0, triangle->AB_.z_());

	ASSERT_EQ(0, triangle->BC_.x_());
	ASSERT_EQ(-1, triangle->BC_.y_());
	ASSERT_EQ(1, triangle->BC_.z_());

	ASSERT_FLOAT_EQ(1, triangle->normal_.x_());
	ASSERT_EQ(0, triangle->normal_.y_());
	ASSERT_EQ(0, triangle->normal_.z_());
}

TEST_F(TestTriangle, AABB) {
	AABB box = triangle->getAABB();

	ASSERT_EQ(0, box.pointMin_.x_());
	ASSERT_EQ(0, box.pointMin_.y_());
	ASSERT_EQ(0, box.pointMin_.z_());

	ASSERT_EQ(0, box.pointMax_.x_());
	ASSERT_EQ(1, box.pointMax_.y_());
	ASSERT_EQ(1, box.pointMax_.z_());
}

TEST_F(TestTriangle, intersectBoxInside01) {
	Point3D min {-1, -1, -1};
	Point3D max {2, 2, 2};
	AABB box {min, max};
	bool intersected = triangle->intersect(box);
	ASSERT_EQ(true, intersected);
}

TEST_F(TestTriangle, intersectBoxInside02) {
	Point3D min {0, 0, 0};
	Point3D max {2, 2, 2};
	AABB box {min, max};
	bool intersected = triangle->intersect(box);
	ASSERT_EQ(true, intersected);
}

TEST_F(TestTriangle, intersectBoxInside03) {
	Point3D min {0, 0, 0};
	Point3D max {0, 1, 1};
	AABB box {min, max};
	bool intersected = triangle->intersect(box);
	ASSERT_EQ(true, intersected);
}

TEST_F(TestTriangle, intersectBoxInside04) {
	Point3D min {0, 0, 0};
	Point3D max {0, 0.5, 0.5};
	AABB box {min, max};
	bool intersected = triangle->intersect(box);
	ASSERT_EQ(true, intersected);
}

TEST_F(TestTriangle, intersectBoxInside05) {
	Point3D min {-1, -1, -1};
	Point3D max {0.1, 0.1, 0.1};
	AABB box {min, max};
	bool intersected = triangle->intersect(box);
	ASSERT_EQ(true, intersected);
}

TEST_F(TestTriangle, intersectBoxInside06) {
	Point3D min {-1, 0.4, 0.4};
	Point3D max {1, 1.4, 1.4};
	AABB box {min, max};
	bool intersected = triangle->intersect(box);
	ASSERT_EQ(true, intersected);
}

TEST_F(TestTriangle, intersectBoxInside07) {
	Point3D min {-1, 0.4, 0.7};
	Point3D max {1, 1.4, 1.4};
	AABB box {min, max};
	bool intersected = triangle->intersect(box);
	ASSERT_EQ(false, intersected);
}

TEST_F(TestTriangle, intersectBoxInside08) {
	Triangle triangle2 {
		::MobileRT::Point3D {10.0f, 0.0f, 10.0f},
		::MobileRT::Point3D {0.0f, 0.0f, 10.0f},
		::MobileRT::Point3D {0.0f, 10.0f, 10.0f}};
	Point3D min {1.25, 1.25, 10};
	Point3D max {2.5, 2.5, 10};
	AABB box {min, max};
	bool intersected = triangle2.intersect(box);
	ASSERT_EQ(true, intersected);
}

TEST_F(TestTriangle, intersectBoxInside09) {
	Triangle triangle2 {
		::MobileRT::Point3D {10.0f, 0.0f, 10.0f},
		::MobileRT::Point3D {0.0f, 0.0f, 10.0f},
		::MobileRT::Point3D {0.0f, 10.0f, 10.0f}};
	Point3D min {-1, -1, 10};
	Point3D max {11, 11, 10};
	AABB box {min, max};
	bool intersected = triangle2.intersect(box);
	ASSERT_EQ(true, intersected);
}

TEST_F(TestTriangle, intersectBoxInside10) {
	Triangle triangle2 {
		::MobileRT::Point3D {1, 1.59000003, -1.03999996},
		::MobileRT::Point3D {-1.01999998, 1.59000003, -1.03999996},
		::MobileRT::Point3D {-0.990000009, 0, -1.03999996}};
	Point3D min {-11.0200005, 0.794949531, -11.04};
	Point3D max {-0.0100002289, 11.5899992, -0.0250005722};
	AABB box {min, max};
	bool intersected = triangle2.intersect(box);
	ASSERT_EQ(true, intersected);
}

/*TEST_F(TestPoint3D, ConstructorCOPY) {
	Point3D point1(*point);

	ASSERT_EQ(1.0f, triangle.);
	ASSERT_EQ(2.0f, point1.y_);
	ASSERT_EQ(3.0f, point1.z_);
}

TEST_F(TestPoint3D, ConstructorMOVE) {
	Point3D point1(::std::move(*point));

	ASSERT_EQ(1.0f, point1.x_);
	ASSERT_EQ(2.0f, point1.y_);
	ASSERT_EQ(3.0f, point1.z_);
}

TEST_F(TestPoint3D, OperatorLESS) {
	Point3D point1(3.0f, 2.0f, 1.0f);
	Vector3D vector(*point - point1);

	ASSERT_EQ(-2.0f, vector.x_);
	ASSERT_EQ(0.0f, vector.y_);
	ASSERT_EQ(2.0f, vector.z_);
}

TEST_F(TestPoint3D, OperatorMORE) {
	Vector3D vector(3.0f, 2.0f, 1.0f);
	Point3D point1(*point + vector);

	ASSERT_EQ(4.0f, point1.x_);
	ASSERT_EQ(4.0f, point1.y_);
	ASSERT_EQ(4.0f, point1.z_);
}
*/