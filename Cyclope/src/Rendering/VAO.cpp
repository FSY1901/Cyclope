#include "VAO.h"

namespace Cyclope {

    VAO::VAO() {
        
    }

    void VAO::Generate() {
        glGenVertexArrays(1, &m_ID);
    }

    void VAO::LinkVBO(VBO* vbo) {
        vbo->Bind();

        //position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        vbo->Unbind();
    }

    void VAO::LinkEBO(VBO* vbo, EBO* ebo) {
        vbo->Bind();
        ebo->Bind();

        //position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        vbo->Unbind();
    }

    void VAO::Bind() {
        glBindVertexArray(m_ID);
    }

    void VAO::Unbind() {
        glBindVertexArray(0);
    }

    void VAO::Delete() {
        glDeleteVertexArrays(1, &m_ID);
    }

    void VAO::DrawVBO(int mode, int first, int count) {
        Bind();
        glDrawArrays(mode, first, count);
        Unbind();
    }

    void VAO::DrawEBO(int mode, int count) {
        Bind();
        glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
        Unbind();
    }

}