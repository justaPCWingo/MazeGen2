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
    //void InitTest();
    void RefreshWithMaze(MazeBuilder* bldr);
    void BuildPath(MazeBuilder* bldr);
    
    void StepInDirection(MazeCellVert & mcv, const MazeBuilder::DIRECTIONS & dir,const GLfloat & stepSize);
    
    void SetPathTime(const GLfloat & currTime);
    GLfloat GetPathTime() const;
    
    static void DbgDumpMaze(MazeBuilder* bldr);
private:
    
    enum {
        VAO_MAZE=0,
        //VAO_TEST,
        VAO_PATH,
        VAO_COUNT
    };
    enum
    {
        BUFF_MAZE=0,
        //BUFF_TEST,

        BUFF_MAZE_COUNT
    };
    
    enum
    {
        BUFF_PATH=0,
        
        BUFF_PATH_COUNT
    };
    ShaderMgr _shdMgr;
    
    GLuint _vaos[VAO_COUNT];
    GLuint _mazeBuffs[BUFF_MAZE_COUNT];
    GLuint _pathBuffs[BUFF_PATH_COUNT];
    
    glm::mat4 _actProj;
    glm::mat4 _testProj;
    
    glm::vec3 _wallColor;
    glm::vec3 _gridColor;
    glm::vec3 _pathColor;
    
    GLfloat _pathTime;
    
    MazeCellVert *_wallVerts;
    MInd _wVertCount;
    
    MazeCellVert *_pathVerts;
    MInd _pathVertsCount;
    MInd _pathBuffSize;
    
    GLuint _wallProg;
    GLuint _pathProg;
    GLuint _passThruProg;
    GLuint _gridProg;

    void InitMazeBuffs();
    void InitPathBuffs();
    
    void DrawWalls();
    void DrawGrid();
    void DrawPath();
    //void DrawTest();

    void StepToEdge(MazeCellVert & mcv, MazeBuilder::DIRECTIONS dir);
    
};
