
#include <iostream>
#include <vector>
#include <ctime>

#define GLEW_STATIC
#include "includes/glew.h"
#include "includes/glfw3.h"


#pragma comment(lib, "opengl32.lib")

struct Point {
    float x;
    float y;
};

void norm_dev(int width, int height, Point* p) {
        // directly changes the point struct
        p->x = (2.0 * p->x) / width - 1;
        p->y = 1 - (2.0 * p->y) / height;
        // p->z = ((2.0 * z) - f - n) / (f - n)) - 1.0; //(f and n are far and near)
        // p->w = 1.0f
}

float determs(Point p1, Point p2, Point p3) {
    // det = x1*y2 + x3*y1 + x2*y3 − x3*y2 − x2*y1 − x1*y3
    return p1.x*p2.y + p3.x*p1.y + p2.x*p3.y - p3.x*p2.y - p2.x*p1.y - p1.x*p3.y;
}

std::vector<Point> halfhull(Point p1, Point p2, std::vector<Point> s, std::vector<float> dets) {

    if(s.size() == 0) {
        std::vector<Point> res;
        res.push_back(p1); res.push_back(p2);
        return res;
    }

    int max_i = 0;
    float max = dets[0];
    
    for(int i=0; i<dets.size(); ++i) {
        if (dets[i] > max) {
            max = dets[i];
            max_i = i;
        }
    }

    Point p_max = s[max_i];

    std::vector<Point> s1;
    std::vector<float> dets1;

    std::vector<Point> s2;
    std::vector<float> dets2;

    for(int i=0; i<s.size(); ++i) {
        float det = determs(p1, p_max, s[i]);
        if (det > 0) {
            s1.push_back(s[i]);
            dets1.push_back(det);
        }

        det = determs(p_max, p2, s[i]);
        if (det > 0) {
            s2.push_back(s[i]);
            dets2.push_back(det);
        }
    }
    std::vector<Point> res = halfhull(p1, p_max, s1, dets1);
    std::vector<Point> r = halfhull(p_max, p2, s2, dets2); 
    
    res.insert(res.end(), r.begin(), r.end());
    return res;
}

std::vector<Point> quickhull(std::vector<Point> points) {
    int min_i = 0;
    float min = points[0].x;
    int max_i = 0;
    float max = points[0].x;

    for(int i=0; i<points.size(); ++i) {
        if (points[i].x < min) {
            min = points[i].x;
            min_i = i;
        }
        if(points[i].x > max) {
            max = points[i].x;
            max_i = i;
        }
    }

    std::vector<Point> s;
    for (int i=0; i<points.size(); ++i) {
        if(i != min_i && i != max_i) {
            s.push_back(points[i]);
        }
    }

    std::vector<float> dets;
    for(int i=0; i<s.size(); ++i) {
        dets.push_back(determs(points[min_i], points[max_i], s[i]));
    }


    std::vector<Point> s1;
    std::vector<float> dets1;

    for(int i=0; i<s.size(); ++i) {
        if (dets[i] > 0) {
            s1.push_back(s[i]);
            dets1.push_back(dets[i]);
        }
    }

    std::vector<Point> top = halfhull(points[min_i], points[max_i], s1, dets1);

    std::vector<Point> s2;
    std::vector<float>dets2;

    for(int i=0; i<s.size(); ++i) {
        if (dets[i] < 0) {
            s2.push_back(s[i]);
            dets2.push_back(-dets[i]);
        }
    }


    std::vector<Point> bottom = halfhull(points[max_i], points[min_i], s2, dets2);


    top.insert(top.end(), bottom.begin(), bottom.end());
    return top;

}



int main(int argc, char** argv) {

    std::vector<Point> points;
    std::vector<Point> lines;

    std::vector<float> draw_points;
    std::vector<float> draw_lines;


    std::srand(std::time(0));

    std::cout << "Enter number of random points to generate: ";
    int n;
    std::cin >> n;

    for(int i=0; i<n; ++i) {
        points.push_back( Point{ float(std::rand()%480), float(480-std::rand()%480)});
    }

    lines = quickhull(points);

    for(int i=0; i<points.size(); ++i) {
        norm_dev(480, 480, &points[i]);
        draw_points.push_back(points[i].x); draw_points.push_back(points[i].y);
        draw_points.push_back(0.0f); draw_points.push_back(1.0f);
    }

    for(int i=0; i<lines.size(); ++i) {
        norm_dev(480, 480, &lines[i]);
        draw_lines.push_back(lines[i].x); draw_lines.push_back(lines[i].y);
        draw_lines.push_back(0.0f); draw_lines.push_back(1.0f);
    }

    
    if (!glfwInit()) {
    std::cerr << "ERROR: could not start GLFW3\n";
    return 1;
    }

    GLFWwindow* window = glfwCreateWindow(480, 480, "Quickhull", nullptr, nullptr);

    if (!window) {
        std::cerr << "ERROR: could not open window with GLFW3\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();  // need the glewInit() or doesnt run

    // shader stuff
    static const char* vs_source = {
        "#version 450 core                              \n"
        "layout(location=0) in vec2 vec_points;         \n"
        "void main() {                                  \n"
        "   gl_Position = vec4(vec_points, 0, 1);       \n"
        "}                                              \n"
    };

    const char* fs_source =
        "#version 450 core\n"
        "out vec4 color;"
        "void main() {"
        " color = vec4(1.0, 0.0, 0.0, 1.0);"
        "}";

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vs_source, nullptr);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fs_source, nullptr);
    glCompileShader(fragment_shader);

    GLuint render_program = glCreateProgram();
    glAttachShader(render_program, vertex_shader);
    glAttachShader(render_program, fragment_shader);
    glLinkProgram(render_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glValidateProgram(render_program);

    // GLint result;
    // glGetProgramiv(render_program, GL_VALIDATE_STATUS, &result);

    // std::cout << "compiled shaders returning program\n";
    // std::cout << "checking program " << result  << "\n";

    // done with shader stuff

    GLuint vb_points;
    glGenBuffers(1, &vb_points);
    GLuint va_points;
    glCreateVertexArrays(1, &va_points);

    GLuint vb_lines;
    glGenBuffers(1, &vb_lines);
    GLuint va_lines;
    glCreateVertexArrays(1, &va_lines);


    glPointSize(3.0f); // we dont want one pixel sized points


    while(!glfwWindowShouldClose(window)) {

        // POINTS!!!
        glUseProgram(render_program);

        if(draw_points.size() > 0) {
            glBindVertexArray(va_points);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vb_points);
            glBufferData(GL_ARRAY_BUFFER, draw_points.size() * sizeof(float), &draw_points.front(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

            glDrawArrays(GL_POINTS, 0, draw_points.size() / 4);
        }
        
        if (draw_lines.size() > 0) {
            glBindVertexArray(va_lines);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vb_lines);
            glBufferData(GL_ARRAY_BUFFER, draw_lines.size() * sizeof(float), &draw_lines.front(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

            glDrawArrays(GL_LINES, 0, draw_lines.size() / 4);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

     return 0;
}
