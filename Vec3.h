#pragma once

#include <stdio.h>

	template <typename T>
class Vec3
{
public:
	T val[3];
public:
	Vec3(void)
	{
		val[0] = (T)0;
		val[1] = (T)0;
		val[2] = (T)0;
	}

	Vec3(double v)
	{
		val[0] = v;
		val[1] = v;
		val[2] = v;

	}

	Vec3(Vec3 const& v)
	{
		val[0] = v.val[0];
		val[1] = v.val[1];
		val[2] = v.val[2];
	}

	Vec3(T const& x, T const& y, T const& z)
	{
		val[0] = x;
		val[1] = y;
		val[2] = z;
	}

	~Vec3() {}

	void x(T d) { val[0] = d; }
	void y(T d) { val[1] = d; }
	void z(T d) { val[2] = d; }

	void set(double v)
	{
		val[0] = v;
		val[1] = v;
		val[2] = v;

	}

	void set(double& v)
	{
		val[0] = v;
		val[1] = v;
		val[2] = v;

	}

	void set(Vec3<T>& v)
	{
		val[0] = v.val[0];
		val[1] = v.val[1];
		val[2] = v.val[2];
	}

	void set(T x, T y, T z)
	{
		val[0] = x;
		val[1] = y;
		val[2] = z;
	}

	void clear(void)
	{
		val[0] = 0.0;
		val[1] = 0.0;
		val[2] = 0.0;
	}

	void normalize(void)
	{
		double norm = getNorm();
		if (norm != 0) {
			val[0] = val[0] / norm;
			val[1] = val[1] / norm;
			val[2] = val[2] / norm;
		}
	}

	T getNorm(void)
	{
		return sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2]);
	}

	T length(void)
	{
		return(T)(sqrt(lengthSquared()));
	}

	T lengthSquared(void)
	{
		return (T)(val[0] * val[0] + val[1] * val[1] + val[2] * val[2]);
	}

	T dot(Vec3<T>& v)
	{
		return (val[0] * v.x() + val[1] * v.y() + val[2] * v.z());
	}
	
	T x(void) { return val[0]; }
	T y(void) { return val[1]; }
	T z(void) { return val[2]; }

	Vec3<T>	cross(Vec3<T>& v)
	{
		Vec3<T> vector;
		vector.x((val[1] * v.z()) - (val[2] * v.y()));
		vector.y((val[2] * v.x()) - (val[0] * v.z()));
		vector.z((val[0] * v.y()) - (val[1] * v.x()));
		return vector;
	}

public:
	bool operator==(Vec3 const& v) const
	{
		return val[0] == v.val[0] && val[1] == v.val[1] && val[2] == v.val[2];
	}

	bool operator!=(Vec3 const& v) const
	{
		return val[0] != v.val[0] && val[1] != v.val[1] && val[2] != v.val[2];
	}

	T& operator()(int index)
	{
		return  *((&val[0]) + index);
	}

	T& operator[](int index)
	{
		return  *((&val[0]) + index);
	}

	T const& operator()(int index) const
	{
		return  *((&val[0]) + index);
	}

	Vec3<T> operator*(Vec3<T> v)
	{
		return Vec3<T>(this->x() * v.x(), this->y() * v.y(), this->z() * v.z());
	}

	T const& operator[](int index) const
	{
		return  *((&val[0]) + index);
	}

	Vec3<T>& operator=(Vec3 const& v)
	{
		val[0] = v.val[0];
		val[1] = v.val[1];
		val[2] = v.val[2];
		return *this;
	}

	Vec3<T>& operator+=(Vec3 const& v)
	{
		val[0] += v.val[0];
		val[1] += v.val[1];
		val[2] += v.val[2];
		return *this;
	}

	Vec3<T>& operator+=(T v)
	{
		val[0] += v;
		val[1] += v;
		val[2] += v;
		return *this;
	}

	Vec3<T>& operator-=(T v)
	{
		val[0] -= v;
		val[1] -= v;
		val[2] -= v;
		return *this;
	}

	Vec3<T>& operator-=(Vec3 const& v)
	{
		val[0] -= v.val[0];
		val[1] -= v.val[1];
		val[2] -= v.val[2];
		return *this;
	}

	Vec3<T>& operator*=(T const& d)
	{
		val[0] *= d;
		val[1] *= d;
		val[2] *= d;
		return *this;
	}

	Vec3<T>& operator*=(Vec3<T>& v)
	{
		val[0] *= v.x();
		val[1] *= v.y();
		val[2] *= v.z();
		return *this;
	}

	Vec3<T>& operator/=(T const& d)
	{
		val[0] /= d;
		val[1] /= d;
		val[2] /= d;
		return *this;
	}
	Vec3<T>	operator/(const T& d)
	{
		return Vec3(val[0] / d, val[1] / d, val[2] / d);
	}
	Vec3<T>	operator*(const T& d)
	{
		return Vec3(val[0] * d, val[1] * d, val[2] * d);
	}
	Vec3<T>	operator-(const T& d)
	{
		return Vec3(val[0] - d, val[1] - d, val[2] - d);
	}
	Vec3<T>	operator+(const T& d)
	{
		return Vec3(val[0] + d, val[1] + d, val[2] + d);
	}
	Vec3<T>	operator-() const
	{
		return Vec3(-val[0], -val[1], -val[2]);
	}
	Vec3<T>	operator+(Vec3 const& v) const
	{
		return Vec3(val[0] + v.val[0], val[1] + v.val[1], val[2] + v.val[2]);
	}
	Vec3<T>	operator-(Vec3 const& v) const
	{
		return Vec3(val[0] - v.val[0], val[1] - v.val[1], val[2] - v.val[2]);
	}
	Vec3<T>	operator/(Vec3<T>& v)
	{
		return Vec3(val[0] / v.val[0], val[1] / v.val[1], val[2] / v.val[2]);
	}
	};
	typedef Vec3<double> vec3;
