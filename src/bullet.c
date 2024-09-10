
#include <glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <bullet.h>
#include <shader.h>
#include <entity.h>
#include <attacks.h>

#include <cglm/call.h>
#include <cglm/util.h>


int windWidth = WINDOWWIDTH;
int windHeight = WINDOWHEIGHT;
float squareEntityCords[8] = 
{
    //Four corners of square clockwise
    //Starting from bottom left
    -1,  -1,
    -1,   1, 
    1,   1, 
    1,  -1
};

int squareEntityOrder[6] =
{
    0 , 1, 3, 
    3 , 1, 2 
};


float arrowEntityCords[8]= 
{
    //Four corners of square clockwise
    //Starting from bottom left
    -1,  -2,
    0,   0.5, 
    1,  -2, 
    0,  -0.5
};

int arrowEntityOrder[6] =
{
    0 , 1, 3, 
    3 , 1, 2 
};

float lineBuffer[4] =
{
    0,0,
    0, 1
};

double LAST_W = 0;
double LAST_A = 0;
double LAST_S = 0;
double LAST_D = 0;
double LAST_SPACE = 0;
vec2 prevPost = {0,0};

double lastTime = 0;
size_t GLOB_GAME_TICK = 0;
ENTITY *player;
ENTITY *other;
LIST_UNORD *E_LIST[2];

int main(int argc, char *argv[])
{

    E_LIST[LIST_ENEM]= LIST_UNORD_create(DEF_MAX_PROJLIST_SIZE);
    E_LIST[LIST_PROJ]= LIST_UNORD_create(DEF_MAX_ENEMYLIST_SIZE);
    if(E_LIST[LIST_ENEM] == NULL || E_LIST[LIST_PROJ] == NULL)
    {
        fprintf(stderr, "Failed initial list setup \n");
        exit(EXIT_FAILURE);
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(windWidth, windHeight, "Bullet Game", NULL, NULL);
    if(window == NULL)
    {
        fprintf(stderr, "Failed To initialize Window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) )
    {
        fprintf(stderr, "Failed to initialize Glad");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    vec2 *playerVerts = malloc(sizeof(vec2) * 4);
    memcpy(playerVerts,squareEntityCords,8 * sizeof(float));
    player = ENTITY_create(TYPE_PLAY_MAIN, playerVerts, 4, 30,0,70, 45);
    glmc_quat_copy(player->pos.rotQuat, player->pos.prevQuat);
    player->hp = 100;
    
    vec2 *otherVerts = malloc(sizeof(vec2) * 4);
    memcpy(otherVerts,squareEntityCords,8 * sizeof(float));
    other = ENTITY_create(TYPE_ENEMY, otherVerts, 4, 50, 0,150, 0);
    glmc_quat_copy(other->pos.rotQuat, other->pos.prevQuat);

    other->hp = 100000;

    E_LIST[LIST_ENEM] = LIST_UNORD_add(E_LIST[LIST_ENEM], other);
    if(E_LIST[LIST_ENEM] == NULL)
    {
        fprintf(stderr, "Error adding to enemy list\n");
        exit(EXIT_FAILURE);
    }
    unsigned int sqrEntityVAO, sqrEntityVBO,sqrEntityEBO;
    glGenVertexArrays(1,&sqrEntityVAO);
    glGenBuffers(1, &sqrEntityVBO);
    glGenBuffers(1, &sqrEntityEBO);
    glBindVertexArray(sqrEntityVAO);
    glBindBuffer(GL_ARRAY_BUFFER,sqrEntityVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareEntityCords) * sizeof(vec2), squareEntityCords, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sqrEntityEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareEntityOrder), squareEntityOrder, GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, 2* sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    unsigned int lineVAO, lineVBO;
    glGenVertexArrays(1,&lineVAO);
    glGenBuffers(1, &lineVBO);
    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER,lineVBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), lineBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, 2* sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    
    SHADER *squareEntityShader = SHADER_build("./shaders/squareEntity.vert", "./shaders/squareEntity.frag");
    if(squareEntityShader == NULL)
        exit(EXIT_FAILURE);

    SHADER *lineShader = SHADER_build("./shaders/line.vert", "./shaders/line.frag");
    if(lineShader == NULL)
        exit(EXIT_FAILURE);

    const float MAX_FPS = DEF_TICK_RATE; 
    const float SKIP_TICK = 1000/MAX_FPS/1000;
    double nextTick = glfwGetTime();
    while(!glfwWindowShouldClose(window))
    {
        lastTime = glfwGetTime();

        glClearColor(1,1,1,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        int loop = 0;
        while(lastTime > nextTick && loop < DEF_MAX_TICK_SKIP)
        {
            userInput(window,player);
            updateEnemy();
            // ENTITY_updateDeg(player,player->pos.degree);
            updateProj();
            nextTick = nextTick + SKIP_TICK;
            GLOB_GAME_TICK++;
            loop++;
        }
        
        float interpolation = (nextTick - lastTime) / SKIP_TICK;
        glBindVertexArray(sqrEntityVAO);
        SHADER_use(squareEntityShader);

        LIST_UNORD *projList = E_LIST[LIST_PROJ];
        for(size_t i = 0; i < E_LIST[LIST_PROJ]->size; i++)
        {
            ENTITY *e = projList->elements[i];
            SHADER_setFloat(squareEntityShader,"windWidth",windWidth);
            SHADER_setFloat(squareEntityShader,"windHeight",windHeight);
            SHADER_setVec3(squareEntityShader,"rgbColor",(vec3){1,0,0});
            if(e->type != TYPE_PLAY_PROJ && e->type != TYPE_ENEMY_PROJ)
            {
                fprintf(stderr,"Critical Error. Non-proj in entity list\n");
                exit(EXIT_FAILURE);
            }    
            SHADER_setFloat(squareEntityShader,"xPos",e->pos.prevXPos + interpolation * (e->pos.xPos - e->pos.prevXPos) );
            SHADER_setFloat(squareEntityShader,"yPos",e->pos.prevYPos + interpolation * (e->pos.yPos - e->pos.prevYPos) );
            // printf("INTER:%f\n", interpolation);
            mat4 rotMat;
            versor q;
            glmc_quat_slerp(e->pos.prevQuat,e->pos.rotQuat,interpolation,q);
            glmc_quat_mat4(q,rotMat);
            SHADER_setFloat(squareEntityShader, "scale", e->pos.scale);
            SHADER_setMat4(squareEntityShader, "rotMat",rotMat);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
        }


        LIST_UNORD *enemyList = E_LIST[LIST_ENEM];
        for(size_t i = 0; i < enemyList->size; i++)
        {
            ENTITY *e = enemyList->elements[i];
            SHADER_setFloat(squareEntityShader,"windWidth",windWidth);
            SHADER_setFloat(squareEntityShader,"windHeight",windHeight);
            SHADER_setVec3(squareEntityShader,"rgbColor",(vec3){1,0,0});
            if(e->type != TYPE_ENEMY)
            {
                fprintf(stderr,"Critical Error. Non-enemy in entity list\n");
                exit(EXIT_FAILURE);
            }    
            SHADER_setFloat(squareEntityShader,"xPos",e->pos.prevXPos + interpolation * (e->pos.xPos - e->pos.prevXPos) );
            SHADER_setFloat(squareEntityShader,"yPos",e->pos.prevYPos + interpolation * (e->pos.yPos - e->pos.prevYPos) );
            // printf("INTER:%f\n", interpolation);
            mat4 rotMat;
            versor q;
            glmc_quat_slerp(e->pos.prevQuat,e->pos.rotQuat,interpolation,q);
            glmc_quat_mat4(q,rotMat);
            SHADER_setFloat(squareEntityShader, "scale", e->pos.scale);
            SHADER_setMat4(squareEntityShader, "rotMat",rotMat);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
        }
        SHADER_setFloat(squareEntityShader,"windWidth",windWidth);
        SHADER_setFloat(squareEntityShader,"windHeight",windHeight);
        SHADER_setVec3(squareEntityShader,"rgbColor",(vec3){1,0,0});
        if(player->type != TYPE_PLAY_MAIN)
        {
            fprintf(stderr,"Critical Error. Not a player\n");
            exit(EXIT_FAILURE);
        }    
        SHADER_setFloat(squareEntityShader,"xPos",player->pos.prevXPos + interpolation * (player->pos.xPos - player->pos.prevXPos) );
        SHADER_setFloat(squareEntityShader,"yPos",player->pos.prevYPos + interpolation * (player->pos.yPos - player->pos.prevYPos) );
        // printf("INTER:%f\n", interpolation);
        mat4 rotMat;
        versor q;
        glmc_quat_slerp(player->pos.prevQuat,player->pos.rotQuat,interpolation,q);
        glmc_quat_mat4(q,rotMat);
        SHADER_setFloat(squareEntityShader, "scale", player->pos.scale);
        SHADER_setMat4(squareEntityShader, "rotMat",rotMat);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    return 0;
}



void userInput(GLFWwindow *window , ENTITY *player)
{   
    // float step = 200 * deltaTime;
    float step = 5; 
    LAST_W = 0;
    LAST_A = 0;
    LAST_S = 0;
    LAST_D = 0;

    player->pos.prevYPos = player->pos.yPos;
    player->pos.prevXPos = player->pos.xPos;

    if(glfwGetKey(window, GLFW_KEY_W))
    {
        LAST_W = 1;
        player->pos.yPos = player->pos.yPos + step;
        if(player->pos.yPos > (windHeight/2-30))
        {
            player->pos.yPos = (windHeight/2-30);
        } 
    }
    if(glfwGetKey(window, GLFW_KEY_S))
    {
        LAST_S = -1;
        player->pos.yPos = player->pos.yPos - step;
        if(player->pos.yPos < -(windHeight/2-30))
            player->pos.yPos = -(windHeight/2-30);
        
    }
    if(glfwGetKey(window, GLFW_KEY_A))
    {
        LAST_A = -1;
        player->pos.xPos = player->pos.xPos - step;
        if(player->pos.xPos < -(windWidth/2-30))
            player->pos.xPos = -(windWidth/2-30);
    }
    if(glfwGetKey(window, GLFW_KEY_D))
    {
        LAST_D = 1;
        player->pos.xPos = player->pos.xPos + step;
        if(player->pos.xPos > (windWidth/2-30))
            player->pos.xPos = (windWidth/2-30);
    }
    
    if(glfwGetKey(window, GLFW_KEY_SPACE))
    {
        if(GLOB_GAME_TICK - LAST_SPACE >= KEY_LOCK_OUT)
        {
            LAST_SPACE = GLOB_GAME_TICK;
            // ATTACKS_singleStraight(player,squareEntityCords,4,player->pos.scale/2,2);
            ATTACKS_spreadShot(player,squareEntityCords,4, player->pos.scale/4,1,120,30);
            // ATTACKS_radiusShot(player,squareEntityCords,4, player->pos.scale/4,1,-180,50,300);
        }
    }

}


void updateProj()
{
    // int rn = rand();
    LIST_UNORD *list = E_LIST[LIST_PROJ];
    for (size_t i = 0; i < list->size ; i++)
    {
        ENTITY *e = list->elements[i];
        if(ENTITY_collide(e,other))
        {
            ENTITY *delE = LIST_UNORD_del(list,i);
            i--;
            ENTITY_delete(delE);
            continue;
        }
        e->pos.prevXPos = e->pos.xPos;
        e->pos.prevYPos = e->pos.yPos;
        // if(e->type == TYPE_PLAY_MAIN || e->type == TYPE_ENEMY) 
        //     continue;
        e->pos.xPos = e->pos.velocity * e->pos.direction[0] + e->pos.xPos;        
        e->pos.yPos = e->pos.velocity * e->pos.direction[1] + e->pos.yPos;
    }
}

void updateEnemy()
{
    LIST_UNORD *list = E_LIST[LIST_PROJ];
    for(size_t i = 0; i < list->size; i++)
    {

    }
}

void drawProj(float interpolation)
{
    
}


