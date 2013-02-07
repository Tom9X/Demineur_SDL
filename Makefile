CXX=g++
CXXFLAGS=-Wall
LIBS=`pkg-config --libs sdl SDL_image SDL_ttf`
TARGET=Demineur_SDL

$(TARGET): main.o champ.o
	$(CXX) $^ $(LIBS) -o $@ $(CXXFLAGS)

main.o: main.cpp
	$(CXX) -c $< $(CXXFLAGS)

champ.o: champ.cpp champ.h
	$(CXX) -c $< $(CXXFLAGS) 

clean:
	@rm -f *.o

proper: clean
	@rm -f $(TARGET)

run: $(TARGET)
	@./$(TARGET)
