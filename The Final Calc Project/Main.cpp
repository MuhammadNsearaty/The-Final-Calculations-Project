// dear imgui: standalone example application for GLUT/FreeGLUT + OpenGL2, using legacy fixed pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.

// !!! GLUT/FreeGLUT IS OBSOLETE SOFTWARE. Using GLUT is not recommended unless you really miss the 90's. !!!
// !!! If someone or something is teaching you GLUT in 2019, you are being abused. Please show some resistance. !!!

#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"
#include<iostream>
#include<math.h>
#include"Cube.h"
#include"Shpere.h"
#include"PhysicsEngine.h"

#include<vector>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <freeglut.h>
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4505) // unreferenced local function has been removed
#endif

static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.00f, 1.00f);
void reshape(int w, int h);
void timer(int t);
GLUquadric *q = gluNewQuadric();
double movX = 0.0f, movY = 0.0f, movZ = 0.0f;
double lX, lY;
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;

//the physics engine
PhysicsEngine engine = PhysicsEngine();
GLUquadric *glu = gluNewQuadric();
Cube cube1(glu, 2, 1, 0, 4, 1.4, 1, 0, 1);
Shpere sp1(glu, 1, 1, 0, 0, 0, 1, 0, 0);





Cube cube2(glu, 2, 1, 4, 0, 0, 1, 1, 1);
Shpere sp2(glu, 1, 1, 0, 0, 0, 0, 1, 0);

Shpere sp(glu,2,1,0,0,0,1,0,0);

void camera();
void drawSnowMan();
void computePos(float deltaMove);
void computeDir(float deltaAngle);
void keyboard(int k, int x, int y);

vec3 testForce = vec3(0.0007,0,0);
vec3 virtualGravity = vec3(0, -0.003, 0);
//CollisionInfo CRes(-1, false,vec3(0.0f));

float mv = 0.001;
int res = 0;
float dist = 0.0;
void my_display_code()
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		//if (show_demo_window)
	//	ImGui::ShowDemoWindow(&show_demo_window);


	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	static float f = 0.0f;
	static int counter = 0;
	ImGuiIO& io = ImGui::GetIO();

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.


	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
															//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
	if ( io.KeysDown[119] /*ImGui::Button("  W  ")*/)
		movY += mv;
	if (io.KeysDown[115]  /*ImGui::Button("  S  ")*/)
		movY -= mv;
	if (io.KeysDown[97]  /*ImGui::Button("  A  ")*/)
		movX -= mv;
	if (io.KeysDown[100] /*ImGui::Button("  D  ")*/)
		movX += mv;

	if (io.KeysDown[357]/*ImGui::Button("  Up  ")*/)
		lY += mv;
	if (io.KeysDown[359]/*ImGui::Button("  Down ")*/)
		lY -= mv;
	if (io.KeysDown[356]/*ImGui::Button("  Left  ")*/)
		lX += mv;
	if (io.KeysDown[358]/*ImGui::Button("  Right  ")*/)
		lX -= mv;
	if (io.KeysDown[120]/*the X key*/)
		movZ -= mv;
	if(io.KeysDown[122]/*the Z key*/)
		movZ += mv;

	/*
	ImGui::Text("MovX %f  MovY %f  MovZ %f", movX, movY, movZ);
	ImGui::Text("Pitch  %f  Yaw %f  Roll %f", cube.getPitch(), cube.getYaw(), cube.getRoll());
	ImGui::Text("mv rate %f", mv);
*/
	ImGui::Text("the Collision detection result %d", res);
//	vec3 p = CRes.getCollisionPoint();
//	ImGui::Text("The Collision Inforamtion dist :: %f , Is Collision  %d , point %f , %f , %f",CRes.getDist(),CRes.getIsCollision(),p.x,p.y,p.z );
	ImGui::Text("cube speed %f  %f  %f   shpere speed  %f  %f  %f ", cube1.getSpeed().x , cube1.getSpeed().y, cube1.getSpeed().z, sp1.getSpeed().x, sp1.getSpeed().y, sp1.getSpeed().z);
	ImGui::Text("pitch  %f , yaw %f , roll %f",cube1.getPitch(),cube1.getYaw(),cube1.getRoll());
	ImGui::SliderFloat("camera speed", &mv, 0.0f, 1.0f);   
	
	// Edit 1 float using a slider from 0.0f to 1.0f

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	glPushMatrix();
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BITS);
		glScaled(0.1, 0.1, 0.1);
	
		
	/*	
     	dist = 	engine.SqDistPointToOBB(cube1.getPostion(),cube2.getOBB());

		//cube1.simulateRotation(vec3(1, 1,0), testForce);
		//cube2.simulateRotation(vec3(1, 1, 0), testForce);
		testForce = vec3(0.0f);

		cube1.applyForce(virtualGravity,cube1.getPostion());
		cube1.Integrate();

		virtualGravity = vec3(0.0f);

		cube2.draw_3D();
		cube1.draw_3D();
		
		 res = engine.TestOBB(cube1.getOBB(), cube2.getOBB());
		*/
		cube1.applyForce(virtualGravity,vec3(0.0f));
	//	sp1.applyForce(-virtualGravity,sp1.getPostion());
		cube1.Integrate();
		sp1.Integrate();
		virtualGravity = vec3(0.0f);
	//	int re = engine.TestOBB(sp1.getOBB(), cube1.getOBB());
		CollisionInfo CRes = engine.ShepreAndOBB(sp1,cube1.getOBB());
		if (CRes.getIsCollision())
		{
			vec3 j = engine.J(sp1, cube1, CRes);
		//	j.x = -j.x;
			//j = 2.0f*j;
			//vec3 n = glm::normalize(CRes.getCollisionPoint());
			vec3 positiveJ = j * CRes.getNormal();
			vec3 minesJ = j*(- CRes.getNormal());
			cube1.applyForce(minesJ,CRes.getCollisionPoint());
			sp1.applyForce(positiveJ, CRes.getCollisionPoint());
			//cube1.Integrate();

		//	sp1.Integrate();
			
			//cube1.simulateRotation(CRes.getCollisionPoint(), j);
			//sp1.applyForce(vec3(0.0f, -0.001f, 0.0f), sp1.getPostion());
			//sp1.simulateRotation(CRes.getCollisionPoint(), -j);
		}
		
		sp1.draw_3D();
		cube1.draw_3D();
	}
	glPopMatrix();
}

void glut_display_func()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();
	camera();
	my_display_code();
	// Rendering
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
//	std::cout<<io.KeysDown[0];
//	std::cout << io.WantCaptureKeyboard;
//	if (io.WantCaptureKeyboard)
	//	io.WantCaptureKeyboard = false;
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	glutSwapBuffers();
	glutPostRedisplay();
}

// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

int main(int argc, char** argv)
{
	// Create GLUT window
	glutInit(&argc, argv);
#ifdef __FREEGLUT_EXT_H__
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(1200, 720);
	glutCreateWindow("Dear ImGui GLUT+OpenGL2 Example");


	// Setup GLUT display function
	// We will also call ImGui_ImplGLUT_InstallFuncs() to get all the other functions installed for us,
	// otherwise it is possible to install our own functions and call the imgui_impl_glut.h functions ourselves.
	glutDisplayFunc(glut_display_func);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutIdleFunc(glut_display_func);
	//glutKeyboardFunc(keyboard);
	//glutSpecialFunc(keyboard);

	//// Setup Dear ImGui context
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
//	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
														   //// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	////ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGLUT_Init();
	ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();



	//scale the cordinates
	glScaled(0.1, 0.1, 0.1);
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

	//Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();

	return 0;
}
void reshape(int w, int h)
{

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	// Set the correct perspective.
	glMatrixMode(GL_MODELVIEW);

}
void timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(1, timer, 0);
}
/*
 void computePos(float deltaMove) {
 x += deltaMove * lx * 0.1f;
 z += deltaMove * lz * 0.1f;
 }

 void computeDir(float deltaAngle) {

 angle += deltaAngle;
 lx = sin(angle);
 lz = -cos(angle);
 }*/
void camera() {
	//if (deltaMove)
	//	computePos(deltaMove);
	//if (deltaAngle)
	//	computeDir(deltaAngle);
	//
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();


	// Reset transformations
	//glLoadIdentity();
	// Set the camera
	gluLookAt(movX, movY, movZ, lX, lY, 1, 0, 1, 0);

	//glutSolidCube(3);
}
void drawSnowMan()
{
	//the snow man
	// Draw Body
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);

	// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f, 20, 20);

	// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f, 10, 10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f, 10, 10);
	glPopMatrix();

	// Draw Nose
	glColor3f(1.0f, 0.5f, 0.5f);
	glutSolidCone(0.08f, 0.5f, 10, 2);
	//glutWireCone(0.08f, 0.5f, 10, 2);
}
void keyboard(int k, int x, int y)
{
	std::cout << k;
}