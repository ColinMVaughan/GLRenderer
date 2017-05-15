#include "Game.h"
#include "Utilities.h"

//DEFFERED UPGRADE1

Game::Game()
: m_Renderer(WINDOW_HEIGHT,WINDOW_WIDTH, &m_Camera)
{


}

Game::~Game()
{
	delete updateTimer;

	
}

void Game::initializeGame()
{
	updateTimer = new Timer();
	InitFullScreenQuad();

	glEnable(GL_DEPTH_TEST);
	GMath::SetFrustumProjection(m_Camera.m_Projection, 45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 10000.0f);


	//Orb stuff for PBR test & New Renderer Test
//-----------------------------------------------------------------
	if (!Orb.LoadFromFile("./Assets/Models/Gun.obj"))
	{
		std::cout << "Couldn't find the monkey.\n";
		system("pause");
		exit(0);
	}
	if (!GunMat.Albedo.Load("./Assets/Textures/Gun/Albedo.png"))
	{
		system("pause");
		exit(0);
	}
	if (!GunMat.Metallic.Load("./Assets/Textures/Gun/Metallic.png"))
	{
		system("pause");
		exit(0);
	}
	if (!GunMat.Roughness.Load("./Assets/Textures/Gun/Roughness.png"))
	{
		system("pause");
		exit(0);
	}
	if (!GunMat.Normal.Load("./Assets/Textures/Gun/Normal.png"))
	{
		system("pause");
		exit(0);
	}

	m_Renderer.Initalize();
	m_Renderer.AddMesh(&Orb, &GunMat);
	m_Renderer.InitalizePBREnvironmentMaps("./Assets/Textures/GCanyon_C_YumaPoint_3k.hdr");
	m_Renderer.AddPointLight(vec3({ 300.0f, 300.0f, 300.0f }), vec3({ 1.0f, 1.0f, -10.0f }), false);
	m_Renderer.AddPointLight(vec3({ 3000.0f, 3000.0f, 3000.0f }), vec3({ 50.0f, -50.0f, -50.0f }), false);
	m_Renderer.AddPointLight(vec3({ 3000.0f, 3000.0f, 3000.0f }), vec3({ -50.0f, -50.0f, 50.0f }), false);
	m_Renderer.AddPointLight(vec3({ 3000.0f, 3000.0f, 3000.0f }), vec3({ -50.0f, 50.0f, -50.0f }), false);


	return;

}

void Game::update()
{

	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	m_Camera.m_Transform = glm::mat4();
	m_Camera.m_Transform = glm::rotate(m_Camera.m_Transform, float(TotalGameTime * 0.7f), glm::vec3(0, 1, 0));
	m_Camera.m_Transform = glm::translate(m_Camera.m_Transform, glm::vec3(0.0f, 7.5f, 150.0f));
	m_Camera.m_Transform = glm::rotate(m_Camera.m_Transform, -0.15f, glm::vec3(1, 0, 0));

}




void Game::draw()
{
	
	m_Renderer.Render();
	return;

}


void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	switch(key)
	{
	case '1':
		break;
	
	case '2':
		break;

	case '3':
		break;

	case '4':
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
