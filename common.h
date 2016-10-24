#pragma once

/*
 * helper code
 */

#include <stdint.h>
#include <math.h>
#include <assert.h>

#include "linear_algebra.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 800
#define HEIGHT 800

#define ARRAY_COUNT( a ) ( sizeof( a ) / sizeof( ( a )[ 0 ] ) )

template< typename To, typename From >
To checked_cast( const From & from ) {
	To result = To( from );
	assert( From( result ) == from );
	return result;
}

template< typename T >
T min( T a, T b ) {
	return a < b ? a : b;
}

template< typename T >
T max( T a, T b ) {
	return a > b ? a : b;
}

template< typename T >
T abs( T x ) {
	return x > 0 ? x : -x;
}

template< typename T >
T clamp( T x, T lo, T hi ) {
	if( x < lo ) return lo;
	if( x > hi ) return hi;
	return x;
}

template< typename T >
T clamp01( T x ) {
	return clamp( x, T( 0 ), T( 1 ) );
}

uint32_t quantize01( float x, uint32_t num_bits ) {
	assert( x >= 0.0f && x <= 1.0f );
	return uint32_t( x * checked_cast< float >( ( 1 << num_bits ) - 1 ) + 0.5f );
}

inline float linear_to_srgb( float x ) {
	if( x <= 0.00031308 ) {
		return 12.92f * x;
	}

	return 1.055f * powf( x, 1.0f / 2.4f ) - 0.055f;
}

inline void save_image( const char * path, v3 * framebuffer ) {
	static uint8_t srgb_framebuffer[ WIDTH * HEIGHT * 3 ];

	/*
	 * convert framebuffer to srgb_framebuffer
	 */
	for( size_t i = 0; i < WIDTH * HEIGHT; i++ ) {
		v3 colour = framebuffer[ i ];
		srgb_framebuffer[ i * 3 + 0 ] = quantize01( clamp01( linear_to_srgb( colour.x ) ), 8 );
		srgb_framebuffer[ i * 3 + 1 ] = quantize01( clamp01( linear_to_srgb( colour.y ) ), 8 );
		srgb_framebuffer[ i * 3 + 2 ] = quantize01( clamp01( linear_to_srgb( colour.z ) ), 8 );
	}

	const int use_rgb = 3;
	const int stride = 0;
	stbi_write_png( path, WIDTH, HEIGHT, use_rgb, srgb_framebuffer, stride );
}

enum ShapeType {
	SHAPE_SPHERE,
	SHAPE_PLANE,
};

struct Shape {
	ShapeType type;

	v3 colour;
	float reflectance;

	// sphere things
	v3 c;
	float r;

	// plane things
	v3 n;
	float d;
};

inline Shape sphere( v3 c, float r, v3 colour, float reflectance = 0 ) {
	Shape shape;
	shape.type = SHAPE_SPHERE;
	shape.colour = colour;
	shape.reflectance = reflectance;
	shape.c = c;
	shape.r = r;
	return shape;
}

inline Shape plane( v3 n, float d, v3 colour, float reflectance = 0 ) {
	Shape shape;
	shape.type = SHAPE_PLANE;
	shape.colour = colour;
	shape.reflectance = reflectance;
	shape.n = normalize( n );
	shape.d = d;
	return shape;
}

static const v3 RED( 1, 0, 0 );
static const v3 ORANGE( 1, 0.5, 0 );
static const v3 GREEN( 0, 1, 0 );
static const v3 BLUE( 0, 0, 1 );
static const v3 WHITE( 1, 1, 1 );
static const v3 BROWN( 1, 0.71, 0.55 );
static const v3 BLACK( 0, 0, 0 );

inline bool ray_vs_sphere( v3 ray_origin, v3 ray_dir, v3 sphere_origin, float sphere_radius, float * t ) {
	v3 m = ray_origin - sphere_origin;

	float b = dot( m, ray_dir );
	float c = dot( m, m ) - sphere_radius * sphere_radius;

	float determinant = b * b - c;
	if( determinant < 0 ) {
		return false;
	}

	*t = -b - sqrtf( determinant );
	return *t >= 0;
}

inline bool ray_vs_plane( v3 ray_origin, v3 ray_dir, v3 plane_normal, float plane_distance, float * t ) {
	const float epsilon = 0.001f;
	float denom = dot( ray_dir, plane_normal );
	if( fabsf( denom ) < epsilon ) {
		return false;
	}

	*t = ( plane_distance - dot( ray_origin, plane_normal ) ) / denom;
	return *t >= 0;
}

inline v3 shape_normal( const Shape & shape, v3 p ) {
	if( shape.type == SHAPE_SPHERE ) {
		return ( p - shape.c ) / shape.r;
	}
	else {
		return shape.n;
	}
}

/*
 * point light struct
 */
struct Light {
	v3 pos;
	v3 colour;
};

inline Light light( v3 pos, v3 colour ) {
	Light l;
	l.pos = pos;
	l.colour = colour;
	return l;
}
