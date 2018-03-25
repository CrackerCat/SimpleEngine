/*******************************************************************
 ** This code is part of Breakout.
 **
 ** Breakout is free software: you can redistribute it and/or modify
 ** it under the terms of the CC BY 4.0 license as published by
 ** Creative Commons, either version 4 of the License, or (at your
 ** option) any later version.
 ******************************************************************/
#include "SpriteRenderer.h"
#include "ResourceManager.h"


SpriteRenderer::SpriteRenderer()
{
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0);

	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
	m_pShader = ResourceManager::GetInstance()->GetShader("sprite");
	m_pShader->Bind();

	glUniform1i(glGetUniformLocation(m_pShader->GetProgramID(), "image"), 0);
	glUniformMatrix4fv(glGetUniformLocation(m_pShader->GetProgramID(), "projection"), 1, GL_FALSE, (GLfloat*)(&projection));
	glUniform1f(glGetUniformLocation(m_pShader->GetProgramID(), "alpha"), 0);

    initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(Texture* texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
    // Prepare transformations
    m_pShader->Bind();
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)
    
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back
    
    model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale
    
    glUniformMatrix4fv(glGetUniformLocation(m_pShader->GetProgramID(), "model"), 1, GL_FALSE, (GLfloat*) (&model));
    
    // Render textured quad
    glUniform3f(glGetUniformLocation(m_pShader->GetProgramID(), "spriteColor"), color.x, color.y, color.z);
    glUniform1f(glGetUniformLocation(m_pShader->GetProgramID(), "alpha"), 0);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0);


    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    
}

void SpriteRenderer::DrawMask(Texture* texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
    // Prepare transformations
    m_pShader->Bind();
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)
    
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back
    
    model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale
    
    glUniformMatrix4fv(glGetUniformLocation(m_pShader->GetProgramID(), "model"), 1, GL_FALSE, (GLfloat*) (&model));
       
    // Render textured quad
    glUniform3f(glGetUniformLocation(m_pShader->GetProgramID(), "spriteColor"), color.x, color.y,0);
    glUniform1f(glGetUniformLocation(m_pShader->GetProgramID(), "alpha"), 0);
    
    glStencilMask(0xFF);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    
    //glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
    glBlendFuncSeparate(GL_ZERO,GL_ONE,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
    //  glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glStencilMask(0x00);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}


void SpriteRenderer::DrawFrameSprite(Texture* texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
    // Prepare transformations
    m_pShader->Bind();
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back
    model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale
    
    glUniformMatrix4fv(glGetUniformLocation(m_pShader->GetProgramID(), "model"), 1, GL_FALSE, (GLfloat*) (&model));
    
    // Render textured quad
    glUniform3f(glGetUniformLocation(m_pShader->GetProgramID(), "spriteColor"), color.x, color.y, color.z);
    glUniform1f(glGetUniformLocation(m_pShader->GetProgramID(), "alpha"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    

    
}

void SpriteRenderer::DrawMapSprite(Texture* texture, glm::vec2 position, glm::vec2 size, GLfloat rotate , GLfloat alpha)
{
    // Prepare transformations
    m_pShader->Bind();
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back
    model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale
    
    glUniformMatrix4fv(glGetUniformLocation(m_pShader->GetProgramID(), "model"), 1, GL_FALSE, (GLfloat*) (&model));
    
    // Render textured quad
    glUniform3f(glGetUniformLocation(m_pShader->GetProgramID(), "spriteColor"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(m_pShader->GetProgramID(), "alpha"),alpha);

    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    
    // glBlendFuncSeparate(GL_ONE,GL_ZERO, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendFuncSeparate(GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA ,GL_ZERO,GL_ONE);
     glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
   
    
}

void SpriteRenderer::DrawSprite( glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
    // Prepare transformations
    m_pShader->Bind();
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)
    
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back
    
    model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale
    
    glUniformMatrix4fv(glGetUniformLocation(m_pShader->GetProgramID(), "model"), 1, GL_FALSE,(GLfloat*) (&model));
    
    // Render textured quad
    glUniform3f(glGetUniformLocation(m_pShader->GetProgramID(), "spriteColor"), color.x, color.y, color.z);
 
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


void SpriteRenderer::initRenderData()
{
    // Configure VAO/VBO
    GLuint VBO;
    GLfloat vertices[] = {
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
