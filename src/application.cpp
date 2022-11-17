#include "GLBasic.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //glew
    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;

    //create and bind buffer
    float tre_vertex[] = {
        -0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3
    };

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), tre_vertex, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint ind;
    glGenBuffers(1, &ind);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    
    ProgramSource programsrc = ParseShader("shaders/Basic.shader");
    std::cout << "VERTEX : " << std::endl;
    std::cout << programsrc.VertexSource << std::endl;
    std::cout << "FRAGMENT : " << std::endl;
    std::cout << programsrc.FragmentSource << std::endl;
    unsigned int shader = createShader(programsrc.VertexSource, programsrc.FragmentSource);

    glUseProgram(shader);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        //glBegin(GL_TRIANGLES);

        //glVertex2f(-0.5, -0.5);
        //glVertex2f(0, 0.5);
        //glVertex2f(0.5, -0.5);

        //glEnd();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}