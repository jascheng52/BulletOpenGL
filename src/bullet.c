
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

double lastTime = 0;
double prevTime = 0;
double deltaTime = 0;
ENTITY *player;

int main(int argc, char *argv[])
{

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
    player = ENTITY_create(TYPE_PLAY_MAIN, playerVerts, 4, 30,0,0, 0);
    player->hp = 100;
    
    // vec2 *otherVerts = malloc(sizeof(vec2) * 4);
    // memcpy(otherVerts,squareEntityCords,8 * sizeof(float));
    // ENTITY *other = ENTITY_create(TYPE_ENEMY, otherVerts, 4, 50, 0,150, 0);
    // other->hp = 1000;

    ENTITY_eListInit(DEF_MAX_ENTITY);
    ENTITY_eListAdd(player);
    // ENTITY_eListAdd(other);

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

    while(!glfwWindowShouldClose(window))
    {
        double temp = lastTime;
        lastTime = glfwGetTime();
        deltaTime = lastTime - prevTime;
        prevTime = temp;
            
        glClearColor(1,1,1,1.0);
        glClear(GL_COLOR_BUFFER_BIT);



        userInput(window,player);
        // glBindVertexArray(sqrEntityVAO);
        // SHADER_use(squareEntityShader);
        // if(ENTITY_collide(player,other))
        // {
        //     SHADER_setVec3(squareEntityShader,"rgbColor",(vec3){0,1,0});
        // }
        // else
        // {
        //     SHADER_setVec3(squareEntityShader,"rgbColor",(vec3){1,0,0});
        // }
        // SHADER_setFloat(squareEntityShader,"windWidth",windWidth);
        // SHADER_setFloat(squareEntityShader,"windHeight",windHeight);
        
        // SHADER_setFloat(squareEntityShader,"xPos",player->xPos);
        // SHADER_setFloat(squareEntityShader,"yPos",player->yPos);
        // SHADER_setFloat(squareEntityShader, "scale", player->scale);
        // SHADER_setMat2(squareEntityShader, "rotMat", player->rotMat);
        // printf("Matrix\n");
        // glm_mat2_print(player->rotMat,stdout);
        // printf("degrees: %lf\n",player->degree);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
        // SHADER_setFloat(squareEntityShader,"xPos",other->xPos);
        // SHADER_setFloat(squareEntityShader,"yPos",other->yPos);
        // SHADER_setFloat(squareEntityShader, "scale", other->scale);
        // SHADER_setMat2(squareEntityShader, "rotMat", other->rotMat);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

        updateProj();
        // printf("%lld, %lf\n",eListSize, deltaTime);
        glBindVertexArray(sqrEntityVAO);
        SHADER_use(squareEntityShader);

        for(size_t i = 0; i < eListSize; i++)
        {
            SHADER_setFloat(squareEntityShader,"windWidth",windWidth);
            SHADER_setFloat(squareEntityShader,"windHeight",windHeight);
            SHADER_setVec3(squareEntityShader,"rgbColor",(vec3){1,0,0});    
            switch (eList[i]->type)
            {
                case TYPE_PLAY_MAIN:
                    // if(ENTITY_collide(player,other))
                    // {
                    //     SHADER_setVec3(squareEntityShader,"rgbColor",(vec3){0,1,0});
                        
                    // }
                    break;
                case TYPE_PLAY_PROJ:
                    // printf("Looping e with degree %f\n", eList[i]->degree);
                    // printf("LOCATED");
                    // ENTITY_printLoc(eList[i]);
                    // if(ENTITY_collide(eList[i],other))
                    // {
                    //     other->hp = other->hp - 10;
                    //     // printf("Other hit. New HP: %d\n", other->hp);
                    //     ENTITY_eListDelete(i);
                    //     i--;
                    //     continue;
                    // }
                    break;
                case TYPE_ENEMY:
                    if(eList[i]->hp == 0)
                    {
                        ENTITY_eListDelete(i);
                        // other = NULL;
                        i--;
                        continue;
                    }
                    break;
                default:
                    break;
            }
            SHADER_setFloat(squareEntityShader,"xPos",eList[i]->xPos);
            SHADER_setFloat(squareEntityShader,"yPos",eList[i]->yPos);
            SHADER_setFloat(squareEntityShader, "scale", eList[i]->scale);
            SHADER_setMat2(squareEntityShader, "rotMat", eList[i]->rotMat);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
        }
  

        SHADER_use(lineShader);
        glBindVertexArray(lineVAO);
        SHADER_setFloat(lineShader,"windWidth",windWidth);
        SHADER_setFloat(lineShader,"windHeight",windHeight);
        SHADER_setFloat(lineShader,"xOffSet",player->xPos);
        SHADER_setFloat(lineShader,"yOffSet",player->yPos );
        
        // glm_vec2_print(player->direction,stdout);
        SHADER_setVec3(lineShader,"rgbColor",(vec3){0,0,1});
        SHADER_setMat2(lineShader,"rotMatrix",player->rotMat);
        glDrawArrays(GL_LINES, 0,2);

        // ENTITY_updateDeg(player,(player->degree)+5 *deltaTime);
        // printf("%lf\n", player->degree);
        glfwPollEvents();
        glfwSwapBuffers(window);
        printf("%f\n", deltaTime);
    }
    return 0;
}



void userInput(GLFWwindow *window , ENTITY *player)
{
    float step = 200 * deltaTime;
    // printf("%f\n", deltaTime);
    if(glfwGetKey(window, GLFW_KEY_W))
    {
        LAST_W = lastTime;
        player->yPos = player->yPos + step;
        if(player->yPos > (windHeight/2-30))
        {
            player->yPos = (windHeight/2-30);
        } 
    }
    if(glfwGetKey(window, GLFW_KEY_S))
    {
        LAST_S = lastTime;
        player->yPos = player->yPos - step;
        if(player->yPos < -(windHeight/2-30))
            player->yPos = -(windHeight/2-30);
        
    }
    if(glfwGetKey(window, GLFW_KEY_A))
    {
        LAST_A = lastTime;
        player->xPos = player->xPos - step;
        if(player->xPos < -(windWidth/2-30))
            player->xPos = -(windWidth/2-30);
    }
    if(glfwGetKey(window, GLFW_KEY_D))
    {
        LAST_D = lastTime;
        player->xPos = player->xPos + step;
        if(player->xPos > (windWidth/2-30))
            player->xPos = (windWidth/2-30);
    }
    
    if(glfwGetKey(window, GLFW_KEY_SPACE))
    {
        if(lastTime - LAST_SPACE >= KEY_LOCK_OUT*deltaTime)
        {
            LAST_SPACE = lastTime;
            // ATTACKS_singleStraight(player,squareEntityCords,4,player->scale/2,200);
            // ATTACKS_spreadShot(player,squareEntityCords,4, player->scale/4,200,120,30);
            ATTACKS_radiusShot(player,squareEntityCords,4, player->scale/4,200,180,30,200);
        }
    }

    
}

void gameTick()
{
    
}

void updateProj()
{
    // int rn = rand();
    for (size_t i = 0; i < eListSize; i++)
    {
        ENTITY *e = eList[i];
        if(e->type == TYPE_PLAY_MAIN || e->type == TYPE_ENEMY) 
            continue;
        // if(rn %1000 < 5)
        //     ENTITY_updateDeg(e,e->degree + 40);
        e->xPos = e->velocity * e->direction[0] * deltaTime + e->xPos;        
        e->yPos = e->velocity * e->direction[1] * deltaTime + e->yPos;
    }
}



