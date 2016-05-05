#pragma once

struct Settings
{
	enum class ShadingType
	{
		Guro,
		Phong,
		Flat
	};

	Settings() :
		shading(ShadingType::Phong),
		fog(false),
		cartoon(false),
		maxLightNumber(5)
	{};
	~Settings() = default;

	ShadingType shading;
	bool fog;
	bool cartoon;
	bool flatShading;
	
	/* Getters */
	unsigned int GetMaxLightNumber() const noexcept { return maxLightNumber; };

private:
	unsigned maxLightNumber;

};