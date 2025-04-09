# Nom de l'exécutable
TARGET = pong

# Compilateur
CXX = g++

# Fichiers source
SRC = SDL.cpp simulation.cpp forces.cpp physique.cpp autopilote.cpp

# Fichiers objets (remplace .cpp par .o)
OBJ = $(SRC:.cpp=.o)

# Librairies SDL
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer

# Règle principale
all: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) -c $< -o $@

# Nettoyage
clean:
	rm -f $(OBJ) $(TARGET)
