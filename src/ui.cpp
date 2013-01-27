#include "ui.h"
#include "config.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

static UI* currui = NULL;

void UI::drawScene() {
        ticks start = getticks();
        _scene.draw4(_accel, _canvas);
        ticks end = getticks();
        INFO( "Cycles per ray " << (end - start) / _scene._rayCount.val);
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

        //glutKeyboardFunc (doKeyboard);
        glutDisplayFunc (glutDrawScene);
        glutReshapeFunc (glutResize);
        glutIdleFunc(glutDrawScene);

        glutMainLoop();
}
