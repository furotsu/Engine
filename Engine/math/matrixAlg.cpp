#include "matrixAlg.h"

/*
XMVECTOR math::rowAsVec3(const glm::mat4& m, int i)
{
	XMVECTOR res;
	res[0] = m[0][i];
	res[1] = m[1][i];
	res[2] = m[2][i];

	return res;
}

void math::setRowAsVec3(glm::mat4& dst, const XMVECTOR& v, int row)
{
	dst[0][row] = v[0];
	dst[1][row] = v[1];
	dst[2][row] = v[2];
}

void math::invertOrthonormal(glm::mat4& src, glm::mat4& dst)
{
	glm::mat4 tmp = src;

	src = glm::transpose(src);

	src[0][3] = tmp[0][3];
	src[1][3] = tmp[1][3];
	src[2][3] = tmp[2][3];
	src[3] = tmp[3];

	const XMVECTOR& pos = XMVECTOR(src[0][3], src[1][3], src[2][3]);

	XMVECTOR tmpV = -pos[0] * rowAsVec3(dst, 0) - pos[1] * rowAsVec3(dst, 1) - pos[2] * rowAsVec3(dst, 2);
	
	dst[0][3] = tmpV[0];
	dst[1][3] = tmpV[1];
	dst[2][3] = tmpV[2];

	dst[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void math::invertOrthogonal(glm::mat4& src, glm::mat4& dst)
{
	glm::mat4 tmp = src;

	src = glm::transpose(src);

	src[0][3] = tmp[0][3];
	src[1][3] = tmp[1][3];
	src[2][3] = tmp[2][3];
	src[3] = tmp[3];

	float lengthsXYZ[3] = { rowAsVec3(dst, 0).length(), rowAsVec3(dst, 1).length(), rowAsVec3(dst, 2).length() };


	setRowAsVec3(dst, XMVECTOR(1.0f, 1.0f, 1.0f) / (rowAsVec3(dst, 0) * lengthsXYZ[0]), 0);
	setRowAsVec3(dst, XMVECTOR(1.0f, 1.0f, 1.0f) / (rowAsVec3(dst, 1) * lengthsXYZ[1]), 1);
	setRowAsVec3(dst, XMVECTOR(1.0f, 1.0f, 1.0f) / (rowAsVec3(dst, 2) * lengthsXYZ[2]), 2);

	const XMVECTOR& pos = XMVECTOR(src[0][3], src[1][3], src[2][3]);

	setRowAsVec3(dst, -pos[0] * rowAsVec3(dst, 0) / lengthsXYZ[0]
		- pos[1] * rowAsVec3(dst, 1) / lengthsXYZ[1]
		- pos[2] * rowAsVec3(dst, 2) / lengthsXYZ[2], 3);

	dst[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
*/