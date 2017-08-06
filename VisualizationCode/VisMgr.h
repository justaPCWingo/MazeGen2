//
//  VisMgr.h
//  MazeGen2
//
//  Created by Patrick Wingo on 7/23/17.
//  Copyright © 2017 Wingo. All rights reserved.
//

#pragma once
#include "../MazeGen2/MazeBuilder.h"
#include "ShaderMgr.h"
#include <glm/glm.hpp>
#include <vector>

struct MazeCellVert;


class VisMgr
{
public:
    VisMgr(const std::string & shadDir);
    
    void InitForOpenGL();
    void Draw();
    void NewMaze(MazeBuilder* bldr);
    void InitTest();
    void RefreshWithMaze(MazeBuilder* bldr);
    
private:
    
    enum {
        VAO_BASE=0,
        VAO_TEST,
        VAO_COUNT
    };
    enum
    {
        BUFF_MAZE=0,
        BUFF_TEST,
        BUFF_COUNT
    };
    ShaderMgr _shdMgr;
    
    GLuint _vaos[VAO_COUNT];
    GLuint _buffs[BUFF_COUNT];
    
    glm::mat4 _actProj;
    glm::mat4 _testProj;
    
    glm::vec3 _wallColor;
    //glm::vec3 _pathColor;
    
    MazeCellVert *_verts;
    MInd _vertCount;
    
    GLuint _wallProg;
    GLuint _passThruProg;

    void DrawWalls();
    void DrawTest();

    void DbgDumpMaze(MazeBuilder* bldr);
};
