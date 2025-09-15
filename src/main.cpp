#include <iostream>
#include "glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Camera.h"
#include "GameTime.h"

const unsigned windowWidth = 1920;
const unsigned windowHeight =  1080;

glm::mat4 projection = glm::mat4(1.0f);
float lastMouseX = windowWidth / 2.0f;
float lastMouseY = windowHeight / 2.0f;

float deltaTime = 0.0; // time between current and last frame
float lastFrame = 0.0; // marks the last frame

Camera mainCamera = Camera(glm::vec3(0.0f,0.0f,3.0f), -90.0f, 0.0f);

// triangle vertices in NDC (Normalized Device Coordinates x,y,z in [-1,1])
// (0,0,0) is center of screen
// pos            // normal           // uv
float vertices[] = {
    -0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f,  0.0f,0.0f,
    0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f,  1.0f,0.0f,
    0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f,  1.0f,1.0f,
    0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f,  1.0f,1.0f,
    -0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f,  0.0f,1.0f,
    -0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f,  0.0f,0.0f,

    -0.5f,-0.5f, 0.5f,  0.0f,0.0f, 1.0f,  0.0f,0.0f,
    0.5f,-0.5f, 0.5f,  0.0f,0.0f, 1.0f,  1.0f,0.0f,
    0.5f, 0.5f, 0.5f,  0.0f,0.0f, 1.0f,  1.0f,1.0f,
    0.5f, 0.5f, 0.5f,  0.0f,0.0f, 1.0f,  1.0f,1.0f,
    -0.5f, 0.5f, 0.5f,  0.0f,0.0f, 1.0f,  0.0f,1.0f,
    -0.5f,-0.5f, 0.5f,  0.0f,0.0f, 1.0f,  0.0f,0.0f,

    -0.5f, 0.5f, 0.5f, -1.0f,0.0f,0.0f,  0.0f,0.0f,
    -0.5f, 0.5f,-0.5f, -1.0f,0.0f,0.0f,  1.0f,0.0f,
    -0.5f,-0.5f,-0.5f, -1.0f,0.0f,0.0f,  1.0f,1.0f,
    -0.5f,-0.5f,-0.5f, -1.0f,0.0f,0.0f,  1.0f,1.0f,
    -0.5f,-0.5f, 0.5f, -1.0f,0.0f,0.0f,  0.0f,1.0f,
    -0.5f, 0.5f, 0.5f, -1.0f,0.0f,0.0f,  0.0f,0.0f,

    0.5f, 0.5f, 0.5f,  1.0f,0.0f,0.0f,  0.0f,0.0f,
    0.5f, 0.5f,-0.5f,  1.0f,0.0f,0.0f,  1.0f,0.0f,
    0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f,  1.0f,1.0f,
    0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f,  1.0f,1.0f,
    0.5f,-0.5f, 0.5f,  1.0f,0.0f,0.0f,  0.0f,1.0f,
    0.5f, 0.5f, 0.5f,  1.0f,0.0f,0.0f,  0.0f,0.0f,

    -0.5f,-0.5f,-0.5f,  0.0f,-1.0f,0.0f,  0.0f,0.0f,
    0.5f,-0.5f,-0.5f,  0.0f,-1.0f,0.0f,  1.0f,0.0f,
    0.5f,-0.5f, 0.5f,  0.0f,-1.0f,0.0f,  1.0f,1.0f,
    0.5f,-0.5f, 0.5f,  0.0f,-1.0f,0.0f,  1.0f,1.0f,
    -0.5f,-0.5f, 0.5f,  0.0f,-1.0f,0.0f,  0.0f,1.0f,
    -0.5f,-0.5f,-0.5f,  0.0f,-1.0f,0.0f,  0.0f,0.0f,

    -0.5f, 0.5f,-0.5f,  0.0f, 1.0f,0.0f,  0.0f,0.0f,
    0.5f, 0.5f,-0.5f,  0.0f, 1.0f,0.0f,  1.0f,0.0f,
    0.5f, 0.5f, 0.5f,  0.0f, 1.0f,0.0f,  1.0f,1.0f,
    0.5f, 0.5f, 0.5f,  0.0f, 1.0f,0.0f,  1.0f,1.0f,
    -0.5f, 0.5f, 0.5f,  0.0f, 1.0f,0.0f,  0.0f,1.0f,
    -0.5f, 0.5f,-0.5f,  0.0f, 1.0f,0.0f,  0.0f,0.0f,
};


// vertices for a square pyramid (base size 1, height 1)
float pyramidVertices[] = {
    // front face
     0.0f,  1.0f,  0.0f,   // top
    -0.5f,  0.0f,  0.5f,   // front-left
     0.5f,  0.0f,  0.5f,   // front-right

    // right face
     0.0f,  1.0f,  0.0f,   // top
     0.5f,  0.0f,  0.5f,   // front-right
     0.5f,  0.0f, -0.5f,   // back-right

    // back face
     0.0f,  1.0f,  0.0f,   // top
     0.5f,  0.0f, -0.5f,   // back-right
    -0.5f,  0.0f, -0.5f,   // back-left

    // left face
     0.0f,  1.0f,  0.0f,   // top
    -0.5f,  0.0f, -0.5f,   // back-left
    -0.5f,  0.0f,  0.5f,   // front-left

    // base (two triangles)
    -0.5f,  0.0f,  0.5f,
     0.5f,  0.0f,  0.5f,
     0.5f,  0.0f, -0.5f,

    -0.5f,  0.0f,  0.5f,
     0.5f,  0.0f, -0.5f,
    -0.5f,  0.0f, -0.5f
};


glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

void onFramebufferSizeChanged(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width,height);
    projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100.0f);
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    float xOffset = xPos - lastMouseX;
    float yOffset = yPos - lastMouseY;
    lastMouseX = xPos;
    lastMouseY = yPos;

    mainCamera.cameraMouseInput(xOffset, yOffset);
}

void handleInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    glm::vec2 input = glm::vec2(0.0f,0.0f);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        input.y = 1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        input.y = -1.0f;
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        input.x = 1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        input.x = -1.0f;
    }

    if(glm::length(input) > 0)
    {
        glm::vec3 movementDirection = mainCamera.getForward() * input.y + mainCamera.getRight() * input.x;
        movementDirection = glm::normalize(movementDirection);
        mainCamera.moveCamera(movementDirection);
    }

    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        mainCamera.moveCamera(glm::vec3(0,-1,0));
    }
    else if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        mainCamera.moveCamera(glm::vec3(0,1,0));
    }
}

int main()
{
    glfwInit();

    // set to OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    GLFWwindow* window = glfwCreateWindow(windowWidth,windowHeight, "LearnOpenGL", NULL, NULL);
    if(window == nullptr)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // init glad that will get opengl drivers implementations
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // dont save layout
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // tell OpenGL the size of our window
    glViewport(0,0, windowWidth, windowHeight);

    // hide mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(window, onFramebufferSizeChanged);

    // bind mouse callback
    glfwSetCursorPosCallback(window, mouseCallback);

    // flip images loaded in the x axis
    stbi_set_flip_vertically_on_load(true);

    unsigned codingClubTexture;
    int imageWidth, imageHeight, imageNumberOfColorChannels;

    unsigned char* imageBytes = stbi_load("assets/logo.png", &imageWidth, &imageHeight, &imageNumberOfColorChannels, 0);
    if(imageBytes)
    {
        // generate and bind texture object

        glGenTextures(1, &codingClubTexture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, codingClubTexture);
        
        // make texture repeat on x and y
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // texel = pixel in texture

        // what to do when we have more texels than pixels (looking from far. shrunk)
        // linear interpolation will smooth between close texels
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        // what to do when we have more pixels than texels (looking from close. magnified)
        // linear interpolation will smooth between close texels
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // generate the actual texture and its mipmap in the gpu
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBytes);
        glGenerateMipmap(GL_TEXTURE_2D);

        // free the image bytes. we don't need it anymore
        stbi_image_free(imageBytes);
    }
    else
    {
        std::cout << "Failed to load image" << std::endl;
    }
    
    // Vertex Buffer
    unsigned vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex array
    unsigned vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Vertex array attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // vertex array for light
    unsigned vertexArrayObjectLight;
    glGenVertexArrays(1, &vertexArrayObjectLight);
    glBindVertexArray(vertexArrayObjectLight);

    // vertex buffer for light
    unsigned vertexBufferObjectLight;
    glGenBuffers(1, &vertexBufferObjectLight);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectLight);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shader basicShader = Shader("./shaders/basic/basic.vert", "./shaders/basic/basic.frag");
    basicShader.setInt("textureData0", 0);
    basicShader.setVec3("ambientLightColor", glm::vec3(1.0f,1.0f,1.0f));

    Shader lightShader = Shader("./shaders/light/light.vert", "./shaders/light/light.frag");
    lightShader.setVec3("lightColor", glm::vec3(1.0f,1.0f,1.0f));

    projection = glm::perspective(glm::radians(90.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

    basicShader.setMatrix4("projection", projection);
    lightShader.setMatrix4("projection", projection);

    glEnable(GL_DEPTH_TEST);


    glm::mat4 lightModel = glm::mat4(1.0f);
    glm::vec3 lightPosition = glm::vec3(0.0f,0.0f, 2.0f);
    lightModel = glm::translate(lightModel, lightPosition);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // new ImGUI frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(ImVec2(400,300));
        ImGui::SetNextWindowPos(ImVec2(0,0));
        // ImGui::ShowDemoWindow();

        // delta time
        GameTime::updateDeltaTime();

        // INPUT //
        handleInput(window);

        // RENDER //

        // clear the last frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // make view matrix
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(mainCamera.getPosition(), mainCamera.getPosition() + mainCamera.getForward(), mainCamera.getUp());

        basicShader.setMatrix4("view", view);
        basicShader.setMatrix4("projection", projection);

        lightShader.setMatrix4("view", view);
        lightShader.setMatrix4("projection", projection);


        // draw cubes
        glBindVertexArray(vertexArrayObject);
        for(unsigned i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            basicShader.setMatrix4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        // draw light
        glBindVertexArray(vertexArrayObjectLight);
        lightShader.setMatrix4("model", lightModel);
        glDrawArrays(GL_TRIANGLES, 0, 18);


        // make ui

        ImGui::Begin("Debug");
        ImGui::Text("Text");
        ImGui::End();

        // draw ui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // show the buffer we drew!
        glfwSwapBuffers(window);

    }

    // clean up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}