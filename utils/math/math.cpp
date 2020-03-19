#include "../../sdk/sdk.h"
#include <cmath>
#include <xmmintrin.h>
#include <pmmintrin.h>

static const float invtwopi = 0.1591549f;
static const float twopi = 6.283185f;
static const float threehalfpi = 4.7123889f;
static const float pi = 3.141593f;
static const float halfpi = 1.570796f;
static const __m128 signmask = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));

static const __declspec(align(16)) float null[4] = { 0.f, 0.f, 0.f, 0.f };
static const __declspec(align(16)) float _pi2[4] = { 1.5707963267948966192f, 1.5707963267948966192f, 1.5707963267948966192f, 1.5707963267948966192f };
static const __declspec(align(16)) float _pi[4] = { 3.141592653589793238f, 3.141592653589793238f, 3.141592653589793238f, 3.141592653589793238f };

typedef __declspec(align(16)) union {
	float f[4];
	__m128 v;
} m128;

__forceinline __m128 sqrt_ps(const __m128 squared) {
	return _mm_sqrt_ps(squared);
}

__forceinline __m128 cos_52s_ps(const __m128 x) {
	const auto c1 = _mm_set1_ps(0.9999932946f);
	const auto c2 = _mm_set1_ps(-0.4999124376f);
	const auto c3 = _mm_set1_ps(0.0414877472f);
	const auto c4 = _mm_set1_ps(-0.0012712095f);
	const auto x2 = _mm_mul_ps(x, x);
	return _mm_add_ps(c1, _mm_mul_ps(x2, _mm_add_ps(c2, _mm_mul_ps(x2, _mm_add_ps(c3, _mm_mul_ps(c4, x2))))));
}

__forceinline __m128 cos_ps(__m128 angle) {
	angle = _mm_andnot_ps(signmask, angle);
	angle = _mm_sub_ps(angle, _mm_mul_ps(_mm_cvtepi32_ps(_mm_cvttps_epi32(_mm_mul_ps(angle, _mm_set1_ps(invtwopi)))), _mm_set1_ps(twopi)));

	auto cosangle = angle;
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(halfpi)), _mm_xor_ps(cosangle, _mm_sub_ps(_mm_set1_ps(pi), angle))));
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(pi)), signmask));
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(threehalfpi)), _mm_xor_ps(cosangle, _mm_sub_ps(_mm_set1_ps(twopi), angle))));

	auto result = cos_52s_ps(cosangle);
	result = _mm_xor_ps(result, _mm_and_ps(_mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(halfpi)), _mm_cmplt_ps(angle, _mm_set1_ps(threehalfpi))), signmask));
	return result;
}

__forceinline __m128 sin_ps(const __m128 angle) {
	return cos_ps(_mm_sub_ps(_mm_set1_ps(halfpi), angle));
}

__forceinline void sincos_ps(__m128 angle, __m128 *sin, __m128 *cos) {
	const auto anglesign = _mm_or_ps(_mm_set1_ps(1.f), _mm_and_ps(signmask, angle));
	angle = _mm_andnot_ps(signmask, angle);
	angle = _mm_sub_ps(angle, _mm_mul_ps(_mm_cvtepi32_ps(_mm_cvttps_epi32(_mm_mul_ps(angle, _mm_set1_ps(invtwopi)))), _mm_set1_ps(twopi)));

	auto cosangle = angle;
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(halfpi)), _mm_xor_ps(cosangle, _mm_sub_ps(_mm_set1_ps(pi), angle))));
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(pi)), signmask));
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(threehalfpi)), _mm_xor_ps(cosangle, _mm_sub_ps(_mm_set1_ps(twopi), angle))));

	auto result = cos_52s_ps(cosangle);
	result = _mm_xor_ps(result, _mm_and_ps(_mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(halfpi)), _mm_cmplt_ps(angle, _mm_set1_ps(threehalfpi))), signmask));
	*cos = result;

	const auto sinmultiplier = _mm_mul_ps(anglesign, _mm_or_ps(_mm_set1_ps(1.f), _mm_and_ps(_mm_cmpgt_ps(angle, _mm_set1_ps(pi)), signmask)));
	*sin = _mm_mul_ps(sinmultiplier, sqrt_ps(_mm_sub_ps(_mm_set1_ps(1.f), _mm_mul_ps(result, result))));
}

void sin_cos(float radian, float* sin, float* cos) {
	*sin = std::sin(radian);
	*cos = std::cos(radian);
}

void matrix_set_column(const Vector &in, int column, matrix3x4_t& out) {
	out[0][column] = in.x;
	out[1][column] = in.y;
	out[2][column] = in.z;
}

void matrix_copy(const matrix3x4_t& src, matrix3x4_t& dest) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			dest[i][j] = src[i][j];
		}
	}
}

void math::normalize_angles(QAngle& angles) {
	angles.x = std::clamp(remainderf(angles.x, 360.f), -89.f, 89.f);
	angles.y = std::clamp(remainderf(angles.y, 360.f), -180.f, 180.f);
	angles.z = std::clamp(remainderf(angles.z, 360.f), -45.f, 45.f);
}

QAngle math::normalize_angle(QAngle angles) {
	angles.x = std::clamp(remainderf(angles.x, 360.f), -89.f, 89.f);
	angles.y = std::clamp(remainderf(angles.y, 360.f), -180.f, 180.f);
	angles.z = std::clamp(remainderf(angles.z, 360.f), -45.f, 45.f);
	return angles;
}

void math::vector_angles(const Vector& forward, QAngle& angles) {
	float tmp, yaw, pitch;
	if (forward[2] == 0.f && forward[0] == 0.f) {
		yaw = 0;

		if (forward[2] > 0.f)
			pitch = 90.f;
		else
			pitch = 270.f;
	}
	else {
		yaw = (fast_atan2(forward[1], forward[0]) * 180.f / M_PI);

		if (yaw < 0.f)
			yaw += 360.f;

		auto sqin = forward[0] * forward[0] + forward[1] * forward[1];
		sse_fast_sqrt(&tmp, &sqin);

		pitch = (fast_atan2(-forward[2], tmp) * 180.f / M_PI);

		if (pitch < 0.f)
			pitch += 360.f;
	}

	pitch -= floorf(pitch / 360.f + 0.5f) * 360.f;
	yaw -= floorf(yaw / 360.f + 0.5f) * 360.f;

	if (pitch > 89.f)
		pitch = 89.f;
	else if (pitch < -89.f)
		pitch = -89.f;

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

void math::vector_angles(const Vector& forward, Vector& up, QAngle& angles) {
	Vector left = CrossProduct(up, forward);
	left.NormalizeInPlace();

	auto forward_dist = forward.Length2D();
	if (forward_dist > 0.001f) {
		angles.x = fast_atan2(-forward.z, forward_dist) * 180.f / M_PI;
		angles.y = fast_atan2(forward.y, forward.x) * 180.f / M_PI;

		auto up_z = (left.y * forward.x) - (left.x * forward.y);
		angles.z = fast_atan2(left.z, up_z) * 180.f / M_PI;
	}
	else {
		angles.x = fast_atan2(-forward.z, forward_dist) * 180.f / M_PI;
		angles.y = fast_atan2(-left.x, left.y) * 180.f / M_PI;
		angles.z = 0;
	}
}

QAngle math::calc_angle(const Vector src, const Vector dst) {
	auto delta = src - dst;
	if (delta == Vector(0, 0, 0))
		return QAngle(0, 0, 0);

	auto len = delta.Length();

	if (delta.z == 0.f
		&& len == 0.f)
		return QAngle(0, 0, 0);

	if (delta.y == 0.f 
		&& delta.x == 0.f)
		return QAngle(0, 0, 0);

	QAngle angles;
	angles.x = (fast_asin(delta.z / delta.Length()) * M_RADPI);
	angles.y = (fast_atan(delta.y / delta.x) * M_RADPI);
	angles.z = 0.f;

	if (delta.x >= 0.f) 
		angles.y += 180.f; 

	return angles.Clamp();
}

void math::vector_transform(const Vector in1, const matrix3x4_t in2, Vector& out) {
	out.x = in1.Dot(in2.m_flMatVal[0]) + in2.m_flMatVal[0][3];
	out.y = in1.Dot(in2.m_flMatVal[1]) + in2.m_flMatVal[1][3];
	out.z = in1.Dot(in2.m_flMatVal[2]) + in2.m_flMatVal[2][3];
}

float math::segment_to_segment(const Vector s1, const Vector s2, const Vector k1, const Vector k2) {
	static auto constexpr epsilon = 0.00000001;

	auto u = s2 - s1;
	auto v = k2 - k1;
	const auto w = s1 - k1;

	const auto a = u.Dot(u);
	const auto b = u.Dot(v);
	const auto c = v.Dot(v);
	const auto d = u.Dot(w);
	const auto e = v.Dot(w);
	const auto D = a * c - b * b;
	float sn, sd = D;
	float tn, td = D;

	if (D < epsilon) {
		sn = 0.0;
		sd = 1.0;
		tn = e;
		td = c;
	}
	else {
		sn = b * e - c * d;
		tn = a * e - b * d;

		if (sn < 0.0) {
			sn = 0.0;
			tn = e;
			td = c;
		}
		else if (sn > sd) {
			sn = sd;
			tn = e + b;
			td = c;
		}
	}

	if (tn < 0.0) {
		tn = 0.0;

		if (-d < 0.0)
			sn = 0.0;
		else if (-d > a)
			sn = sd;
		else {
			sn = -d;
			sd = a;
		}
	}
	else if (tn > td) {
		tn = td;

		if (-d + b < 0.0)
			sn = 0;
		else if (-d + b > a)
			sn = sd;
		else {
			sn = -d + b;
			sd = a;
		}
	}

	const float sc = abs(sn) < epsilon ? 0.0 : sn / sd;
	const float tc = abs(tn) < epsilon ? 0.0 : tn / td;

	m128 n;
	auto dp = w + u * sc - v * tc;
	n.f[0] = dp.Dot(dp);
	const auto calc = sqrt_ps(n.v);
	return reinterpret_cast<const m128*>(&calc)->f[0];
}

bool math::intersect_line_with_bb(Vector& start, Vector& end, Vector& min, Vector& max) {
	float d1, d2, f;
	auto start_solid = true;
	auto t1 = -1.f, t2 = 1.f;

	const float s[3] = { start.x, start.y, start.z };
	const float e[3] = { end.x, end.y, end.z };
	const float mi[3] = { min.x, min.y, min.z };
	const float ma[3] = { max.x, max.y, max.z };

	for (auto i = 0; i < 6; i++) {
		if (i >= 3) {
			const auto j = i - 3;

			d1 = s[j] - ma[j];
			d2 = d1 + e[j];
		}
		else {
			d1 = -s[i] + mi[i];
			d2 = d1 - e[i];
		}

		if (d1 > 0.f 
			&& d2 > 0.f)
			return false;

		if (d1 <= 0.f 
			&& d2 <= 0.f)
			continue;

		if (d1 > 0)
			start_solid = false;

		if (d1 > d2) {
			f = d1;
			if (f < 0.f)
				f = 0.f;

			f /= d1 - d2;
			if (f > t1)
				t1 = f;
		}
		else {
			f = d1 / (d1 - d2);
			if (f < t2)
				t2 = f;
		}
	}

	return start_solid || (t1 < t2 && t1 >= 0.f);
}

void math::vector_i_transform(const Vector& in1, const matrix3x4_t& in2, Vector& out) {
	out.x = (in1.x - in2[0][3]) * in2[0][0] + (in1.y - in2[1][3]) * in2[1][0] + (in1.z - in2[2][3]) * in2[2][0];
	out.y = (in1.x - in2[0][3]) * in2[0][1] + (in1.y - in2[1][3]) * in2[1][1] + (in1.z - in2[2][3]) * in2[2][1];
	out.z = (in1.x - in2[0][3]) * in2[0][2] + (in1.y - in2[1][3]) * in2[1][2] + (in1.z - in2[2][3]) * in2[2][2];
}

Vector math::vector_rotate(const Vector& in1, const matrix3x4_t& in2) {
	return Vector(in1.Dot(in2[0]), in1.Dot(in2[1]), in1.Dot(in2[2]));
}

Vector math::matrix_position(matrix3x4_t& matrix) {
	return Vector(matrix[0][3], matrix[1][3], matrix[2][3]);
}

matrix3x4_t math::multiply_matrix(matrix3x4_t in1, matrix3x4_t in2) {
	matrix3x4_t result{};
	result[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
		in1[0][2] * in2[2][0];
	result[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
		in1[0][2] * in2[2][1];
	result[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
		in1[0][2] * in2[2][2];
	result[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] +
		in1[0][2] * in2[2][3] + in1[0][3];
	result[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
		in1[1][2] * in2[2][0];
	result[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
		in1[1][2] * in2[2][1];
	result[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
		in1[1][2] * in2[2][2];
	result[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] +
		in1[1][2] * in2[2][3] + in1[1][3];
	result[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
		in1[2][2] * in2[2][0];
	result[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
		in1[2][2] * in2[2][1];
	result[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
		in1[2][2] * in2[2][2];
	result[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] +
		in1[2][2] * in2[2][3] + in1[2][3];
	return result;
}

QAngle math::matrix_angles(matrix3x4_t& matrix) {
	float forward[3];
	float left[3];
	float up[3];

	forward[0] = matrix[0][0];
	forward[1] = matrix[1][0];
	forward[2] = matrix[2][0];

	left[0] = matrix[0][1];
	left[1] = matrix[1][1];
	left[2] = matrix[2][1];

	up[2] = matrix[2][2];

	m128 a1;
	a1.f[0] = forward[0] * forward[0] + forward[1] * forward[1];
	auto calc = sqrt_ps(a1.v);
	const auto xy_dist = reinterpret_cast<const m128*>(&calc)->f[0];

	if (xy_dist > 0.001f)
		return Vector(RAD2DEG(atan2f(-forward[2], xy_dist)),
			RAD2DEG(atan2f(forward[1], forward[0])),
			RAD2DEG(atan2f(left[2], up[2])));

	return Vector(RAD2DEG(atan2f(-forward[2], xy_dist)),
		RAD2DEG(atan2f(-left[0], left[1])),
		0);
}

matrix3x4_t math::angle_matrix(const QAngle angles) {
	matrix3x4_t result;

	m128 angle, sin, cos;
	angle.f[0] = DEG2RAD(angles.x);
	angle.f[1] = DEG2RAD(angles.y);
	angle.f[2] = DEG2RAD(angles.z);
	sincos_ps(angle.v, &sin.v, &cos.v);

	result[0][0] = cos.f[0] * cos.f[1];
	result[1][0] = cos.f[0] * sin.f[1];
	result[2][0] = -sin.f[0];

	const auto crcy = cos.f[2] * cos.f[1];
	const auto crsy = cos.f[2] * sin.f[1];
	const auto srcy = sin.f[2] * cos.f[1];
	const auto srsy = sin.f[2] * sin.f[1];

	result[0][1] = sin.f[0] * srcy - crsy;
	result[1][1] = sin.f[0] * srsy + crcy;
	result[2][1] = sin.f[2] * cos.f[0];

	result[0][2] = sin.f[0] * crcy + srsy;
	result[1][2] = sin.f[0] * crsy - srcy;
	result[2][2] = cos.f[2] * cos.f[0];

	return result;
}

Vector math::vector_rotate(const Vector& in1, const Vector& in2) {
	const auto matrix = angle_matrix(in2);
	return vector_rotate(in1, matrix);
}

void math::vector_i_rotate(const Vector& in1, const matrix3x4_t& in2, Vector& out) {
	out.x = in1.x * in2[0][0] + in1.y * in2[1][0] + in1.z * in2[2][0];
	out.y = in1.x * in2[0][1] + in1.y * in2[1][1] + in1.z * in2[2][1];
	out.z = in1.x * in2[0][2] + in1.y * in2[1][2] + in1.z * in2[2][2];
}

void math::angle_vectors(const QAngle& angles, Vector *forward, Vector *right, Vector *up) {
	float sr, sp, sy, cr, cp, cy;
	sin_cos(DEG2RAD(angles.x), &sp, &cp);
	sin_cos(DEG2RAD(angles.y), &sy, &cy);
	sin_cos(DEG2RAD(angles.z), &sr, &cr);

	if (forward) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right) {
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up) {
		up->x = (cr*sp*cy + -sr * -sy);
		up->y = (cr*sp*sy + -sr * cy);
		up->z = cr * cp;
	}
}

float math::get_fov(const QAngle& view, const QAngle& angle) {
	Vector ang, aim;
	angle_vectors(view, &aim);
	angle_vectors(angle, &ang);
	return RAD2DEG(fast_acos(aim.Dot(ang) / aim.LengthSqr()));
}

Vector math::make_vector(QAngle angle) {
	Vector ret;
	auto pitch = angle[0] * 3.141f / 180.f;
	auto yaw = angle[1] * 3.141f / 180.f;
	auto tmp = fast_cos(pitch);
	ret[0] = -tmp * -fast_cos(yaw);
	ret[1] = fast_sin(yaw)*tmp;
	ret[2] = -fast_sin(pitch);
	return ret;
}

bool math::is_behind_smoke(Vector src, Vector point) {
	static const auto line_goes_through_smoke_fn = reinterpret_cast<bool(__cdecl*)(Vector, Vector)>(SIG("client_panorama.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0"));
	if (!line_goes_through_smoke_fn)
		return false;

	return line_goes_through_smoke_fn(src, point);
}

float math::angle_diff(float dest, float src) {
	float delta;
	delta = fmodf(dest - src, 360.f);

	(dest > src)
		? (delta >= 180.f) ? delta -= 360.f
		: (delta <= -180.f) ? delta += 360.f
		: 0 : 0;

	return delta;
}

bool math::can_hit_hitbox(const Vector start, const Vector end, matrix3x4_t* bones, studiohdr_t* hdr, int hitbox) {
	auto studio_box = hdr->pHitbox(hitbox, 0);
	if (!studio_box)
		return false;

	Vector min, max;
	auto is_capsule = (studio_box->radius != -1.f);
	if (is_capsule) {
		vector_transform(studio_box->min, bones[studio_box->bone], min);
		vector_transform(studio_box->max, bones[studio_box->bone], max);
		auto dist = segment_to_segment(start, end, min, max);

		if (dist < studio_box->radius)
			return true;
	}
	else {
		vector_transform(vector_rotate(studio_box->min, studio_box->rotation), bones[studio_box->bone], min);
		vector_transform(vector_rotate(studio_box->max, studio_box->rotation), bones[studio_box->bone], max);

		vector_i_transform(start, bones[studio_box->bone], min);
		vector_i_transform(end, bones[studio_box->bone], max);

		if (intersect_line_with_bb(min, max, studio_box->min, studio_box->max))
			return true;
	}

	return false;
}

float math::angle_mod(float angle) {
	return((360.f / 65536.f) * ((int32_t)(angle * (65536.f / 360.f)) & 65535));
}

float math::approach_angle(float target, float value, float speed) {
	target = angle_mod(target);
	value = angle_mod(value);

	auto delta = target - value;
	speed < 0 ? speed = -speed : 0;

	if (delta < -180.f)
		delta += 360.f;
	else if (delta > 180.f)
		delta -= 360.f;

	if (delta > speed)
		value += speed;
	else if (delta < -speed)
		value -= speed;
	else
		value = target;

	return value;
}

void math::random_seed(int seed) {
	static const auto random_seed_fn = reinterpret_cast<void(*)(int)>(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomSeed"));
	random_seed_fn(seed);
}

float math::random_float(float min, float max) {
	static const auto random_float_fn = reinterpret_cast<float(*)(float, float)>(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomFloat"));
	return random_float_fn(min, max);
}

Vector math::project_point(Vector origin, double yaw, double distance) {
	return (origin + Vector((distance * fast_sin(DEG2RAD(yaw))), -(distance * fast_cos(DEG2RAD(yaw))), 0));
}

void math::angle_matrix(const QAngle &angles, const Vector &position, matrix3x4_t& matrix_out) {
	matrix_out = angle_matrix(angles);
	matrix_set_column(position, 3, matrix_out);
}

void math::matrix_multiply(matrix3x4_t& in1, const matrix3x4_t& in2) {
	matrix3x4_t out;
	if (&in1 == &out) {
		matrix3x4_t in1b;
		matrix_copy(in1, in1b);
		matrix_multiply(in1b, in2);
		return;
	}
	if (&in2 == &out) {
		matrix3x4_t in2b;
		matrix_copy(in2, in2b);
		matrix_multiply(in1, in2b);
		return;
	}

	out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
		in1[0][2] * in2[2][0];
	out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
		in1[0][2] * in2[2][1];
	out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
		in1[0][2] * in2[2][2];
	out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] +
		in1[0][2] * in2[2][3] + in1[0][3];
	out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
		in1[1][2] * in2[2][0];
	out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
		in1[1][2] * in2[2][1];
	out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
		in1[1][2] * in2[2][2];
	out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] +
		in1[1][2] * in2[2][3] + in1[1][3];
	out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
		in1[2][2] * in2[2][0];
	out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
		in1[2][2] * in2[2][1];
	out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
		in1[2][2] * in2[2][2];
	out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] +
		in1[2][2] * in2[2][3] + in1[2][3];

	in1 = out;
}

void math::sse_fast_sqrt(float * __restrict p_out, float * __restrict p_in) {
	_mm_store_ss(p_out, _mm_sqrt_ss(_mm_load_ss(p_in)));
}

float math::fast_asin(float x) {
	const auto negate = float(x < 0);
	x = abs(x);
	auto ret = -0.0187293;
	ret *= x;
	ret += 0.0742610;
	ret *= x;
	ret -= 0.2121144;
	ret *= x;
	ret += 1.5707288;
	ret = 3.14159265358979 * 0.5 - sqrt(1.0 - x)*ret;
	return float(ret - 2 * negate * ret);
}

float math::fast_atan2(const float y, const float x) {
	const auto xabs = fabs(x);
	const auto yabs = fabs(y);
	double t3 = xabs;
	double t1 = yabs;
	auto t0 = max(t3, t1);
	t1 = min(t3, t1);
	t3 = 1 / t0;
	t3 = t1 * t3;

	const auto t4 = t3 * t3;
	t0 = -0.013480470;
	t0 = t0 * t4 + 0.057477314;
	t0 = t0 * t4 - 0.121239071;
	t0 = t0 * t4 + 0.195635925;
	t0 = t0 * t4 - 0.332994597;
	t0 = t0 * t4 + 0.999995630;
	t3 = t0 * t3;

	t3 = (yabs > xabs) ? 1.570796327 - t3 : t3;
	t3 = (x < 0) ? 3.141592654 - t3 : t3;
	t3 = (y < 0) ? -t3 : t3;

	return float(t3);
}

float math::fast_atan(const float x) {
	return fast_atan2(x, float(1));
}

float math::fast_sin(float x) {
	x *= float(0.159155);
	x -= floor(x);
	const auto xx = x * x;
	auto y = -6.87897;
	y = y * xx + 33.7755;
	y = y * xx - 72.5257;
	y = y * xx + 80.5874;
	y = y * xx - 41.2408;
	y = y * xx + 6.28077;
	return float(x * y);
}

float math::fast_cos(const float x) {
	return fast_sin(x + 1.5708f);
}

float math::fast_acos(float x) {
	const auto negate = float(x < 0);
	x = abs(x);
	auto ret = float(-0.0187293);
	ret = ret * x;
	ret = ret + 0.0742610;
	ret = ret * x;
	ret = ret - 0.2121144;
	ret = ret * x;
	ret = ret + 1.5707288;
	ret = ret * sqrt(1.0 - x);
	ret = ret - 2 * negate * ret;
	return negate * 3.14159265358979 + ret;
}

void math::fast_rsqrt(float a, float* out) {
	const auto xx = _mm_load_ss(&a);
	auto xr = _mm_rsqrt_ss(xx);
	auto xt = _mm_mul_ss(xr, xr);
	xt = _mm_mul_ss(xt, xx);
	xt = _mm_sub_ss(_mm_set_ss(3.f), xt);
	xt = _mm_mul_ss(xt, _mm_set_ss(0.5f));
	xr = _mm_mul_ss(xr, xt);
	_mm_store_ss(out, xr);
}

float math::fast_vec_normalize(Vector& vec) {
	const auto sqrlen = vec.LengthSqr() + 1.0e-10f;
	float invlen;
	fast_rsqrt(sqrlen, &invlen);
	vec.x *= invlen;
	vec.y *= invlen;
	vec.z *= invlen;
	return sqrlen * invlen;
}