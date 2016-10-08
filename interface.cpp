#include "interface.h"

bool keys[1024];

GLfloat deltaTime = 0;
GLfloat lastFrame = 0;
const GLfloat defaultCameraSpeed = 5;

GLfloat lastX, lastY;
GLfloat xoffset, yoffset;
const GLfloat sensitivity = 0.01f;
bool moved = false;
bool firstMouse = true;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(action == GLFW_PRESS)
        keys[key]=1;
    else if(action == GLFW_RELEASE)
        keys[key]=0;
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    xoffset = (xpos - lastX) * sensitivity;
    yoffset = (lastY - ypos) * sensitivity; // Reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;
    moved = false;
}

void doMovement(viewPoint* camera){
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    GLfloat cameraSpeed = defaultCameraSpeed*deltaTime;
    if(!moved)
    {camera->rotate(glm::vec3(xoffset,yoffset,0));moved = true;}

    glm::vec3 eulers = camera->getEulers();

    glm::vec3 cameraTarget(-glm::sin(eulers.x)*glm::cos(eulers.y),
                           0,
                           glm::cos(eulers.x)*glm::cos(eulers.y));
    glm::vec3 cameraUp(0,1,0);

    if(keys[GLFW_KEY_W])camera->move(cameraTarget*cameraSpeed);
    if(keys[GLFW_KEY_S])camera->move(cameraTarget*(-cameraSpeed));
    if(keys[GLFW_KEY_A])
        camera->move(-glm::normalize(glm::cross(cameraTarget,cameraUp))*cameraSpeed);
    if(keys[GLFW_KEY_D])
        camera->move(glm::normalize(glm::cross(cameraTarget,cameraUp))*cameraSpeed);
    if(keys[GLFW_KEY_SPACE])
        camera->move(cameraUp*cameraSpeed);
    if(keys[GLFW_KEY_LEFT_CONTROL])
        camera->move(cameraUp*(-cameraSpeed));
}



GLFWwindow* initializeInterface(GLuint width, GLuint height){
    GLFWwindow* wind;
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    wind = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(wind);

    glfwSetInputMode(wind, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(wind, mouse_callback);

    // Set the required callback functions
    glfwSetKeyCallback(wind, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, width, height);

    //models[0]=model("path/to/model","");
    //mainCam = &viewPoint(&Shader("",""),WIDTH,HEIGHT);
    glEnable(GL_DEPTH_TEST);
    return wind;
}
