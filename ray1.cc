/*
 * a ray tracer with diffuse materials
 */

#include <stdlib.h>
#include <math.h>

#include "common.h"

static v3 trace_ray( v3 ray_origin, v3 ray_dir, const Shape * scene, size_t num_shapes ) {
	bool hit = false;
	float min_t;
	const Shape * nearest_shape;

	for( size_t i = 0; i < num_shapes; i++ ) {
		const Shape & shape = scene[ i ];
		bool hit_shape;
		float t;

		if( shape.type == SHAPE_SPHERE ) {
			hit_shape = ray_vs_sphere( ray_origin, ray_dir, shape.c, shape.r, &t );
		}
		else {
			hit_shape = ray_vs_plane( ray_origin, ray_dir, shape.n, shape.d, &t );
		}

		if( hit_shape ) {
			if( !hit || t < min_t ) {
				hit = true;
				min_t = t;
				nearest_shape = &shape;
			}
		}
	}

	if( !hit ) {
		/*
		 * fake sky gradient
		 */
		v3 horizon( 0.5, 0.7, 0.92 );
		v3 sky( 0.1, 0.4, 0.8 );
		return lerp( horizon, clamp01( sqrtf( clamp01( ray_dir.y ) ) * 1.5 ), sky );
	}

	return nearest_shape->colour;
}

int main( int argc, char ** argv ) {
	/*
	 * initialise scene
	 */
	const Shape scene[] = {
		sphere( v3( 2, 0, 6 ), 1.5, RED ),
		sphere( v3( -2, -1, 6 ), 1, GREEN ),
		plane( v3( 0, 1, 0 ), -2, BROWN ),
	};
	const size_t num_shapes = ARRAY_COUNT( scene );

	/*
	 * trace rays into the scene and draw into framebuffer
	 */
	static v3 framebuffer[ WIDTH * HEIGHT ];
	const v3 screen_top_left( -1, 1, 1 );
	const v3 screen_top_right( 1, 1, 1 );
	const v3 screen_bot_left( -1, -1, 1 );
	const v3 screen_bot_right( 1, -1, 1 );

	for( size_t y = 0; y < HEIGHT; y++ ) {
		for( size_t x = 0; x < WIDTH; x++ ) {
			v3 t = lerp( screen_top_left, float( x ) / float( WIDTH - 1 ), screen_top_right );
			v3 b = lerp( screen_bot_left, float( x ) / float( WIDTH - 1 ), screen_bot_right );

			v3 screen_pos = lerp( t, float( y ) / float( HEIGHT - 1 ), b );

			v3 ray_origin( 0, 0, 0 );
			v3 ray_dir = normalize( screen_pos - ray_origin );

			v3 colour = trace_ray( ray_origin, ray_dir, scene, num_shapes );
			framebuffer[ y * WIDTH + x ] = colour;
		}
	}

	/*
	 * save as PNG (ray#.cc.png)
	 */
	save_image( __FILE__ ".png", framebuffer );

	return 0;
}
