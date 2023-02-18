#include "VAO.h"

namespace Cyclope {

    VAO::VAO() {

    }

    void VAO::Generate() {
        glGenVertexArrays(1, &m_ID);
    }

    void VAO::Link(VBO* VBO) {
        VBO->Bind();

        //position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        VBO->Unbind();
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

}