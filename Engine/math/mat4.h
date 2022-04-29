#ifndef MAT4
#define MAT4

#include "vec3.h"
#include "vec4.h"

// Matrix is row time like in DirectX
/* Visual reference:
	v11 v12 v13 v14
	v21 v22 v23 v24
	v31 v32 v33 v34
	v41 v42 v42 v44
*/

class mat4
{
public:
	vec4 s[4];

	mat4() 
	{
		s[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);
		s[1] = vec4(0.0f, 1.0f, 0.0f, 0.0f);
		s[2] = vec4(0.0f, 0.0f, 1.0f, 0.0f);
		s[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	mat4(const vec4& v0, const vec4& v1, const vec4& v2, const vec4& v3)
	{
		s[0] = v0;
		s[1] = v1;
		s[2] = v2;
		s[3] = v3;
	}
	mat4(const mat4& m2)
	{
		s[0] = m2[0];
		s[1] = m2[1];
		s[2] = m2[2];
		s[3] = m2[3];
	}

	inline vec4 operator[](int i) const { return s[i]; }
	inline vec4& operator[](int i) { return s[i]; }

	inline vec4 operator*=(const vec4& v);
	//inline vec3& operator[](int i) { return s[i]; }

};

inline mat4 translate(const mat4& m, const vec3& v)
{
	mat4 res(m);
	res[0] = m[0] * v[0];
	res[1] = m[1] * v[1];
	res[2] = m[2] * v[2];
	res[3] = m[3];
	return res;
}

inline vec4 mat4::operator*=(const vec4& v)
{
	return vec4(s[0].x() * v.x() + s[0].y() * v.y() + s[0].z() * v.z() + s[0].w() * v.w(),
				s[1].x() * v.x() + s[1].y() * v.y() + s[1].z() * v.z() + s[1].w() * v.w(),
				s[2].x() * v.x() + s[2].y() * v.y() + s[2].z() * v.z() + s[2].w() * v.w(),
				s[3].x() * v.x() + s[3].y() * v.y() + s[3].z() * v.z() + s[3].w() * v.w());
}


#endif // !MAT4
