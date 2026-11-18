//AUTOR: Iva Bojanic AI 14/2022
//Zadatak: Brod


#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "model.hpp"

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);


const unsigned int wWidth = 900;
const unsigned int wHeight = 600;

glm::vec3 cameraPos = glm::vec3(0.0f, -1.0f, -2.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -1.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float cameraSpeed = 0.01f;

float kameraX = 0.0f;
float kameraY = -1.0f;
float kameraZ = -2.0f;
float kameraFX = 0.0f;
float kameraFZ = 0.0f;


float modelX = -0.19f;
float modelY = -4.90f;
float modelZ = 13.0f;

float modelSX = 0.0f;
float modelSY = -16.99f;
float modelSZ = 10.0f;

float brzinaBroda;
float rotationAngle = 0.0f;


int main()
{
    if(!glfwInit())
    {
        std::cout << "GLFW fail!\n" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(wWidth, wHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Window fail!\n" << std::endl;
        glfwTerminate();
        return -2;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() !=GLEW_OK)
    {
        std::cout << "GLEW fail! :(\n" << std::endl;
        return -3;
    }

    //ucitavanje modela
    Model more("res/more.obj");
    Model santa("res/santa.obj");
    Model svetionik("res/svetionik.obj");
    Model brod("res/brod.obj");
    Model talasi("res/talasi.obj");
    Model talasi1("res/talasi1.obj");
    Model staklo("res/staklo1.obj");

    Shader unifiedShader("basic.vert", "basic.frag");
    Shader lightingShader("lightShader.vert", "lightShader.frag");
    Shader phongShader("phong.vert", "phong.frag");
    

    //render petlja
    unifiedShader.use();
    unifiedShader.setVec3("uLightPos", 0, 1, 3);
    unifiedShader.setVec3("uViewPos", 0, 0, 5);
    unifiedShader.setVec3("uLightColor", 1, 1, 1);


    //perspektivna projekcija
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f); 
    unifiedShader.setMat4("uP", projection);
    glm::mat4 view = glm::lookAt(cameraPos, cameraFront, cameraUp);
    unifiedShader.setMat4("uV", view);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    while (!glfwWindowShouldClose(window))
    {

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

        //kamera
        
            glm::mat4 view = glm::lookAt(cameraPos, cameraFront, cameraUp);
            unifiedShader.setMat4("view", view);
    
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {

                kameraX += 0.01f;

                if (kameraX > 10.0f)
                    kameraX = 10.0f;
                view = glm::lookAt(glm::vec3(kameraX, kameraY, -2.0f), cameraFront, cameraUp);
                unifiedShader.setMat4("uV", view);
               
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {

                kameraX -= 0.01f;
                if (kameraX < -10.0f)
                    kameraX = -10.0f;
                view = glm::lookAt(glm::vec3(kameraX, kameraY, -2.0f), cameraFront, cameraUp);
                unifiedShader.setMat4("uV", view);

            }


        //more
        glm::mat4 modelMora = glm::mat4(1.0f);
        unifiedShader.setMat4("uM", modelMora);
        more.Draw(unifiedShader);

        //talasi
        glm::mat4 modelTalasi = glm::mat4(1.0f);
        unifiedShader.setMat4("uM", modelTalasi);
        talasi.Draw(unifiedShader);

        glm::mat4 modelTalasi1 = glm::mat4(1.0f);
        unifiedShader.setMat4("uM", modelTalasi1);
        talasi1.Draw(unifiedShader);

        //santa leda
        glm::mat4 modelSante = glm::mat4(1.0f);
        unifiedShader.setMat4("uM", modelSante);
        santa.Draw(unifiedShader);

        //svetionik
        glm::mat4 modelSvetionik = glm::mat4(1.0f);
        unifiedShader.setMat4("uM", modelSvetionik);
        svetionik.Draw(unifiedShader);

        //staklo ispred broda
        glm::mat4 modelStaklo = glm::mat4(1.0f);
        unifiedShader.use();
        //brod
        glm::mat4 modelBrod = glm::mat4(1.0f);
        //transformacije broda

        //podesavanje brzine
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            brzinaBroda = 0.01f;
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            brzinaBroda = 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            brzinaBroda = 1.0f;
        }
        
        //kretanja broda
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
              modelZ += brzinaBroda;
              kameraZ += brzinaBroda;
              kameraFZ += brzinaBroda;
              modelSZ += brzinaBroda;

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
              modelX -= brzinaBroda;
              kameraX -= brzinaBroda;
              kameraFX -= brzinaBroda;
              modelSX -= brzinaBroda;
            }

            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
              modelX += brzinaBroda;
              kameraX += brzinaBroda;
              kameraFX += brzinaBroda;
              modelSX += brzinaBroda;
            }

        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            modelZ -= brzinaBroda;
            kameraZ -= brzinaBroda;
            kameraFZ - brzinaBroda;
            modelSZ -= brzinaBroda;
            
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                modelX -= brzinaBroda;
                kameraX -= brzinaBroda;
                kameraFX -= brzinaBroda;
                modelSX -= brzinaBroda;
            }

            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                modelX += brzinaBroda;
                kameraX += brzinaBroda;
                kameraFX += brzinaBroda;
                modelSX += brzinaBroda;
            }

        }

        //printf(" %.2f", modelZ);
        
        modelStaklo = glm::translate(modelStaklo, glm::vec3(modelSX, -16.99f, modelSZ));
        modelBrod = glm::translate(modelBrod, glm::vec3(modelX, -4.90f, modelZ));
        cameraPos = glm::vec3(0.0f, -1.0f, kameraZ);
        cameraFront = glm::vec3(0.0f, -1.0f, kameraFZ);

        
        //udarac u santu leda
        if (modelZ >= -70.0f && modelZ<=-65.0f)
        {
            if (modelX >= 30.0f && modelX <= 60.0f)
            {
                glfwSetWindowShouldClose(window, true);
            }
        }

        //udarac u svetionik
        if (modelZ >= 40.0f && modelZ <= 45.0f)
        {
            if (modelX >= -30.0f && modelX <= 0.0f)
            {
                glfwSetWindowShouldClose(window, true);
            }
        }

        view = glm::lookAt(cameraPos, cameraFront, cameraUp);
        unifiedShader.setMat4("uV", view);


        unifiedShader.setMat4("uM", modelBrod);
        brod.Draw(unifiedShader);

        unifiedShader.setMat4("uM", modelBrod);
        staklo.Draw(unifiedShader);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
    std::string temp = ss.str();
    const char* sourceCode = temp.c_str();

    int shader = glCreateShader(type);

    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{
    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    program = glCreateProgram();

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}



