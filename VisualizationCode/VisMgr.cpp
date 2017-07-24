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
#include<string>

#ifdef __APPLE__
#include <Opengl/gl3.h>
#else
//...
#endif


#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

struct MazeCellVert {
    GLfloat x,y,z;
    GLuint wall;  //0x0 no wall
    //0x1 horizWall
    //0x2 vertWall
    
    MazeCellVert()
    : x(0.0f)
    , y(0.0f)
    , z(0.0f)
    , wall(0)
    {}
    
    MazeCellVert(GLfloat inX, GLfloat inY)
    : x(inX)
    , y(inY)
    , z(0.0f)
    , wall(0)
    {}
    
};

VisMgr::VisMgr(const std::string & shadDir)
{
    _shdMgr=ShaderMgr(shadDir);
    
    _wallColor={1.0,0.0,0.0};
    _actProj=glm::mat4();
    
}

void VisMgr::InitForOpenGL()
{
    GLuint vao;
    
    glGenVertexArrays( 1, &vao );
    _vaos.push_back(vao);
    
    //load shaders
    //_shdMgr.LoadShaderProgramSet("passthru");
    _wallProg=_shdMgr.LoadShaderProgramSet("wall");
    
}

void VisMgr::Draw()
{
    // Drawing code here.
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// clear the window
    
    //[self drawGrid]
    DrawWalls();
    
    //[self drawPath]
    
    glFlush();
}

void VisMgr::DrawWalls()
{
    glUseProgram(_wallProg);
    glUniformMatrix4fv(glGetUniformLocation(_wallProg,"projMat"), 1, GL_FALSE, glm::value_ptr(_actProj));
    glUniform3fv(glGetUniformLocation(_wallProg,"wallColor"),1,glm::value_ptr(_wallColor));
    glDrawArrays(GL_POINTS,0,_vertCount);
    glUseProgram(0);
}


void VisMgr::NewMaze(MazeBuilder* bldr)
{
    //prepare buffers
    GLuint buff;
    if(!_buffs.empty())
        glDeleteBuffers(_buffs.size(),&_buffs[0]);
    glGenBuffers( 1, &buff );
    _buffs.clear();
    _buffs.push_back(buff);
    
    //realloc vertices
    _vertCount=(bldr->GetRowCount()+1)*(bldr->GetColCount()+1)-1;
    delete[] _verts;
    _verts=new MazeCellVert[_vertCount];
    
    //build vert records and alloc space
    MInd currInd=0;
    MInd r=0;
    for(; r<bldr->GetRowCount();++r)
    {
        MInd c=0;
        for(; c<bldr->GetColCount();++c)
        {
            _verts[currInd++]=MazeCellVert(c,r);
        }
        
        //handle extra column of walls
        _verts[currInd++]=MazeCellVert(c,r);
    }
    
    //handle last row of walls
    MInd c=0;
    for(; c<bldr->GetColCount();++c)
    {
        _verts[currInd++]=MazeCellVert(c,r);
    }
    
    glBindVertexArray(_vaos[0]);
    glBindBuffer( GL_ARRAY_BUFFER, buff );
    glBufferData( GL_ARRAY_BUFFER, sizeof(MazeCellVert)*_vertCount, nullptr, GL_STATIC_DRAW );
    
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(MazeCellVert), BUFFER_OFFSET(0) );
    
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer(1,1,GL_UNSIGNED_INT, GL_FALSE,sizeof(MazeCellVert),BUFFER_OFFSET(sizeof(GLfloat)*3));
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
    //set ortho projection
    _actProj=glm::ortho(-2.f, bldr->GetRowCount()+2.f, -2.f, bldr->GetColCount()+2.f,-1.f,1.f);
    //set up wall values
    RefreshWithMaze(bldr);
}

void VisMgr::RefreshWithMaze(MazeBuilder* bldr)
{
    // Update walls
    
    MInd currInd=0;
    MInd r=0;
    for(; r<bldr->GetRowCount();++r)
    {
        MInd c=0;
        for(; c<bldr->GetColCount();++c)
        {
            MazeCellVert & v=_verts[currInd++];
            v.wall=0;
            unsigned short w=bldr->WallsForCell(r,c);
            if(w & MazeBuilder::UP_DIR)
                v.wall|=MazeBuilder::UP_DIR;
            if(w & MazeBuilder::LEFT_DIR)
                v.wall|=MazeBuilder::LEFT_DIR;
        }
        
        //handle extra column of walls
        if(bldr->WallsForCell(r,c-1) & MazeBuilder::RIGHT_DIR)
            _verts[currInd++].wall=MazeBuilder::LEFT_DIR;
    }
    
    //handle last row of walls
    MInd c=0;
    for(; c<bldr->GetColCount();++c)
    {
        unsigned short w=bldr->WallsForCell(r,c);
        if(w & MazeBuilder::DOWN_DIR)
            _verts[currInd++].wall|=MazeBuilder::UP_DIR;
    }
    
    glBindVertexArray(_vaos[0]);
    glBindBuffer( GL_ARRAY_BUFFER, _buffs[0] );
    glBufferSubData( GL_ARRAY_BUFFER,0,sizeof(MazeCellVert)*_vertCount, _verts);
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

}
