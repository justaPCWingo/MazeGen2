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

typedef std::vector<GLuint> IndVec;

struct MazeCellVert;


class VisMgr
{
public:
    VisMgr(const std::string & shadDir);
    
    void InitForOpenGL();
    void Draw();
    void NewMaze(MazeBuilder* bldr);
    
    void RefreshWithMaze(MazeBuilder* bldr);
    
private:
    ShaderMgr _shdMgr;
    IndVec _vaos;
    IndVec _buffs;
    
    glm::mat4 _actProj;
    
    glm::vec3 _wallColor;
    //glm::vec3 _pathColor;
    
    MazeCellVert *_verts;
    MInd _vertCount;
    
    GLuint _wallProg;

    void DrawWalls();

};
