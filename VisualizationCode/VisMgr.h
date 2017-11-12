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
    VisMgr(const std::string & shadDir,GLint x,GLint y,GLint width,GLint height);
    
    void InitForOpenGL();
    void Draw();
    void NewMaze(MazeBuilder* bldr);
    //void InitTest();
    void RefreshWithMaze(MazeBuilder* bldr);
    void BuildPath(MazeBuilder* bldr);
    
    void StepInDirection(MazeCellVert & mcv, const MazeBuilder::DIRECTIONS & dir,const GLfloat & stepSize);
    
    void SetPathTime(const GLfloat & currTime);
    GLfloat GetPathTime() const;
    
    void SetViewport(GLint x,GLint y,GLint width,GLint height);
    
    //getters for display properties
    glm::vec3 GetGridColor() const;
    glm::vec3 GetWallColor() const;
    glm::vec3 GetPathColor() const;
    glm::vec3 GetDecayedPathColor() const;
    bool GetShowFullPath() const;
    bool GetShowPathDecay() const;
    float GetDecayDelay() const;
    
    //setters for display properties
    void SetGridColor(const glm::vec3 & gc);
    void SetWallColor(const glm::vec3 & wc);
    void SetPathColor(const glm::vec3 & pc);
    void SetDecayedPathColor(const glm::vec3 & dpc);
    void SetShowFullPath(bool showFull);
    void SetShowPathDecay(bool showDecay);
    void SetDecayDelay(float delay);
    
    bool ReadyToDraw() const;
    
    //debug utilties
    static void DbgDumpMaze(MazeBuilder* bldr);
private:
    
    enum {
        VAO_MAZE=0,
        //VAO_TEST,
        VAO_PATH,
        VAO_COMPOSITE,
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
    
    enum
    {
        TEX_128=0,
        TEX_64,
        TEX_32,
        TEX_16,
        
        TEX_COUNT
    };
    
    ShaderMgr _shdMgr;
    
    GLuint _vaos[VAO_COUNT];
    GLuint _mazeBuffs[BUFF_MAZE_COUNT];
    GLuint _pathBuffs[BUFF_PATH_COUNT];
    GLuint _textures1[TEX_COUNT];
    GLuint _textures2[TEX_COUNT];
    GLuint _fbos1[TEX_COUNT];
    GLuint _fbos2[TEX_COUNT];
    
    GLuint _compTexture;
    GLuint _compFBO;
    GLuint _depthBuff;
    
    glm::mat4 _actProj;
    glm::mat4 _testProj;
    
    glm::vec3 _wallColor;
    glm::vec3 _gridColor;
    glm::vec3 _pathColor;
    glm::vec3 _decayPathColor;
    
    bool _showPath;
    bool _showDecay;
    
    float _decayDelay;
    GLfloat _glowIntensity;
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
    GLuint _blurProg;
    GLuint _combineProg;
    GLuint _compositeProg;
    GLuint _compositeBuff;

    void InitMazeBuffs();
    void InitPathBuffs();
    void InitCompositeBuff();
    
    void CreateRenderFBO(GLuint & fbo,GLuint & tex,GLuint xdim,GLuint ydim);
    
    void DrawMaze(bool isBloom);
    void DrawWalls();
    void DrawGrid();
    void DrawPath(bool isBloom);
    void DrawBlur(GLfloat xOff,GLfloat yOff,const GLfloat* kernel,const unsigned int & kLen,GLuint inTex);
    void DrawCombine(GLuint* texes);
    void DrawComposite();
    //void DrawTest();

    //viewport boundries
    GLint _vpX;
    GLint _vpY;
    GLint _vpWidth;
    GLint _vpHeight;
    
    void StepToEdge(MazeCellVert & mcv, MazeBuilder::DIRECTIONS dir);
    
};
