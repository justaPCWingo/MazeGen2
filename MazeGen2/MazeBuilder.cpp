//
//  MazeBuilder.cpp
//  MazeGen2
//
//  Created by Patrick  Wingo on 7/8/17.
//  Copyright (c) 2017 Wingo. All rights reserved.
//

#include "MazeBuilder.h"
#include<limits>

const MInd MazeBuilder::NO_VAL=std::numeric_limits<MInd>::max();
std::default_random_engine MazeBuilder::s_randGen;

MazeBuilder::MazeBuilder(MInd rows, MInd cols)
:_rows(rows)
,_cols(cols)
,_startInd(NO_VAL)
,_finishInd(NO_VAL)

{
    _cellHits.assign(rows*cols, false);
    _horizWalls.assign((rows+1)*cols,false);
    _vertWalls.assign(rows*(cols+1),false);

}

void MazeBuilder::GenerateMaze()
{
    //pick non-adjacent start and stop
    using namespace std;
    
    uniform_int_distribution<MInd> sfDistrib(0,(2*_rows+2*_cols)-4);
    
    do
    {
        _startInd=EdgeNumToIndex(sfDistrib(s_randGen));
        _finishInd=EdgeNumToIndex(sfDistrib(s_randGen));
        
    } while(_startInd==_finishInd || CellsAreAdjacent(_startInd, _finishInd));
    
    //prepare outside
    SealMazeEdges();
    
//    uniform_int_distribution<MInd> hDistrib(0,_horizWalls.size()-1);
//    uniform_int_distribution<MInd> vDistrib(0,_vertWalls.size()-1);
//    
//    //seed with random walls; for now, seed with picks 1/2 of dimensions.
//    for(MInd i=0; i<_rows / 2; ++i)
//        _horizWalls[hDistrib(s_randGen)]=true;
//    
//    for(MInd i=0; i<_cols / 2; ++i)
//        _vertWalls[vDistrib(s_randGen)]=true;
//    
//    /*
//     from here, keep tweaking until the three criteria are met:
//     
//     1. single path to start-->finish.
//     2. No loops.
//     3. No unreachable cells.
//     */
//    
//    //open edges
//    OpenEdge(_startInd);
//    OpenEdge(_finishInd);
}

unsigned short MazeBuilder::WallsForCell(MInd currInd) const
{
    unsigned short ret=NO_DIR;
    
    if(CanUp(currInd))
        ret|=UP_DIR;
    if(CanDown(currInd))
        ret|=DOWN_DIR;
    if(CanLeft(currInd))
        ret|=LEFT_DIR;
    if(CanRight(currInd))
        ret|=RIGHT_DIR;
    
    return ret;
}

unsigned short MazeBuilder::WallsForCell(MInd row, MInd col) const
{
    return WallsForCell(RCToIndex(row, col));
}

unsigned short MazeBuilder::MazeEdgesForCell(MInd currInd) const
{
    unsigned short ret=NO_DIR;
    
    if(currInd<_cols)
        ret|=UP_DIR;
    if(currInd>=(_rows-1)*_cols)
        ret|=DOWN_DIR;
    if(currInd % _cols==0)
        ret|=LEFT_DIR;
    if((currInd+1) % _cols==0)
        ret|=RIGHT_DIR;
    
    return ret;
}

MInd MazeBuilder::GetStart() const
{
    return _startInd;
}

MInd MazeBuilder::GetFinish() const
{
    return _finishInd;
}

MInd MazeBuilder::GetRowCount() const
{
    return _rows;
}

MInd MazeBuilder::GetColCount() const
{
    return _cols;
}



void MazeBuilder::SealMazeEdges()
{
    //top/bottom
    MInd bOffs=_rows*_cols;
    for(MInd i=0; i<_cols;++i)
    {
        _horizWalls[i]=true;
        _horizWalls[i+bOffs]=true;
    }
    
    //left/right
    for(MInd i=0;i<_rows;++i)
    {
        MInd cInd=i*_cols;
        _vertWalls[cInd]=true;
        _vertWalls[cInd+_cols]=true;
    }
}

void MazeBuilder::OpenEdge(MInd cInd)
{
    unsigned short hit=MazeEdgesForCell(cInd);
    
    //walk down possibilities (more than one if corner)
    if (hit!=NO_DIR)
    {
        if (hit & UP_DIR)
            SetCellEdge(cInd, UP_DIR, false);
        else if (hit & DOWN_DIR)
            SetCellEdge(cInd, DOWN_DIR, false);
        else if (hit & LEFT_DIR)
            SetCellEdge(cInd, LEFT_DIR, false);
        else if (hit & RIGHT_DIR)
            SetCellEdge(cInd, RIGHT_DIR, false);
    }
}

MInd MazeBuilder::EdgeNumToIndex(MInd eInd) const
{
    //number of edge cells
    MInd eCount=(2*_rows+2*_cols)-4;
    
    //if top
    if(eInd<_cols)
        return eInd;
    
    //if bottom
    MInd offset=eCount-_cols;
    if(eInd>=offset)
        return ((_rows-1)*_cols)+(eInd-offset);
    
    //intermediate edges
    offset=eInd-_cols;
    
    //find row
    MInd row=(offset / 2)+1;
    MInd col=offset % 2 ==1 ? 0 : _cols -1;
    
    return RCToIndex(row, col);
}

void MazeBuilder::SetCellEdge(MInd cellInd,MazeBuilder::DIRECTIONS dir, bool value)
{
    switch(dir)
    {
        case LEFT_DIR:
            _vertWalls[(cellInd/_cols)+cellInd]=value;
            break;
        case RIGHT_DIR:
            _vertWalls[(cellInd/_cols)+cellInd+1]=value;
            break;
        case UP_DIR:
            _horizWalls[cellInd]=value;
            break;
        case DOWN_DIR:
            _horizWalls[cellInd+_cols]=value;
            break;
    }
}
