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

LISTUNORDER_FILE = unorderList.c
LISTUNORDER_TEST_FILE = 
LISTUNORDER_FOLDER = $(SRC_FOLDER)list/
LISTUNORDER = $(addprefix $(LISTUNORDER_FOLDER), $(LISTUNORDER_FILE))
LISTUNORDER_OBJ = $(LISTUNORDER:.c=.o)
LISTUNORDER_TEST= $(LISTUNORDER_OBJ) $(addprefix $(LISTUNORDER_FOLDER), $(LISTUNORDER_FILE))

ATTACKS_FILE = attacks.c
ATTACKS_TEST_FILE = 
ATTACKS_FOLDER = $(SRC_FOLDER)attacks/
ATTACKS = $(addprefix $(ATTACKS_FOLDER), $(ATTACKS_FILE))
ATTACKS_OBJ = $(ATTACKS:.c=.o)
ATTACKS_TEST= $(ATTACKS_OBJ) $(addprefix $(ATTACKS_FOLDER), $(ATTACKS_FILE))

AI_ACTIONS_FILE = ai_actions.c
AI_ACTIONS_TEST_FILE = 
AI_ACTIONS_FOLDER = $(SRC_FOLDER)attacks/
AI_ACTIONS = $(addprefix $(AI_ACTIONS_FOLDER), $(AI_ACTIONS_FILE))
AI_ACTIONS_OBJ = $(AI_ACTIONS:.c=.o)
AI_ACTIONS_TEST= $(AI_ACTIONS_OBJ) $(addprefix $(AI_ACTIONS_FOLDER), $(AI_ACTIONS_FILE))

SHADER_FILE = shader.c
SHADER_FOLDER = ./mod/
SHADER = $(addprefix $(SHADER_FOLDER), $(SHADER_FILE)) 
SHADER_OBJ = $(SHADER:.c=.o)


OBJECTS = $(GLAD_OBJ) $(SRC_OBJ) $(SHADER_OBJ) $(ENTITY_OBJ) \
	$(ATTACKS_OBJ) $(AI_ACTIONS_OBJ) $(LISTUNORDER_OBJ)
$(info   OBJECTS is $(OBJECTS))

.PHONY: all clean entity reset


all: $(TARGET)

reset:
	make clean
	make

$(TARGET) : $(OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)


%.o: %.c
	$(CC) -I$(INCLUDE) -c $< -o $@ $(FLAGS)

clean:
	rm -f bullet.exe 
	rm -f test_entity.exe 
	rm -f $(SRC_FOLDER)*.o 
	rm -f $(SHADER_FOLDER)*.o 
	rm -f $(ENTITY_FOLDER)*.o
	rm -f $(ATTACKS_FOLDER)*.o
	rm -f $(AI_ACTIONS_FOLDER)*.o


entity: $(TEST_ENTITY)

	$(TEST_ENTITY) : $(ENTITY_TEST_OBJ)
		$(CC) -I$(INCLUDE) $^ -o $@ $(LFLAGS) -g


attacks: $(ATTACKS_TEST)
	$(ATTACKS_TEST) : $(ATTACKS_TEST)
		$(CC) -I$(INCLUDE) $^ -o $@ $(LFLAGS) -g

