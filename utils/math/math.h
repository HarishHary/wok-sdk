#pragma once

class math {
public:
	__forceinline static float fast_sqrt(float x) {
		unsigned int i = *(unsigned int*)&x;
		i += 127 << 23;
		i >>= 1;
		return *(float*)&i;
	}

	template <typename t>
	__forceinline static t lerp(const t& from, const t& to, float percent) {
		return from + (to - from) * percent;
	}

	static void normalize_angles(QAngle& angles);
	static QAngle normalize_angle(QAngle angles);
	static void vector_angles(const Vector& forward, QAngle& angles);
	static void vector_angles(const Vector& forward, Vector& up, QAngle& angles);
	static QAngle calc_angle(const Vector src, const Vector dst);
	static void vector_transform(const Vector in1, const matrix3x4_t in2, Vector& out);
	static float segment_to_segment(const Vector s1, const Vector s2, const Vector k1, const Vector k2);
	static bool intersect_line_with_bb(Vector& start, Vector& end, Vector& min, Vector& max);
	static void vector_i_transform(const Vector& in1, const matrix3x4_t& in2, Vector& out);
	static Vector vector_rotate(const Vector& in1, const matrix3x4_t& in2);
	static Vector matrix_position(matrix3x4_t& matrix);
	static matrix3x4_t multiply_matrix(matrix3x4_t in1, matrix3x4_t in2);
	static QAngle matrix_angles(matrix3x4_t& matrix);
	static matrix3x4_t angle_matrix(const QAngle angles);
	static Vector vector_rotate(const Vector& in1, const Vector& in2);
	static void vector_i_rotate(const Vector& in1, const matrix3x4_t& in2, Vector& out);
	static void angle_vectors(const QAngle& angles, Vector *forward, Vector *right = nullptr, Vector *up = nullptr);
	static float get_fov(const Vector& view, const QAngle& angle);
	static Vector make_vector(QAngle angle);
	static bool is_behind_smoke(Vector src, Vector point);
	static float angle_diff(float dest, float src);
	static bool can_hit_hitbox(const Vector start, const Vector end, matrix3x4_t* bones, studiohdr_t* hdr, int hitbox);
	static float angle_mod(float angle);
	static float approach_angle(float target, float value, float speed);
	static void random_seed(int seed);
	static float random_float(float min, float max);
	static Vector project_point(Vector origin, double yaw, double distance);
	static void angle_matrix(const QAngle &angles, const Vector &position, matrix3x4_t& matrix_out);
	static void matrix_multiply(matrix3x4_t& in1, const matrix3x4_t& in2);

	static inline void fast_sqrt(float * __restrict p_out, float * __restrict p_in);
	static float fast_asin(float x);
	static float fast_atan2(float y, float x);
	static float fast_atan(float x);
	static float fast_cos(float x);
	static float fast_acos(float x);
	static void fast_rsqrt(float a, float * out);
	static float fast_vec_normalize(Vector& vec);
	static float fast_sin(float x);
};