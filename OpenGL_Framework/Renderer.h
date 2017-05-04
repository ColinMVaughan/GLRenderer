#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "Timer.h"
#include "FrameBuffer.h"

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


class Renderer
{
public:
	Renderer(unsigned windowHeight, unsigned windowWidth, GMath::mat4f* cameraTransform, GMath::mat4f* cameraProjection)
		:m_WindowWidth(windowWidth), m_WindowHeight(windowHeight), 
		m_CameraTransform(cameraTransform), m_CameraProjection(cameraProjection){}

	void Initalize();
	void AddMesh(Mesh* mesh, Material* material);
	void AddPointLight(GMath::vec3f lightColor, GMath::vec3f lightpPsition, bool castsShadows);
	void AddDirectionalLight(GMath::vec3f lightColor, GMath::vec3f lightDirection, bool castsShadows);
	void Render();

private:
	unsigned m_WindowWidth;
	unsigned m_WindowHeight;

	GMath::mat4f* m_CameraTransform;
	GMath::mat4f* m_CameraProjection;

	std::vector<Mesh*> MeshList;
	std::vector<Material*> MaterialList;

	std::vector<GMath::vec3f> m_PointLightColors;
	std::vector<GMath::vec3f> m_PointLightPositions;

	Timer* m_UpdateTimer;


	FrameBuffer GBuffer;
	FrameBuffer DefferedComposite;

	//Shaders used for rendering
	ShaderProgram StaticGeometry;
	ShaderProgram BloomHighPass;
	ShaderProgram BlurHorizontal;
	ShaderProgram BlurVertical;
	ShaderProgram BloomComposite;
	ShaderProgram GBufferPass;
	ShaderProgram DefferedLighting;
	
};
