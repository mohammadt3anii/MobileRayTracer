#include "../MobileRT/src/main/cpp/MobileRT/Vector3D.hpp"
#include "../MobileRT/src/main/cpp/MobileRT/Point3D.hpp"
#include <gtest/gtest.h>

using MobileRT::Point3D;
using MobileRT::Vector3D;

class TestVector3D : public testing::Test {
protected:
	Vector3D *vector;

	virtual void SetUp() {
		vector = new Vector3D(1.0f, 2.0f, 3.0f);
	}

	virtual void TearDown() {
		delete vector;
	}
};

TEST_F(TestVector3D, ConstructorVALUES) {
	ASSERT_EQ(1.0f, vector->x_);
	ASSERT_EQ(2.0f, vector->y_);
	ASSERT_EQ(3.0f, vector->z_);
}

TEST_F(TestVector3D, ConstructorCOPY) {
	Vector3D vector1(*vector);
	const float magnitude(std::sqrt(1.0f*1.0f + 2.0f*2.0f + 3.0f*3.0f));

	ASSERT_EQ(1.0f, vector1.x_);
	ASSERT_EQ(2.0f, vector1.y_);
	ASSERT_EQ(3.0f, vector1.z_);
}

TEST_F(TestVector3D, ConstructorMOVE) {
	Vector3D vector1(std::move(*vector));

	ASSERT_EQ(1.0f, vector1.x_);
	ASSERT_EQ(2.0f, vector1.y_);
	ASSERT_EQ(3.0f, vector1.z_);
}