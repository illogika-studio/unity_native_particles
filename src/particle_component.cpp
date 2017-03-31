#include "particle_component.h"
#include "globals.h"
#include "memory_pool.h"

#include <cassert>
#include <cstdlib>
#include <utility>

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
				heap = Memory::allocate_mem_pool(total_byte_size());
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

				heap = std::move(p.heap);
				size = std::move(p.size);
				pos = std::move(p.pos);
				rot = std::move(p.rot);
				scale = std::move(p.scale);
				speed = std::move(p.speed);
				return *this;
			}

			inline size_t total_byte_size() {
				return size * 4 * soa_vec3::struct_byte_size();
			}

			void* heap = nullptr;
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
		OUTPUT_MSG("PARTICLE INIT");
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

	size_t total_byte_size() {
		return _data.total_byte_size();
	}

	void* heap() {
		return heap;
	}

	soa_vec3& pos() {
		return _data.pos;
	}

	soa_vec3& rot() {
		return _data.rot;
	}

	soa_vec3& scale() {
		return _data.scale;
	}

	soa_vec3& speed() {
		return _data.speed;
	}
};