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

	if (!StaticGeometry.Load("./Assets/Shaders/StaticGeometry.vert", "./Assets/Shaders/GBufferPass.frag"))
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

	if (!DefferedLighting.Load("./Assets/Shaders/PassThorugh.vert", "./Assets/Shaders/PBR.frag"))
	{
		std::cout << "Shaders failed to initalize.\n";
		system("pause");
		exit(0);
	}

	//Initalize Gbuffer
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	GBuffer.InitDepthTexture(m_WindowWidth, m_WindowHeight);
	GBuffer.InitColorTexture(0, m_WindowWidth, m_WindowHeight, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);	//Flat Color  ///Might fuck things up!!!
	GBuffer.InitColorTexture(1, m_WindowWidth, m_WindowHeight, GL_RGB8, GL_NEAREST, GL_CLAMP_TO_EDGE);	//Normals (xyz)
	GBuffer.InitColorTexture(2, m_WindowWidth, m_WindowHeight, GL_RGB32F, GL_NEAREST, GL_CLAMP_TO_EDGE);	//ViewSpace Positions (xyz)
	GBuffer.InitColorTexture(3, m_WindowWidth, m_WindowHeight, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);	//Roughness Factor
	GBuffer.InitColorTexture(4, m_WindowWidth, m_WindowHeight, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);	//Metallic Factor
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
	GMath::vec4f camPos({ 0,0,0,1 });
	camPos = *m_CameraTransform * camPos;

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
	GBufferPass.SendUniformMat4("uView", m_CameraTransform->GetData(), false);
	GBufferPass.SendUniformMat4("uProj", m_CameraProjection->GetData(), false);

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

		glBindVertexArray(MeshList[i]->VAO);
		glDrawArrays(GL_TRIANGLES, 0, MeshList[i]->GetNumVertices());
		glBindVertexArray(0);

		MaterialList[i]->Albedo.UnBind();
		glActiveTexture(GL_TEXTURE1);
		MaterialList[i]->Roughness.UnBind();
		glActiveTexture(GL_TEXTURE0);
		MaterialList[i]->Metallic.UnBind();
	}

	GBufferPass.UnBind();
	GBuffer.UnBind();

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

	//DefferedLighting.SendUniform("aoMap", 5);
	DefferedLighting.SendUniform("camPos", GMath::vec3f({ camPos[0], camPos[1], camPos[2] }));
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
	DrawFullScreenQuad();
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

	DefferedComposite.MoveToBackBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutSwapBuffers();
	//------------------------------------------------------------------------------
}