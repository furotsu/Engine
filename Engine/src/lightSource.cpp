#define _USE_MATH_DEFINES

#include "scene.h"
#include <cmath>

// Schlick's approximation of Fresnel reflectance
XMVECTOR frensel(float NdotL, const XMVECTOR& F0)
{
	return F0 + (XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f) - F0) * powf(1 - NdotL, 5.0);
}


// Height-correlated Smith G2 for GGX,
// Filament, 4.4.2 Geometric shadowing
float smith(float rough2, float NoV, float NoL)
{
	NoV *= NoV;
	NoL *= NoL;
	return 2.0f / (sqrtf(1.0f + rough2 * (1 - NoV) / NoV) + sqrtf(1.0f + rough2 * (1 - NoL) / NoL));
}

// GGX normal distribution,
// Real-Time Rendering 4th Edition, page 340, equation 9.41
float ggx(float rough2, float NoH, float LightAngleSin, float lightAngleCos)
{
	float denom = NoH * NoH * (rough2 - 1.0f) + 1.0f;
	denom = M_PI * denom * denom;
	return rough2 / denom;
}

XMVECTOR Scene::PointLight::illuminate(const XMVECTOR& fragPos, const XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& material) const
{
	XMVECTOR viewDirection = XMVector3Normalize(cameraPos - fragPos);
	XMVECTOR lightDirection = XMVector3Normalize(this->center - fragPos);

	XMVECTOR halfDir = XMVector3Normalize(lightDirection + viewDirection);

	float NoH = max(XMVectorGetX(XMVector3Dot(fragNorm, halfDir)), 0.0f);
	float NoV = max(XMVectorGetX(XMVector3Dot(fragNorm, viewDirection)) , 0.0f);
	float NoL = max(XMVectorGetX(XMVector3Dot(fragNorm, lightDirection)), 0.0f);
	float LoH = max(XMVectorGetX(XMVector3Dot(lightDirection, halfDir)), 0.0f);

	float rough2 = material->roughness * material->roughness;
	//spec
	float D = min(ggx(rough2, NoH), 1.0f);
	XMVECTOR Fspek = math::clamp3(frensel(LoH, material->F0), 0.0f, 1.0f);
	float G = min(smith(rough2, NoV, NoL), 1.0f);

	XMVECTOR metalAlb = math::lerp(XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), material->albedoMetallic, material->metalness);

	XMVECTOR spec =  XMVectorScale(Fspek, G*D*0.25f / (NoV + 0.001f));

	//diffuse
	XMVECTOR Fdiff = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f) - math::clamp3(frensel(NoL, material->F0), 0.0f, 1.0f);

	float distance = XMVectorGetX(XMVector3Length(this->center - fragPos));
	float solidAngle = 2.0f * std::asin(this->radius / distance);
	float attenuation = solidAngle / (2.0f * M_PI);	

	XMVECTOR diff = Fdiff* material->albedo * m_lightColor * attenuation;

	return  math::maxVec3( diff + spec, XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)) * m_lightPower;
}


XMVECTOR Scene::DirectionalLight::illuminate(const XMVECTOR& fragPos, const XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& material) const
{
	XMVECTOR viewDirection = XMVector3Normalize(fragPos - cameraPos);
	XMVECTOR lightDirection = XMVector3Normalize(-m_direction);

	XMVECTOR halfDir = XMVector3Normalize(lightDirection + viewDirection);

	float NoH = max(XMVectorGetX(XMVector3Dot(fragNorm, halfDir)), 0.0f);
	float NoV = max(XMVectorGetX(XMVector3Dot(fragNorm, viewDirection)), 0.0f);
	float NoL = max(XMVectorGetX(XMVector3Dot(fragNorm, lightDirection)), 0.0f);
	float LoH = max(XMVectorGetX(XMVector3Dot(lightDirection, halfDir)), 0.0f);

	float rough2 = material->roughness * material->roughness;

	//spec
	float D = min(ggx(rough2, NoH), 1.0f);
	XMVECTOR Fspec = math::clamp3(frensel(LoH, material->F0), 0.0f, 1.0f);
	float G = min(smith(rough2, NoV, NoL), 1.0f);

	XMVECTOR metalAlb = math::lerp(XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), material->albedoMetallic, material->metalness);

	XMVECTOR spec = metalAlb * XMVectorScale(Fspec, G * D * 0.25f / (NoV + 0.001f));

	//diffuse
	XMVECTOR Fdiff = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f) - math::clamp3(frensel(NoL, material->F0), 0.0f, 1.0f);
	XMVECTOR diff = Fdiff * material->albedo * m_lightColor;

	return  math::maxVec3(diff + spec, XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)) * m_lightPower;
}

XMVECTOR Scene::SpotLight::illuminate(const XMVECTOR& fragPos, const XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& material) const
{
	XMVECTOR viewDirection = XMVector3Normalize(fragPos - cameraPos);
	XMVECTOR lightDirection = XMVector3Normalize(this->center - fragPos);

	XMVECTOR halfDir = XMVector3Normalize(lightDirection + viewDirection);

	float NoH = max(XMVectorGetX(XMVector3Dot(fragNorm, halfDir)), 0.0f);
	float NoV = max(XMVectorGetX(XMVector3Dot(fragNorm, viewDirection)), 0.0f);
	float NoL = max(XMVectorGetX(XMVector3Dot(fragNorm, lightDirection)), 0.0f);
	float LoH = max(XMVectorGetX(XMVector3Dot(lightDirection, halfDir)), 0.0f);

	float rough2 = material->roughness * material->roughness;
	//spec
	float D = min(ggx(rough2, NoH), 1.0f);
	XMVECTOR Fspec = math::clamp3(frensel(LoH, material->F0), 0.0f, 1.0f);
	float G = min(smith(rough2, NoV, NoL), 1.0f);

	XMVECTOR metalAlb = math::lerp(XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), material->albedoMetallic, material->metalness);

	XMVECTOR spec = metalAlb * XMVectorScale(Fspec, G * D * 0.25f / (NoV + 0.001f));

	//diffuse
	XMVECTOR Fdiff = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f) - math::clamp3(frensel(NoL, material->F0), 0.0f, 1.0f);

	float distance = XMVectorGetX(XMVector3Length(this->center - fragPos));
	float solidAngle = 2.0f * std::asin(this->radius / distance);
	float attenuation = solidAngle / (2.0f * M_PI);

	XMVECTOR diff = Fdiff * material->albedo * m_lightColor * attenuation;

	// spotlight (soft edges)
	float theta = XMVectorGetX(XMVector3Dot(lightDirection, -XMVector3Normalize(m_direction)));
	float intensity = smoothstep(m_outerCutOff, m_innerCutOff , theta);

	return  math::maxVec3(diff + spec, XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)) * m_lightPower * intensity;
}

Scene::PointLight::PointLight(XMVECTOR position, XMVECTOR color, float power, float radius)
{
	m_lightColor = color;
	m_lightPower = power;

	this->center = position;
	this->radius = radius;

	this->material = Material(color, XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f),  1.0f, 1.0f, XMVectorScale(XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), power));
	this->type = IntersectedType::PointLight;
}

Scene::DirectionalLight::DirectionalLight(XMVECTOR direction, XMVECTOR color, float power)
	: m_direction(direction), m_lightColor(color), m_lightPower(power)
{

}

Scene::SpotLight::SpotLight(XMVECTOR position, XMVECTOR color, XMVECTOR direction, float innerCutOff, float outerCutOff, float power, float radius)
	: m_lightColor(color), m_direction(XMVector3Normalize(direction)), m_lightPower(power)
{
	this->center = position;
	this->radius = radius;

	this->material = Material(color, XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), 1.0f, 1.0f, XMVectorScale(XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), power));
	this->type = IntersectedType::FlashLight;

	m_innerCutOff = innerCutOff;
	m_outerCutOff = outerCutOff;
}
