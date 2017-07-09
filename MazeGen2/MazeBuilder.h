//
//  MazeBuilder.h
//  MazeGen2
//
//  Created by Patrick  Wingo on 7/8/17.
//  Copyright (c) 2017 Wingo. All rights reserved.
//

#pragma once

#include<random>
#include<vector>

typedef unsigned int MInd;
typedef std::vector<bool> ActVec;

class MazeBuilder
{
public:

    enum DIRECTIONS : unsigned short
    {
        NO_DIR=0,
        UP_DIR=0x1,
        LEFT_DIR=0x2,
        DOWN_DIR=0x4,
        RIGHT_DIR=0x8
    };
    
    MazeBuilder(MInd rows, MInd cols);
    
    void GenerateMaze();
    virtual ~MazeBuilder() {};
    
    unsigned short WallsForCell(MInd currInd) const;
    unsigned short WallsForCell(MInd row, MInd col) const;
    unsigned short MazeEdgesForCell(MInd currInd) const;
    
    MInd GetStart() const;
    MInd GetFinish() const;

    MInd GetRowCount() const;
    MInd GetColCount() const;
    
protected:
    
    static const MInd NO_VAL;
    static std::default_random_engine s_randGen;
    
    MInd _rows;
    MInd _cols;
    
    MInd _startInd;
    MInd _finishInd;
    ActVec _horizWalls;
    ActVec _vertWalls;
    ActVec _cellHits;
    
    //manipulations
    void SealMazeEdges();
    void OpenEdge(MInd cInd);
    
    //lookup utils
    MInd EdgeNumToIndex(MInd eInd) const;
    void SetCellEdge(MInd cellInd,DIRECTIONS dir, bool value);
    inline MInd RCToIndex(MInd r, MInd c) const;
    inline MInd IndLeft(MInd currInd) const;
    inline MInd IndRight(MInd currInd) const;
    inline MInd IndUp(MInd currInd) const;
    inline MInd IndDown(MInd currInd) const;
    inline bool CanLeft(MInd currInd) const;
    inline bool CanRight(MInd currInd) const;
    inline bool CanUp(MInd currInd) const;
    inline bool CanDown(MInd currInd) const;
    inline bool CellsAreAdjacent(MInd c1, MInd c2) const;

};

// inlines
MInd MazeBuilder::RCToIndex(MInd r, MInd c) const
{
    return r*_cols+c;
}

MInd MazeBuilder::IndLeft(MInd currInd) const
{
    return currInd-1;
}

MInd MazeBuilder::IndRight(MInd currInd) const
{
    return currInd+1;
}

MInd MazeBuilder::IndUp(MInd currInd) const
{
    return currInd-_cols;
}

MInd MazeBuilder::IndDown(MInd currInd) const
{
    return currInd+_cols;
}

bool MazeBuilder::CanLeft(MInd currInd) const
{
    return _vertWalls[(currInd/_cols)+currInd];
}

bool MazeBuilder::CanRight(MInd currInd) const
{
    return _vertWalls[(currInd/_cols)+currInd+1];
}

bool MazeBuilder::CanUp(MInd currInd) const
{
    return _horizWalls[currInd];
}

bool MazeBuilder::CanDown(MInd currInd) const
{
    return _horizWalls[currInd+_cols];
}

bool MazeBuilder::CellsAreAdjacent(MInd c1, MInd c2) const
{
    return c1==c2-1 || c1==c2+1 || c1==c2+_cols || c1==c2-_cols;
}
