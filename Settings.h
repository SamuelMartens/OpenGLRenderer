#pragma once

struct Settings
{
	Settings() :
		fog(false),
		cartoon(false),
		maxLightNumber(5)
	{};
	~Settings() = default;

	bool fog;
	bool cartoon;
	bool flatShading;
	
	/* Getters */
	unsigned int GetMaxLightNumber() const noexcept { return maxLightNumber; };

private:
	unsigned maxLightNumber;

};