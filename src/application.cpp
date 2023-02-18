#include "GLBasic.h"
#include "indexBuffer.h"
#include "vertexBuffer.h"
#include "vertexArray.h"
#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //ʹ�ú�������
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

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    /**
     * �������������������֮ǰ�ȴ���֡����ͨ����Ϊv-sync
     * Ĭ������£��������Ϊ0
     * ��������Ϊ1����ÿ֡����һ��
     **/
    glfwSwapInterval(1);

    //glew
    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;

    int nMaxAttribes;
    GLCALL(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nMaxAttribes));
    std::cout << "Max Supported Attribs : " << nMaxAttribes << std::endl;

    //create and bind buffer
    float tre_vertex[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3
    };

    {

        VertexArray va;//���������������

        VertexBuffer vb(tre_vertex, 4 * 6 * sizeof(float));//���㻺��������

        VertexBufferLayout vblayout;
        vblayout.Push<float>(3);//ÿ������������float���
        vblayout.Push<float>(3);//ÿ������������float���
        va.AddBuffer(vb, vblayout);

        IndexBuffer ib(indices, 6);//

        Shader program("shaders/Basic.shader");
        program.Bind();
        //�ڰ�GPU��ɫ���� ������������cpp����shader����
        //GLCALL(int locationColor = glGetUniformLocation(shader, "u_Color"));
        //ASSERT(locationColor != -1);
        //GLCALL(glUniform4f(locationColor, 0.4f, 0.5f, 0.0f, 1.0f));
        GLfloat r = 0.0f;

        //������а�
    #if 1
        //GLCALL(glBindVertexArray(0));
        va.UnBind();
        program.UnBind();
        //GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
        //GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
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
            program.Bind();
            //GLCALL(glUniform4f(locationColor, r, 0.4f, 0.0f, 1.0f));

            va.Bind();
            //glBindVertexArray(vao);
            //ib.Bind();
            //GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
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
    }
    glfwTerminate();
    return 0;
}