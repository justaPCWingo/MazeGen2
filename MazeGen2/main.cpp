//
//  main.cpp
//  MazeGen2
//
//  Created by Patrick  Wingo on 7/8/17.
//  Copyright © 2017 Wingo. All rights reserved.
//

#include <iostream>
#include <sstream>

#include "MazeBuilder.h"

void PrintMaze(MazeBuilder & b);

int main(int argc, const char * argv[]) {

    MazeBuilder builder(16,16);
    //builder.SetDebugPrint(PrintMaze);
    builder.GenerateMaze();
    PrintMaze(builder);
    return 0;
}

void PrintMaze(MazeBuilder & b)
{
    using namespace std;
    for(MInd r=0; r<b.GetRowCount();++r)
    {
        ostringstream upStr;
        ostringstream downStr;
        
        unsigned short cWalls=MazeBuilder::NO_DIR;
        for(MInd c=0;c<b.GetColCount();++c)
        {
            cWalls=b.WallsForCell(r, c);
            upStr<<' '<<(cWalls & MazeBuilder::UP_DIR ? '-' : ' ');
            downStr<<(cWalls & MazeBuilder::LEFT_DIR ? '|' : ' ')<<' ';
        }
        //use last cell to check for last cell
        downStr<<(cWalls & MazeBuilder::RIGHT_DIR ? '|' : ' ')<<' ';
        cout<<upStr.str()<<endl<<downStr.str()<<endl;
    }
    
    //final row
    for(MInd i=(b.GetRowCount()-1)*b.GetColCount(); i<b.GetRowCount()*b.GetColCount();++i)
        cout<<' '<<(b.WallsForCell(i) & MazeBuilder::DOWN_DIR ? '-' : ' ');
    cout<<endl;
}
