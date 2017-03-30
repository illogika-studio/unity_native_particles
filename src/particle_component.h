#pragma once
#include "globals.h"
#include "memory_pool.h"
#include <GL/gl.h>

#include <cassert>
#include <cstdlib>
#include <utility>

struct vec3 {
	GLfloat x;
	GLfloat y;
	GLfloat z;

	inline vec3& operator+=(const vec3& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	
	inline friend vec3 operator+(vec3 lhs, const vec3& rhs) {
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
	}

	inline friend vec3 operator*(float lhs, const vec3& rhs) {
		return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z };
	}

	inline friend vec3 operator*(const vec3& rhs, float lhs) {
		return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z };
	}
};

struct soa_vec3 {
	GLfloat* x = nullptr;
	GLfloat* y = nullptr;
	GLfloat* z = nullptr;

	~soa_vec3()
	{
		x = nullptr;
		y = nullptr;
		z = nullptr;
	}

	struct vec3_view {
		inline vec3_view& operator=(vec3&& v) {
			x = std::move(v.x);
			y = std::move(v.y);
			z = std::move(v.z);
			return *this;
		}

		inline vec3_view& operator+=(const vec3& rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
		
		inline vec3_view& operator*=(const vec3& rhs) {
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
			return *this;
		}

		inline friend vec3 operator*(const vec3_view& lhs, float rhs) {
			return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
		}

		GLfloat& x;
		GLfloat& y;
		GLfloat& z;
	};

	vec3_view operator[](size_t idx) {
		return {x[idx], y[idx], z[idx]};
	}

	static size_t element_byte_size() {
		return sizeof(GLfloat);
	}

	static size_t total_byte_size() {
		return 3 * sizeof(GLfloat);
	}
};


namespace ParticleComponent {
	namespace {
		/* TODO: Expose some of these. */
		const vec3 _gravity = { 0.f, -9.8f, 0.f };
		const int _random_seed = 1029831;
		const float _velocity = 0.01f;
		const float _y_extent = 20.f;
		const float _spread = 1.5f;
		const float _global_speed = 0.1f;

		struct ParticleData {
			ParticleData() = default;
			ParticleData(const ParticleData&) = delete;
			ParticleData(ParticleData&&) = default;

			ParticleData(GLsizei qty)
				: size(qty)
			{
				Memory::allocate_mem_pool(size * 4 * soa_vec3::total_byte_size());
				pos.x = (float*)Memory::get_sub_pool(qty * soa_vec3::element_byte_size());
				pos.y = (float*)Memory::get_sub_pool(qty * soa_vec3::element_byte_size());
				pos.z = (float*)Memory::get_sub_pool(qty * soa_vec3::element_byte_size());
				rot.x = (float*)Memory::get_sub_pool(qty * soa_vec3::element_byte_size());
				rot.y = (float*)Memory::get_sub_pool(qty * soa_vec3::element_byte_size());
				rot.z = (float*)Memory::get_sub_pool(qty * soa_vec3::element_byte_size());
				scale.x = (float*)Memory::get_sub_pool(qty * soa_vec3::element_byte_size());
				scale.y = (float*)Memory::get_sub_pool(qty * soa_vec3::element_byte_size());
				scale.z = (float*)Memory::get_sub_pool(qty * soa_vec3::element_byte_size());
				speed.x = (float*)Memory::get_sub_pool(qty * soa_vec3::element_byte_size());
				speed.y = (float*)Memory::get_sub_pool(qty * soa_vec3::element_byte_size());
				speed.z = (float*)Memory::get_sub_pool(qty * soa_vec3::element_byte_size());

				srand(_random_seed); // Intentional, use deterministic randomness. TODO: gpu_rand()
				float rand_float = 0.f;
				vec3 main_dir = { 0.f, 5.f, 0.f };

				for (int i = 0; i < size; ++i) {
					rand_float = (static_cast<float>(rand() % 2000) - 1000.f) / 1000.f;

					pos[i] = { 0.f, 0.f, 0.f };
					rot[i] = {
						((static_cast<float>(rand()) / RAND_MAX) * 2.f - 1.f) * 360.f,
						((static_cast<float>(rand()) / RAND_MAX) * 2.f - 1.f) * 360.f,
						((static_cast<float>(rand()) / RAND_MAX) * 2.f - 1.f) * 360.f 
					};
					scale[i] = { 0.25f + rand_float, 0.25f + rand_float, 1.f };
					speed[i] = main_dir + vec3 {
						(static_cast<float>(rand() % 2000) - 1000.f) / 1000.f,
						(static_cast<float>(rand() % 2000) - 1000.f) / 1000.f,
						(static_cast<float>(rand() % 2000) - 1000.f) / 1000.f
					} * _spread;
				}
			}

			~ParticleData()
			{
				Memory::deallocate_mem_pool();
			}

			inline ParticleData& operator=(ParticleData&& p) {
				if (this == &p)
					return *this;

				/* TODO : Redesign memory pool to support multiple pools. */
				//if (size != 0) {
				//	Memory::deallocate_mem_pool();
				//}

				size = std::move(p.size);
				pos = std::move(p.pos);
				rot = std::move(p.rot);
				scale = std::move(p.scale);
				speed = std::move(p.speed);
			}

			GLsizei size = 0;
			soa_vec3 pos;
			soa_vec3 rot;
			soa_vec3 scale;
			soa_vec3 speed;
		} _data;
	}

	void init(GLsizei qty)
	{
		if (qty == 0) {
			OUTPUT_ERROR("Please set the quantity of particles desired.\n");
			return;
		}

		_data = ParticleData(qty);
	}

	void update(float delta_time)
	{
		OUTPUT_MSG("Updating");
		float r = 0.f;

		/* Speed */
		for (int i = 0; i < _data.size; ++i) {
			_data.speed[i] += _gravity * delta_time * _global_speed;
		}

		/* Position */
		for (int i = 0; i < _data.size; ++i) {
			if (_data.pos[i].y > _y_extent && _data.speed[i].y > 0.f) {
				_data.speed[i].y = -_data.speed[i].y;
			} else if (_data.pos[i].y < -_y_extent && _data.speed[i].y < 0.f) {
				_data.speed[i].y = -_data.speed[i].y;
			}
			_data.pos[i] += _data.speed[i] * delta_time;
		}

		/* Rotation */
		for (int i = 0; i < _data.size; ++i) {
			_data.rot[i].x += 0.1f;
			_data.rot[i].y += 0.0001f;
		}

		/* Scale */
		//for (int i = 0; i < _data.size; ++i) {
		//	_data.scale[i].x = r;
		//	_data.scale[i].y = r;
		//}

	}

	size_t size() {
		return _data.size;
	}

	//size_t byte_size() {
	//}

	vec3* pos_array() {
		return _data.pos;
	}

	vec3* rot_array() {
		return _data.rot;
	}

	vec3* scale_array() {
		return _data.scale;
	}
};