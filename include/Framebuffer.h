#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>

class Framebuffer {
public:
    unsigned int FBO;
    unsigned int texture;
    unsigned int RBO;
    
    Framebuffer(int width, int height);
    ~Framebuffer();
    
    void Bind();
    void Unbind();
    void Resize(int width, int height);
    
private:
    int width;
    int height;
    void Create();
    void Cleanup();
};

#endif 