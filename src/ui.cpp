#include "ui.h"
#include "config.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

static UI* currui = NULL;

void UI::drawScene() {
        _renderer(_canvas);
}

void UI::renderCanvas() {
        static char* pixels = NULL;
        if (pixels == NULL)
                pixels = new char[_canvas.getHeight() * _canvas.getWidth() *4];
        int idx = 0;

        for (int i = 0; i < _canvas.getHeight(); ++i) {
		for (int j = 0; j < _canvas.getWidth(); ++j) {
			color& c = _canvas.get(i, j);
                        pixels[idx++] = 255 * c.getRed();
                        pixels[idx++] = 255 * c.getGreen();
                        pixels[idx++] = 255 * c.getBlue();
                        pixels[idx++] = 0;
		}
	}

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //Without this image is upside down
        glPixelZoom(1.0f, -1.0f);
        glRasterPos2f(-1,1.0);
        glDrawPixels(_canvas.getWidth()
                     , _canvas.getHeight()
                     , GL_RGBA
                     , GL_UNSIGNED_BYTE
                     , pixels);

        glutSwapBuffers();
}

static void glutDrawScene() {
        currui->drawScene();
        currui->renderCanvas();
}

static void glutResize (int w, int h)
{
/*
        //prevent bad aspect ratios
        glViewport( 0, 0, w, h );
        static float eye = 100.0;
        static float near = 90.0;
        static float far = 100.0;
        static float bord = 5.0;
        float eye_pos[] = { 0, 0, eye};
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-bord, bord, -bord, bord, near, far);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt( eye_pos[0], eye_pos[1], eye_pos[2], 0, 0 , 0, 0, -1, 0 );
        glutPostRedisplay ();
*/
}

static void keyboardFunc(unsigned char k, int, int) {
        switch(k) {
        case 'q': ::exit(0); break;
        }
}

static void specialFunc(int k, int, int)
{
        camera& camera = currui->getCamera();
        switch (k) {
        case GLUT_KEY_UP : camera.setAngle(camera.getAngle() * 5/6); break;
        case GLUT_KEY_DOWN : camera.setAngle(camera.getAngle() * 6/5); break;
/*
        case GLUT_KEY_LEFT      : g_camSpace = AffineSpace3f::rotate(g_camSpace.p,g_camUp,-0.01f) * g_camSpace; break;
        case GLUT_KEY_RIGHT     : g_camSpace = AffineSpace3f::rotate(g_camSpace.p,g_camUp,+0.01f) * g_camSpace; break;
        case GLUT_KEY_UP        : g_camSpace = g_camSpace * AffineSpace3f::translate(Vec3f(0,0,g_speed)); break;
        case GLUT_KEY_DOWN      : g_camSpace = g_camSpace * AffineSpace3f::translate(Vec3f(0,0,-g_speed)); break;
        case GLUT_KEY_PAGE_UP   : g_speed *= 1.2f; std::cout << "speed = " << g_speed << std::endl; break;
        case GLUT_KEY_PAGE_DOWN : g_speed /= 1.2f; std::cout << "speed = " << g_speed << std::endl; break;
*/
        }
}


void UI::draw() {
        if (!_usegl) {
                drawScene();
                _canvas.save("image.png");
                return;
        }

        int argc = 0;
        char **argv = NULL;
        currui = this;

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize (_canvas.getWidth(), _canvas.getHeight());
        glutCreateWindow (PACKAGE_STRING);

        glutKeyboardFunc(keyboardFunc);
        glutSpecialFunc(specialFunc);
        glutDisplayFunc(glutDrawScene);
        glutReshapeFunc(glutResize);
        glutIdleFunc(glutDrawScene);

        glutMainLoop();
}
