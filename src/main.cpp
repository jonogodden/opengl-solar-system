#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

// TODO: Add STB Image library for real texture loading
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Enhanced procedural texture loading function with more realistic patterns
unsigned int createProceduralTexture(const std::string &name)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    const int width = 512, height = 512;
    std::vector<unsigned char> data(width * height * 3);

    // Initialize random seed for this texture
    srand(std::hash<std::string>{}(name));

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = (y * width + x) * 3;
            float u = (float)x / width;
            float v = (float)y / height;

            if (name == "Sun")
            {
                // Sun texture - solar surface with granules and sunspots
                float noise = (float)(rand() % 100) / 100.0f;
                float granule = sin(u * 50) * cos(v * 50) * 0.1f;

                // Base solar color
                int r = 255;
                int g = 220 + (int)(noise * 35) + (int)(granule * 255);
                int b = 150 + (int)(noise * 50);

                // Add sunspots (dark regions)
                float sunspot = sin(u * 3.14f) * sin(v * 2.1f);
                if (sunspot > 0.8f && noise > 0.95f)
                {
                    r = 180;
                    g = 150;
                    b = 100;
                }

                data[index] = r;
                data[index + 1] = g;
                data[index + 2] = b;
            }
            else if (name == "Earth")
            {
                // Earth texture - continents, oceans, clouds
                float noise = (float)(rand() % 100) / 100.0f;
                float continent = sin(u * 8) * cos(v * 6) + sin(u * 12) * cos(v * 9);

                if (continent > 0.3f)
                {
                    // Land - green/brown continents
                    data[index] = 50 + (int)(noise * 100);
                    data[index + 1] = 120 + (int)(noise * 80);
                    data[index + 2] = 50 + (int)(noise * 50);
                }
                else
                {
                    // Ocean - blue water
                    data[index] = 20 + (int)(noise * 30);
                    data[index + 1] = 80 + (int)(noise * 60);
                    data[index + 2] = 150 + (int)(noise * 50);
                }

                // Add white clouds
                if (noise > 0.85f)
                {
                    data[index] = 200;
                    data[index + 1] = 200;
                    data[index + 2] = 200;
                }
            }
            else if (name == "Mars")
            {
                // Mars texture - red surface with craters and dust
                float noise = (float)(rand() % 100) / 100.0f;
                float crater = sin(u * 20) * cos(v * 15) + sin(u * 30) * cos(v * 25);

                // Base red surface
                int r = 180 + (int)(noise * 40);
                int g = 80 + (int)(noise * 30);
                int b = 40 + (int)(noise * 20);

                // Add craters (darker regions)
                if (crater > 0.7f)
                {
                    r = 120;
                    g = 50;
                    b = 20;
                }

                data[index] = r;
                data[index + 1] = g;
                data[index + 2] = b;
            }
            else if (name == "Jupiter")
            {
                // Jupiter texture - gas giant with bands and storms
                float band = sin(v * 20) * 0.5f + 0.5f;
                float storm = sin(u * 15) * cos(v * 10);

                // Alternating bands
                if (band > 0.5f)
                {
                    data[index] = 200;
                    data[index + 1] = 150;
                    data[index + 2] = 100; // Light band
                }
                else
                {
                    data[index] = 160;
                    data[index + 1] = 100;
                    data[index + 2] = 60; // Dark band
                }

                // Add Great Red Spot
                float redSpot = sqrt((u - 0.7f) * (u - 0.7f) + (v - 0.5f) * (v - 0.5f));
                if (redSpot < 0.1f)
                {
                    data[index] = 180;
                    data[index + 1] = 80;
                    data[index + 2] = 60;
                }
            }
            else if (name == "Saturn")
            {
                // Saturn texture - pale bands with subtle rings shadow
                float band = sin(v * 15) * 0.3f + 0.7f;

                data[index] = 200 + (int)(band * 30);
                data[index + 1] = 180 + (int)(band * 20);
                data[index + 2] = 140 + (int)(band * 20);
            }
            else if (name == "Uranus")
            {
                // Uranus texture - pale blue-green with subtle bands
                float band = sin(v * 10) * 0.2f + 0.8f;

                data[index] = 120 + (int)(band * 20);
                data[index + 1] = 160 + (int)(band * 30);
                data[index + 2] = 180 + (int)(band * 20);
            }
            else if (name == "Neptune")
            {
                // Neptune texture - deep blue with white clouds
                float noise = (float)(rand() % 100) / 100.0f;
                float cloud = sin(u * 8) * cos(v * 6);

                data[index] = 60 + (int)(noise * 20);
                data[index + 1] = 100 + (int)(noise * 30);
                data[index + 2] = 180 + (int)(noise * 40);

                // Add white clouds
                if (cloud > 0.8f && noise > 0.7f)
                {
                    data[index] = 200;
                    data[index + 1] = 200;
                    data[index + 2] = 200;
                }
            }
            else
            {
                // Default texture for other planets
                float noise = (float)(rand() % 100) / 100.0f;
                data[index] = 128 + (int)(noise * 127);
                data[index + 1] = 128 + (int)(noise * 127);
                data[index + 2] = 128 + (int)(noise * 127);
            }
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}

// Vertex Shader source code for 3D
const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    layout (location = 2) in vec2 aTexCoord;
    
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    
    out vec3 FragPos;
    out vec3 Normal;
    out vec2 TexCoord;
    
    void main()
    {
        FragPos = vec3(model * vec4(aPos, 1.0));
        Normal = mat3(transpose(inverse(model))) * aNormal;
        TexCoord = aTexCoord;
        gl_Position = projection * view * vec4(FragPos, 1.0);
    }
)";

// Fragment Shader source code with lighting and textures
const char *fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    
    in vec3 FragPos;
    in vec3 Normal;
    in vec2 TexCoord;
    
    uniform vec3 lightPos;
    uniform vec3 lightColor;
    uniform vec3 objectColor;
    uniform vec3 viewPos;
    uniform sampler2D diffuseTexture;
    uniform bool useTexture;
    
    void main()
    {
        // Get base color from texture or object color
        vec3 baseColor;
        if (useTexture) {
            baseColor = texture(diffuseTexture, TexCoord).rgb;
        } else {
            baseColor = objectColor;
        }
        
        // Check if this is the sun (bright yellow/orange color)
        bool isSun = (baseColor.r > 0.8 && baseColor.g > 0.8 && baseColor.b < 0.3);
        
        if (isSun) {
            // Enhanced sun rendering with glow and corona effects
            
            // Calculate distance from center for corona effect
            vec3 center = vec3(0.0, 0.0, 0.0);
            float distFromCenter = length(FragPos - center);
            float corona = 1.0 - smoothstep(0.8, 1.2, distFromCenter);
            
            // Add texture detail to the sun surface
            vec3 sunSurface = baseColor;
            
            // Create corona glow effect
            vec3 coronaColor = vec3(1.0, 0.8, 0.4) * corona * 0.5;
            
            // Add rim lighting for depth
            vec3 viewDir = normalize(viewPos - FragPos);
            float rim = 1.0 - max(dot(normalize(Normal), viewDir), 0.0);
            rim = pow(rim, 3.0);
            
            // Combine all effects
            vec3 finalColor = sunSurface + coronaColor + rim * vec3(1.0, 0.7, 0.3) * 0.3;
            
            // Make sun glow brighter
            finalColor *= 2.5;
            
            FragColor = vec4(finalColor, 1.0);
        } else {
            // Normal lighting for planets
            
            // Ambient lighting - planets in shadow should still be visible
            float ambientStrength = 0.2;
            vec3 ambient = ambientStrength * lightColor;
            
            // Diffuse lighting
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;
            
            // Specular lighting
            float specularStrength = 0.3;
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
            vec3 specular = specularStrength * spec * lightColor;
            
            // Distance attenuation - planets farther from sun get less light
            float distance = length(lightPos - FragPos);
            float attenuation = 1.0 / (1.0 + 0.01 * distance + 0.0001 * distance * distance);
            
            vec3 result = (ambient + (diffuse + specular) * attenuation) * baseColor;
            FragColor = vec4(result, 1.0);
        }
    }
)";

// Camera class for 3D navigation
class Camera
{
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float yaw;
    float pitch;
    float fov;

    Camera() : position(0.0f, 0.0f, 10.0f), front(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f),
               yaw(-90.0f), pitch(0.0f), fov(45.0f)
    {
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(position, position + front, up);
    }

    void updateCameraVectors()
    {
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);
    }
};

// Celestial body class
class CelestialBody
{
public:
    std::string name;
    float radius;
    float distanceFromParent;
    float orbitalPeriod;
    float rotationPeriod;
    float orbitalAngle;
    float rotationAngle;
    glm::vec3 color;
    CelestialBody *parent;
    std::vector<CelestialBody *> children;
    unsigned int textureID;
    bool useTexture;

    CelestialBody(const std::string &n, float r, float dist, float orbPeriod,
                  float rotPeriod, const glm::vec3 &c, CelestialBody *p = nullptr, float initialOrbitalAngle = 0.0f)
        : name(n), radius(r), distanceFromParent(dist), orbitalPeriod(orbPeriod),
          rotationPeriod(rotPeriod), orbitalAngle(initialOrbitalAngle), rotationAngle(0.0f),
          color(c), parent(p), useTexture(true), textureID(0)
    {
        if (parent)
        {
            parent->children.push_back(this);
        }
    }

    void initializeTexture()
    {
        textureID = createProceduralTexture(name);
    }

    void update(float deltaTime)
    {
        // Update orbital position - planets with longer periods move slower
        if (orbitalPeriod > 0)
        {
            orbitalAngle += (360.0f / orbitalPeriod) * deltaTime * 0.5f; // Reduced scale factor for slower motion
        }

        // Update rotation
        if (rotationPeriod > 0)
        {
            rotationAngle += (360.0f / rotationPeriod) * deltaTime * 0.5f; // Reduced scale factor for slower motion
        }

        // Update children
        for (auto child : children)
        {
            child->update(deltaTime);
        }
    }

    glm::mat4 getModelMatrix()
    {
        glm::mat4 model = glm::mat4(1.0f);

        // Apply parent's transformation
        if (parent)
        {
            model = parent->getModelMatrix();
        }

        // Correct order: rotate first, then translate
        if (distanceFromParent > 0)
        {
            model = glm::rotate(model, glm::radians(orbitalAngle), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(distanceFromParent, 0.0f, 0.0f));
        }

        // Rotation transformation
        model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));

        // Scale to radius
        model = glm::scale(model, glm::vec3(radius));

        return model;
    }
};

// Global variables
Camera camera;
std::vector<CelestialBody *> solarSystem;
float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse = true;
bool mousePressed = false;

// Error callback for GLFW
void errorCallback(int error, const char *description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// Window resize callback
void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Mouse callback
void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (!mousePressed)
        return;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.yaw += xoffset;
    camera.pitch += yoffset;

    if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    camera.updateCameraVectors();
}

// Mouse button callback
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            mousePressed = true;
            firstMouse = true;
        }
        else if (action == GLFW_RELEASE)
        {
            mousePressed = false;
        }
    }
}

// Scroll callback for zoom
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.fov -= (float)yoffset;
    if (camera.fov < 1.0f)
        camera.fov = 1.0f;
    if (camera.fov > 45.0f)
        camera.fov = 45.0f;
}

// Process input
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Reset view with 'R' key
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        camera.position = glm::vec3(0.0f, 15.0f, 30.0f); // Good overview position
        camera.yaw = -90.0f;
        camera.pitch = -20.0f; // Look down slightly
        camera.fov = 45.0f;
        camera.updateCameraVectors();
    }

    float cameraSpeed = 2.0f; // Increased speed for better exploration

    // Forward/Backward movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.position += cameraSpeed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.position -= cameraSpeed * camera.front;

    // Left/Right movement
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;

    // Up/Down movement (like a spaceship)
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.position += cameraSpeed * camera.up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.position -= cameraSpeed * camera.up;

    // Fast movement for long distances
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        cameraSpeed *= 5.0f; // 5x faster when holding Ctrl
    }
}

// Function to create sphere vertices with texture coordinates
std::vector<float> createSphereVertices(float radius, int sectors, int stacks)
{
    std::vector<float> vertices;

    for (int i = 0; i <= stacks; ++i)
    {
        float stackAngle = M_PI / 2 - i * M_PI / stacks;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j)
        {
            float sectorAngle = j * 2 * M_PI / sectors;

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            // Position
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Normal
            vertices.push_back(x / radius);
            vertices.push_back(y / radius);
            vertices.push_back(z / radius);

            // Texture coordinates
            float s = (float)j / sectors;
            float t = (float)i / stacks;
            vertices.push_back(s);
            vertices.push_back(t);
        }
    }

    return vertices;
}

// Function to create sphere indices
std::vector<unsigned int> createSphereIndices(int sectors, int stacks)
{
    std::vector<unsigned int> indices;

    for (int i = 0; i < stacks; ++i)
    {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stacks - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    return indices;
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set GLFW error callback
    glfwSetErrorCallback(errorCallback);

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow *window = glfwCreateWindow(1200, 800, "Solar System Simulation", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set callbacks
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set initial viewport
    glViewport(0, 0, 1200, 800);

    // Build and compile our shader program
    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for vertex shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for fragment shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    // Delete shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Create sphere geometry
    auto sphereVertices = createSphereVertices(1.0f, 30, 30);
    auto sphereIndices = createSphereIndices(30, 30);

    // Vertex Buffer Object (VBO), Vertex Array Object (VAO), and Element Buffer Object (EBO)
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO first, then bind and set VBO, and then configure vertex attributes
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), sphereIndices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Create solar system with realistic relative scales and orbital periods
    // Sun
    CelestialBody *sun = new CelestialBody("Sun", 2.0f, 0.0f, 0.0f, 27.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    solarSystem.push_back(sun);

    // Mercury - smallest planet, fastest orbit (scaled down for visibility)
    CelestialBody *mercury = new CelestialBody("Mercury", 0.08f, 4.0f, 10.0f, 58.6f, glm::vec3(0.7f, 0.7f, 0.7f), sun, 0.0f);
    solarSystem.push_back(mercury);

    // Venus - similar size to Earth, slow rotation
    CelestialBody *venus = new CelestialBody("Venus", 0.15f, 6.0f, 25.0f, -243.0f, glm::vec3(1.0f, 0.8f, 0.6f), sun, 45.0f);
    solarSystem.push_back(venus);

    // Earth - our home planet
    CelestialBody *earth = new CelestialBody("Earth", 0.16f, 8.0f, 40.0f, 1.0f, glm::vec3(0.2f, 0.5f, 1.0f), sun, 90.0f);
    solarSystem.push_back(earth);

    // Mars - smaller than Earth
    CelestialBody *mars = new CelestialBody("Mars", 0.12f, 10.0f, 75.0f, 1.03f, glm::vec3(1.0f, 0.3f, 0.2f), sun, 135.0f);
    solarSystem.push_back(mars);

    // Jupiter - largest planet, gas giant
    CelestialBody *jupiter = new CelestialBody("Jupiter", 0.45f, 14.0f, 200.0f, 0.41f, glm::vec3(0.9f, 0.7f, 0.5f), sun, 180.0f);
    solarSystem.push_back(jupiter);

    // Saturn - second largest, with rings (we'll add rings later)
    CelestialBody *saturn = new CelestialBody("Saturn", 0.38f, 18.0f, 500.0f, 0.45f, glm::vec3(0.9f, 0.8f, 0.6f), sun, 225.0f);
    solarSystem.push_back(saturn);

    // Uranus - ice giant, tilted on its side
    CelestialBody *uranus = new CelestialBody("Uranus", 0.27f, 22.0f, 1000.0f, -0.72f, glm::vec3(0.6f, 0.8f, 0.9f), sun, 270.0f);
    solarSystem.push_back(uranus);

    // Neptune - farthest planet, similar to Uranus
    CelestialBody *neptune = new CelestialBody("Neptune", 0.26f, 26.0f, 2000.0f, 0.67f, glm::vec3(0.3f, 0.5f, 0.9f), sun, 315.0f);
    solarSystem.push_back(neptune);

    // Initialize random seed and textures
    srand(time(0));
    for (auto body : solarSystem)
    {
        body->initializeTexture();
    }

    // Set up lighting
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

    float lastFrame = 0.0f;

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);

        // Update solar system
        for (auto body : solarSystem)
        {
            body->update(deltaTime);
        }

        // Render
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f); // Dark blue background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader
        glUseProgram(shaderProgram);

        // Set up view and projection matrices
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), 1200.0f / 800.0f, 0.1f, 1000.0f);

        // Pass matrices to shader
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Pass lighting uniforms - use sun's position as light source
        glm::vec3 lightPos = sun->getModelMatrix()[3]; // Get sun's world position
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(camera.position));

        // Draw all celestial bodies
        for (auto body : solarSystem)
        {
            glm::mat4 model = body->getModelMatrix();
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(body->color));

            // Special lighting for the sun - it should glow and not be affected by shadows
            if (body->name == "Sun")
            {
                glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), true);                                            // Enable texture for sun
                glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(glm::vec3(3.0f, 2.5f, 2.0f))); // Brighter, warmer light

                // Draw sun with larger scale for better visibility
                glm::mat4 sunModel = glm::scale(model, glm::vec3(1.2f)); // Make sun slightly larger
                glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(sunModel));
            }
            else
            {
                glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), body->useTexture);
                glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor)); // Normal light
            }

            // Bind texture and set texture uniforms
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, body->textureID);
            glUniform1i(glGetUniformLocation(shaderProgram, "diffuseTexture"), 0);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
        }

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    for (auto body : solarSystem)
    {
        delete body;
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}