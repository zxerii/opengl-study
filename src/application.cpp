#include "GLBasic.h"
#include "indexBuffer.h"
#include "vertexBuffer.h"
#include "vertexArray.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //使用核心配置
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    /**
     * 交换间隔，交换缓冲区之前等待的帧数，通常称为v-sync
     * 默认情况下，交换间隔为0
     * 这里设置为1，即每帧更新一次
     **/
    glfwSwapInterval(1);

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
    {

        VertexArray va;//创建顶点数组对象

        VertexBuffer vb(tre_vertex, 4 * 2 * sizeof(float));//顶点缓存区创建

        VertexBufferLayout vblayout;
        vblayout.Push<float>(2);//每个顶点由两个float组成
        va.AddBuffer(vb, vblayout);

        IndexBuffer ib(indices, 6);//

        ProgramSource programsrc = ParseShader("shaders/Basic.shader");
        std::cout << "VERTEX : " << std::endl;
        std::cout << programsrc.VertexSource << std::endl;
        std::cout << "FRAGMENT : " << std::endl;
        std::cout << programsrc.FragmentSource << std::endl;
        unsigned int shader = createShader(programsrc.VertexSource, programsrc.FragmentSource);

        GLCALL(glUseProgram(shader));

        //在绑定GPU着色器后， 检索参数，由cpp设置shader变量
        GLCALL(int locationColor = glGetUniformLocation(shader, "u_Color"));
        ASSERT(locationColor != -1);
        //GLCALL(glUniform4f(locationColor, 0.4f, 0.5f, 0.0f, 1.0f));
        GLfloat r = 0.0f;

        //解绑所有绑定
    #if 1
        GLCALL(glBindVertexArray(0));
        GLCALL(glUseProgram(0));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    #endif

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCALL(glClear(GL_COLOR_BUFFER_BIT));
            if (r > 1.0f) {
                r = 0.0f;
            }
            else {
                r += 0.01f;
            }
            //glDrawArrays(GL_TRIANGLES, 0, 3);
            GLCALL(glUseProgram(shader));
            GLCALL(glUniform4f(locationColor, r, 0.4f, 0.0f, 1.0f));

            va.Bind();
            //glBindVertexArray(vao);
            ib.Bind();

            GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
            //glBegin(GL_TRIANGLES);

            //glVertex2f(-0.5, -0.5);
            //glVertex2f(0, 0.5);
            //glVertex2f(0.5, -0.5);

            //glEnd();
            /* Swap front and back buffers */
            GLCALL(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCALL(glfwPollEvents());
        }
        glDeleteProgram(shader);
    }
    glfwTerminate();
    return 0;
}