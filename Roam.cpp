#include "LoadObject.h"
#include "BMPLoader.h"
#include <gl/glut.h>
#include <iostream>
#include <vector>

struct XYZ {
	float x;
	float y;
	float z;
	}angle = { 0 , 0 , 0 },
		player_pos = { 0, -5, 0 },
		player_speed,
		camera;
	bool KeyBuffer[256];
	bool AutoRotate;
	POINT cp1, cp2;
	float mouse_acceleration = 1;
	float speed = 0.04;
	float fov;
	bool scopeOn;
	bool Paused;
	bool TPP;
	int jumpCount = 0;

	void KeyCheck ( );
	void Display ( );
	void Timer ( int );
	void KeyDown ( unsigned char, int, int );
	void KeyUp ( unsigned char, int, int );
	void Mouse ( int, int, int, int );
	void Drawings2D ( );
	void Physics ( );

	std::vector<unsigned int>TextureID;

	void loadTexture ( const char * filename ) {
		BmpLoader bl ( filename );
		TextureID.push_back ( int ( ) );
		glGenTextures ( 1, &TextureID[TextureID.size ( ) - 1] );
		glBindTexture ( GL_TEXTURE_2D, TextureID[TextureID.size ( ) - 1] );
		glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		gluBuild2DMipmaps ( GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
		}

	std::vector <LoadObject> object;

	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat light_specular[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat light_position0[] = { 0.0f, 0.0f, 0.1f, 0.0f };

	const GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	const GLfloat mat_diffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat high_shininess[] = { 128.0f };

	int main ( int argc, char * argv[] ) {

		object.push_back ( LoadObject ( "Objects\\walls.obj", 100 ) );
		object.push_back ( LoadObject ( "Objects\\ladders.obj", 100 ) );
		object.push_back ( LoadObject ( "Objects\\ground.obj", 100 ) );
		object.push_back ( LoadObject ( "Objects\\floor.obj", 100 ) );
		object.push_back ( LoadObject ( "Objects\\player.obj", 100 ) );

		glutInit ( &argc, argv );
		glutInitDisplayMode ( GL_RGB | GL_DOUBLE | GL_DEPTH | GL_ALPHA );
		glutInitWindowPosition ( 360, 225 );
		glutInitWindowSize ( 720, 450 );
		glutCreateWindow ( "" );
		//glutFullScreen ( );
		glutSetCursor ( GLUT_CURSOR_NONE );

		glutDisplayFunc ( Display );
		glutTimerFunc ( 0, Timer, 0 );
		glutKeyboardFunc ( KeyDown );
		glutKeyboardUpFunc ( KeyUp );
		glutMouseFunc ( Mouse );

		glEnable ( GL_DEPTH_TEST );

		glEnable ( GL_LIGHT0 );
		glEnable ( GL_LIGHT1 );
		glEnable ( GL_NORMALIZE );
		glEnable ( GL_COLOR_MATERIAL );
		glEnable(GL_LIGHTING);

		glLightfv ( GL_LIGHT0, GL_AMBIENT, light_ambient );
		glLightfv ( GL_LIGHT0, GL_DIFFUSE, light_diffuse );
		glLightfv ( GL_LIGHT0, GL_SPECULAR, light_specular );
		glLightfv ( GL_LIGHT0, GL_POSITION, light_position0 );

		glMaterialfv ( GL_FRONT, GL_AMBIENT, mat_ambient );
		glMaterialfv ( GL_FRONT, GL_DIFFUSE, mat_diffuse );
		glMaterialfv ( GL_FRONT, GL_SPECULAR, mat_specular );
		glMaterialfv ( GL_FRONT, GL_SHININESS, high_shininess );

		scopeOn = true;
		TPP = true;
		glMatrixMode ( GL_PROJECTION );
		glLoadIdentity ( );
		gluPerspective ( 60.0, 1.6, 0.1, 300 );
		glMatrixMode ( GL_MODELVIEW );
		glClearColor ( 0, 0, 0, 1 );

		loadTexture ( "Textures//exit.bmp" );
		loadTexture ( "Textures//box.bmp" );
		loadTexture ( "Textures//wall.bmp" );
		loadTexture ( "Textures//box.bmp" );
		loadTexture ( "Textures//floor.bmp" );
		loadTexture ( "Textures//floor.bmp" );

		glutMainLoop ( );

		}

	void Display ( ) {
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glLoadIdentity ( );

		if(TPP)glTranslatef ( -1, 0, -3 );

		glPushMatrix ( );
		glRotatef ( angle.y, 1, 0, 0 );
		glRotatef ( angle.x, 0, 1, 0 );
		glTranslatef ( player_pos.x, player_pos.y, player_pos.z );
		glColor3f ( 1, 1, 1 );
		glEnable ( GL_TEXTURE_2D );
		for ( unsigned int o = 0; o < object.size ( )-1; o++ ) {
			glBindTexture ( GL_TEXTURE_2D, TextureID[o + 2] );
			for ( unsigned int i = 0; i < object[o].face.size ( ); i++ ) {
				glBegin ( GL_POLYGON );
				if ( object[o].face[i].xn != NULL )glNormal3f ( object[o].nor[object[o].face[i].xn].x, object[o].nor[object[o].face[i].yn].y, object[o].nor[object[o].face[i].zn].z );
				glTexCoord2f ( 0.0, 0.0 );	glVertex3f ( object[o].ver[object[o].face[i].x].x, object[o].ver[object[o].face[i].x].y, object[o].ver[object[o].face[i].x].z );
				glTexCoord2f ( 0.0, 1.0 );	glVertex3f ( object[o].ver[object[o].face[i].y].x, object[o].ver[object[o].face[i].y].y, object[o].ver[object[o].face[i].y].z );
				glTexCoord2f ( 1.0, 1.0 ); glVertex3f ( object[o].ver[object[o].face[i].z].x, object[o].ver[object[o].face[i].z].y, object[o].ver[object[o].face[i].z].z );
				if ( object[o].face[i].a != NULL ) {
					glTexCoord2f ( 1.0, 0.0 ); glVertex3f ( object[o].ver[object[o].face[i].a].x, object[o].ver[object[o].face[i].a].y, object[o].ver[object[o].face[i].a].z );
					}
				glEnd ( );
				}
			}
		glDisable ( GL_TEXTURE_2D );

		camera.x = light_position0[0] -1;
		camera.y = light_position0[1];
		camera.z = light_position0[2] -3;

		if ( TPP ) {
			glLoadIdentity ( );
			glTranslatef ( camera.x, camera.y, camera.z );
			glRotatef ( angle.y, 1, 0, 0 );
			int o = 4;
			for ( unsigned int i = 0; i < object[o].face.size ( ); i++ ) {
				glBegin ( GL_POLYGON );
				if ( object[o].face[i].xn != NULL )glNormal3f ( object[o].nor[object[o].face[i].xn].x, object[o].nor[object[o].face[i].yn].y, object[o].nor[object[o].face[i].zn].z );
				glTexCoord2f ( 0.0, 0.0 );	glVertex3f ( object[o].ver[object[o].face[i].x].x, object[o].ver[object[o].face[i].x].y, object[o].ver[object[o].face[i].x].z );
				glTexCoord2f ( 0.0, 1.0 );	glVertex3f ( object[o].ver[object[o].face[i].y].x, object[o].ver[object[o].face[i].y].y, object[o].ver[object[o].face[i].y].z );
				glTexCoord2f ( 1.0, 1.0 ); glVertex3f ( object[o].ver[object[o].face[i].z].x, object[o].ver[object[o].face[i].z].y, object[o].ver[object[o].face[i].z].z );
				if ( object[o].face[i].a != NULL ) {
					glTexCoord2f ( 1.0, 0.0 ); glVertex3f ( object[o].ver[object[o].face[i].a].x, object[o].ver[object[o].face[i].a].y, object[o].ver[object[o].face[i].a].z );
					}
				glEnd ( );
				}
			}
		glPopMatrix ( );

		Drawings2D ( );

		glutSwapBuffers ( );
		}

	void Timer ( int ) {
		GetCursorPos ( &cp2 );
		if ( !Paused )Physics ( );
		if ( !Paused )KeyCheck ( );
		glutTimerFunc ( 1000 / 60, Timer, 0 );
		glutPostRedisplay ( );
		if ( !Paused )SetCursorPos ( GetSystemMetrics ( SM_CXSCREEN ) / 2, GetSystemMetrics ( SM_CYSCREEN ) / 2 );
		GetCursorPos ( &cp1 );
		}

	void KeyDown ( unsigned char key, int, int ) {
		if ( key == 'R' && AutoRotate == true )AutoRotate = false;
		if ( key == 'r' && AutoRotate == false )AutoRotate = true;
		if ( key == 27 )Paused ? Paused = false : Paused = true;
		if ( key == 'p' )TPP ? TPP = false : TPP = true;
		if ( key == 32 && jumpCount == 0 )player_speed.y = -0.2;// jumpCount++;
		KeyBuffer[key] = true;
		}

	void KeyUp ( unsigned char key, int, int ) {
		KeyBuffer[key] = false;
		}

	void Mouse ( int button, int state, int x, int y ) {
		bool tpp{};
		if ( button == GLUT_RIGHT_BUTTON && !Paused )
			if ( state == 0 ) {
				tpp = TPP;
				scopeOn = true;
				}
		if ( button == GLUT_RIGHT_BUTTON && !Paused )
			if ( state == 1 ) {
				TPP = tpp;
				fov = 60.0f;
				scopeOn = false;
				glMatrixMode ( GL_PROJECTION );
				glLoadIdentity ( );
				gluPerspective ( fov, 1.6, 0.1, 300 );
				glMatrixMode ( GL_MODELVIEW );
				glClearColor ( 0, 0, 0, 1 );
				mouse_acceleration = 1.0f;
				speed = 0.04;
				}
		if ( Paused ) {
			if ( button == GLUT_LEFT_BUTTON && state == 0 )
				if ( x > GetSystemMetrics ( SM_CXSCREEN ) / 2 - 50 && x < GetSystemMetrics ( SM_CXSCREEN ) / 2 + 50 )
					if ( y > GetSystemMetrics ( SM_CYSCREEN ) / 2 - 25 && y < GetSystemMetrics ( SM_CYSCREEN ) / 2 + 25 )exit ( 0 );
			}
		}

	void Drawings2D ( ) {
		glDisable ( GL_LIGHTING );
		glDisable ( GL_CULL_FACE );
		glDisable ( GL_DEPTH_TEST );
		glMatrixMode ( GL_PROJECTION );
		glPushMatrix ( );
		glLoadIdentity ( );
		gluOrtho2D ( -720, 720, -450, 450 );
		glMatrixMode ( GL_MODELVIEW );
		glPushMatrix ( );
		glLoadIdentity ( );

		glEnable ( GL_TEXTURE_2D );
		glBindTexture ( GL_TEXTURE_2D, TextureID[1] );
		glColor3f ( 1, 1, 1 );
		glBegin ( GL_QUADS );
		glTexCoord2f ( 0.0, 0.0 );		glVertex2f ( -700, 300 );
		glTexCoord2f ( 1.0, 0.0 );		glVertex2f ( -600, 300 );
		glTexCoord2f ( 1.0, 1.0 );		glVertex2f ( -600, 400 );
		glTexCoord2f ( 0.0, 1.0 );		glVertex2f ( -700, 400 );
		glEnd ( );
		glDisable ( GL_TEXTURE_2D );

		glColor3f ( 1, 1, 1 );
		glBegin ( GL_LINE_LOOP );
		glVertex2f ( -1, -10 ); glVertex2f ( 1, -10 );
		glVertex2f ( 1, -1 ); glVertex2f ( 10, -1 );
		glVertex2f ( 10, 1 ); glVertex2f ( 1, 1 );
		glVertex2f ( 1, 10 ); glVertex2f ( -1, 10 );
		glVertex2f ( -1, 1 ); glVertex2f ( -10, 1 );
		glVertex2f ( -10, -1 ); glVertex2f ( -1, -1 );
		glEnd ( );

		if ( !Paused ) {
			glColor3f ( 0, 0, 0 );
			glBegin ( GL_LINES );
			glVertex3f ( 0, -9, 0.1 ); glVertex3f ( 0, 9, 0.1 );
			glVertex3f ( -9, 0, 0.1 ); glVertex3f ( 9, 0, 0.1 );
			glEnd ( );
			}

		if ( Paused ) {

			glutSetCursor ( GLUT_CURSOR_INHERIT );
			glEnable ( GL_BLEND );
			glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glColor4f ( 0.5, 0.5, 0.7, 0.6 );
			glBegin ( GL_QUADS );
			glVertex2f ( -720, -360 );
			glVertex2f ( 720, -360 );
			glVertex2f ( 720, 360 );
			glVertex2f ( -720, 360 );
			glEnd ( );
			glDisable ( GL_BLEND );

			glEnable ( GL_TEXTURE_2D );
			glBindTexture ( GL_TEXTURE_2D, TextureID[0] );
			glColor3f ( 1, 1, 1 );
			glBegin ( GL_QUADS );
			glTexCoord2f ( 0.0, 0.0 );	glVertex2f ( -50, -25 );
			glTexCoord2f ( 1.0, 0.0 );	glVertex2f ( 50, -25 );
			glTexCoord2f ( 1.0, 1.0 );	glVertex2f ( 50, 25 );
			glTexCoord2f ( 0.0, 1.0 );	glVertex2f ( -50, 25 );
			glEnd ( );
			glDisable ( GL_TEXTURE_2D );
			} else {
			glutSetCursor ( GLUT_CURSOR_NONE );
			}

		glMatrixMode ( GL_PROJECTION );
		glPopMatrix ( );
		glMatrixMode ( GL_MODELVIEW );
		glPopMatrix ( );

		glEnable ( GL_LIGHTING );
		glEnable ( GL_CULL_FACE );
		glEnable ( GL_DEPTH_TEST );
		}

	void Physics ( ) {
		if ( fov > 7.5 )if ( scopeOn ) {
			TPP = false;
			fov *= 0.8;
			glMatrixMode ( GL_PROJECTION );
			glLoadIdentity ( );
			gluPerspective ( fov, 1.6, 0.1, 600 );
			glMatrixMode ( GL_MODELVIEW );
			glClearColor ( 0, 0, 0, 1 );
			mouse_acceleration = 0.2f;
			speed = 0.02;
			} else TPP = true;
		XYZ old;
		old.x = player_pos.x;
		old.z = player_pos.z;
		old.y = player_pos.y;
		player_speed.x /= 1.1;
		player_speed.y /= 1.1;
		player_speed.z /= 1.1;
		player_pos.x += player_speed.x;
		player_pos.y += player_speed.y;
		player_pos.z += player_speed.z;
		XYZ s, b;
		for ( unsigned int o = 0; o < object.size ( ); o++ ) {
			for ( unsigned int i = 0; i < object[o].face.size ( ); i++ ) {
				s.x = object[o].ver[object[o].face[i].x].x;
				if ( s.x > object[o].ver[object[o].face[i].y].x )s.x = object[o].ver[object[o].face[i].y].x;
				if ( s.x > object[o].ver[object[o].face[i].z].x )s.x = object[o].ver[object[o].face[i].z].x;
				if ( object[o].face[i].a != NULL && s.x > object[o].ver[object[o].face[i].a].x )s.x = object[o].ver[object[o].face[i].a].x;
				s.z = object[o].ver[object[o].face[i].x].z;
				if ( s.z > object[o].ver[object[o].face[i].y].z )s.z = object[o].ver[object[o].face[i].y].z;
				if ( s.z > object[o].ver[object[o].face[i].z].z )s.z = object[o].ver[object[o].face[i].z].z;
				if ( object[o].face[i].a != NULL && s.z > object[o].ver[object[o].face[i].a].z )s.z = object[o].ver[object[o].face[i].a].z;
				s.y = object[o].ver[object[o].face[i].x].y;
				if ( s.y > object[o].ver[object[o].face[i].y].y )s.y = object[o].ver[object[o].face[i].y].y;
				if ( s.y > object[o].ver[object[o].face[i].z].y )s.y = object[o].ver[object[o].face[i].z].y;
				if ( object[o].face[i].a != NULL && s.y > object[o].ver[object[o].face[i].a].y )s.y = object[o].ver[object[o].face[i].a].y;
				b.x = object[o].ver[object[o].face[i].x].x;
				if ( b.x < object[o].ver[object[o].face[i].y].x )b.x = object[o].ver[object[o].face[i].y].x;
				if ( b.x < object[o].ver[object[o].face[i].z].x )b.x = object[o].ver[object[o].face[i].z].x;
				if ( object[o].face[i].a != NULL && b.x < object[o].ver[object[o].face[i].a].x )b.x = object[o].ver[object[o].face[i].a].x;
				b.z = object[o].ver[object[o].face[i].x].z;
				if ( b.z < object[o].ver[object[o].face[i].y].z )b.z = object[o].ver[object[o].face[i].y].z;
				if ( b.z < object[o].ver[object[o].face[i].z].z )b.z = object[o].ver[object[o].face[i].z].z;
				if ( object[o].face[i].a != NULL && b.z < object[o].ver[object[o].face[i].a].z )b.z = object[o].ver[object[o].face[i].a].z;
				b.y = object[o].ver[object[o].face[i].x].y;
				if ( b.y < object[o].ver[object[o].face[i].y].y )b.y = object[o].ver[object[o].face[i].y].y;
				if ( b.y < object[o].ver[object[o].face[i].z].y )b.y = object[o].ver[object[o].face[i].z].y;
				if ( object[o].face[i].a != NULL && b.y < object[o].ver[object[o].face[i].a].y )b.y = object[o].ver[object[o].face[i].a].y;
				/*if ( -player_pos.x > s.x - 0.2 && -player_pos.x < b.x + 0.2 && -player_pos.z > s.z - 0.2 && -player_pos.z < b.z + 0.2 && -player_pos.y > s.y - 0.5 && -player_pos.y < b.y + 4 ) {
					if ( -player_pos.x > s.x - 0.2 && -player_pos.x < b.x + 0.2 ) {
						player_pos.x = old.x;
						player_speed.x = 0;
						}
					if ( -player_pos.z > s.z - 0.2 && -player_pos.z < b.z + 0.2 ) {
						player_pos.z = old.z;
						player_speed.z = 0;
						}
					if ( -player_pos.y > s.y - 0.5 && -player_pos.y < b.y + 4 ) {
						player_pos.y = old.y;
						player_speed.y = 0;
						}
					}*/
				}
			}
		}

	void KeyCheck ( ) {
		angle.x += ( ( cp2.x - cp1.x ) / 30.0f ) * mouse_acceleration;
		angle.y += ( ( cp2.y - cp1.y ) / 30.0f ) * mouse_acceleration;
		if ( angle.y > 90 )angle.y = 90;
		if ( angle.y < -90 )angle.y = -90;
		if ( angle.x > 360 )angle.x = angle.x - 360;
		if ( angle.x < 0 )angle.x = 360 + angle.x;
		if ( AutoRotate ) angle.x += 1;
		// LEFT
		if ( KeyBuffer['a'] && angle.x >= 0.0f && angle.x <= 90.0f ) {
			player_speed.x += speed * ( ( 90 - angle.x ) / 90 );
			player_speed.z += speed * ( ( angle.x ) / 90 );
			}
		if ( KeyBuffer['a'] && angle.x > 90.0f && angle.x <= 180.0f ) {
			player_speed.x -= speed * ( ( angle.x - 90 ) / 90 );
			player_speed.z += speed * ( ( 180 - angle.x ) / 90 );
			}
		if ( KeyBuffer['a'] && angle.x > 180.0f && angle.x <= 270.0f ) {
			player_speed.x -= speed * ( ( 270 - angle.x ) / 90 );
			player_speed.z -= speed * ( ( angle.x - 180 ) / 90 );
			}
		if ( KeyBuffer['a'] && angle.x > 270.0f && angle.x <= 360.0f ) {
			player_speed.x += speed * ( ( angle.x - 270 ) / 90 );
			player_speed.z -= speed * ( ( 360 - angle.x ) / 90 );
			}
		// RIGHT
		if ( KeyBuffer['d'] && angle.x >= 0.0f && angle.x <= 90.0f ) {
			player_speed.x -= speed * ( ( 90 - angle.x ) / 90 );
			player_speed.z -= speed * ( ( angle.x ) / 90 );
			}
		if ( KeyBuffer['d'] && angle.x > 90.0f && angle.x <= 180.0f ) {
			player_speed.x += speed * ( ( angle.x - 90 ) / 90 );
			player_speed.z -= speed * ( ( 180 - angle.x ) / 90 );
			}
		if ( KeyBuffer['d'] && angle.x > 180.0f && angle.x <= 270.0f ) {
			player_speed.x += speed * ( ( 270 - angle.x ) / 90 );
			player_speed.z += speed * ( ( angle.x - 180 ) / 90 );
			}
		if ( KeyBuffer['d'] && angle.x > 270.0f && angle.x <= 360.0f ) {
			player_speed.x -= speed * ( ( angle.x - 270 ) / 90 );
			player_speed.z += speed * ( ( 360 - angle.x ) / 90 );
			}
		// FRONT
		if ( KeyBuffer['w'] && angle.x >= 0.0f && angle.x <= 90.0f ) {
			player_speed.z += speed * ( ( 90 - angle.x ) / 90 );
			player_speed.x -= speed * ( ( angle.x ) / 90 );
			}
		if ( KeyBuffer['w'] && angle.x > 90.0f && angle.x <= 180.0f ) {
			player_speed.z -= speed * ( ( angle.x - 90 ) / 90 );
			player_speed.x -= speed * ( ( 180 - angle.x ) / 90 );
			}
		if ( KeyBuffer['w'] && angle.x > 180.0f && angle.x <= 270.0f ) {
			player_speed.z -= speed * ( ( 270 - angle.x ) / 90 );
			player_speed.x += speed * ( ( angle.x - 180 ) / 90 );
			}
		if ( KeyBuffer['w'] && angle.x > 270.0f && angle.x <= 360.0f ) {
			player_speed.z += speed * ( ( angle.x - 270 ) / 90 );
			player_speed.x += speed * ( ( 360 - angle.x ) / 90 );
			}
		// BACK
		if ( KeyBuffer['s'] && angle.x >= 0.0f && angle.x <= 90.0f ) {
			player_speed.z -= speed * ( ( 90 - angle.x ) / 90 );
			player_speed.x += speed * ( ( angle.x ) / 90 );
			}
		if ( KeyBuffer['s'] && angle.x > 90.0f && angle.x <= 180.0f ) {
			player_speed.z += speed * ( ( angle.x - 90 ) / 90 );
			player_speed.x += speed * ( ( 180 - angle.x ) / 90 );
			}
		if ( KeyBuffer['s'] && angle.x > 180.0f && angle.x <= 270.0f ) {
			player_speed.z += speed * ( ( 270 - angle.x ) / 90 );
			player_speed.x -= speed * ( ( angle.x - 180 ) / 90 );
			}
		if ( KeyBuffer['s'] && angle.x > 270.0f && angle.x <= 360.0f ) {
			player_speed.z -= speed * ( ( angle.x - 270 ) / 90 );
			player_speed.x -= speed * ( ( 360 - angle.x ) / 90 );
			}
		if ( KeyBuffer['e'] )player_speed.y -= speed;
		if ( KeyBuffer['q'] )player_speed.y += speed;
		}
