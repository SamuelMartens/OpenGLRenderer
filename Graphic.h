#pragma once

#include <vector>
#include <memory>
#include <map>

#include "gl_core_4_3.h"
#include "glm\mat4x4.hpp"
#include "glm\vec4.hpp"

#include "Light.h"
#include "model.h"
#include "Settings.h"
#include "ShaderProgram.h"
#include "CubeTexture.h"

namespace Graphic 
{
	void InitFigure(std::vector<float>& vertices, std::vector<float>& colors);

	enum class VertexAtrib
	{
		VertexCoors = 0,
		Normals = 1,
		TextureCoord = 2,
		Tangents = 3,
		Bitangents = 4
	};

	class Renderer
	{
	public:
		Renderer();
		~Renderer()
		{
			for (auto& model : models)
				model.ClearGLBuffers();
		};
		/* Initializing */
		int Init();
		int InitUniforms();

		/* Drawing */
		void Draw(float angle);
		void ClearScreen() const;

		/* Getters */
		ShaderProgram* GetShaderProgramWithType(ShaderProgram::Type t)
		{
			auto res = shaderPrograms.find(t);
			if (shaderPrograms.end() == res)
				return nullptr;

			return res->second;
		}
		const ShaderProgram* GetShaderProgramWithType(ShaderProgram::Type t) const
		{
			auto res = shaderPrograms.find(t);
			if (shaderPrograms.end() == res)
				return nullptr;

			return res->second;
		}

		/* Set OpenGL data */
		void SetTransMatrix(glm::mat4 &transMat);
		void LoadLightDataToOpenGL() const;
		void ActivateAppropriteToModelSubroutines(const Model& model) const;

		/* Other setters */
		void SetSkyBoxTexture(const CubeTexture& cubeTex)
		{
			if (!skyBox.get())
				skyBox = std::make_unique<Model>();

			skyBox->material.SetTexture(std::make_shared<CubeTexture>(cubeTex));
		}

		/* Adding of elements */
		void AddModel(const Model&& m);
		void AddModel(const Model& m);
		void AddLight(const Light&& l);
		void AddLight(const Light& l);
		void AddLight(std::unique_ptr<Light> l);
		void AddShaderProgram(ShaderProgram* newShaderProg);


		bool HasShaderProgramWithType(ShaderProgram::Type t) const noexcept { return shaderPrograms.find(t) != shaderPrograms.end(); };

	private:
		std::map<ShaderProgram::Type, ShaderProgram*> shaderPrograms;

		std::vector<Model> models;
		std::vector<Light> lights;

		std::unique_ptr<Model> skyBox;

		/* Uniforms */ 
		GLint transMatLoc;
		GLuint modelSubroutine;
		GLuint lightSubroutine;

		GLuint normalTextureSubroutines[2];
		GLuint noNormalTextureSubroutines[2];

	};
}