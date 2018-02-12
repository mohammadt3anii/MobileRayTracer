#include "MobileRT/Vector3D.hpp"
#include "MobileRT/Point3D.hpp"
#include <gtest/gtest.h>

using ::MobileRT::Point3D;
using ::MobileRT::Vector3D;

class TestVector3D : public testing::Test {
protected:
	Vector3D *vector;

	virtual void SetUp() {
		vector = new Vector3D(1.0f, 2.0f, 3.0f);
		const float magnitude (::std::sqrt(1*1 + 2*2 + 3*3));

		ASSERT_EQ(1.0f, vector->x_());
		ASSERT_EQ(2.0f, vector->y_());
		ASSERT_EQ(3.0f, vector->z_());
		ASSERT_EQ(magnitude, vector->magnitude_);
	}

	virtual void TearDown() {
		delete vector;
	}
};

TEST_F(TestVector3D, ConstructorVALUES) {
	ASSERT_EQ(1.0f, vector->x_());
	ASSERT_EQ(2.0f, vector->y_());
	ASSERT_EQ(3.0f, vector->z_());
}

TEST_F(TestVector3D, ConstructorCOPY) {
	Vector3D vector1(*vector);
	const float magnitude(::std::sqrt(1.0f*1.0f + 2.0f*2.0f + 3.0f*3.0f));

	ASSERT_EQ(1.0f, vector1.x_());
	ASSERT_EQ(2.0f, vector1.y_());
	ASSERT_EQ(3.0f, vector1.z_());
	ASSERT_EQ(magnitude, vector1.magnitude_);
}

TEST_F(TestVector3D, ConstructorMOVE) {
	Vector3D vector1(::std::move(*vector));
	const float magnitude(::std::sqrt(1.0f*1.0f + 2.0f*2.0f + 3.0f*3.0f));

	ASSERT_EQ(1.0f, vector1.x_());
	ASSERT_EQ(2.0f, vector1.y_());
	ASSERT_EQ(3.0f, vector1.z_());
	ASSERT_EQ(magnitude, vector1.magnitude_);
}

TEST_F(TestVector3D, ConstructorLESS) {
	/*Vector3D vector1(3.0f, 2.0f, 1.0f);
	Vector3D vector2(*vector, vector1, 2.0f);
	const float magnitude(::std::sqrt(-5*-5 + -2*-2 + 1*1));

	ASSERT_EQ(-5.0f, vector2.x_);
	ASSERT_EQ(-2.0f, vector2.y_);
	ASSERT_EQ(1.0f, vector2.z_);
	ASSERT_EQ(magnitude, vector2.magnitude_);*/
}

TEST_F(TestVector3D, ConstructorPoints) {
	Point3D point1(3.0f, 2.0f, 1.0f);
	Point3D point2(1.0f, 2.0f, 3.0f);
	Vector3D vector1(point1, point2);
	const float magnitude(::std::sqrt(2*2 + 0*0 + -2*-2));

	ASSERT_EQ(3.0f, point1.x_());
	ASSERT_EQ(2.0f, point1.y_());
	ASSERT_EQ(1.0f, point1.z_());

	ASSERT_EQ(1.0f, point2.x_());
	ASSERT_EQ(2.0f, point2.y_());
	ASSERT_EQ(3.0f, point2.z_());
	
	ASSERT_EQ(2.0f, vector1.x_());
	ASSERT_EQ(0.0f, vector1.y_());
	ASSERT_EQ(-2.0f, vector1.z_());
	ASSERT_EQ(magnitude, vector1.magnitude_);
}

TEST_F(TestVector3D, ConstructorCrossProduct) {
	Vector3D vector1(3.0f, 2.0f, 1.0f);
	Vector3D vector2(*vector, vector1);
	const float magnitude(::std::sqrt(-4*-4 + 8*8 + -4*-4));

	ASSERT_EQ(3.0f, vector1.x_());
	ASSERT_EQ(2.0f, vector1.y_());
	ASSERT_EQ(1.0f, vector1.z_());

	ASSERT_EQ(-4.0f, vector2.x_());
	ASSERT_EQ(8.0f, vector2.y_());
	ASSERT_EQ(-4.0f, vector2.z_());
	ASSERT_EQ(magnitude, vector2.magnitude_);
}
