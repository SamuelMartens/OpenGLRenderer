#pragma once 

#include <string>

#include "gl_core_4_3.h"

class Shader
{
public:
	enum class Type
	{
		None
		, Vertex
		, Fragment
	};
	
	Shader():
		  id(-1)
		, type(Type::None)
		, compiled(false)
		{};
	explicit Shader(Type t):
		  id(-1)
		, type(t)
		, compiled(false)
	{};
	Shader(Type t, std::string&& filePath):
		  type(t)
		, compiled(false)
	{
		ParseAndCompile(std::move(filePath));
	};
	~Shader()=default;
	
	GLuint id;
	Type type;
	
	void ParseAndCompile(const std::string&& filePath)
	{
		Compile(ParseSourceFile(std::move(filePath)));
	};
	bool isCompiled() const noexcept{ return compiled; };
	
private:
	std::string ParseSourceFile(const std::string&&  filePath) const;
	void Compile(std::string&& shaderSource);

	bool compiled;
	
};