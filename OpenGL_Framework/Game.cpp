#include "Game.h"
#include "Utilities.h"

//DEFFERED UPGRADE1

Game::Game()
: GBuffer(5), DefferedComposite(1), ShadowMap(0), WorkBuffer1(1), WorkBuffer2(1)
{
	LightPositions[0] = vec3({ 10.0f, 10.0f, 10.0f });
	LightPositions[1] = vec3({10.0f, -10.0f, -10.0f});
	LightPositions[2] = vec3({-10.0f, -10.0f, 10.0f});
	LightPositions[3] = vec3({-10.0f, 10.0f, -10.0f});

	LightColors[0] = vec3({300, 300, 300});
	LightColors[1] = vec3({300, 300, 300});
	LightColors[2] = vec3({300, 300, 300});
	LightColors[3] = vec3({300, 300, 300});

}

Game::~Game()
{
	delete updateTimer;

	StaticGeometry.UnLoad();

	Sword.Unload();
	TexSword.Unload();

	House.Unload();
	TexHouse.Unload();

	Ground.Unload();
	TexGround.Unload();

	Stone.Unload();
	TexStone.Unload();
	
}

void Game::initializeGame()
{
	updateTimer = new Timer();
	InitFullScreenQuad();

	glEnable(GL_DEPTH_TEST);


	//Orb stuff for PBR test
//-----------------------------------------------------------------
	if (!Orb.LoadFromFile("./Assets/Models/Orb.obj"))
	{
		std::cout << "Couldn't find the monkey.\n";
		system("pause");
		exit(0);
	}
	if (!Albedo.Load("./Assets/Textures/Rust/Albedo.png"))
	{
		system("pause");
		exit(0);
	}
	if (!Metallic.Load("./Assets/Textures/Rust/Metallic1.png"))
	{
		system("pause");
		exit(0);
	}
	if (!Roughness.Load("./Assets/Textures/Rust/Roughness1.png"))
	{
		system("pause");
		exit(0);
	}
//------------------------------------------------------------------



	if (!StaticGeometry.Load("./Assets/Shaders/StaticGeometry.vert", "./Assets/Shaders/GBufferPass.frag"))
	{
		std::cout << "Shaders failed to initalize.\n";
		system("pause");
		exit(0);
	}

	//if (!BloomHighPass.Load("./Assets/Shaders/PassThorugh.vert", "./Assets/Shaders/Bloom/BloomHighPass.frag"))
	//{
	//	std::cout << "Shaders failed to initalize.\n";
	//	system("pause");
	//	exit(0);
	//}

	//if (!BlurHorizontal.Load("./Assets/Shaders/PassThorugh.vert", "./Assets/Shaders/Bloom/BlurHorizontal.frag"))
	//{
	//	std::cout << "Shaders failed to initalize.\n";
	//	system("pause");
	//	exit(0);
	//}

	//if (!BlurVertical.Load("./Assets/Shaders/PassThorugh.vert", "./Assets/Shaders/Bloom/BlurVertical.frag"))
	//{
	//	std::cout << "Shaders failed to initalize.\n";
	//	system("pause");
	//	exit(0);
	//}

	//if (!BloomComposite.Load("./Assets/Shaders/PassThorugh.vert", "./Assets/Shaders/Bloom/BloomComposite.frag"))
	//{
	//	std::cout << "Shaders failed to initalize.\n";
	//	system("pause");
	//	exit(0);
	//}

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

	//if (!Stone.LoadFromFile("./Assets/Models/Stone.obj"))
	//{
	//	std::cout << "Couldn't find the monkey.\n";
	//	system("pause");
	//	exit(0);
	//}

	//if (!Sword.LoadFromFile("./Assets/Models/Sword.obj"))
	//{
	//	std::cout << "Couldn't find the monkey.\n";
	//	system("pause");
	//	exit(0);
	//}

	//if (!Ground.LoadFromFile("./Assets/Models/Ground.obj"))
	//{
	//	std::cout << "Couldn't find the monkey.\n";
	//	system("pause");
	//	exit(0);
	//}

	//if (!House.LoadFromFile("./Assets/Models/House.obj"))
	//{
	//	std::cout << "Couldn't find the monkey.\n";
	//	system("pause");
	//	exit(0);
	//}
	
	//if (!TexStone.Load("./Assets/Textures/Stone.png"))
	//{
	//	system("pause");
	//	exit(0);
	//}

	//if (!TexSword.Load("./Assets/Textures/Sword.png"))
	//{
	//	system("pause");
	//	exit(0);
	//}

	//if (!TexGround.Load("./Assets/Textures/Ground.png"))
	//{
	//	system("pause");
	//	exit(0);
	//}

	//if (!TexHouse.Load("./Assets/Textures/House.png"))
	//{
	//	system("pause");
	//	exit(0);
	//}
	//
	GBuffer.InitDepthTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
	GBuffer.InitColorTexture(0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);	//Flat Color  ///Might fuck things up!!!
	GBuffer.InitColorTexture(1, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB8, GL_NEAREST, GL_CLAMP_TO_EDGE);	//Normals (xyz)
	GBuffer.InitColorTexture(2, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB32F, GL_NEAREST, GL_CLAMP_TO_EDGE);	//ViewSpace Positions (xyz)
	GBuffer.InitColorTexture(3, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);	//Roughness Factor
	GBuffer.InitColorTexture(4, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);	//Metallic Factor
	if (!GBuffer.CheckFBO())
	{
		std::cout << "FBO Failed to Initalize.\n";
		system("pause");
		exit(0);
	}

	DefferedComposite.InitColorTexture(0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	if (!DefferedComposite.CheckFBO())
	{
		std::cout << "FBO Failed to Initalize.\n";
		system("pause");
		exit(0);
	}

	ShadowMap.InitDepthTexture(SHADOW_RESOLUTION, SHADOW_RESOLUTION);
	if (!ShadowMap.CheckFBO())
	{
		std::cout << "FBO Failed to Initalize.\n";
		system("pause");
		exit(0);
	}


	WorkBuffer1.InitColorTexture(0, WINDOW_WIDTH / BLOOM_DOWNSCALE , WINDOW_HEIGHT / BLOOM_DOWNSCALE, GL_RGB8, GL_LINEAR, GL_CLAMP_TO_EDGE);
	if (!WorkBuffer1.CheckFBO())
	{
		std::cout << "FBO Failed to Initalize.\n";
		system("pause");
		exit(0);
	}

	WorkBuffer2.InitColorTexture(0, WINDOW_WIDTH / BLOOM_DOWNSCALE, WINDOW_HEIGHT / BLOOM_DOWNSCALE, GL_RGB8, GL_LINEAR, GL_CLAMP_TO_EDGE);
	if (!WorkBuffer2.CheckFBO())
	{
		std::cout << "FBO Failed to Initalize.\n";
		system("pause");
		exit(0);
	}

	GMath::SetFrustumProjection(CameraProjection, 60.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 10000.0f);
	//CameraProjection.FrustumProjection(60.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 10000.0f);
	//ShadowProjection.OrthoProjection(35.0f, -35.0f, -35.0f, 35.0f, -10.0f, 100.0f);

	//MonkeyTransform.RotateX(90.0f);
}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	GMath::SetIdentity(CameraTransform);
	GMath::RotateY(CameraTransform, TotalGameTime * 10.0f);
	GMath::SetTranslate(CameraTransform, vec3({ 0.0f, 7.5f, 20.0f }));
	GMath::RotateX(CameraTransform, -15.0f);

	//CameraTransform.LoadIdentity();
	//CameraTransform.RotateY(TotalGameTime * 10.f);
	//CameraTransform.Translate(0.0f, 7.5f, 20.0f);
	//CameraTransform.RotateX(-15.0f);

	//ShadowTransform.LoadIdentity();
	//ShadowTransform.RotateY(TotalGameTime * -2.5f);
	//ShadowTransform.Translate(0.0f, 7.5f, 20.0f);
	//ShadowTransform.RotateX(-15.0f);

	//mat4 bias = mat4(0.5f, 0.0f, 0.0f, 0.5f,
	//				0.0f, 0.5f, 0.0f, 0.5f,
	//				0.0f, 0.0f, 0.5f, 0.5f,
	//				0.0f, 0.0f, 0.0f, 1.0f);

	//ViewToShadowMap.LoadIdentity();
	//ViewToShadowMap = bias * ShadowProjection * ShadowTransform.GetInverse() * CameraTransform;



}

void Game::draw()
{
	vec4 camPos({ 0,0,0,1 });
	camPos = CameraTransform * camPos;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0);
	DefferedComposite.Clear();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	GBuffer.Clear();
	ShadowMap.Clear();
	WorkBuffer1.Clear();
	WorkBuffer2.Clear();

	////////////-Shadow-/////////////////
	//glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);
	//
	//StaticGeometry.Bind();
	//StaticGeometry.SendUniformMat4("uModel", mat4::Identity().data, true);
	//StaticGeometry.SendUniformMat4("uView", ShadowTransform.GetInverse().data, true);
	//StaticGeometry.SendUniformMat4("uProj", ShadowProjection.data, true);
	//
	////////////////////////////////////////
	//
	//ShadowMap.Bind();
	//
	//glBindVertexArray(Sword.VAO);
	//glDrawArrays(GL_TRIANGLES, 0, Sword.GetNumVertices());
	//
	//glBindVertexArray(Ground.VAO);
	//glDrawArrays(GL_TRIANGLES, 0, Ground.GetNumVertices());
	//
	//glBindVertexArray(House.VAO);
	//glDrawArrays(GL_TRIANGLES, 0, House.GetNumVertices());
	//
	//glBindVertexArray(Stone.VAO);
	//glDrawArrays(GL_TRIANGLES, 0, Stone.GetNumVertices());
	//
	//glBindVertexArray(0);
	//StaticGeometry.UnBind();
	//ShadowMap.UnBind();




////////////-Geometry-/////////////////
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	GBufferPass.Bind();
	GBufferPass.SendUniformMat4("uModel", GMath::GetMat4Identity().GetData(), false);
	GBufferPass.SendUniformMat4("uView", GMath::GetInverse(CameraTransform).GetData(), false);
	GBufferPass.SendUniformMat4("uProj", CameraProjection.GetData(), false);


	GBufferPass.SendUniform("Albedo", 2);
	GBufferPass.SendUniform("Roughness", 1);
	GBufferPass.SendUniform("Metallic", 0);



/////////////////////////////


///////////////////////////////
	GBuffer.Bind();
	
	glActiveTexture(GL_TEXTURE0);
	Metallic.Bind();
	glActiveTexture(GL_TEXTURE1);
	Roughness.Bind();
	glActiveTexture(GL_TEXTURE2);
	Albedo.Bind();
	
	
	glBindVertexArray(Orb.VAO);
	glDrawArrays(GL_TRIANGLES, 0, Orb.GetNumVertices());
	glBindVertexArray(0);

	Albedo.UnBind();
	glActiveTexture(GL_TEXTURE1);
	Roughness.UnBind();
	glActiveTexture(GL_TEXTURE0);
	Metallic.UnBind();


	GBufferPass.UnBind();
	GBuffer.UnBind();
///////////////////////////////
	//TexSword.Bind();
	//glBindVertexArray(Sword.VAO);
	//glDrawArrays(GL_TRIANGLES, 0, Sword.GetNumVertices());
	//glBindVertexArray(0);
	//TexSword.UnBind();
///////////////////////////////
	
///////////////////////////////
	//TexGround.Bind();
	//glBindVertexArray(Ground.VAO);
	//glDrawArrays(GL_TRIANGLES, 0, Ground.GetNumVertices());
	//glBindVertexArray(0);
	//TexGround.UnBind();
///////////////////////////////
	
///////////////////////////////
	//TexHouse.Bind();
	//glBindVertexArray(House.VAO);
	//glDrawArrays(GL_TRIANGLES, 0, House.GetNumVertices());
	//glBindVertexArray(0);
	//TexHouse.UnBind();
///////////////////////////////
	
///////////////////////////////
	//TexStone.Bind();
	//glBindVertexArray(Stone.VAO);
	//glDrawArrays(GL_TRIANGLES, 0, Stone.GetNumVertices());
	//glBindVertexArray(0);
	//TexStone.UnBind();
/////////////////////////////

////////////////////////////

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, GL_NONE);

	//GBufferPass.UnBind();
	//GBuffer.UnBind();


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Create Scene from GBUFFER//
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	DefferedLighting.Bind();

	DefferedLighting.SendUniform("albedoMap", 0);
	DefferedLighting.SendUniform("normalMap", 1);
	DefferedLighting.SendUniform("positionMap", 2);

	DefferedLighting.SendUniform("roughnessMap", 3);
	DefferedLighting.SendUniform("metallicMap", 4);

	//DefferedLighting.SendUniform("aoMap", 5);
	DefferedLighting.SendUniform("camPos", vec3({ camPos[0], camPos[1], camPos[2] }));
	DefferedLighting.SendUniformArray("lightPositions", LightPositions, 4);
	DefferedLighting.SendUniformArray("lightColors", LightColors, 4);

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

	
	if (ShaderEffect == true)
	{
		//glViewport(0, 0, WINDOW_WIDTH / BLOOM_DOWNSCALE, WINDOW_HEIGHT / BLOOM_DOWNSCALE);

		////Binding shader----------------
		//BloomHighPass.Bind();
		//BloomHighPass.SendUniform("uTex", 0);
		//BloomHighPass.SendUniform("uThreshold", BLOOM_THRESHOLD);

		//WorkBuffer1.Bind();

		//
		//glBindTexture(GL_TEXTURE_2D, DefferedComposite.GetColorHandle(0));
		//DrawFullScreenQuad();
		//glBindTexture(GL_TEXTURE_2D, GL_NONE);

		//WorkBuffer1.UnBind();
		//BloomHighPass.UnBind();

		//for (int i = 0; i < BLOOM_BLUR_PASSES; i++)
		//{
		//	///Horizontal///
		//	BlurHorizontal.Bind();
		//	BlurHorizontal.SendUniform("uTex", 0);
		//	BlurHorizontal.SendUniform("uPixelSize", 1.0f / (WINDOW_WIDTH/BLOOM_DOWNSCALE));

		//	WorkBuffer2.Bind();

		//	glBindTexture(GL_TEXTURE_2D, WorkBuffer1.GetColorHandle(0));
		//	DrawFullScreenQuad();
		//	glBindTexture(GL_TEXTURE_2D, GL_NONE);

		//	WorkBuffer2.UnBind();
		//	BlurHorizontal.UnBind();

		//	///Vertical///
		//	BlurVertical.Bind();
		//	BlurVertical.SendUniform("uTex", 0);
		//	BlurVertical.SendUniform("uPixelSize", 1.0f / WINDOW_HEIGHT);

		//	WorkBuffer1.Bind();

		//	glBindTexture(GL_TEXTURE_2D, WorkBuffer2.GetColorHandle(0));
		//	DrawFullScreenQuad();
		//	glBindTexture(GL_TEXTURE_2D, GL_NONE);

		//	WorkBuffer1.UnBind();
		//	BlurHorizontal.UnBind();
		//}

		/////Composite///
		//glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		//BloomComposite.Bind();
		//BloomComposite.SendUniform("uScene", 0);
		//BloomComposite.SendUniform("uBloom", 1);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, DefferedComposite.GetColorHandle(0));
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, WorkBuffer1.GetColorHandle(0));
		//	DrawFullScreenQuad();
		//glBindTexture(GL_TEXTURE_2D, GL_NONE);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, GL_NONE);

		//BloomComposite.UnBind();

	}
	else
	{
		DefferedComposite.MoveToBackBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	
	glutSwapBuffers();
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	switch(key)
	{
	case '1':
		if (Wireframe == false)
		{
			ShaderEffect = false;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			Wireframe = true;
		}
		else
		{
			//ShaderEffect = true;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			Wireframe = false;
		}

		break;
	
	case '2':
		ShaderEffect = !ShaderEffect;
		break;

	case '3':
		LightPositions[0][0] += 0.01f;
		std::cout <<"\n pos: "<< LightPositions[0][0];
		break;

	case '4':
		LightPositions[0][0] -= 0.01f;
		break;

	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	switch(key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}
}

void Game::mouseClicked(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN) 
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:

			break;
		case GLUT_RIGHT_BUTTON:
		
			break;
		case GLUT_MIDDLE_BUTTON:

			break;
		}
	}
	else
	{

	}
}

/*
 * mouseMoved(x,y)
 * - this occurs only when the mouse is pressed down
 *   and the mouse has moved.  you are given the x,y locations
 *   in window coordinates (from the top left corner) and thus 
 *   must be converted to screen coordinates using the screen to window pixels ratio
 *   and the y must be flipped to make the bottom left corner the origin.
 */
void Game::mouseMoved(int x, int y)
{
}
