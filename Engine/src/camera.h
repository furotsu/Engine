#pragma once

#include <iostream>

#include <DirectXMath.h>

#include "matrixAlg.h"
#include "model.h"
#include "quaternion.h"

using namespace DirectX;


class Camera
{
public:
	XMMATRIX m_view       = XMMatrixIdentity();
	XMMATRIX m_proj       = XMMatrixIdentity();
	XMMATRIX m_viewProj   = XMMatrixIdentity();
	 
	XMMATRIX m_viewInv	   = XMMatrixIdentity();
	XMMATRIX m_projInv	   = XMMatrixIdentity();
	XMMATRIX m_viewProjInv = XMMatrixIdentity();

	Angles m_angles;

	Transform transformation;

	bool m_updatedBasis = false;
	bool m_updatedMatrices = false;


	Camera() = default;
	Camera(XMVECTOR position, Angles rotation)
	{
		setWorldOffset(position);
		setWorldAngles(rotation);

		updateMatrices();
	}	

	void setPerspective(float fov, float width, float height, float nearP, float farP) // sets m_proj and m_projInv
	{
		float rad = XMConvertToRadians(fov);
		float const h = XMScalarCos((float)0.5 * rad) / XMScalarSin((float)0.5 * rad);
		float const w = h * width / height; 

		m_proj = XMMatrixSet(
			h, 0.0f, 0.0f, 0.0f,
			0.0f, w, 0.0f, 0.0f,
			0.0f, 0.0f, -(farP ) / (-nearP + farP), -1,
			0.0f, 0.0f, -(nearP * farP) / (farP - nearP), 0
		);

		m_projInv = XMMatrixInverse(nullptr, m_proj);

		std::cout << "projInv" << std::endl;

		m_updatedMatrices = false;
		updateMatrices();
	}

	const XMVECTOR& right() const   { return m_viewInv.r[0]; }
	const XMVECTOR& top() const     { return m_viewInv.r[1]; }
	const XMVECTOR& forward() const { return m_viewInv.r[2]; }
	const XMVECTOR& position()const { return m_viewInv.r[3]; }


	//const XMVECTOR& position()	const { return transformation.position; }

	void setWorldOffset(const XMVECTOR& offset)
	{
		m_updatedMatrices = false;

		// overwrites
		m_viewInv.r[3] = offset;

		transformation.position = offset;
	}

	void addWorldOffset(const XMVECTOR& offset)
	{
		m_updatedMatrices = false;

		m_viewInv.r[3] += offset;

		transformation.position += offset;
	}

	void addRelativeOffset(const XMVECTOR& offset)
	{
		updateBasis(); // requires m_rotation to be decoded into 3x3 matrix stored in m_viewInv
		m_updatedMatrices = false;

		transformation.position += offset;

		m_viewInv.r[3] += XMVectorGetX(offset) * right() + XMVectorGetY(offset) * top() + XMVectorGetZ(offset) * forward();

	}

	void setWorldAngles(const Angles& angles)
	{
		m_updatedBasis = false;
		m_updatedMatrices = false;

		transformation.rotation = math::Quaternion({ 0.0f, 0.0f, 1.0f, 0.0f }, angles.roll);
		transformation.rotation *= math::Quaternion({ 1.0f, 0.0f, 0.0f, 0.0f }, angles.pitch);
		transformation.rotation *= math::Quaternion({ 0.0f, 1.0f, 1.0f, 0.0f }, angles.yaw);

		m_angles = angles;

		math::normalizeQuat(transformation.rotation);

			// and keep the quaternion of length 1.0, so it represents a valid 3D rotation.
			// Also we can do it only from time to time, like every 100th call of rotating functions.
	}

	void addWorldAngles(const Angles& angles)
	{
		m_updatedBasis = false;
		m_updatedMatrices = false;

		transformation.rotation *= math::Quaternion({ 0.0f, 0.0f, 1.0f, 0.0f }, angles.roll);
		transformation.rotation *= math::Quaternion({ 1.0f, 0.0f, 0.0f, 0.0f }, angles.pitch);
		transformation.rotation *= math::Quaternion({ 0.0f, 1.0f, 1.0f, 0.0f }, angles.yaw);

		math::normalizeQuat(transformation.rotation);

			// and keep the quaternion of length 1.0, so it represents a valid 3D rotation
			// Also we can do it only from time to time, like every 100th call of rotating functions.
	}

	void addRelativeAngles(const Angles& angles)
	{
		m_updatedBasis = false;	
		m_updatedMatrices = false;

		transformation.rotation *= math::Quaternion(forward(), angles.roll);
		transformation.rotation *= math::Quaternion(right(), angles.pitch);
		transformation.rotation *= math::Quaternion(top(), angles.yaw);

		math::normalizeQuat(transformation.rotation);


			// optional, to remove accumulating floating point errors
			// and keep the quaternion of length 1.0, so it represents a valid 3D rotation
			// Also we can do it only from time to time, like every 100th call of rotating functions.
	}

	void updateBasis()
	{
		if (m_updatedBasis) return;
		m_updatedBasis = true;

		transformation.rotation.toMat3(m_viewInv);
	}

	// This function must be called each frame to ensure the matrices are updated after camera movement
	void updateMatrices()
	{
		if (m_updatedMatrices) return;
		m_updatedMatrices = true;

		updateBasis();

		m_view = XMMatrixInverse(nullptr, m_viewInv);


		m_viewProj = m_view * m_proj;
		m_viewProjInv = m_projInv * m_viewInv;
	}
};
