#include "MobileRT/Shapes/Triangle.hpp"
#include <gtest/gtest.h>

using ::MobileRT::AABB;
using ::MobileRT::Triangle;

class TestTriangle : public testing::Test {
protected:
	Triangle *triangle;

	virtual void SetUp() {
		triangle = new Triangle(::glm::vec3(0,0,0),
														::glm::vec3(0,1,0),
														::glm::vec3(0,0,1));
	}

	virtual void TearDown() {
		delete triangle;
	}
};

TEST_F(TestTriangle, ConstructorVALUES) {
	ASSERT_EQ(0, triangle->pointA_.x);
	ASSERT_EQ(0, triangle->pointA_.y);
	ASSERT_EQ(0, triangle->pointA_.z);

	::glm::vec3 pointB {triangle->pointA_ + triangle->AB_};
	::glm::vec3 pointC {triangle->pointA_ + triangle->AC_};
	ASSERT_EQ(0, pointB.x);
	ASSERT_EQ(1, pointB.y);
	ASSERT_EQ(0, pointB.z);

	ASSERT_EQ(0, pointC.x);
	ASSERT_EQ(0, pointC.y);
	ASSERT_EQ(1, pointC.z);

	ASSERT_EQ(0, triangle->AC_.x);
	ASSERT_EQ(0, triangle->AC_.y);
	ASSERT_EQ(1, triangle->AC_.z);

	ASSERT_EQ(0, triangle->AB_.x);
	ASSERT_EQ(1, triangle->AB_.y);
	ASSERT_EQ(0, triangle->AB_.z);

	::glm::vec3 bc {pointC - pointB};
	ASSERT_EQ(0, bc.x);
	ASSERT_EQ(-1, bc.y);
	ASSERT_EQ(1, bc.z);

	ASSERT_FLOAT_EQ(1, triangle->normal_.x);
	ASSERT_EQ(0, triangle->normal_.y);
	ASSERT_EQ(0, triangle->normal_.z);
}

TEST_F(TestTriangle, AABB) {
	AABB box {triangle->getAABB()};
	LOG("A = ", box.pointMin_.x);

	ASSERT_FLOAT_EQ(0, box.pointMin_.x);
	/*ASSERT_EQ(0, box.pointMin_.y_());
	ASSERT_EQ(0, box.pointMin_.z_());

	ASSERT_EQ(0, box.pointMax_.x_());
	ASSERT_EQ(1, box.pointMax_.y_());
	ASSERT_EQ(1, box.pointMax_.z_());*/
}

/*TEST_F(TestTriangle, intersectBoxInside01) {
	::glm::vec3 min {-1, -1, -1};
	::glm::vec3 max {2, 2, 2};
	AABB box {min, max};
	bool intersected = triangle->intersect(box);
	ASSERT_EQ(true, intersected);
}

TEST_F(TestTriangle, intersectBoxInside02) {
	::glm::vec3 min {0, 0, 0};
	::glm::vec3 max {2, 2, 2};
	AABB box {min, max};
	bool intersected = triangle->intersect(box);
	ASSERT_EQ(true, intersected);
}

TEST_F(TestTriangle, intersectBoxInside03) {
	::glm::vec3 min {0, 0, 0};
	::glm::vec3 max {0, 1, 1};
	AABB box {min, max};
	bool intersected = triangle->intersect(box);
	ASSERT_EQ(true, intersected);
}

TEST_F(TestTriangle, intersectBoxInside04) {
	::glm::vec3 min {0, 0, 0};
	::glm::vec3 max {0, 0.5, 0.5};
	AABB box {min, max};
	bool intersected = triangle->intersect(box);
	ASSERT_EQ(true, intersected);
}

TEST_F(TestTriangle, intersectBoxInside05) {
	::glm::vec3 min {-1, -1, -1};
	::glm::vec3 max {0.1f, 0.1f, 0.1f};
	AABB box {min, max};
	bool intersected = triangle->intersect(box);
	ASSERT_EQ(true, intersected);
}

TEST_F(TestTriangle, intersectBoxInside06) {
	::glm::vec3 min {-1, 0.4f, 0.4f};
	::glm::vec3 max {1, 1.4f, 1.4f};
	AABB box {min, max};
	bool intersected = triangle->intersect(box);
	ASSERT_EQ(true, intersected);
}

TEST_F(TestTriangle, intersectBoxInside07) {
	::glm::vec3 min {-1, 0.4f, 0.7f};
	::glm::vec3 max {1, 1.4f, 1.4f};
	AABB box {min, max};
	bool intersected = triangle->intersect(box);
	ASSERT_EQ(false, intersected);
}

TEST_F(TestTriangle, intersectBoxInside08) {
	Triangle triangle2 {
		::MobileRT::glm::vec3 {10.0f, 0.0f, 10.0f},
		::MobileRT::glm::vec3 {0.0f, 0.0f, 10.0f},
		::MobileRT::glm::vec3 {0.0f, 10.0f, 10.0f}};
	::glm::vec3 min {1.25f, 1.25f, 10};
	::glm::vec3 max {2.5f, 2.5f, 10};
	AABB box {min, max};
	bool intersected = triangle2.intersect(box);
	ASSERT_EQ(true, intersected);
}

TEST_F(TestTriangle, intersectBoxInside09) {
	Triangle triangle2 {
		::MobileRT::glm::vec3 {10.0f, 0.0f, 10.0f},
		::MobileRT::glm::vec3 {0.0f, 0.0f, 10.0f},
		::MobileRT::glm::vec3 {0.0f, 10.0f, 10.0f}};
	::glm::vec3 min {-1, -1, 10};
	::glm::vec3 max {11, 11, 10};
	AABB box {min, max};
	bool intersected = triangle2.intersect(box);
	ASSERT_EQ(true, intersected);
}

TEST_F(TestTriangle, intersectBoxInside10) {
	Triangle triangle2 {
		::MobileRT::glm::vec3 {1, 1.59000003f, -1.03999996f},
		::MobileRT::glm::vec3 {-1.01999998f, 1.59000003f, -1.03999996f},
		::MobileRT::glm::vec3 {-0.990000009f, 0, -1.03999996f}};
	::glm::vec3 min {-11.0200005f, 0.794949531f, -11.04f};
	::glm::vec3 max {-0.0100002289f, 11.5899992f, -0.0250005722f};
	AABB box {min, max};
	bool intersected = triangle2.intersect(box);
	ASSERT_EQ(true, intersected);
}*/

/*TEST_F(Test::glm::vec3, ConstructorCOPY) {
	::glm::vec3 point1(*point);

	ASSERT_EQ(1.0f, triangle.);
	ASSERT_EQ(2.0f, point1.y_);
	ASSERT_EQ(3.0f, point1.z_);
}

TEST_F(Test::glm::vec3, ConstructorMOVE) {
	::glm::vec3 point1(::std::move(*point));

	ASSERT_EQ(1.0f, point1.x_);
	ASSERT_EQ(2.0f, point1.y_);
	ASSERT_EQ(3.0f, point1.z_);
}

TEST_F(Test::glm::vec3, OperatorLESS) {
	::glm::vec3 point1(3.0f, 2.0f, 1.0f);
	::glm::vec3 vector(*point - point1);

	ASSERT_EQ(-2.0f, vector.x_);
	ASSERT_EQ(0.0f, vector.y_);
	ASSERT_EQ(2.0f, vector.z_);
}

TEST_F(Test::glm::vec3, OperatorMORE) {
	::glm::vec3 vector(3.0f, 2.0f, 1.0f);
	::glm::vec3 point1(*point + vector);

	ASSERT_EQ(4.0f, point1.x_);
	ASSERT_EQ(4.0f, point1.y_);
	ASSERT_EQ(4.0f, point1.z_);
}
*/
