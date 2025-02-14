#include <iostream>
#include <string>

using namespace std;

class Jeu_ping_pong {
    public:
        int number;
        void sayHello() {
            cout << "Hello, my number is " << number << endl;
        }
};

class table {
    public:
        int largeur = 183;
        int longueur = 274;
        void sayHello() {
            cout << "Hello, my taille is " << largeur << ","<< longueur << endl;
        }
};

int main() {
    return 0;
}

