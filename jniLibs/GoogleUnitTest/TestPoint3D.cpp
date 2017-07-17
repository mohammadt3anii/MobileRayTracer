#include "../MobileRT/src/main/cpp/MobileRT/Point3D.hpp"
#include "../MobileRT/src/main/cpp/MobileRT/Vector3D.hpp"
#include <gtest/gtest.h>

using MobileRT::Point3D;
using MobileRT::Vector3D;

class TestPoint3D : public testing::Test {
protected:
	Point3D *point;

	virtual void SetUp() {
		point = new Point3D(1.0f, 2.0f, 3.0f);
	}

	virtual void TearDown() {
		delete point;
	}
};

TEST_F(TestPoint3D, ConstructorVALUES) {
	ASSERT_EQ(1.0f, point->x_);
	ASSERT_EQ(2.0f, point->y_);
	ASSERT_EQ(3.0f, point->z_);
}

TEST_F(TestPoint3D, ConstructorCOPY) {
	Point3D point1(*point);

	ASSERT_EQ(1.0f, point1.x_);
	ASSERT_EQ(2.0f, point1.y_);
	ASSERT_EQ(3.0f, point1.z_);
}

TEST_F(TestPoint3D, ConstructorMOVE) {
	Point3D point1(std::move(*point));

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
