//
//  MazeBuilder.cpp
//  MazeGen2
//
//  Created by Patrick  Wingo on 7/8/17.
//  Copyright (c) 2017 Wingo. All rights reserved.
//

#include "MazeBuilder.h"
#include<chrono>
#include<limits>

const MInd MazeBuilder::NO_VAL=std::numeric_limits<MInd>::max();
const MIndPair MazeBuilder::NO_PAIR_VAL=MIndPair(MazeBuilder::NO_VAL,MazeBuilder::NO_VAL);
#ifndef MB_DISABLE_RANDOM
std::default_random_engine MazeBuilder::s_randGen(std::chrono::system_clock::now().time_since_epoch().count());
#else
std::default_random_engine MazeBuilder::s_randGen;
#endif

MazeBuilder::MazeBuilder(MInd rows, MInd cols)
:_rows(rows)
,_cols(cols)
,_startInd(NO_VAL)
,_finishInd(NO_VAL)
,_DbgPrint(nullptr)

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
    
    uniform_int_distribution<MInd> hDistrib(0,_horizWalls.size()-1);
    uniform_int_distribution<MInd> vDistrib(0,_vertWalls.size()-1);
    
    //seed with random walls; for now, seed with picks 1/2 of dimensions.
    for(MInd i=0; i<_rows*4; ++i)
        _horizWalls[hDistrib(s_randGen)]=true;
    
    for(MInd i=0; i<_cols*4; ++i)
        _vertWalls[vDistrib(s_randGen)]=true;
    
    /*
     from here, keep tweaking until the three criteria are met:
     
     1. single path to start-->finish (should be achieved with other criteria)
     2. No loops.
     3. No unreachable cells.
     */
    
    bool go=true;
    do
    {
        if(_DbgPrint!=nullptr)
            _DbgPrint(*this);
        
        //check for unreachable cells
        MarkAccessibleCells();
        PathList unvisited=GetUnvisited();
        if(!unvisited.empty())
        {
            SetRandomCellWall(RandomCellFromList(unvisited), false);
            continue;
        }
        
        //check for loops
        MIndPair hit=FindFirstLoop();
        if(hit!=NO_PAIR_VAL)
        {
            DIRECTIONS sharedWall=CellsAreAdjacent(hit.first, hit.second);
            SetCellEdge(hit.first,sharedWall,true);
            continue;
        }
        
        //if we make it here, then we should have a valid maze. kill loop
        go=false;
    } while(go);
    
    //open edges
    OpenEdge(_startInd);
    OpenEdge(_finishInd);
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

PathList MazeBuilder::PathToFinish()
{
    PathList ret;
    ClearVisits();
    
    FindFinish(_startInd,ret);
    return ret;
}

void MazeBuilder::SetDebugPrint(DBG_MazePrint DbgFunc)
{
    _DbgPrint=DbgFunc;
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
        MInd cInd=i*_cols+i;
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

void MazeBuilder::ClearVisits()
{
    for(MInd i=0; i<_cellHits.size(); ++i)
        _cellHits[i]=false;
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
        case NO_DIR:
            //do nothing
            break;
    }
}

void MazeBuilder::SetRandomCellWall(MInd cInd, bool enable)
{
    //grab walls
    unsigned short checkWalls=WallsForCell(cInd) | MazeEdgesForCell(cInd);
    
    //invert check if looking for walls to enable
    if(enable)
        checkWalls= ~checkWalls;
    
    //mark edges as off-limits
    checkWalls&= ~MazeEdgesForCell(cInd);
    
    //check to see if there are no options.
    if(checkWalls==NO_DIR)
        return;
    
    //brute force;
    std::uniform_int_distribution<MInd> wDistrib(0,3);
    DIRECTIONS toChange=NO_DIR;
    do
    {
        unsigned short testDir=1 << wDistrib(s_randGen);
        if(testDir & checkWalls)
            toChange=static_cast<DIRECTIONS>(testDir);
    } while(toChange==NO_DIR);
    
    SetCellEdge(cInd,toChange,enable);
    
}

bool MazeBuilder::FindFinish(MInd cInd,PathList & path)
{
    bool pathFound=cInd==_finishInd;
    if (!pathFound && !_cellHits[cInd])
    {
        _cellHits[cInd]=true;
        
        //check the maze edge too just in case at start
        unsigned short walls=WallsForCell(cInd) | MazeEdgesForCell(cInd);
        
        if(!(walls & UP_DIR))
            pathFound=FindFinish(IndUp(cInd),path);
        if(!pathFound && !(walls & DOWN_DIR))
            pathFound=FindFinish(IndDown(cInd), path);
        if(!pathFound && !(walls & LEFT_DIR))
            pathFound=FindFinish(IndLeft(cInd), path);
        if(!pathFound && !(walls & RIGHT_DIR))
            pathFound=FindFinish(IndRight(cInd), path);
    }
    
    if(pathFound)
        path.push_front(cInd);
    return pathFound;
}

MazeBuilder::DIRECTIONS MazeBuilder::CellsAreAdjacent(MInd c1, MInd c2) const
{
    DIRECTIONS dir=NO_DIR;
    
    if(c1==c2-1)
        dir=RIGHT_DIR;
    else if (c1==c2+1)
        dir=LEFT_DIR;
    else if(c1==c2+_cols)
        dir=UP_DIR;
    else if(c1==c2-_cols)
        dir=DOWN_DIR;
    return dir;
}

void MazeBuilder::MarkAccessibleCells()
{
    ClearVisits();
    MarkAccessibleCells(_startInd);
}

void MazeBuilder::MarkAccessibleCells(MInd cInd)
{
    if(!_cellHits[cInd])
    {
        _cellHits[cInd]=true;
        
        unsigned short walls=WallsForCell(cInd) | MazeEdgesForCell(cInd);
        
        if(!(walls & UP_DIR))
            MarkAccessibleCells(IndUp(cInd));
        if(!(walls & DOWN_DIR))
            MarkAccessibleCells(IndDown(cInd));
        if(!(walls & LEFT_DIR))
            MarkAccessibleCells(IndLeft(cInd));
        if(!(walls & RIGHT_DIR))
            MarkAccessibleCells(IndRight(cInd));

    }
}

PathList MazeBuilder::GetUnvisited()
{
    PathList ret;
    
    for(MInd i=0; i<_cellHits.size();++i)
        if(!_cellHits[i])
            ret.push_back(i);
    
    return ret;
}

MIndPair MazeBuilder::FindFirstLoop(MInd cInd,MInd lastVisited)
{
    if(_cellHits[cInd])
        return MIndPair(cInd,lastVisited);
    
    _cellHits[cInd]=true;
    
    unsigned short walls=WallsForCell(cInd) | MazeEdgesForCell(cInd);
    
    MIndPair ret=NO_PAIR_VAL;
    MInd next=IndUp(cInd);
    if(!(walls & UP_DIR) && next!=lastVisited)
        ret=FindFirstLoop(next,cInd);
    
    next=IndDown(cInd);
    if(ret==NO_PAIR_VAL && !(walls & DOWN_DIR) && next!=lastVisited)
        ret=FindFirstLoop(next,cInd);
    
    next=IndLeft(cInd);
    if(ret==NO_PAIR_VAL && !(walls & LEFT_DIR) && next!=lastVisited)
        ret=FindFirstLoop(next,cInd);
    
    next=IndRight(cInd);
    if(ret==NO_PAIR_VAL && !(walls & RIGHT_DIR) && next!=lastVisited)
        ret=FindFirstLoop(next,cInd);
    
    return ret;
}

MIndPair MazeBuilder::FindFirstLoop()
{
    ClearVisits();
    return FindFirstLoop(_startInd);
}

MInd MazeBuilder::RandomCellFromList(const PathList & pl)
{
    using namespace std;
    
    if(pl.empty())
        return NO_VAL;
    
    uniform_int_distribution<MInd> pDistrib(0,pl.size()-1);
    
    return *(next(pl.begin(),pDistrib(s_randGen)));

}
