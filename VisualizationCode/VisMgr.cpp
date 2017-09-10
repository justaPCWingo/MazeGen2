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


#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

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

VisMgr::VisMgr(const std::string & shadDir)
:_wallVerts(nullptr)
,_pathVerts(nullptr)
,_pathVertsCount(0)
,_pathBuffSize(0)
,_pathTime(0.0f)
{
    _shdMgr=ShaderMgr(shadDir);
    
    _wallColor={1.0,0.0,0.0};
    _gridColor={0.4,0.0,0.0};
    _pathColor={0.0,1.0,1.0};
    _actProj=glm::mat4();
    
    for(unsigned int i=0; i<VAO_COUNT;++i)
        _vaos[i]=0;
    for(unsigned int i=0; i<BUFF_MAZE_COUNT;++i)
        _mazeBuffs[i]=0;
    
    for(unsigned int i=0; i<BUFF_PATH_COUNT;++i)
        _pathBuffs[i]=0;

    
    _wallProg=0;
    _passThruProg=0;
    _gridProg=0;
    _pathProg=0;
}

void VisMgr::InitForOpenGL()
{
    
    glGenVertexArrays( VAO_COUNT, _vaos );
    
    //load shaders
    //_shdMgr.LoadShaderProgramSet("passthru");
    _wallProg=_shdMgr.LoadShaderProgramSet("wall");
    _pathProg=_shdMgr.LoadShaderProgramSet("path");
    //std::string relDir=_shdMgr.GetRelativeDirectory();
//    _wallProg=_shdMgr.LoadShaderProgram("wall",relDir+"wall.vert",relDir+"wall.frag");
    _passThruProg=_shdMgr.LoadShaderProgramSet("passThru");
    _gridProg=_shdMgr.LoadShaderProgram("grid","wall.vert","wall.frag","grid.geom");
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
}

void VisMgr::Draw()
{
    // Drawing code here.
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// clear the window
    
    //[self drawGrid]
    DrawGrid();
    DrawWalls();
    DrawPath();
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

void VisMgr::DrawWalls()
{
    glBindVertexArray(_vaos[VAO_MAZE]);
    glBindBuffer(GL_ARRAY_BUFFER, _mazeBuffs[BUFF_MAZE]);
    ASSERT_GL("Pre Draw");
    glUseProgram(_wallProg);
    ASSERT_GL("Use Program");
    glUniformMatrix4fv(glGetUniformLocation(_wallProg,"projMat"), 1, GL_FALSE, glm::value_ptr(_actProj));
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
    glUniformMatrix4fv(glGetUniformLocation(_gridProg,"projMat"), 1, GL_FALSE, glm::value_ptr(_actProj));
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

void VisMgr::DrawPath()
{
    glBindVertexArray(_vaos[VAO_PATH]);
    glBindBuffer(GL_ARRAY_BUFFER, _pathBuffs[BUFF_PATH]);
    ASSERT_GL("Pre Draw");
    glUseProgram(_pathProg);
    ASSERT_GL("Use Program");
    glUniformMatrix4fv(glGetUniformLocation(_pathProg,"projMat"), 1, GL_FALSE, glm::value_ptr(_actProj));
    glUniform1f(glGetUniformLocation(_pathProg,"currTime"),_pathTime);
    ASSERT_GL("ProjMat assign");
    glUniform3fv(glGetUniformLocation(_pathProg,"pathColor"),1,glm::value_ptr(_pathColor));
        glDrawArrays(GL_LINE_STRIP,0,_pathVertsCount);
    ASSERT_GL("draw arrays");
    glUseProgram(0);
    ASSERT_GL("End Draw");
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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
    
    
    //set ortho projection
    _actProj=glm::ortho(-2.f, bldr->GetRowCount()+2.f, -2.f, bldr->GetColCount()+2.f, 1.f,-1.f);
    //set up wall values
    RefreshWithMaze(bldr);
}

void VisMgr::RefreshWithMaze(MazeBuilder* bldr)
{
    //useful for some shaders
    const MInd BOUNDARY_FLAG=0x4;
    // Update walls
    
    MInd currInd=0;
    MInd r=0;
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

    DbgDumpMaze(bldr);
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
        StepToEdge(_pathVerts[i], bldr->GetStartEdge());
        ++i;
        for(MInd ind : path)
        {
            _pathVerts[i]=MazeCellVert(bldr->ColForIndex(ind)+0.5,bldr->RowForIndex(ind)+0.5);
            _pathVerts[i].step=i;
            std::cout<<_pathVerts[i].x<<", "<<_pathVerts[i].y<<std::endl;
            ++i;
        }
        MInd fInd=bldr->GetFinish();
        _pathVerts[i]=MazeCellVert(bldr->ColForIndex(fInd)+0.5,bldr->RowForIndex(fInd)+0.5);
        _pathVerts[i].step=i;
        StepToEdge(_pathVerts[i], bldr->GetFinishEdge());
        
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

void VisMgr::StepToEdge(MazeCellVert & mcv, MazeBuilder::DIRECTIONS dir)
{
    const GLfloat DISTANCE=0.75;
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
}

