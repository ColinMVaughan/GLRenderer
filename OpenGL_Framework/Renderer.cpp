#include "Renderer.h"
#include "Utilities.h"
#include "Timer.h"
#include "Game.h"


//---------------------------------------------------
// Purpose: Initalizes the Renderer's default shaders and framebuffers
//
// TODO: Allow for user submitted shaders
//---------------------------------------------------
void Renderer::Initalize()
{

	m_UpdateTimer = new Timer();
	InitFullScreenQuad();

	glEnable(GL_DEPTH_TEST);
	//-----------------------------------------

	if (!StaticGeometry.Load("./Assets/Shaders/CubeMap.vert", "./Assets/Shaders/CubeMap.frag"))
	{
		std::cout << "Shaders failed to initalize.\n";
		system("pause");
		exit(0);
	}

	//Default Gbuffer Shader.
	//TODO: Make this shader customizable
	if (!GBufferPass.Load("./Assets/Shaders/StaticGeometry.vert", "./Assets/Shaders/GBufferPass.frag"))
	{
		std::cout << "Shaders failed to initalize.\n";
		system("pause");
		exit(0);
	}

	if (!DefferedLighting.Load("./Assets/Shaders/PassThorugh.vert", "./Assets/Shaders/PBR_IBL.frag"))
	{
		std::cout << "Shaders failed to initalize.\n";
		system("pause");
		exit(0);
	}

	//Initalize Gbuffer
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	GBuffer.InitDepthTexture(m_WindowWidth, m_WindowHeight);
	GBuffer.InitColorTexture(0, m_WindowWidth, m_WindowHeight, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);	//Flat Color  ///Might fuck things up!!!
	GBuffer.InitColorTexture(1, m_WindowWidth, m_WindowHeight, GL_RGB32F, GL_NEAREST, GL_CLAMP_TO_EDGE);	//Normals (xyz)
	GBuffer.InitColorTexture(2, m_WindowWidth, m_WindowHeight, GL_RGB32F, GL_NEAREST, GL_CLAMP_TO_EDGE);	//ViewSpace Positions (xyz)
	GBuffer.InitColorTexture(3, m_WindowWidth, m_WindowHeight, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);	//Roughness Factor
	GBuffer.InitColorTexture(4, m_WindowWidth, m_WindowHeight, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);	//Metallic Factor
	GBuffer.InitColorTexture(5, m_WindowWidth, m_WindowHeight, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);	//AO Factor
	if (!GBuffer.CheckFBO())
	{
		std::cout << "FBO Failed to Initalize.\n";
		system("pause");
		exit(0);
	}

	//--------------------------------------------------------------------------------------------------------------------------------------------------
	DefferedComposite.InitColorTexture(0, m_WindowWidth, m_WindowHeight, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	if (!DefferedComposite.CheckFBO())
	{
		std::cout << "FBO Failed to Initalize.\n";
		system("pause");
		exit(0);
	}

}

void Renderer::InitalizeDefaultMaterial()
{
	m_DefaultMaterial.Roughness.Load("");
	m_DefaultMaterial.Metallic.Load("");

	m_DefaultMaterial.Albedo.Load("");
	m_DefaultMaterial.Normal.Load("");
	m_DefaultMaterial.AO.Load("");

}

//-----------------------------------------------------------
// Purpose: Loads an HDR equairectangular map and converts it to a cube map.
//			Then creates nessisary textures for IBL rendering.
//
// Params: -filepath to an HDR Equirectangular map.
//------------------------------------------------------------
void Renderer::InitalizePBREnvironmentMaps(std::string filepath)
{
	//Load & convert equarectangular environment map (HDR) to Cube Map
	EnvironmentmapToCubemap(filepath, m_CubeMap);

	//Generate diffuse irradiance map from the environment cube map
	CubemapToIrradianceMap(m_CubeMap, m_IrradianceMap);

	//Genrate prefilter map from cube map
	CubemapToPrefiltermap(m_CubeMap, m_PrefilterMap);

	//Calculate and generate a BRDF lookup texture
	CalculateBRDF(m_BDRFMap);
}

//---------------------------------------------------
// Purpose: Adds mesh to be rendered to the scene with material
// Params: -Pointer to mesh class, -Pointer to material class
//
// TODO: Actually add directional Light
//---------------------------------------------------
void Renderer::AddMesh(Mesh* mesh, Material* material)
{
	MeshList.push_back(mesh);
	MaterialList.push_back(material);
}

//---------------------------------------------------
// Purpose: Adds point light to the scene
// Params: -colour of the light, -direction vector of the light, -if the light casts shadows
//
// TODO: Add safegaurds
//---------------------------------------------------
void Renderer::AddPointLight(GMath::vec3f lightColor, GMath::vec3f lightpPosition, bool castsShadows)
{
	m_PointLightPositions.push_back(lightpPosition);
	m_PointLightColors.push_back(lightColor);
}


//---------------------------------------------------
// Purpose: Adds Directional light to the scene
// Params: -colour of the light, -direction vector of the light, -if the light casts shadows
//
// TODO: Actually add directional Light
//---------------------------------------------------
void Renderer::AddDirectionalLight(GMath::vec3f lightColor, GMath::vec3f lightDirection, bool castsShadows)
{

}



//---------------------------------
// Purpose: Renders the scene with the current list of renderables & default shaders
//
//---------------------------------
void Renderer::Render()
{

	//--------------------------------------------------------
	//			Get Ready for Render
	//--------------------------------------------------------

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0);
	DefferedComposite.Clear();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	GBuffer.Clear();

	//-------------------------------------------------------------------------------
	//			Render Geometry to GBuffer
	//-------------------------------------------------------------------------------
	glViewport(0, 0, m_WindowWidth, m_WindowHeight);

	GBufferPass.Bind();
	GBufferPass.SendUniformMat4("uModel", &glm::mat4()[0][0], false);
	GBufferPass.SendUniformMat4("uView", &glm::inverse(m_Camera->m_Transform)[0][0], false);
	GBufferPass.SendUniformMat4("uProj", m_Camera->m_Projection.GetData(), false);

	GBufferPass.SendUniform("AO", 4);
	GBufferPass.SendUniform("Normal", 3);
	GBufferPass.SendUniform("Albedo", 2);
	GBufferPass.SendUniform("Roughness", 1);
	GBufferPass.SendUniform("Metallic", 0);


	//Draw each mesh in Meshlist.
	GBuffer.Bind();
	for(int i = 0; i < MeshList.size(); ++i)
	{

		glActiveTexture(GL_TEXTURE0);
		MaterialList[i]->Metallic.Bind();
		glActiveTexture(GL_TEXTURE1);
		MaterialList[i]->Roughness.Bind();
		glActiveTexture(GL_TEXTURE2);
		MaterialList[i]->Albedo.Bind();
		glActiveTexture(GL_TEXTURE3);
		MaterialList[i]->Normal.Bind();
		glActiveTexture(GL_TEXTURE4);
		MaterialList[i]->AO.Bind();

		glBindVertexArray(MeshList[i]->VAO);
		glDrawArrays(GL_TRIANGLES, 0, MeshList[i]->GetNumVertices());
		glBindVertexArray(0);

		MaterialList[i]->AO.UnBind();
		glActiveTexture(GL_TEXTURE3);
		MaterialList[i]->Normal.UnBind();
		glActiveTexture(GL_TEXTURE2);
		MaterialList[i]->Albedo.UnBind();
		glActiveTexture(GL_TEXTURE1);
		MaterialList[i]->Roughness.UnBind();
		glActiveTexture(GL_TEXTURE0);
		MaterialList[i]->Metallic.UnBind();
	}

	GBufferPass.UnBind();
	GBuffer.UnBind();

	//-----------------------------------------------
	//			Render Skybox
	//----------------------------------------------
	DefferedComposite.Bind();
	StaticGeometry.Bind();


	StaticGeometry.SendUniformMat4("view", &glm::inverse(m_Camera->m_Transform)[0][0], false);
	StaticGeometry.SendUniformMat4("projection", m_Camera->m_Projection.GetData(), false);
	StaticGeometry.SendUniform("environmentMap", 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap.TexObj);
	DrawCube();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	StaticGeometry.UnBind();
	DefferedComposite.UnBind();

	//--------------------------------------------------------
	//			Deffered Lighting Pass
	//--------------------------------------------------------
	glViewport(0, 0, m_WindowWidth, m_WindowHeight);
	DefferedLighting.Bind();

	DefferedLighting.SendUniform("albedoMap", 0);
	DefferedLighting.SendUniform("normalMap", 1);
	DefferedLighting.SendUniform("positionMap", 2);

	DefferedLighting.SendUniform("roughnessMap", 3);
	DefferedLighting.SendUniform("metallicMap", 4);

	DefferedLighting.SendUniform("irradianceMap", 5);
	DefferedLighting.SendUniform("prefilterMap", 6);
	DefferedLighting.SendUniform("brdfLUT", 7);

	DefferedLighting.SendUniform("aoMap", 8);


	//DefferedLighting.SendUniform("aoMap", 8);
	DefferedLighting.SendUniform("camPos", m_Camera->GetPosition());
	DefferedLighting.SendUniformArray("lightPositions", m_PointLightPositions.data(), 4);
	DefferedLighting.SendUniformArray("lightColors", m_PointLightColors.data(), 4);

	DefferedComposite.Bind();

	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(0));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(1));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(2));
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(3));
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(4));
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap.TexObj);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMap.TexObj);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, m_BDRFMap.TexObj);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, GBuffer.GetColorHandle(5));
	DrawFullScreenQuad();
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, GL_NONE);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, GL_NONE);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	DefferedComposite.UnBind();
	DefferedLighting.UnBind();

	DefferedComposite.MoveToBackBuffer(m_WindowWidth, m_WindowHeight);
	glutSwapBuffers();
	//------------------------------------------------------------------------------
}
