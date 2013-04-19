INCLUDES=-I. -I$(MOCKVISION_PATH)/ -I$(MOCKVISION_PATH)/Controllers -I$(MOCKVISION_PATH)/EEPROM \
             -I$(MOCKVISION_PATH)/TVout $(addprefix -I./libraries/,$(ARDUINO_LIBS)) \
             -I$(MOCKVISION_PATH)/TVoutfonts
SDL_INCLUDES=-I/usr/include/SDL
LIBS=-lSDL -lSDL_mixer
FLAGS=-Wall -DF_CPU=$(F_CPU)

F_CPU=16000000

OBJECTS=build/beeper.o build/video_gen.o build/TVout.o build/Paddle.o build/ButtonController.o \
		build/EEPROM.o build/TVoutPrint.o
FONTS=font4x6.o font6x8.o font8x8.o font8x8ext.o

$(TARGET): build $(OBJECTS) $(ADDITIONAL_TARGETS) $(FONTS) build/mock_$(TARGET).cpp virtual_EEPROM
	$(CXX) $(ADDITIONAL_TARGETS) $(OBJECTS) $(FONTS) build/mock_$(TARGET).cpp -o $@ $(INCLUDES) $(SDL_INCLUDES) $(LIBS) $(FLAGS)

build:
	mkdir -p build

virtual_EEPROM:
	cp $(MOCKVISION_PATH)/virtual_EEPROM .

# Build additional targets
%.o: %.c
	$(CXX) -c $< -o $@ $(FLAGS) $(INCLUDES)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(FLAGS) $(INCLUDES)

#Build fonts
%.o: $(MOCKVISION_PATH)/TVoutfonts/%.cpp
	$(CXX) -c $< -o $@ $(FLAGS) $(INCLUDES)

# Build a joint .cpp file with the code and some of our definitions
build/mock_%.cpp: $(MOCKVISION_PATH)/arduino.h $(MOCKVISION_PATH)/arduino.cpp %.ino
	cat $^ > $@

build/mock_%.cpp: $(MOCKVISION_PATH)/arduino.h $(MOCKVISION_PATH)/arduino.cpp %.pde
	cat $^ > $@

build/TVout.o: $(MOCKVISION_PATH)/TVout/TVout.cpp
	$(CXX) -o $@ -c $< $(INCLUDES) $(FLAGS) $(SDL_INCLUDES)

build/TVoutPrint.o: $(MOCKVISION_PATH)/TVout/TVoutPrint.cpp
	$(CXX) -o $@ -c $< $(INCLUDES) $(FLAGS) $(SDL_INCLUDES)

build/video_gen.o: $(MOCKVISION_PATH)/TVout/video_gen.cpp
	$(CXX) -o $@ -c $< $(INCLUDES) $(FLAGS) $(SDL_INCLUDES)

build/ButtonController.o: $(MOCKVISION_PATH)/Controllers/ButtonController.cpp
	$(CXX) -o $@ -c $^ $(INCLUDES) $(FLAGS)

build/Paddle.o: $(MOCKVISION_PATH)/Controllers/Paddle.cpp
	$(CXX) -o $@ -c $^ $(INCLUDES) $(FLAGS)

build/EEPROM.o: $(MOCKVISION_PATH)/EEPROM/EEPROM.cpp
	$(CXX) -o $@ -c $^ $(INCLUDES) $(FLAGS)

build/beeper.o: $(MOCKVISION_PATH)/beeper.cpp
	$(CXX) -o $@ -c $^ $(INCLUDES) $(FLAGS)

clean:
	rm $(TARGET)
	rm build/ -rf
	rm virtual_EEPROM