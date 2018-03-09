#ifndef _CSCI441_UNIFORM_H_
#define _CSCI441_UNIFORM_H_

class Uniform {
public:
    static void set(GLuint shader, const std::string &name, const Matrix& m) {
        GLuint loc = location(shader, name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, m.values);
    }

private:

    static GLuint location(GLuint shader, const std::string &name) {
        return glGetUniformLocation(shader, name.c_str());
    }
};

#endif

