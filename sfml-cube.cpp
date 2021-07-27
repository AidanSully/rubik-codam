#define GL_SILENCE_DEPRECATION

// #include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
// #include <SFML/System.hpp>
// #include <GL/gl.h>
#include <GL/glu.h>
// #include <GL/glext.h>
// #include <GLUT/glut.h>
#include <cmath>

// BUILDING ____________________________________

// macos
// clang++ sfml-cube.cpp -I /Users/tide-jon/.brew/Cellar/sfml/2.5.1_1/include -L /Users/tide-jon/.brew/Cellar/sfml/2.5.1_1/lib/ -lsfml-window -lsfml-graphics -lsfml-system -lsfml-audio -lsfml-network -framework OpenGL -framework GLUT

// windows 32-bit
// g++ -I C:\Users\Tim\Downloads\SFML-2.5.1\include -L C:\Users\Tim\Downloads\SFML-2.5.1\lib sfml-cube.cpp -lsfml-window -lsfml-graphics -lsfml-system -lsfml-audio -lsfml-network -lopengl32 -lglu32 -std=c++17 -o output.exe

// windows 64-bit (let op verschillende .dll en .a libraries)
// C:\"Program Files"\mingw-w64\x86_64-7.3.0-win32-seh-rt_v5-rev0\mingw64\bin\gcc -I C:\Users\Tim\Downloads\SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit\SFML-2.5.1\include -L C:\Users\Tim\Downloads\SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit\SFML-2.5.1\lib sfml-cube.cpp -lsfml-window -lsfml-graphics -lsfml-system -lsfml-audio -lsfml-network -lopengl32 -lglu32 -lstdc++ -o output.exe

// globals, moeten naar een .hpp file en een class worden
bool rotate;
bool mousePressed;
float angle;
float paused_angle;
float mouse_y;
float mouse_x;
float mouse_yd;
float mouse_xd;
float previous_mouse_y;
float previous_mouse_x;
float y_angle;
float x_angle;

void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW, fH;

	//fH = tan( (fovY / 2) / 180 * pi ) * zNear;
	fH = std::tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;

	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

// veranderen in macros / defines
void setColour(char colour)
{
	switch ((char)colour)
	{
	case 'Y':
		glColor3f(1, 1, 0);
		break;
	case 'B':
		glColor3f(0, 0, 1);
		break;
	case 'R':
		glColor3f(1, 0, 0);
		break;
	case 'G':
		glColor3f(0, 1, 0);
		break;
	case 'O':
		glColor3f(1.0f, 0.5f, 0.0f);
		break;
	case 'W':
		glColor3f(1, 1, 1);
		break;
	case 'X':
		glColor3f(0, 0, 0);
		break;
	default:
		glColor3f(0, 1, 1);
		break;
	}
}

void rotateFace(float x, float y, float z, char face)
{
	float theta = 1.5707963268;
	switch (face)
	{
	case 'B':
		theta *= 2;
		glVertex3f(x * cos(theta) - z * sin(theta), y, x * sin(theta) + z * cos(theta));
		break;
	case 'F':
		glVertex3f(x, y, z);
		break;
	case 'R':
		theta *= -1;
		glVertex3f(x * cos(theta) - z * sin(theta), y, x * sin(theta) + z * cos(theta));
		break;
	case 'L':
		glVertex3f(x * cos(theta) - z * sin(theta), y, x * sin(theta) + z * cos(theta));
		break;
	case 'U':
		theta *= -1;
		glVertex3f(x, y * cos(theta) - z * sin(theta), y * sin(theta) + z * cos(theta));
		break;
	case 'D':
		glVertex3f(x, y * cos(theta) - z * sin(theta), y * sin(theta) + z * cos(theta));
		break;
	}
}

void drawFace(std::string stickers, char face)
{
	for (int i = 0; i < 9; i++)
	{
		setColour(stickers[i]);
		rotateFace(100.f * (float((i % 3) + 1) / 3) - 50, 50 - (100.f * (float((i / 3) + 1) / 3)), 50.f, face);
		rotateFace(100.f * (float((i % 3) + 1) / 3) - 50, 50 - (100.f * (float(i / 3) / 3)), 50.f, face);
		rotateFace(100.f * (float(i % 3) / 3) - 50, 50 - (100.f * (float(i / 3) / 3)), 50.f, face);
		rotateFace(100.f * (float(i % 3) / 3) - 50, 50 - (100.f * (float((i / 3) + 1) / 3)), 50.f, face);
	}
}

void drawLines()
{
	setColour('X');
	std::string face = "BFRLUD";
	for (int j = 0; j < 6; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			rotateFace(-50.5, 50.5 - i * (101.0/3), 50.5, face[j]);
			rotateFace(50.5, 50.5 - i * (101.0/3), 50.5, face[j]);
		}
		for (int i = 0; i < 4; i++)
		{
			rotateFace(-50.5 + i * (101.0/3), 50.5, 50.5, face[j]);
			rotateFace(-50.5 + i * (101.0/3), -50.5, 50.5, face[j]);
		}
	}
}

void reset_values(sf::Clock *clock)
{
	rotate = true;
	mousePressed = false;
	angle = 0.0;
	paused_angle = 0.0;
	mouse_y = 0.0;
	mouse_x = 0.0;
	mouse_yd = 0.0;
	mouse_xd = 0.0;
	previous_mouse_y = 0.0;
	previous_mouse_x = 0.0;
	y_angle = 0.0;
	x_angle = 0.0;
	clock->restart();
}

int main()
{
	sf::ContextSettings settings;

	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 12;
	settings.majorVersion = 4;
	settings.minorVersion = 6;

	// create the window
	sf::Window window(sf::VideoMode(800, 600), "Rubik", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	// activate the window
	window.setActive(true);

	// Create a clock for measuring time elapsed
	sf::Clock clock;

	// initialize vars
	reset_values(&clock);

	//prepare OpenGL surface for HSR
	glClearDepth(1.f);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(3.0);

	//// Setup a perspective projection & Camera position
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	perspectiveGL(90.f, float(window.getSize().x) / window.getSize().y, 1.f, 300.0f); //fov, aspect, zNear, zFar

	// load resources, initialize the OpenGL states, ...

	// run the main loop
	bool running = true;
	while (running)
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Space)
				{
					rotate = 1 - rotate;
					if (rotate == true)
						clock.restart();
					else
						paused_angle = angle;
					break;
				}
				else if (event.key.code == sf::Keyboard::R)
				{
					reset_values(&clock);
					break;
				}
				else if (event.key.code == sf::Keyboard::Escape)
				{
					running = false;
					break;
				}
				else
					break;
			case sf::Event::MouseButtonPressed:
				mousePressed = true;
				previous_mouse_y = event.mouseButton.y;
				previous_mouse_x = event.mouseButton.x;
				break;
			case sf::Event::MouseButtonReleased:
				mousePressed = false;
				break;
			case sf::Event::MouseMoved:
				if (mousePressed)
				{
					mouse_y = event.mouseMove.y;
					mouse_x = event.mouseMove.x;
					mouse_yd = previous_mouse_y - mouse_y;
					mouse_xd = previous_mouse_x - mouse_x;
					y_angle -= mouse_yd;
					x_angle -= mouse_xd;
					previous_mouse_y = event.mouseMove.y;
					previous_mouse_x = event.mouseMove.x;
					printf("mouse y angle: %f\nmouse x angle: %f\nangle: %f\n", y_angle, x_angle, angle);
				}
				break;
			case sf::Event::Closed:
				running = false;
				break;
			case sf::Event::Resized:
				glViewport(0, 0, event.size.width, event.size.height);
				break;
			default:
				break;
			}
		}

		// clear the buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Apply some transformations for the cube
		// glLoadMatrixf(view1.getTransform().getMatrix());
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.f, 0.f, -200.f);
		gluLookAt(0.5, 0.5, 0.5, 0, 0, 0, 0, 1.f, 0);

		if (rotate)
			angle = paused_angle + clock.getElapsedTime().asSeconds();

		// rotate cube from initial angle
		glRotatef(y_angle / 10, 1.f, 0.f, -1.f);
		glRotatef((x_angle / 10) + (angle * 30), 0.f, 1.f, 0.f);

		// draw...
		glBegin(GL_QUADS); //draw some squares

		std::string stickers = "YORBGWYOR";
		drawFace(stickers, 'F');

		stickers = "BBBBBBBBB";
		drawFace(stickers, 'B');

		stickers = "RRRRRRRRR";
		drawFace(stickers, 'R');

		stickers = "WWWWWWWWW";
		drawFace(stickers, 'U');

		stickers = "YYYYYYYYY";
		drawFace(stickers, 'D');

		stickers = "OOOOOOOOO";
		drawFace(stickers, 'L');

		glEnd();

		glBegin(GL_LINES);

		drawLines();

		glEnd();

		// end the current frame (internally swaps the front and back buffers)
		window.display();
	}

	// release resources...

	return 0;
}