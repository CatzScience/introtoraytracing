/*
 * vector code
 */

#pragma once

#include <math.h>
#include <assert.h>

struct v3 {
	float x, y, z;

	v3() { }

	explicit v3( float a ) {
		x = a;
		y = a;
		z = a;
	}

	explicit v3( float a, float b, float c ) {
		x = a;
		y = b;
		z = c;
	}
};

v3 operator+( v3 lhs, v3 rhs ) {
	return v3( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}

void operator+=( v3 & lhs, v3 rhs ) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
}

v3 operator-( v3 lhs, v3 rhs ) {
	return v3( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}

v3 operator*( v3 v, float scale ) {
	return v3( v.x * scale, v.y * scale, v.z * scale );
}

v3 operator*( float scale, v3 v ) {
	return v * scale;
}

v3 operator*( v3 u, v3 v ) {
	return v3( u.x * v.x, u.y * v.y, u.z * v.z );
}

v3 operator/( v3 v, float inv_scale ) {
	float scale = 1.0f / inv_scale;
	return v * scale;
}

void operator/=( v3 & lhs, float inv_scale ) {
	float scale = 1.0f / inv_scale;
	lhs.x *= scale;
	lhs.y *= scale;
	lhs.z *= scale;
}

float dot( v3 u, v3 v ) {
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

v3 cross( v3 u, v3 v ) {
	return v3(
		u.y * v.z - v.y * u.z,
		v.x * u.z - u.x * v.z,
		u.x * v.y - v.x * u.y
	);
}

float length( v3 v ) {
	return sqrtf( v.x * v.x + v.y * v.y + v.z * v.z );
}

v3 normalize( v3 v ) {
	return v / length( v );
}

v3 reflect( v3 u, v3 n ) {
	return u - 2 * dot( u, n ) * n;
}

float lerp( float a, float t, float b ) {
	assert( t >= 0.0f && t <= 1.0f );
	return ( 1.0f - t ) * a + t * b;
}

v3 lerp( v3 u, float t, v3 v ) {
	return v3(
		lerp( u.x, t, v.x ),
		lerp( u.y, t, v.y ),
		lerp( u.z, t, v.z )
	);
}
