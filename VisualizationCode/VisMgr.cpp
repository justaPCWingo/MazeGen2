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

VisMgr::VisMgr(const std::string & shadDir)
:_verts(nullptr)
{
    _shdMgr=ShaderMgr(shadDir);
    
    _wallColor={1.0,0.0,0.0};
    _actProj=glm::mat4();
    
    for(unsigned int i=0; i<VAO_COUNT;++i)
        _vaos[i]=0;
    for(unsigned int i=0; i<BUFF_COUNT;++i)
    _buffs[i]=0;
    
    _wallProg=0;
    _passThruProg=0;
    
}

void VisMgr::InitForOpenGL()
{
    
    glGenVertexArrays( VAO_COUNT, _vaos );
    
    //load shaders
    //_shdMgr.LoadShaderProgramSet("passthru");
    _wallProg=_shdMgr.LoadShaderProgramSet("wall");
    //std::string relDir=_shdMgr.GetRelativeDirectory();
//    _wallProg=_shdMgr.LoadShaderProgram("wall",relDir+"wall.vert",relDir+"wall.frag");
    _passThruProg=_shdMgr.LoadShaderProgramSet("passThru");
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
}

void VisMgr::Draw()
{
    // Drawing code here.
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// clear the window
    
    //[self drawGrid]
    DrawWalls();
    //DrawTest();
    //[self drawPath]
    
    glFlush();
}

void VisMgr::DrawWalls()
{
    glBindVertexArray(_vaos[VAO_BASE]);
    glBindBuffer(GL_ARRAY_BUFFER, _buffs[BUFF_MAZE]);
    ASSERT_GL("Pre Draw");
    glUseProgram(_wallProg);
    ASSERT_GL("Use Program");
    glUniformMatrix4fv(glGetUniformLocation(_wallProg,"projMat"), 1, GL_FALSE, glm::value_ptr(_actProj));
    ASSERT_GL("ProjMat assign");
    glUniform3fv(glGetUniformLocation(_wallProg,"wallColor"),1,glm::value_ptr(_wallColor));
    ASSERT_GL("color assing");
    glDrawArrays(GL_POINTS,0,_vertCount);
    ASSERT_GL("draw arrays");
    glUseProgram(0);
    ASSERT_GL("End Draw");
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    

}

void VisMgr::DrawTest()
{
    ASSERT_GL("pre draw");

    glBindVertexArray(_vaos[VAO_TEST]);
    glBindBuffer(GL_ARRAY_BUFFER,_buffs[BUFF_TEST]);
    glUseProgram(_passThruProg);
    glUniformMatrix4fv(glGetUniformLocation(_passThruProg, "projMat"), 1, GL_FALSE, glm::value_ptr(_testProj));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    ASSERT_GL("post draw");
}

void VisMgr::InitTest()
{
    GLfloat testVerts[]={ -0.5, 0.5, 0.0, 1.0,
                        -0.5,-0.5, 0.0, 1.0,
                         0.5, 0.5, 0.0, 1.0,
                         0.5,-0.5, 0.0, 1.0};
    
    glGenBuffers( 1, &_buffs[BUFF_TEST] );
    //    glGenBuffers( BUFF_COUNT, _buffs );
    
    
    glBindVertexArray(_vaos[VAO_TEST]);
    glBindBuffer( GL_ARRAY_BUFFER, _buffs[BUFF_TEST] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(testVerts), testVerts, GL_STATIC_DRAW );
    
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
    //set ortho projection
    _testProj=glm::ortho(-1.f, 1.f, -1.f, 1.f, 1.f,-1.f);

}

void VisMgr::NewMaze(MazeBuilder* bldr)
{
    //prepare buffers
    glGenBuffers( 1, &_buffs[BUFF_MAZE] );
//    glGenBuffers( BUFF_COUNT, _buffs );
    
    
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
    
    glBindVertexArray(_vaos[VAO_BASE]);
    glBindBuffer( GL_ARRAY_BUFFER, _buffs[BUFF_MAZE] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(MazeCellVert)*_vertCount, nullptr, GL_STATIC_DRAW );
    
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(MazeCellVert), BUFFER_OFFSET(0) );
    
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer(1,1,GL_UNSIGNED_INT, GL_FALSE,sizeof(MazeCellVert),BUFFER_OFFSET(sizeof(GLfloat)*3));
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
    //set ortho projection
    _actProj=glm::ortho(-2.f, bldr->GetRowCount()+2.f, -2.f, bldr->GetColCount()+2.f,1.f,-1.f);
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
            //std::cout<<v.wall<<' ';
        }
        
        
        //handle extra column of walls
        if(bldr->WallsForCell(r,c-1) & MazeBuilder::RIGHT_DIR)
            _verts[currInd++].wall=MazeBuilder::LEFT_DIR;
        
        //std::cout<<std::endl;
    }
    //std::cout<<std::endl;
    
    //handle last row of walls
    MInd c=0;
    for(; c<bldr->GetColCount();++c)
    {
        unsigned short w=bldr->WallsForCell(r,c);
        if(w & MazeBuilder::DOWN_DIR)
            _verts[currInd++].wall|=MazeBuilder::UP_DIR;
    }
    ASSERT_GL("PreSubData");

    DbgDumpMaze(bldr);
    
    glBindVertexArray(_vaos[VAO_BASE]);
    glBindBuffer( GL_ARRAY_BUFFER, _buffs[BUFF_MAZE] );
    glBufferSubData( GL_ARRAY_BUFFER,0,sizeof(MazeCellVert)*_vertCount, _verts);
    ASSERT_GL("PostSubData");

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
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

