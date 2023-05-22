#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

#define AST_NUM 10 // Number of asteroids.

struct Point3D {
    GLfloat x, y, z;
};

typedef unsigned char Color[3];

///////////////////////////////////////////////// Animation Globals //////////////////////////////////////////////////
static float latAngle = 0.0; // Latitudinal angle.
static int isAnimate = 0; // Animated?
static int animationPeriod = 100; // Time interval between frames.

float speeds[] = {
        0.0, 1, 0.7, 0.6, 0.6, 0.5, 0.1, 0.085, 0.07, 0.065
};

// Timer function.
void animate(int value) {
    if (isAnimate) {
        latAngle += 5.0;
        glutPostRedisplay();
        glutTimerFunc(animationPeriod, animate, 1);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////// Spacecraft Globals /////////////////////////////////////////////////
static int width, height; // Size of the OpenGL window.
static float angle = 0.0; // Angle of the spacecraft.
static float xVal = 0, zVal = 150; // Co-ordinates of the spacecraft.
static unsigned int spacecraft; // Display lists base index.
static int frameCount = 0; // Number of frames

double earthR = 3.0;

const int offset = 15;

double radii[] = {
        earthR * 15.0,
        earthR * 0.5,
        earthR * 0.95,
        earthR,
        earthR * 0.4,
        earthR * 0.53,
        earthR * 5.0,
        earthR * 3.45,
        earthR * 2.00,
        earthR * 1.88,
};

Point3D positions[] = {
        {0.0,                                                            0.0, 0.0},
        {static_cast<GLfloat>(radii[0] + radii[1] + offset),             0.0, 0.0},
        {static_cast<GLfloat>(positions[1].x + radii[2] + offset),       0.0, 0.0},
        {static_cast<GLfloat>(positions[2].x + radii[3] + offset),       0.0, 0.0},
        {static_cast<GLfloat>(positions[2].x + radii[3] + offset),       0.0,
                                                                              static_cast<GLfloat>(radii[3] + radii[4] +
                                                                                                   offset * 0.2)},
        {static_cast<GLfloat>(positions[4].x + radii[5] + offset),       0.0, 0.0},
        {static_cast<GLfloat>(positions[5].x + radii[6] + offset),       0.0, 0.0},
        {static_cast<GLfloat>(positions[6].x + radii[7] + offset * 2),   0.0, 0.0},
        {static_cast<GLfloat>(positions[7].x + radii[8] + offset * 1.5), 0.0, 0.0},
        {static_cast<GLfloat>(positions[8].x + radii[9] + offset * 1.5), 0.0, 0.0},
};

Color colors[] = {
        {255, 165, 0},
        {128, 128, 128},
        {255, 255, 224},
        {25,  155, 225},
        {211, 211, 211},
        {217, 87,  99},
        {239, 223, 173},
        {236, 201, 136},
        {148, 224, 233},
        {68,  110, 212}
};

// Asteroid class.
class Asteroid {
public:
    Asteroid();

    Asteroid(Point3D center, double r, const Color color);

    float getX() const { return center.x; }

    void draw(float speed);

private:
    Point3D center{};
    double radius;
    Color color{};
};

// Asteroid default constructor.
Asteroid::Asteroid() {
    center = {0.0, 0.0, 0.0};
    radius = 0.0; // Indicates no asteroid exists in the position.
    for (unsigned char &val: color) val = 0;
}

// Asteroid constructor.
Asteroid::Asteroid(Point3D center, double r, const Color color) {
    this->center = center;
    radius = r;
    for (int i = 0; i < 3; ++i) this->color[i] = color[i];
}

int saturn = 0;
int sun = 0;
int plan = 0;
int moon = 0;

// Function to draw asteroid.
void Asteroid::draw(float speed) {
    if (radius > 0.0) // If asteroid exists.
    {
        glPushMatrix();

        // lighting part
        // Turn on OpenGL lighting.
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        if (!sun) {
            // Light property vectors.
            GLfloat sunPosition[] = {0.0, 0.0, 0.0, 1.0};  // Sun Position
            GLfloat lightColor[] = {1.0, 1.0, 1.0, 1.0};  // White light
            glLightfv(GL_LIGHT0, GL_POSITION, sunPosition);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);

            // Material property vectors.
            float matAmbAndDif[] = {color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, 1.0};
            float matSpec[] = {1.0, 1.0, 1, 0, 1.0};
            float matShine[] = {50.0};

            // Material properties of ball.
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
            glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
            glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
        } else {
            if (plan) {
                // make the sunlight properly as you are viewing from the spacecraft
                GLfloat spacecraftPosition[] = {xVal, 300.0, zVal, 1.0};  // Spacecraft Position
                GLfloat lightColor[] = {1.0, 1.0, 1.0, 1.0};  // White light
                glLightfv(GL_LIGHT0, GL_POSITION, spacecraftPosition);
                glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
            } else {
                // make the sunlight properly as you are viewing from the spacecraft
                GLfloat spacecraftPosition[] = {xVal, 0.0, zVal, 1.0};  // Spacecraft Position
                GLfloat lightColor[] = {1.0, 1.0, 1.0, 1.0};  // White light
                glLightfv(GL_LIGHT0, GL_POSITION, spacecraftPosition);
                glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
            }
            // Material property vectors.
            float matAmbAndDif[] = {color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, 1.0};
            float matSpec[] = {0.5, 0.3, 0, 0.3, 0.0};
            float matShine[] = {25.0};

            // Material properties of ball.
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
            glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
            glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
        }

        // rotate around the sun
        if (moon) {
            // also rotate around the earth.
            glRotatef(latAngle * speed, 0.0, 1.0, 0.0);
            glRotatef(latAngle * speed * 3, 1.0, 0.0, 0.0);
        } else {
            glRotatef(latAngle * speed, 0.0, 1.0, 0.0);
        }
        if (saturn) {
            // draw the circles around saturn
            glLineWidth(3.0f);
            glRotatef(60, 1.0, 0.0, 0.0);
            glTranslatef(center.x, center.y, center.z);
            for (int i = 1; i < 6; i++) glutWireTorus(i, radii[7] + i, 1, 50);
            glTranslatef(-center.x, -center.y, -center.z);
            glRotatef(-60, 1.0, 0.0, 0.0);
        }
        glTranslatef(center.x, center.y, center.z);
        glLineWidth(2.0f);
        glColor3ubv(color);
        glutSolidSphere(radius, (int) (radius * 6), (int) (radius * 6));

        glPopMatrix();

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
    }
}

Asteroid arrayAsteroids[AST_NUM]; // Global array of asteroids.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<Point3D> stars;
bool generate = true;

// Stars Illusion
void generateStars() {
    stars = {};
    srand(time(nullptr)); // Seed the random number generator with the current time
    for (int i = 0; i < 1000; ++i) {
        stars.push_back({rand() % width - width / 3.0f, rand() % height - height / 3.0f, rand() % height - height / 3.0f});
    }
}

// Routine to count the number of frames drawn every second.
void frameCounter(int value) {
    if (value != 0) // No output the first time frameCounter() is called (from main()).
        std::cout << "FPS = " << frameCount << std::endl;
    frameCount = 0;
    glutTimerFunc(1000, frameCounter, 1);
}

// Initialization routine.
void setup() {
    int j;

    spacecraft = glGenLists(1);
    glNewList(spacecraft, GL_COMPILE);
    glPushMatrix();
    glRotatef(180.0, 0.0, 1.0, 0.0); // To make the spacecraft point down the $z$-axis initially.
    glColor3f(1.0, 1.0, 1.0);
    glutWireCone(5.0, 10.0, 10, 10);
    glPopMatrix();
    glEndList();

    // Initialize global arrayAsteroids.
    for (j = 0; j < AST_NUM; j++)
        arrayAsteroids[j] = Asteroid(positions[j], radii[j], colors[j]);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutTimerFunc(0, frameCounter, 0); // Initial call of frameCounter().
}

// MAIN VIEWPORT
void drawMainViewport() {
    int j;

    // Enable scissor test for the current viewport
    glEnable(GL_SCISSOR_TEST);

    glScissor(0, 0, width - (width / 3), height);

    // Clear the color and depth buffers within the scissor rectangle
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Disable scissor test after drawing is complete
    glDisable(GL_SCISSOR_TEST);

    // Begin main viewport.
    glViewport(0, 0, width, height);
    glLoadIdentity();

    // Write text in isolated (i.e., before gluLookAt) translate block.
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos3f(-28.0, 25.0, -30.0);
    glPopMatrix();

    // Locate the camera at the tip of the cone and pointing in the direction of the cone.
    gluLookAt(xVal - 10 * sin((M_PI / 180.0) * angle),
              0.0,
              zVal - 10 * cos((M_PI / 180.0) * angle),
              xVal - 11 * sin((M_PI / 180.0) * angle),
              0.0,
              zVal - 11 * cos((M_PI / 180.0) * angle),
              0.0,
              1.0,
              0.0);

    glBegin(GL_POINTS);
    glPointSize(2.0);
    glColor3f(1.0, 1.0, 1.0); // White color for stars
    for (Point3D star: stars) glVertex3f(star.x, star.y, star.z);
    glEnd();

    // Draw all the asteroids in arrayAsteroids.
    sun = 1;
    for (j = 0; j < AST_NUM; j++) {
        if (j == 4) moon = 1;
        if (j == 7) saturn = 1;
        arrayAsteroids[j].draw(speeds[j]);
        sun = 0;
        moon = 0;
        saturn = 0;
    }
    // End main viewport.
}

// EDGE VIEWPORT
void drawEdgeViewport() {
    int j;

    // Enable scissor test for the current viewport
    glEnable(GL_SCISSOR_TEST);

    glScissor(width - (width / 3), 0, width / 3, height / 3);

    // Clear the color and depth buffers within the scissor rectangle
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Disable scissor test after drawing is complete
    glDisable(GL_SCISSOR_TEST);

    // Start edge viewport.
    glViewport(width - (width / 3), 0, width / 3, height / 3);

    glLoadIdentity();

    // Write text in isolated (i.e., before gluLookAt) translate block.
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos3f(-28.0, 25.0, -30.0);
    glPopMatrix();

    // Draw lines to separate the plan from the elevation.
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3f(-5.0, -5.0, -5.0);
    glVertex3f(-5.0, 5.0, -5.0);
    glVertex3f(5.0, 5.0, -5.0);
    glVertex3f(-5.0, 5.0, -5.0);
    glEnd();

    // Fixed camera.
    gluLookAt(0.0, 250.0, 0.0, 0.0, 150.0, 0.0, 0.0, 0.0, -1.0);

    glBegin(GL_POINTS);
    glPointSize(2.0);
    glColor3f(1.0, 1.0, 1.0); // White color for stars
    for (Point3D star: stars) glVertex3f(star.x, star.y, star.z);
    glEnd();

    sun = 1;
    plan = 1;

    // Draw all the asteroids in arrayAsteroids.
    for (j = 0; j < AST_NUM; j++) {
        if (j == 4) moon = 1;
        if (j == 7) saturn = 1;
        arrayAsteroids[j].draw(speeds[j]);
        sun = 0;
        moon = 0;
        saturn = 0;

        // draw asteroid belt.
        glLineWidth(1.0f);
        glColor3f(1.0, 1.0, 1.0);
        glRotatef(-90, 1, 0, 0);
        glutWireTorus(0, arrayAsteroids[j].getX(), 1, 50);
        glRotatef(90, 1, 0, 0);
    }

    plan = 0;

    // Draw spacecraft.
    glPushMatrix();
    glTranslatef(xVal, 0.0, zVal);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glCallList(spacecraft);
    glPopMatrix();

    // End edge viewport.
}

// Drawing routine.
void drawScene() {
    frameCount++; // Increment number of frames every redraw.

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw stars
    if (generate) generateStars();

    drawMainViewport();

    drawEdgeViewport();

    glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 500.0);
    glMatrixMode(GL_MODELVIEW);

    // Pass the size of the OpenGL window.
    width = w;
    height = h;
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            exit(0);
        case ' ':
            if (isAnimate) isAnimate = 0;
            else {
                isAnimate = 1;
                animate(1);
            }
            break;
        case 'S':
        case 's':
            generate = !generate;
            break;
        case '+':
            if (animationPeriod > 5) animationPeriod -= 5;
            break;
        case '-':
            animationPeriod += 5;
            break;
        default:
            break;
    }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y) {
    float tempxVal = xVal, tempzVal = zVal, tempAngle = angle;

    // Compute next position.
    if (key == GLUT_KEY_LEFT) tempAngle = angle + 5.0;
    if (key == GLUT_KEY_RIGHT) tempAngle = angle - 5.0;
    if (key == GLUT_KEY_UP) {
        tempxVal = xVal - sin(angle * M_PI / 180.0);
        tempzVal = zVal - cos(angle * M_PI / 180.0);
    }
    if (key == GLUT_KEY_DOWN) {
        tempxVal = xVal + sin(angle * M_PI / 180.0);
        tempzVal = zVal + cos(angle * M_PI / 180.0);
    }

    // Angle correction.
    if (tempAngle > 360.0) tempAngle -= 360.0;
    if (tempAngle < 0.0) tempAngle += 360.0;

    // Move spacecraft to next position only if there will not be collision with an asteroid.
    xVal = tempxVal;
    zVal = tempzVal;
    angle = tempAngle;

    glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction() {
    std::cout << "Interactions:" << std::endl;
    std::cout << "\tPress the left/right arrow keys to turn the craft." << std::endl
              << "\tPress the up/down arrow keys to move the craft." << std::endl
              << "\tPress space to toggle between animation on and off." << std::endl
              << "\tPress the +/- keys to speed up/slow down animation." << std::endl
              << "\tPress S/s to stop generating stars." << std::endl;
}

// Main routine.
int main(int argc, char **argv) {
    printInteraction();
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1366, 768);
    glutInitWindowPosition(250, 100);
    glutCreateWindow("Solar System.cpp");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}