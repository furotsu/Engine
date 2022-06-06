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

	bool m_rollEnabled;
	bool m_updatedBasis = false;
	bool m_updatedMatrices = false;


	Camera() = default;
	Camera(XMVECTOR position, Angles rotation, bool rollEnabled = false)
		:m_rollEnabled(rollEnabled)
	{
		setWorldOffset(position);
		setWorldAngles(rotation);

		updateMatrices();
	}	

	void setPerspective(float fov, float width, float height, float nearP, float farP); // sets m_proj and m_projInv


	const XMVECTOR& right() const   { return m_viewInv.r[0]; }
	const XMVECTOR& top() const     { return m_viewInv.r[1]; }
	const XMVECTOR& forward() const { return m_viewInv.r[2]; }
	const XMVECTOR& position()const { return m_viewInv.r[3]; }

	void setWorldOffset(const XMVECTOR& offset);

	void addWorldOffset(const XMVECTOR& offset);

	void addRelativeOffset(const XMVECTOR& offset);

	void setWorldAngles(const Angles& angles);

	void addWorldAngles(const Angles& angles);

	void addRelativeAngles(const Angles& angles);

	void updateBasis();

	// This function must be called each frame to ensure the matrices are updated after camera movement
	void updateMatrices();
};
