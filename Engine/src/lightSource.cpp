#define _USE_MATH_DEFINES

#include "scene.h"

// calculate attenuation as solidAgnle / (2.0f * M_PI)
float calcAttenuation(float radius, float distance)
{
	distance = max(distance, radius);
	float h = 1.0f - sqrtf(1.0f - (radius / distance) * (radius / distance));

	return h;
}	

// May return direction pointing beneath surface horizon (dot(N, dir) < 0), use clampDirToHorizon to fix it.
// sphereCos is cosine of light sphere angular diameter.
// sphereRelPos is position of a sphere relative to surface:
// 'sphereDir == normalize(sphereRelPos)' and 'sphereDir * sphereDist == sphereRelPos'
XMVECTOR approximateClosestSphereDir(bool& intersects, XMVECTOR reflectionDir, float sphereCos,
	XMVECTOR sphereRelPos, XMVECTOR sphereDir, float sphereDist, float sphereRadius)
{
	float RoS = XMVectorGetX(XMVector3Dot(reflectionDir, sphereDir));

	intersects = (RoS >= sphereCos);
	if (intersects) {return reflectionDir;}
	if (RoS < 0.0f) return sphereDir;
	XMVECTOR closestPointDir = XMVector3Normalize(reflectionDir * sphereDist * RoS - sphereRelPos);
	return XMVector3Normalize(sphereRelPos + sphereRadius * closestPointDir);
}

// Input dir and NoD is L and NoL in a case of lighting computation 
void clampDirToHorizon(XMVECTOR& dir, float& NoD, XMVECTOR normal, float minNoD)
{
	if (NoD < minNoD)
	{
		dir = XMVector3Normalize(dir + (minNoD - NoD) * normal);
		NoD = minNoD;
	}
}

XMVECTOR CookTorrance_GGX(const XMVECTOR& viewDir, const XMVECTOR& fragNorm, float attenuation, const XMVECTOR& lightCenter,
	const XMVECTOR& fragPos, const XMVECTOR& F0, float distance, float radius, float rough2)
{
	XMVECTOR reflectionDir = XMVector3Reflect(viewDir, fragNorm);
	bool intersects = false;

	XMVECTOR lightSpecDirection = approximateClosestSphereDir(intersects, -reflectionDir, -(attenuation - 1.0f), (lightCenter - fragPos),
		(lightCenter - fragPos)/distance, distance, radius);

	float NoL = max(XMVectorGetX(XMVector3Dot(fragNorm, lightSpecDirection)), 0.0f) + SMALL_OFFSET;

	clampDirToHorizon(lightSpecDirection, NoL, fragNorm, 0.0f);

	XMVECTOR halfDir = XMVector3Normalize(lightSpecDirection + viewDir);
	float NoH = max(XMVectorGetX(XMVector3Dot(fragNorm, halfDir)), 0.0f);
	float LoH = max(XMVectorGetX(XMVector3Dot(lightSpecDirection, halfDir)), 0.0f);
	float NoV = max(XMVectorGetX(XMVector3Dot(fragNorm, viewDir)), 0.0f) + SMALL_OFFSET;

	XMVECTOR Fspec = math::clamp3(frensel(LoH, F0), 0.0f, 1.0f);
	float D = ggx(rough2, NoH);
	float G = smith(rough2, NoV, NoL);
	XMVECTOR spec = XMVectorScale(Fspec, G * min(1.0f, attenuation * (2.0f * M_PI) * D * 0.25f / (NoL * NoV)));
	return spec;
}

XMVECTOR Scene::PointLight::illuminate(const XMVECTOR& fragPos, const XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& material, bool reflOn ) const
{
	XMVECTOR viewDirection = XMVector3Normalize(cameraPos - fragPos);

	float distance = XMVectorGetX(XMVector3Length(this->center - fragPos));

	//solid angle
	float attenuation = calcAttenuation(this->radius, distance);

	XMVECTOR lightDirection = XMVector3Normalize(this->center - fragPos);
	float NoL = max(XMVectorGetX(XMVector3Dot(fragNorm, lightDirection)), 0.0f) + SMALL_OFFSET;

	float rough2 = material->roughness * material->roughness;
	//spec
	XMVECTOR spec = CookTorrance_GGX(viewDirection, fragNorm, attenuation, this->center, fragPos, material->F0, distance, this->radius, rough2);

	//diffuse
	XMVECTOR Fdiff = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f) - frensel(NoL, material->F0);
	XMVECTOR diff = XMVectorScale(Fdiff * material->albedo * m_lightColor * attenuation, 1.0f/ (M_PI));

	return XMVectorScale(diff + spec, NoL) * (m_lightPower);
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


	XMVECTOR spec = XMVectorScale(Fspec, G * min(1.0f, solidAngle * D * 0.25f / (NoL * NoV)));

	//diffuse
	XMVECTOR Fdiff = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f) - math::clamp3(frensel(NoL, material->F0), 0.0f, 1.0f);

	XMVECTOR diff = XMVectorScale(Fdiff * material->albedo * m_lightColor, 1.0f / (M_PI));

	return  XMVectorScale(diff + spec, NoL) * (m_lightPower);
}

XMVECTOR Scene::SpotLight::illuminate(const XMVECTOR& fragPos, const XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& material, bool reflOn) const
{
	XMVECTOR viewDirection = XMVector3Normalize(cameraPos - fragPos);

	float distance = XMVectorGetX(XMVector3Length(this->center - fragPos));
	float attenuation = calcAttenuation(this->radius, distance);

	XMVECTOR lightDirection = XMVector3Normalize(this->center - fragPos);
	float NoL = max(XMVectorGetX(XMVector3Dot(fragNorm, lightDirection)), 0.0f) + SMALL_OFFSET;

	float rough2 = material->roughness * material->roughness;
	//spec
	XMVECTOR spec = CookTorrance_GGX(viewDirection, fragNorm, attenuation, this->center, fragPos, material->F0, distance, this->radius, rough2);

	//diffuse
	XMVECTOR Fdiff = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f) - frensel(NoL, material->F0);
	XMVECTOR diff = XMVectorScale(Fdiff * material->albedo * m_lightColor * attenuation, 1.0f / (M_PI));

	// spotlight (soft edges)
	float theta = XMVectorGetX(XMVector3Dot(lightDirection, -XMVector3Normalize(m_direction)));
	float intensity = smoothstep(m_outerCutOff, m_innerCutOff , theta);

	return  XMVectorScale(diff + spec, NoL) * intensity * (m_lightPower);
}

Scene::PointLight::PointLight(XMVECTOR position, XMVECTOR color, float power, float radius)
{
	m_lightColor = color;
	m_lightPower = power * LIGHT_POWER_MULTIPLIER;

	this->center = position;
	this->radius = radius;

	this->material = Material(color,  1.0f, 1.0f, XMVectorScale(color, LIGHT_POWER_MULTIPLIER * power));
	this->type = IntersectedType::PointLight;
}

Scene::DirectionalLight::DirectionalLight(XMVECTOR direction, XMVECTOR color, float power, float solidAngle)
	: m_direction(direction), m_lightColor(color), m_lightPower(power * LIGHT_POWER_MULTIPLIER), solidAngle(solidAngle)
{

}

Scene::SpotLight::SpotLight(XMVECTOR position, XMVECTOR color, XMVECTOR direction, float innerCutOff, float outerCutOff, float power, float radius)
	: m_lightColor(color), m_direction(XMVector3Normalize(direction)), m_lightPower(power * LIGHT_POWER_MULTIPLIER)
{
	this->center = position;
	this->radius = radius;

	this->material = Material(color, 1.0f, 1.0f, XMVectorScale(XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), LIGHT_POWER_MULTIPLIER  * power));
	this->type = IntersectedType::FlashLight;

	m_innerCutOff = innerCutOff;
	m_outerCutOff = outerCutOff;
}
