
#include <glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <bullet.h>
#include <shader.h>
#include <entity.h>

#include <cglm/call.h>
#include <cglm/util.h>


int windWidth = 600;
int windHeight = 900;



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
    player = ENTITY_create(PLAYER, playerVerts, 4, 30,0,0, 0);


    vec2 *otherVerts = malloc(sizeof(vec2) * 4);
    memcpy(otherVerts,squareEntityCords,8 * sizeof(float));
    ENTITY *other = ENTITY_create(ENEMY, otherVerts, 4, 50, 0,150, 0);

    
    // printf("Player Verts\n");
    // ENTITY_printLoc(player);

    // printf("Other Verts\n");
    // ENTITY_printLoc(other);

    ENTITY_collide(player,other);

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

    
    ENTITY_eListInit(DEF_MAX_ENTITY);
    while(!glfwWindowShouldClose(window))
    {
        double temp = lastTime;
        lastTime = glfwGetTime();
        deltaTime = lastTime - prevTime;
        prevTime = temp;
            
        glClearColor(1,1,1,1.0);
        glClear(GL_COLOR_BUFFER_BIT);



        userInput(window,player);
        glBindVertexArray(sqrEntityVAO);
        SHADER_use(squareEntityShader);
        if(ENTITY_collide(player,other))
        {
            // printf("collide detected\n");
            SHADER_setVec3(squareEntityShader,"rgbColor",(vec3){0,1,0});
        }
        else
        {
            SHADER_setVec3(squareEntityShader,"rgbColor",(vec3){1,0,0});
        }
        SHADER_setFloat(squareEntityShader,"windWidth",windWidth);
        SHADER_setFloat(squareEntityShader,"windHeight",windHeight);
        
        SHADER_setFloat(squareEntityShader,"xPos",player->xPos);
        SHADER_setFloat(squareEntityShader,"yPos",player->yPos);
        SHADER_setFloat(squareEntityShader, "scale", player->scale);
        SHADER_setMat2(squareEntityShader, "rotMat", player->rotMat);
        // printf("Matrix\n");
        // glm_mat2_print(player->rotMat,stdout);
        // printf("degrees: %lf\n",player->degree);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
        SHADER_setFloat(squareEntityShader,"xPos",other->xPos);
        SHADER_setFloat(squareEntityShader,"yPos",other->yPos);
        SHADER_setFloat(squareEntityShader, "scale", other->scale);
        SHADER_setMat2(squareEntityShader, "rotMat", other->rotMat);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

        updateProj();
        // printf("%lld, %lf\n",eListSize, deltaTime);
        for(size_t i = 0; i < eListSize; i++)
        {
            if(ENTITY_collide(eList[i],other))
            {
                ENTITY_eListDelete(i);
                i--;
                continue;
            }
                // SHADER_setVec3(squareEntityShader,"rgbColor",(vec3){0,0,1});
            SHADER_setVec3(squareEntityShader,"rgbColor",(vec3){1,0,0});    
            SHADER_setFloat(squareEntityShader,"xPos",eList[i]->xPos);
            SHADER_setFloat(squareEntityShader,"yPos",eList[i]->yPos);
            SHADER_setFloat(squareEntityShader, "scale", eList[i]->scale);
            SHADER_setMat2(squareEntityShader, "rotMat",  eList[i]->rotMat);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
            // ENTITY_updateDeg(eList[i], eList[i]->degree + deltaTime);
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

        ENTITY_updateDeg(player,(player->degree)+5 *deltaTime);
        // printf("%lf\n", player->degree);
        glfwPollEvents();
        glfwSwapBuffers(window);
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
            vec2 *verts = malloc(sizeof(vec2) * 4);
            memcpy(verts, squareEntityCords, sizeof(vec2) * 4);
            vec2 res;
            ENTITY_vertexDirection(player,res, windHeight,windWidth);
            
            ENTITY *proj = ENTITY_create(PLAYER,verts, 4, 20,
                res[0],res[1],player->degree);
            proj->velocity = 200;
            proj->xPos = proj->xPos + proj->direction[0] *proj->scale/2;
            proj->yPos = proj->yPos + proj->direction[1] *proj->scale/2;

            ENTITY_eListAdd(proj);
        }
    }
}

void updateProj()
{
    int rn = rand();
    for (size_t i = 0; i < eListSize; i++)
    {
        ENTITY *e = eList[i];
        if(rn %1000 < 5)
            ENTITY_updateDeg(e,e->degree + 40);
        e->xPos = e->velocity * e->direction[0] * deltaTime + e->xPos;        
        e->yPos = e->velocity * e->direction[1] * deltaTime + e->yPos;
    }
}

