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
#include "Camera.h"

namespace Graphic 
{
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
		
		Renderer(const Renderer&) = delete;
		Renderer(const Renderer&&) = delete;
		void Operator(const Renderer&) = delete;
		void Operator (const Renderer&&) = delete;

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
		static Renderer& Instance()
		{
			static Renderer instance;
			return instance;
		}
		const ShaderProgram* GetShaderProgramWithType(ShaderProgram::Type t) const
		{
			auto res = shaderPrograms.find(t);
			if (shaderPrograms.end() == res)
				return nullptr;

			return res->second;
		}
		ShaderProgram* GetShaderProgramWithType(ShaderProgram::Type t)
		{
			// Here to avoid code duplication for const and non const getter.
			// This code is madness but I trust Scott Meyer, as it is his advice :)
			return const_cast<ShaderProgram*>(
				static_cast<const Renderer&>(*this).GetShaderProgramWithType(t));
		}
		const std::unique_ptr<Camera>& GetCamera() const noexcept { return camera; };
		std::unique_ptr<Camera>& GetCamera()
		{
			return const_cast<std::unique_ptr<Camera>&>(
				static_cast<const Renderer&>(*this).GetCamera());
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
		void SetCamera(std::unique_ptr<Camera>&& newCam)
		{
			assert(newCam.get());
			camera.swap(newCam);
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
		Renderer();

		std::map<ShaderProgram::Type, ShaderProgram*> shaderPrograms;

		std::vector<Model> models;
		std::vector<Light> lights;

		std::unique_ptr<Camera> camera;
		// DEBUG What is that?
		std::unique_ptr<Model> skyBox;

		/* Uniforms */ 
		GLint transMatLoc;
		GLuint modelSubroutine;
		GLuint lightSubroutine;

		GLuint normalTextureSubroutines[2];
		GLuint noNormalTextureSubroutines[2];

	};
}