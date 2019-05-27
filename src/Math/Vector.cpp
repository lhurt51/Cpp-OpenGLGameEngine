#include "Vector.hpp"

bool Vector3f::operator==(const Vector3f& other) const
{
	return (vec != other.vec).isZero3f();
}

bool Vector3f::operator!=(const Vector3f& other) const
{

}

bool Vector3f::equals(const Vector3f& other, float errorMargin) const
{

}

bool Vector3f::equals(float val, float errorMargin) const
{

}

float Vector3f::operator[](uint32 index) const
{

}

void Vector3f::set(float x, float y, float z)
{

}

void Vector3f::set(uint32 index, float val)
{

}

float Vector3f::max() const
{

}

float Vector3f::min() const
{

}

float Vector3f::absMax() const
{

}

float Vector3f::absMin() const
{

}

Vector3f Vector3f::abs() const
{

}

Vector3f Vector3f::min(const Vector3f& other) const
{

}

Vector3f Vector3f::max(const Vector3f& other) const
{

}

Vector3f Vector3f::normalized(float errorMargin) const
{

}

Vector3f Vector3f::isNormalized(float errorMargin) const
{

}

void Vector3f::dirAndLength(Vector3f& dir, float& length) const
{

}

Vector3f Vector3f::project() const
{

}

Vector3f Vector3f::reciprocal() const
{

}

Vector3f Vector3f::rotate(const Vector3f& axis, float angle) const
{

}

Vector3f Vector3f::reflect(const Vector3f& normal) const
{

}

Vector3f Vector3f::refract(const Vector3f& normal, float indexOfRefraction) const
{

}

Vector3f Vector3f::toDegrees() const
{

}

Vector3f Vector3f::toRadians() const
{

}

Vector Vector3f::toVector(float w) const
{

}