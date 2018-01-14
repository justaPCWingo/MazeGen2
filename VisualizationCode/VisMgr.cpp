//
//  VisMgr.cpp
//  MazeGen2
//
//  Created by Patrick Wingo on 7/23/17.
//  Copyright © 2017 Wingo. All rights reserved.
//

#include "VisMgr.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLErrStream.hpp"
#include<string>
#include<sstream>
#include<iomanip>

#ifdef __APPLE__
#include <Opengl/gl3.h>
#else
//...
#endif

#define HIGH_DIM 512
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
#define ROT_COEFF 0.3

struct MazeCellVert {
    GLfloat x,y,z;
    union
    {
        GLuint wall;  //0x0 no wall
        GLfloat step;
    };
    //0x1 horizWall
    //0x2 vertWall
    
    MazeCellVert()
    : x(0.0f)
    , y(0.0f)
    , z(0.0f)
    , wall(0)
    {}
    
    MazeCellVert(GLfloat inX, GLfloat inY,bool horizWall=false,bool vertWall=false)
    : x(inX)
    , y(inY)
    , z(0.0f)
    , wall(0)
    {
        if (horizWall)
            wall+=1;
        if (vertWall)
            wall+=2;
    }
    
};

VisMgr::VisMgr(const std::string & shadDir,GLint x,GLint y,GLint width,GLint height)
:_wallVerts(nullptr)
,_pathVerts(nullptr)
,_pathVertsCount(0)
,_pathBuffSize(0)
,_pathTime(0.0f)
,_vpX(x)
,_vpY(y)
,_vpWidth(width)
,_vpHeight(height)
,_glowIntensity(0.25)
{
    _shdMgr=ShaderMgr(shadDir);
    
    _wallColor={1.0,0.0,0.0};
    _gridColor={0.4,0.0,0.0};
    _pathColor={0.0,1.0,1.0};
    _decayPathColor={.2,.2,.2};
    _showPath=true;
    _showDecay=true;
    _applyRot=true;
    _decayDelay=5.0;
    
    _actProj=glm::mat4();
    
    //set translate column to offset
#define MDL_OFFS -8.0
    _mdlMat[3]=glm::vec4(MDL_OFFS,MDL_OFFS,MDL_OFFS,1.0);
    
    for(unsigned int i=0; i<VAO_COUNT;++i)
        _vaos[i]=0;
    for(unsigned int i=0; i<BUFF_MAZE_COUNT;++i)
        _mazeBuffs[i]=0;
    
    for(unsigned int i=0; i<BUFF_PATH_COUNT;++i)
        _pathBuffs[i]=0;

    _compFBO=0;
    _compTexture=0;
    _depthBuff=0;
    
    _wallProg=0;
    _passThruProg=0;
    _gridProg=0;
    _pathProg=0;
    _blurProg=0;
    _combineProg=0;
}

void VisMgr::InitForOpenGL()
{
    
    glGenVertexArrays( VAO_COUNT, _vaos );
    ASSERT_GL("Post VAO Generation");
    //load shaders
    //_shdMgr.LoadShaderProgramSet("passthru");
    _wallProg=_shdMgr.LoadShaderProgramSet("wall");
    _pathProg=_shdMgr.LoadShaderProgramSet("path");
    //std::string relDir=_shdMgr.GetRelativeDirectory();
//    _wallProg=_shdMgr.LoadShaderProgram("wall",relDir+"wall.vert",relDir+"wall.frag");
    _passThruProg=_shdMgr.LoadShaderProgramSet("passThru");
    _compositeProg=_shdMgr.LoadShaderProgramSet("composite");
    _gridProg=_shdMgr.LoadShaderProgram("grid","wall.vert","wall.frag","grid.geom");
    _blurProg=_shdMgr.LoadShaderProgram("blur","composite.vert","transfer.frag");
    _combineProg=_shdMgr.LoadShaderProgram("combine","composite.vert","combine.frag");
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    ASSERT_GL("All Shaders Loaded");
}

void VisMgr::DrawMaze(bool isBloom)
{
    if (!isBloom)
        DrawGrid();
    DrawWalls();
    DrawPath(isBloom);
}

void VisMgr::Draw()
{
    //update rotation
    if(_applyRot)
    {
        glm::fquat rotQuat(cosf((_pathTime/_pathVertsCount)*3.),_rotAxis);
        //update mvp mat
        _mvp=_actProj*glm::mat4_cast(rotQuat)*_mdlMat;
    }
    else
        _mvp=_actProj*_mdlMat;
    
    // Drawing code here.
    ASSERT_GL("BASE DRAW START");
    glBindFramebuffer(GL_FRAMEBUFFER, _compFBO);
    GLenum buffsToDraw[]={GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, buffsToDraw);
    ASSERT_GL("BLAM");
    glViewport(0, 0, 1024, 1024);

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                // clear the window
    ASSERT_GL("BASE DRAW STOP");
    //draw base maze
    DrawMaze(false);
    
    //do blur composites for downscale
    
    GLuint dims=HIGH_DIM;
    for(unsigned int i=0;i<TEX_COUNT;++i,dims/=2)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _fbos1[i]);
        glViewport(0,0,dims,dims);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawMaze(true);
    }
    
    //http://harkal.sylphis3d.com/2006/05/20/how-to-do-good-bloom-for-hdr-rendering/
    dims=HIGH_DIM;
    //GLfloat kernel3[]={1./16.,2./16.,1./16.};
    GLfloat kernel5[]={1./16.0,4./16.0,6./16.0,4./16.0,1./16.0};
    GLfloat kernel11[]={1./1024.0,10./1024.0,45./1024.0,120./1024.0,210./1024.0,252./1024.0,210./1024.0,120./1024.0,45./1024.0,10./1024.0,1./1024.0};
    for(unsigned int i=0;i<TEX_COUNT;++i,dims/=2)
    {
        //do horizontal blur
        glBindFramebuffer(GL_FRAMEBUFFER, _fbos2[i]);
        glViewport(0,0,dims,dims);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawBlur(1./dims, 0.0, kernel11, sizeof(kernel11)/sizeof(GLfloat),_textures1[i]);
        
        //do vertical blur
        glBindFramebuffer(GL_FRAMEBUFFER, _fbos1[i]);
        glViewport(0,0,dims,dims);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawBlur(0.0, 1./dims, kernel5, sizeof(kernel5)/sizeof(GLfloat),_textures2[i]);
    }
    
    
    //put it all together.
    glBindFramebuffer(GL_FRAMEBUFFER, _compFBO);
    glViewport(0, 0, 1024, 1024);
    DrawCombine(_textures1);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //draw to screen
    glViewport(_vpX, _vpY, _vpWidth, _vpHeight);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawComposite();
    
    
    //DrawCombine(_textures1);
//    glViewport(0,0,HIGH_DIM,HIGH_DIM);
//    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    DrawMaze(true);
    //DrawTest();
    //[self drawPath]
    
    glFlush();
}

void VisMgr::InitMazeBuffs()
{
    glGenBuffers( BUFF_MAZE_COUNT, _mazeBuffs );
    
    glBindVertexArray(_vaos[VAO_MAZE]);
    glBindBuffer( GL_ARRAY_BUFFER, _mazeBuffs[BUFF_MAZE] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(MazeCellVert)*_wVertCount, nullptr, GL_DYNAMIC_DRAW );
    
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(MazeCellVert), BUFFER_OFFSET(0) );
    
    glEnableVertexAttribArray( 1 );
    glVertexAttribIPointer(1,1,GL_UNSIGNED_INT,sizeof(MazeCellVert),BUFFER_OFFSET(sizeof(GLfloat)*3));
    
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

}

void VisMgr::InitPathBuffs()
{
    glGenBuffers( BUFF_PATH_COUNT, _pathBuffs );
    
    glBindVertexArray(_vaos[VAO_PATH]);
    glBindBuffer( GL_ARRAY_BUFFER, _pathBuffs[BUFF_PATH] );
    //glBufferData( GL_ARRAY_BUFFER, sizeof(MazeCellVert)*_pathBuffSize, nullptr, GL_DYNAMIC_DRAW );
    
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(MazeCellVert), BUFFER_OFFSET(0) );
    
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer(1,1,GL_FLOAT,GL_FALSE,sizeof(MazeCellVert),BUFFER_OFFSET(sizeof(GLfloat)*3));
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

}

void VisMgr::InitCompositeBuff()
{
    GLfloat verts[]={-1.0, 1.0,0.0,
                     -1.0,-1.0,0.0,
                      1.0, 1.0,0.0,
                      1.0,-1.0,0.0};
    
    glGenBuffers(1,&_compositeBuff);
    glBindVertexArray(_vaos[VAO_COMPOSITE]);
    glBindBuffer(GL_ARRAY_BUFFER, _compositeBuff);
    glBufferData( GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW );
    
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, BUFFER_OFFSET(0) );
    
    glEnableVertexAttribArray(0);
    
    // - prepare depth buffer
    glGenRenderbuffers(1, &_depthBuff);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthBuff);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
    
    CreateRenderFBO(_compFBO,_compTexture,1024,1024);
    glGenFramebuffers(TEX_COUNT, _fbos1);
    glGenFramebuffers(TEX_COUNT, _fbos2);
    glGenTextures(TEX_COUNT, _textures1);
    glGenTextures(TEX_COUNT, _textures2);
    
    GLuint currDim=HIGH_DIM;
    for(unsigned int i=0; i<TEX_COUNT;++i)
    {
        CreateRenderFBO(_fbos1[i], _textures1[i], currDim, currDim);
        CreateRenderFBO(_fbos2[i], _textures2[i], currDim, currDim);
        currDim/=2;
    }
    
    //unbind everything
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D,0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void VisMgr::CreateRenderFBO(GLuint & fbo,GLuint & tex,GLuint xdim,GLuint ydim)
{
    //configure FBO (see http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/ )
    
    if(fbo==0)  //only allocate if not preallocated
        glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuff);

    // - prepare Texture target
    if(tex==0)  //only allocate if not preallocated
        glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_2D,tex);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, xdim, ydim, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    // - attach texture
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0);
    
    GLenum fboStatus=glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(fboStatus!=GL_FRAMEBUFFER_COMPLETE)
    {
        const char *msg="no Err";
        switch(fboStatus)
        {
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                msg="Incomplete attachment";
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                msg="Missing attachment";
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                msg="Incomplete Draw buffer";
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                msg="Incomplete read buffer";
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                msg="Framebuff unsupported";
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                msg="Incomplete multisample";
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                msg="Incomplete layer targets";
                break;
        }
        
        throw std::runtime_error(msg);
    }
    
}

void VisMgr::DrawWalls()
{
    glBindVertexArray(_vaos[VAO_MAZE]);
    glBindBuffer(GL_ARRAY_BUFFER, _mazeBuffs[BUFF_MAZE]);
    ASSERT_GL("Pre Draw");
    glUseProgram(_wallProg);
    ASSERT_GL("Use Program");
    glUniformMatrix4fv(glGetUniformLocation(_wallProg,"projMat"), 1, GL_FALSE, glm::value_ptr(_mvp));
    ASSERT_GL("ProjMat assign");
    glUniform3fv(glGetUniformLocation(_wallProg,"wallColor"),1,glm::value_ptr(_wallColor));
    ASSERT_GL("color assing");
    glDrawArrays(GL_POINTS,0,_wVertCount);
    ASSERT_GL("draw arrays");
    glUseProgram(0);
    ASSERT_GL("End Draw");
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    

}

void VisMgr::DrawGrid()
{
    glBindVertexArray(_vaos[VAO_MAZE]);
    glBindBuffer(GL_ARRAY_BUFFER, _mazeBuffs[BUFF_MAZE]);
    ASSERT_GL("Pre Draw");
    glUseProgram(_gridProg);
    ASSERT_GL("Use Program");
    glUniformMatrix4fv(glGetUniformLocation(_gridProg,"projMat"), 1, GL_FALSE, glm::value_ptr(_mvp));
    ASSERT_GL("ProjMat assign");
    glUniform3fv(glGetUniformLocation(_gridProg,"wallColor"),1,glm::value_ptr(_gridColor));
    ASSERT_GL("color assing");
    glDrawArrays(GL_POINTS,0,_wVertCount);
    ASSERT_GL("draw arrays");
    glUseProgram(0);
    ASSERT_GL("End Draw");
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    
}

void VisMgr::DrawPath(bool isBloom)
{
    glBindVertexArray(_vaos[VAO_PATH]);
    glBindBuffer(GL_ARRAY_BUFFER, _pathBuffs[BUFF_PATH]);
    ASSERT_GL("Pre Draw");
    glUseProgram(_pathProg);
    ASSERT_GL("Use Program");
    glUniformMatrix4fv(glGetUniformLocation(_pathProg,"projMat"), 1, GL_FALSE, glm::value_ptr(_mvp));
    glUniform1f(glGetUniformLocation(_pathProg,"currTime"),_pathTime);
    ASSERT_GL("ProjMat assign");
    glUniform3fv(glGetUniformLocation(_pathProg,"pathColor"),1,glm::value_ptr(_pathColor));
    glUniform3fv(glGetUniformLocation(_pathProg,"decayColor"),1,glm::value_ptr(_showDecay ? _decayPathColor : _pathColor));
    glUniform1f(glGetUniformLocation(_pathProg, "decayDelay"),_decayDelay);
    glUniform1i(glGetUniformLocation(_pathProg,"isBloom"),isBloom || !_showPath);
        glDrawArrays(GL_LINE_STRIP_ADJACENCY,0,_pathVertsCount);
    ASSERT_GL("draw arrays");
    glUseProgram(0);
    ASSERT_GL("End Draw");
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    
}

void VisMgr::DrawBlur(GLfloat xOff,GLfloat yOff,const GLfloat* kernel,const unsigned int & kLen,GLuint inTex)
{

    
    glBindVertexArray(_vaos[VAO_COMPOSITE]);
    glBindBuffer(GL_ARRAY_BUFFER,_compositeBuff);
    glUseProgram(_blurProg);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glUniform1i(glGetUniformLocation(_blurProg,"src"),0);
    glUniform2f(glGetUniformLocation(_blurProg,"offset"),xOff,yOff);
    glUniform1fv(glGetUniformLocation(_blurProg, "kernel"),kLen,kernel);
    glUniform1i(glGetUniformLocation(_blurProg, "kBound"),kLen/2); //rely on integer to truncate
    
    ASSERT_GL("Texture bound");
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    ASSERT_GL("Comp drawn");
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void VisMgr::DrawCombine(GLuint* texes)
{
    ASSERT_GL("Pre Composite");
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_MAX);
    glBindVertexArray(_vaos[VAO_COMPOSITE]);
    glBindBuffer(GL_ARRAY_BUFFER,_compositeBuff);
    glUseProgram(_combineProg);
    
    ASSERT_GL("All samplers bound");
    for(int i=0; i<TEX_COUNT;++i)
    {
        glActiveTexture(GL_TEXTURE0+i);
        ASSERT_GL("ith texture active");
        glBindTexture(GL_TEXTURE_2D, texes[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        ASSERT_GL("ith Texture bound");
    }
    glUniform1i(glGetUniformLocation(_combineProg,"src16"),0);
    glUniform1i(glGetUniformLocation(_combineProg,"src4"),1);
    glUniform1i(glGetUniformLocation(_combineProg,"src2"),2);
    glUniform1i(glGetUniformLocation(_combineProg,"src1"),3);
    glUniform1f(glGetUniformLocation(_combineProg,"intense"),_glowIntensity);

    ASSERT_GL("Texture bound");
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    ASSERT_GL("Comp drawn");
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

void VisMgr::DrawComposite()
{
    ASSERT_GL("Pre Composite");
    glBindVertexArray(_vaos[VAO_COMPOSITE]);
    glBindBuffer(GL_ARRAY_BUFFER,_compositeBuff);
    glUseProgram(_compositeProg);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _compTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glUniform1i(glGetUniformLocation(_compositeProg,"srcTx"),0);
    ASSERT_GL("Texture bound");
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    ASSERT_GL("Comp drawn");
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    
}
//void VisMgr::DrawTest()
//{
//    ASSERT_GL("pre draw");
//
//    glBindVertexArray(_vaos[VAO_TEST]);
//    glBindBuffer(GL_ARRAY_BUFFER,_buffs[BUFF_TEST]);
//    glUseProgram(_passThruProg);
//    glUniformMatrix4fv(glGetUniformLocation(_passThruProg, "projMat"), 1, GL_FALSE, glm::value_ptr(_testProj));
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    glUseProgram(0);
//    glBindVertexArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER,0);
//
//    ASSERT_GL("post draw");
//}

//void VisMgr::InitTest()
//{
//    GLfloat testVerts[]={ -0.5, 0.5, 0.0, 1.0,
//                        -0.5,-0.5, 0.0, 1.0,
//                         0.5, 0.5, 0.0, 1.0,
//                         0.5,-0.5, 0.0, 1.0};
//    
//    glGenBuffers( 1, &_buffs[BUFF_TEST] );
//    //    glGenBuffers( BUFF_COUNT, _buffs );
//    
//    
//    glBindVertexArray(_vaos[VAO_TEST]);
//    glBindBuffer( GL_ARRAY_BUFFER, _buffs[BUFF_TEST] );
//    glBufferData( GL_ARRAY_BUFFER, sizeof(testVerts), testVerts, GL_STATIC_DRAW );
//    
//    glEnableVertexAttribArray( 0 );
//    glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
//    
//    glBindBuffer(GL_ARRAY_BUFFER,0);
//    glBindVertexArray(0);
//    
//    //set ortho projection
//    _testProj=glm::ortho(-1.f, 1.f, -1.f, 1.f, 1.f,-1.f);
//
//}

void VisMgr::NewMaze(MazeBuilder* bldr)
{
    const GLfloat OFFSET=0.00;
    //prepare buffers
    //glGenBuffers( 1, &_buffs[BUFF_MAZE] );
    
    
    
    //realloc vertices
    _wVertCount=(bldr->GetRowCount()+1)*(bldr->GetColCount()+1)-1;
    delete[] _wallVerts;
    _wallVerts=new MazeCellVert[_wVertCount];
    
    //build vert records and alloc space
    MInd currInd=0;
    MInd r=0;
    for(; r<bldr->GetRowCount();++r)
    {
        MInd c=0;
        for(; c<bldr->GetColCount();++c)
        {
            _wallVerts[currInd++]=MazeCellVert(c+(OFFSET*c),r+(OFFSET*r));
        }
        
        //handle extra column of walls
        _wallVerts[currInd++]=MazeCellVert(c+(OFFSET*c),r+(OFFSET*r));
    }
    
    //handle last row of walls
    MInd c=0;
    for(; c<bldr->GetColCount();++c)
    {
        _wallVerts[currInd++]=MazeCellVert(c+(OFFSET*c),r+(OFFSET*r));
    }
    
    InitMazeBuffs();
    InitPathBuffs();
    InitCompositeBuff();
    
    //set ortho projection
    //_actProj=glm::ortho(-2.f, bldr->GetRowCount()+2.f, -2.f, bldr->GetColCount()+2.f, 1.f,-1.f);
    _actProj=glm::perspective(90.0, (1.0*bldr->GetRowCount())/bldr->GetColCount(), 0.001, 1000.0);
    
    //set up wall values
    RefreshWithMaze(bldr);
}

void VisMgr::RefreshWithMaze(MazeBuilder* bldr)
{
    if(_wallVerts==nullptr)
    {
        std::cout<<"WALL!"<<std::endl;
        ASSERT_GL("PreNew");
        NewMaze(bldr);
        ASSERT_GL("PostNew");
    }
    //useful for some shaders
    const MInd BOUNDARY_FLAG=0x4;
    // Update walls
    
    MInd currInd=0;
    MInd r=0;
    std::cout<<"building walls"<<std::endl;
    for(; r<bldr->GetRowCount();++r)
    {
        MInd c=0;
        for(; c<bldr->GetColCount();++c)
        {
            MazeCellVert & v=_wallVerts[currInd++];
            v.wall=0;
            unsigned short w=bldr->WallsForCell(r,c);
            if(w & MazeBuilder::UP_DIR)
                v.wall|=MazeBuilder::UP_DIR;
            if(w & MazeBuilder::LEFT_DIR)
                v.wall|=MazeBuilder::LEFT_DIR;
            //std::cout<<v.wall<<' ';
        }
        
        
        //handle extra column of walls
        _wallVerts[currInd].wall=BOUNDARY_FLAG;
        if(bldr->WallsForCell(r,c-1) & MazeBuilder::RIGHT_DIR)
            _wallVerts[currInd].wall|=MazeBuilder::LEFT_DIR;
        ++currInd;
        
        //std::cout<<std::endl;
    }
    //std::cout<<std::endl;
    
    //handle last row of walls
    MInd c=0;
    for(; c<bldr->GetColCount();++c)
    {
        unsigned short w=bldr->WallsForCell(r-1,c);
        _wallVerts[currInd].wall=BOUNDARY_FLAG;
        if(w & MazeBuilder::DOWN_DIR)
            _wallVerts[currInd].wall|=MazeBuilder::UP_DIR;
        ++currInd;
    }
    ASSERT_GL("PreSubData");

    //DbgDumpMaze(bldr);
//    for (MInd i=0; i< _wVertCount; ++i)
//        std::cout<<std::setw(2)<<i<<" Wall: "<<_wallVerts[i].wall<<std::endl;
    glBindVertexArray(_vaos[VAO_MAZE]);
    glBindBuffer( GL_ARRAY_BUFFER, _mazeBuffs[BUFF_MAZE] );
    glBufferSubData( GL_ARRAY_BUFFER,0,sizeof(MazeCellVert)*_wVertCount, _wallVerts);
    ASSERT_GL("PostSubData");

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
    BuildPath(bldr);
}

void VisMgr::BuildPath(MazeBuilder* bldr)
{
    PathList path=bldr->PathToFinish();
    if(!path.empty() && path[0]!=MazeBuilder::NO_VAL)
    {
        glBindVertexArray(_vaos[VAO_PATH]);
        glBindBuffer( GL_ARRAY_BUFFER, _pathBuffs[BUFF_PATH] );
        
        _pathVertsCount=path.size()+2;
        if(_pathVertsCount>_pathBuffSize)
        {
            _pathBuffSize=_pathVertsCount;
            delete[] _pathVerts;
            _pathVerts=new MazeCellVert[_pathBuffSize];
            glBufferData( GL_ARRAY_BUFFER, sizeof(MazeCellVert)*_pathBuffSize, nullptr, GL_DYNAMIC_DRAW );
            glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(MazeCellVert), BUFFER_OFFSET(0) );
            glVertexAttribPointer(1,1,GL_FLOAT,GL_FALSE,sizeof(MazeCellVert),BUFFER_OFFSET(sizeof(GLfloat)*3));
        }
        
        size_t i=0;
        MInd sInd=bldr->GetStart();
        _pathVerts[i]=MazeCellVert(bldr->ColForIndex(sInd)+0.5,bldr->RowForIndex(sInd)+0.5);
        _pathVerts[i].step=i;
        GLfloat offs=StepToEdge(_pathVerts[i], bldr->GetStartEdge());
        ++i;
        for(MInd ind : path)
        {
            _pathVerts[i]=MazeCellVert(bldr->ColForIndex(ind)+0.5,bldr->RowForIndex(ind)+0.5);
            _pathVerts[i].step=i+offs;
            //std::cout<<_pathVerts[i].x<<", "<<_pathVerts[i].y<<std::endl;
            ++i;
        }
        MInd fInd=bldr->GetFinish();
        _pathVerts[i]=MazeCellVert(bldr->ColForIndex(fInd)+0.5,bldr->RowForIndex(fInd)+0.5);
        GLfloat finalStep=StepToEdge(_pathVerts[i], bldr->GetFinishEdge());
        _pathVerts[i].step=finalStep+_pathVerts[i-1].step;
        
        glBufferSubData( GL_ARRAY_BUFFER,0,sizeof(MazeCellVert)*_pathVertsCount, _pathVerts);
        ASSERT_GL("PostSubData");
        
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);

    }
}

void VisMgr::StepInDirection(MazeCellVert & mcv, const MazeBuilder::DIRECTIONS & dir,const GLfloat & stepSize)
{
    switch(dir)
    {
    case MazeBuilder::UP_DIR:
        mcv.y+=stepSize;
        break;
    case MazeBuilder::DOWN_DIR:
        mcv.y-=stepSize;
        break;
    case MazeBuilder::LEFT_DIR:
        mcv.x-=stepSize;
        break;
    case MazeBuilder::RIGHT_DIR:
        mcv.x+=stepSize;
        break;
    case MazeBuilder::NO_DIR:
        //do nothing
        break;
    }
}

void VisMgr::SetPathTime(const GLfloat & currTime)
{
    _pathTime=currTime;
}

GLfloat VisMgr::GetPathTime() const
{
    return _pathTime;
}

void VisMgr::DbgDumpMaze(MazeBuilder* bldr)
{
    using namespace std;
    for(MInd r=0; r<bldr->GetRowCount();++r)
    {
        ostringstream upStr;
        ostringstream downStr;
        
        unsigned short cWalls=MazeBuilder::NO_DIR;
        for(MInd c=0;c<bldr->GetColCount();++c)
        {
            cWalls=bldr->WallsForCell(r, c);
            upStr<<' '<<(cWalls & MazeBuilder::UP_DIR ? '-' : ' ');
            downStr<<(cWalls & MazeBuilder::LEFT_DIR ? '|' : ' ')<<' ';
        }
        //use last cell to check for last cell
        downStr<<(cWalls & MazeBuilder::RIGHT_DIR ? '|' : ' ')<<' ';
        cout<<upStr.str()<<endl<<downStr.str()<<endl;
    }
    
    //final row
    for(MInd i=(bldr->GetRowCount()-1)*bldr->GetColCount(); i<bldr->GetRowCount()*bldr->GetColCount();++i)
        cout<<' '<<(bldr->WallsForCell(i) & MazeBuilder::DOWN_DIR ? '-' : ' ');
    cout<<endl;
}

GLfloat VisMgr::StepToEdge(MazeCellVert & mcv, MazeBuilder::DIRECTIONS dir)
{
    const GLfloat DISTANCE=2.0;
    switch(dir)
    {
        case MazeBuilder::UP_DIR:
            mcv.y-=DISTANCE;
            break;
        case MazeBuilder::DOWN_DIR:
            mcv.y+=DISTANCE;
            break;
        case MazeBuilder::LEFT_DIR:
            mcv.x-=DISTANCE;
            break;
        case MazeBuilder::RIGHT_DIR:
            mcv.x+=DISTANCE;
            break;
        default:
            break;
    }
    return DISTANCE;
}

void VisMgr::SetViewport(GLint x, GLint y, GLint width, GLint height)
{ 
    _vpX=x;
    _vpY=y;
    _vpWidth=width;
    _vpHeight=height;
}

glm::vec3 VisMgr::GetGridColor() const
{
    return _gridColor;
}

glm::vec3 VisMgr::GetWallColor() const
{ 
    return _wallColor;
}

glm::vec3 VisMgr::GetPathColor() const
{
    return _pathColor;
}

glm::vec3 VisMgr::GetDecayedPathColor() const
{ 
    return _decayPathColor;
}

bool VisMgr::GetShowFullPath() const
{
    return _showPath;
}

bool VisMgr::GetShowPathDecay() const
{
    return _showDecay;
}

void VisMgr::SetGridColor(const glm::vec3 &gc)
{
    _gridColor=gc;
}

void VisMgr::SetWallColor(const glm::vec3 &wc)
{
    _wallColor=wc;
}

void VisMgr::SetPathColor(const glm::vec3 &pc)
{
    _pathColor=pc;
}

void VisMgr::SetDecayedPathColor(const glm::vec3 &dpc)
{
    _decayPathColor=dpc;
}

void VisMgr::SetShowFullPath(bool showFull)
{
    _showPath=showFull;
}

void VisMgr::SetShowPathDecay(bool showDecay)
{
    _showDecay=showDecay;
}

float VisMgr::GetDecayDelay() const
{
    return _decayDelay;
}

void VisMgr::SetDecayDelay(float delay)
{
    _decayDelay=delay;
}

bool VisMgr::ReadyToDraw() const { 
    return _compFBO>0;
}

void VisMgr::SetRotAxis(float x, float y, float z) { 
    
    _rotAxis.x=x;
    _rotAxis.y=y;
    _rotAxis.z=z;
}

unsigned int VisMgr::GetPathLength() const {
    //returns only internal count;
    return _pathVertsCount-2;
}

bool VisMgr::GetApplyRot() const { 
    return _applyRot;
}

void VisMgr::SetApplyRot(bool apply) { 
    _applyRot=apply;
}
























