/*
 * a ray tracer with diffuse materials, lighting, working shadows and reflections
 */

#include <stdlib.h>
#include <math.h>

#include "common.h"

static bool ray_vs_scene( v3 ray_origin, v3 ray_dir, const Shape * scene, size_t num_shapes, const Shape ** shape, float * t ) {
	bool hit = false;
	float min_t;
	const Shape * nearest_shape;

	for( size_t i = 0; i < num_shapes; i++ ) {
		const Shape & shape = scene[ i ];
		bool hit_shape;
		float shape_t;

		if( shape.type == SHAPE_SPHERE ) {
			hit_shape = ray_vs_sphere( ray_origin, ray_dir, shape.c, shape.r, &shape_t );
		}
		else {
			hit_shape = ray_vs_plane( ray_origin, ray_dir, shape.n, shape.d, &shape_t );
		}

		if( hit_shape ) {
			if( !hit || shape_t < min_t ) {
				hit = true;
				min_t = shape_t;
				nearest_shape = &shape;
			}
		}
	}

	if( hit ) {
		*t = min_t;
		*shape = nearest_shape;
		return true;
	}

	return false;
}

static v3 trace_ray( v3 ray_origin, v3 ray_dir, const Shape * scene, size_t num_shapes, const Light * lights, size_t num_lights ) {
	const Shape * shape;
	float scene_t;
	bool hit = ray_vs_scene( ray_origin, ray_dir, scene, num_shapes, &shape, &scene_t );

	if( !hit ) {
		/*
		 * fake sky gradient
		 */
		v3 horizon( 0.5, 0.7, 0.92 );
		v3 sky( 0.1, 0.4, 0.8 );
		return lerp( horizon, clamp01( sqrtf( clamp01( ray_dir.y ) ) * 1.5 ), sky );
	}

	v3 intersection_point = ray_origin + ray_dir * scene_t;
	v3 normal = shape_normal( *shape, intersection_point );
	v3 colour( 0, 0, 0 );
	for( size_t j = 0; j < num_lights; j++ ) {
		const Light & light = lights[ j ];

		v3 light_dir = light.pos - intersection_point;
		float sqlen = dot( light_dir, light_dir );
		light_dir /= sqrtf( sqlen );

		const Shape * dont_care;
		float light_t;
		const float epsilon = 0.001f;
		bool light_hit = ray_vs_scene( intersection_point + epsilon * light_dir, light_dir,
			scene, num_shapes, &dont_care, &light_t );

		if( !light_hit || light_t >= sqlen ) {
			float lambert = clamp01( dot( light_dir, normal ) );
			float falloff = 1 / sqlen;
			colour += lambert * falloff * light.colour * shape->colour;
		}
	}

	if( shape->reflectance > 0 ) {
		const float epsilon = 0.001f;
		v3 reflected = reflect( ray_dir, normal );
		colour += shape->reflectance *
			trace_ray( intersection_point + epsilon * reflected, reflected,
				scene, num_shapes, lights, num_lights );
	}

	return colour;
}

int main( int argc, char ** argv ) {
	/*
	 * initialise scene
	 */
	const Shape scene[] = {
		sphere( v3( 2, 0, 6 ), 1.5, RED ),
		sphere( v3( -2, -1, 6 ), 1, GREEN, 0.3 ),
		sphere( v3( 0, -1, 8 ), 1, WHITE, 1.0 ),
		plane( v3( 0, 1, 0 ), -2, BROWN, 0.1 ),
	};
	const size_t num_shapes = ARRAY_COUNT( scene );

	const Light lights[] = {
		light( v3( 0, 3, 6 ), WHITE * 2 ),
		light( v3( 2, 2, 3 ), ORANGE ),
	};
	const size_t num_lights = ARRAY_COUNT( lights );

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

			v3 colour = trace_ray( ray_origin, ray_dir, scene, num_shapes, lights, num_lights );
			framebuffer[ y * WIDTH + x ] = colour;
		}
	}

	/*
	 * save as PNG (ray#.cc.png)
	 */
	save_image( __FILE__ ".png", framebuffer );

	return 0;
}
