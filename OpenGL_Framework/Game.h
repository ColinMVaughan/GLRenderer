#pragma once
#include <windows.h>
#include <GMath/MathLibrary.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "ParticleEffect.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Timer.h"

#define WINDOW_WIDTH		1200
#define WINDOW_HEIGHT		720
#define FRAMES_PER_SECOND	60
#define BLOOM_THRESHOLD		0.35f
#define BLOOM_DOWNSCALE		4.0f
#define BLOOM_BLUR_PASSES	2
#define SHADOW_RESOLUTION	4096

class Game
{

public:
	typedef GMath::vec3f vec3;
	typedef GMath::vec4f vec4;
	typedef GMath::mat4f mat4;
public:
	Game();
	~Game();

	void initializeGame();
	void update();
	void draw();
	void newDraw();


	GLuint ConvertEQtoCube(std::string filePath);

	/* input callback functions */
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);

	/* Data Members */
	Timer *updateTimer	= nullptr;
	float TotalGameTime = 0.0f;


	ShaderProgram StaticGeometry;
	ShaderProgram BloomHighPass;
	ShaderProgram BlurHorizontal;
	ShaderProgram BlurVertical;
	ShaderProgram BloomComposite;
	ShaderProgram GBufferPass;
	ShaderProgram DefferedLighting;
	

	Mesh Stone;
	Mesh Sword;
	Mesh Ground;
	Mesh House;



	Mesh Orb;
	Texture Albedo;
	Texture Metallic;
	Texture Roughness;

	Texture TexStone;
	Texture TexSword;
	Texture TexGround;
	Texture TexHouse;

	FrameBuffer GBuffer;
	FrameBuffer DefferedComposite;
	
	FrameBuffer WorkBuffer1;
	FrameBuffer WorkBuffer2;

	FrameBuffer ShadowMap;

	FrameBuffer CubePass;

	glm::mat4 CameraTransform;
	mat4 CameraProjection;
	//mat4 ShadowTransform;
	//mat4 ShadowProjection;
	//mat4 ViewToShadowMap;

	vec3 LightPositions[4];
	vec3 LightColors[4];

	GLuint cubeMap;
private:

	bool Wireframe = false;
	bool ShaderEffect = false;

};
