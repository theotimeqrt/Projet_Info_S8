#include <iostream>
#include <GL/glu.h>
#include <string>
#include "classes.hpp"
#include "fonctions.hpp"
#include <chrono>

const float GRAVITY = 9.81; 

constexpr std::chrono::milliseconds pas_t(1); // pas de 1ms


using namespace std;

// ==================== OBJETS =======================

jeu_ping_pong jeu1;
balle balle1;
table table1;
filet filet1;
raquette raquette1;
raquette raquette2;


// ================== ACCELERATION =======================

// Calcul de la nouvelle acceleration
coo new_a(int masse, coo v, coo spin, float ro) {
    coo a;
    coo ft = force_frottement(v, ro);
    coo fm = force_magnus(v, spin, ro);
    a.x = (ft.x + fm.x) / masse;
    a.y = (ft.y + fm.y) / masse;
    a.z = (ft.z + fm.z - masse * GRAVITY) / masse;
    return a;
}


// ================= EULER ====================
// comparer

// Calcul de la nouvelle vitesse
coo new_v(coo a, coo old_v, float dt) { 

    coo new_v;
    new_v.x = old_v.x +  a.x*dt ;
    new_v.y = old_v.y +  a.y*dt ;
    new_v.z = old_v.z +  a.z*dt ;

    return new_v;
}


// Calcul des nouvelles coordonnées
coo new_coo(coo old_pos, coo v, float dt) { 

    coo new_coo;
    new_coo.x = old_pos.x + v.x * dt;
    new_coo.y = old_pos.y + v.y * dt;
    new_coo.z = old_pos.z + v.z * dt;

    return new_coo;
}

float ballY = 2.0f; // Position de la balle
// ================== GRAPHIQUE 3D =======================
void init() {
    glClearColor(1, 1, 1, 1);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 0.01, 100.0); // Set perspective projection

    glMatrixMode(GL_MODELVIEW); // Switch back to model-view matrix
    glLoadIdentity();

    // Set the camera position
    gluLookAt(0.0f, 0.5f, 5.0f, // Camera position
              0.0f, 0.0f, 0.0f, // Look-at point
              0.0f, 1.0f, 0.0f); // Up direction
}

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightPos[] = {1.0f, 4.0f, 4.0f, 0.0f}; 
    GLfloat lightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f}; 
    GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f}; 
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f}; 

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}

void drawBall() {
    glPushMatrix();
    glTranslatef(0.0f, ballY, 0.0f); // Position de la balle

    GLfloat matAmbient[] = {0.1f, 0.1f, 0.8f, 1.0f}; // Bleu
    GLfloat matDiffuse[] = {0.1f, 0.1f, 0.8f, 1.0f}; 
    GLfloat matSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
    GLfloat matShininess[] = {50.0f}; 

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    glutSolidSphere(0.2f, 20, 20); // Sphère représentant la balle
    glPopMatrix();
}

void drawGroundPlane() {
    GLfloat matAmbient[] = {0.8f, 0.1f, 0.1f, 1.0f}; // Rouge
    GLfloat matDiffuse[] = {0.8f, 0.1f, 0.1f, 1.0f}; 
    GLfloat matSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
    GLfloat matShininess[] = {25.0f}; 

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    glPushMatrix();
    glTranslatef(0, -1.2f, -1.f);
    glScalef(6.0f, 1.f, 6.0f);
    glutSolidCube(0.5f);
    glPopMatrix();
}

void update() {
    float dt = std::chrono::duration_cast<std::chrono::milliseconds>(pas_t).count() / 1000.0f;

    // Mise à jour de l'accélération
    balle1.acceleration_balle = new_a(balle1.masse, balle1.vitesse_balle, balle1.spin, 1.2);

    // Mise à jour de la vitesse
    balle1.vitesse_balle = new_v(balle1.acceleration_balle, balle1.vitesse_balle, dt);

    // Mise à jour de la position
    balle1.centre_balle = new_coo(balle1.centre_balle, balle1.vitesse_balle, dt);

    // Gestion du rebond sur la table
    if (balle1.centre_balle.z - balle1.rayon / 100.0f < table1.centre_table.z) {
        balle1.centre_balle.z = table1.centre_table.z + balle1.rayon / 100.0f;
        balle1.vitesse_balle.z *= -0.7f; // Facteur d'élasticité
    }
}

void timer(int) {
    update();
    glutPostRedisplay(); // Redessiner la scène
    glutTimerFunc(16, timer, 0); // Relancer le timer (60 FPS)
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawGroundPlane();
    drawBall();

    glutSwapBuffers();
}

// ============================= MAIN ===============================


int main(int argc, char **argv) {

    // Paramètres initiaux
    balle1.centre = {0, 0, 2000};   
    balle1.v = {0, 0, 0}; 
    balle1.a = {0, 0, 0};
    balle1.spin = {10, 10, 0};

    // paramètres de  temps
    //float dt = std::chrono::duration_cast<std::chrono::milliseconds>(pas_t).count() / 1000.0; // Pas de temps de 1 ms
    float dt = std::chrono::duration_cast<std::chrono::milliseconds>(pas_t).count() / 1.0 ; // Pas de temps de 1 s
    int simulation_time_ms = 10; // combien de step de simulation

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 800);
    glutCreateWindow("Bouncing Ball");

    init();
    setupLighting();

    glutDisplayFunc(display);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();

    // Afficher les résultats au début
    std::cout << "Début de la simulation:" << std::endl;
    std::cout << "Acceleration initiale: (" << balle1.a.x << ", " << balle1.a.y << ", " <<  balle1.a.z << ")" << std::endl;
    std::cout << "Position initiale: (" << balle1.centre.x << ", " << balle1.centre.y << ", " << balle1.centre.z << ")" << std::endl;
    std::cout << "Vitesse initiale: (" << balle1.v.x << ", " << balle1.v.y << ", " << balle1.v.z << ")" << std::endl;

    // Simulation sur 30ms
    for (int t = 1; t <= simulation_time_ms; ++t) {

        //calculer la nouvelle acceleration
        coo new_acc = new_a(balle1.masse, balle1.v, balle1.spin, 1.2);
        balle1.a = new_acc;


        // Calculer la nouvelle vitesse
        coo new_vi = new_v(balle1.a, balle1.v, dt);
        balle1.v = new_vi;

        // Calculer les nouvelles coordonnées
        coo new_pos = new_coo(balle1.centre, balle1.v, dt);
        balle1.centre = new_pos;
        
        

        // Affichage des résultats
        std::cout << "Temps: " << t << " step" << std::endl;
        std::cout << "Nouvelle position: (" << balle1.centre.x << ", " << balle1.centre.y << ", " << balle1.centre.z << ")" << std::endl;
        std::cout << "Nouvelle vitesse: (" << balle1.centre.x << ", " << balle1.centre.y << ", " << balle1.centre.z << ")" << std::endl;
        std::cout << std::endl;

        
        
    }

    std::cout << "Fin de la simulation." << std::endl;
    return 0;
}

