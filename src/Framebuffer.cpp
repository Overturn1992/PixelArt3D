#include "Framebuffer.h"
#include <iostream>

Framebuffer::Framebuffer(int width, int height) : width(width), height(height) {
    Create();
}

Framebuffer::~Framebuffer() {
    Cleanup();
}

void Framebuffer::Create() {
    // 生成帧缓冲对象
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // 生成纹理附件
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // 生成渲染缓冲对象
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    // 检查帧缓冲是否完整
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Cleanup() {
    glDeleteFramebuffers(1, &FBO);
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &RBO);
}

void Framebuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void Framebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(int newWidth, int newHeight) {
    if (width != newWidth || height != newHeight) {
        width = newWidth;
        height = newHeight;
        Cleanup();
        Create();
    }
} 