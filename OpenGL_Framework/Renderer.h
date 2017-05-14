#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "Timer.h"
#include "FrameBuffer.h"
#include <glm/mat4x4.hpp>

struct Material
{
	Material(){}
	~Material(){}

	Texture Albedo;
	Texture Normal;
	Texture AO;

	Texture Metallic;
	Texture Roughness;

	static Texture defaultTex;
};

struct PointLight
{
	
};

class Camera
{
public:
	GMath::vec3f GetPosition()
	{
		glm::vec4 pos(0,0,0,1);
		pos = m_Transform * pos;

		return GMath::vec3f({ pos[0], pos[1], pos[2] });
	}


	GMath::mat4f m_Projection;
	glm::mat4 m_Transform;
};

class Renderer
{
public:
	Renderer(unsigned windowHeight, unsigned windowWidth, Camera* camera)
		:m_WindowWidth(windowWidth), m_WindowHeight(windowHeight), 
		m_Camera(camera),
		GBuffer(5), DefferedComposite(1){}

	void Initalize();
	void InitalizePBREnvironmentMaps(std::string filepath);
	void AddMesh(Mesh* mesh, Material* material);
	void AddPointLight(GMath::vec3f lightColor, GMath::vec3f lightpPsition, bool castsShadows);
	void AddDirectionalLight(GMath::vec3f lightColor, GMath::vec3f lightDirection, bool castsShadows);
	void Render();

private:
	unsigned m_WindowWidth;
	unsigned m_WindowHeight;

	 Camera* m_Camera;

	std::vector<Mesh*> MeshList;
	std::vector<Material*> MaterialList;

	std::vector<GMath::vec3f> m_PointLightColors;
	std::vector<GMath::vec3f> m_PointLightPositions;

	Timer* m_UpdateTimer;

	FrameBuffer GBuffer;
	FrameBuffer DefferedComposite;

	//Shaders used for rendering
	ShaderProgram StaticGeometry;
	ShaderProgram GBufferPass;
	ShaderProgram DefferedLighting;
	

	Texture m_CubeMap;
	Texture m_IrradianceMap;
	Texture m_PrefilterMap;
	Texture m_BDRFMap;
};
