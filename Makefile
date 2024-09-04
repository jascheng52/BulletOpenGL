CC = gcc
FLAGS = -Wextra -Wall -Wno-unused-parameter -g 
LFLAGS = -lopengl32 -lglfw3 -lgdi32 -lcglm
GLAD = ./glad/glad.c
GLAD_OBJ = $(GLAD:.c=.o) 


TARGET = bullet.exe
INCLUDE = ./include

TEST_ENTITY = test_entity.exe

SRC_FILE = bullet.c
SRC_FOLDER = ./src/
SRC = $(addprefix $(SRC_FOLDER), $(SRC_FILE))
SRC_OBJ = $(SRC:.c=.o)

ENTITY_FILE = entity.c
ENTITY_TEST_FILE = test_entity.c 
ENTITY_FOLDER = $(SRC_FOLDER)entity/
ENTITY = $(addprefix $(ENTITY_FOLDER), $(ENTITY_FILE))
ENTITY_OBJ = $(ENTITY:.c=.o)
ENTITY_TEST_OBJ = $(ENTITY_OBJ) $(addprefix $(ENTITY_FOLDER), $(ENTITY_TEST_FILE))

SHADER_FILE = shader.c
SHADER_FOLDER = ./mod/
SHADER = $(addprefix $(SHADER_FOLDER), $(SHADER_FILE)) 
SHADER_OBJ = $(SHADER:.c=.o)


OBJECTS = $(GLAD_OBJ) $(SRC_OBJ) $(SHADER_OBJ) $(ENTITY_OBJ) 
$(info   OBJECTS is $(OBJECTS))

.PHONY: all clean entity


all: $(TARGET)


$(TARGET) : $(OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)


%.o: %.c
	$(CC) -I$(INCLUDE) -c $< -o $@ $(FLAGS)

clean:
	rm -f bullet.exe 
	rm -f test_entity.exe 
	rm -f $(SRC_FOLDER)/*.o 
	rm -f $(SHADER_FOLDER)/*.o 
	rm -f $(ENTITY_FOLDER)/*.o


entity: $(TEST_ENTITY)

$(TEST_ENTITY) : $(ENTITY_TEST_OBJ)
	$(CC) -I$(INCLUDE) $^ -o $@ $(LFLAGS) -g