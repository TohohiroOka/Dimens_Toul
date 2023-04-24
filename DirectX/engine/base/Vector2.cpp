#include "Vector2.h"
#include <cmath>

//コンストラクタ
Vector2::Vector2()
	:x(0), y(0)
{
}


// 値を用いての初期化
Vector2::Vector2(float x, float y)
	:x(x), y(y)
{
}

// ベクトルの大きさ
float Vector2::length() const {
	return float(sqrt(x * x + y * y));
}

// ベクトルの正規化
Vector2& Vector2::normalize() {
	float len = length();
	if (len != 0) {
		*this /= len;
	}
	return *this;
}

// ベクトルの内積
float Vector2::dot(const Vector2& v) const {
	return x * v.x + y * v.y;
}

//ベクトルの外積
float Vector2::cross(const Vector2& v) const {
	return x * v.x - y * v.y;
}

//単項演算子+の追加
Vector2 Vector2::operator+() const {
	return *this;
}

//単項演算子+の追加
Vector2 Vector2::operator-() const {
	return Vector2(-x, -y);
}

//+=の追加
Vector2& Vector2:: operator+=(const Vector2& v) {
	x += v.x;
	y += v.y;
	return *this;
}

//-=の追加
Vector2& Vector2::operator-=(const Vector2& v) {
	x -= v.x;
	y -= v.y;
	return *this;
}

//*=の追加
Vector2& Vector2::operator*=(float s) {
	x *= s;
	y *= s;
	return *this;
}

// /=の追加
Vector2& Vector2::operator/=(float s) {
	x /= s;
	y /= s;
	return *this;
}

// ベクトルの加算
const Vector2 operator+(const Vector2& v1, const Vector2& v2) {
	Vector2 temp(v1);
	return temp += v2;
}

// ベクトルの減算
const Vector2 operator-(const Vector2& v1, const Vector2& v2) {
	Vector2 temp(v1);
	return temp -= v2;
}

const Vector2 operator*(const Vector2& v, float s) {
	Vector2 temp(v);
	return temp *= s;
}

//ベクトルの乗算
const Vector2 operator*(float s, const Vector2& v) {
	return v * s;
}

// ベクトルの除算
const Vector2 operator/(const Vector2& v, float s) {
	Vector2 temp(v);
	return temp /= s;
}