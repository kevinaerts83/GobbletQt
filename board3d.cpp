#include "board3d.h"

Board3d::Board3d() : Shape3d(800.0, 600.0)
{
    //this.matrix = matrix;
    //this.tileSize = s;
    //this.tileCenters = [];

    int s = 20;
    int points[29][4] = {{-2*s,0,-2*s,1}, {-s,0,-2*s,1}, {0,0,-2*s,1}, {s,0,-2*s,1}, {2*s,0,-2*s,1},
                         {-2*s,0,-s,1}, {-s,0,-s,1}, {0,0,-s,1}, {s,0,-s,1}, {2*s,0,-s,1},
                         {-2*s,0,0,1}, {-s,0,0,1}, {0,0,0,1}, {s,0,0,1}, {2*s,0,0,1},
                         {-2*s,0,s,1}, {-s,0,s,1}, {0,0,s,1}, {s,0,s,1}, {2*s,0,s,1},
                         {-2*s,0,2*s,1}, {-s,0,2*s,1}, {0,0,2*s,1}, {s,0,2*s,1}, {2*s,0,2*s,1},
                         {-2*s,10,-2*s,1}, {2*s,10,-2*s,1},
                         {-2*s,10,2*s,1}, {2*s,10,2*s,1}};

    int faces[26][4] = {{4, 26, 25, 0}, {25, 0, 4, 0},//back
        {28, 26, 4, 0}, {4, 24, 28, 0},//right
        {0, 25, 27, 0}, {27, 20, 0, 0},//left
        {24, 20, 27, 0}, {27, 28, 24, 0},//front
        {20, 24, 4, 1}, {4, 0, 20, 1},
        {5 ,6, 1, 1}, {1, 0, 5, 1}, {7, 8, 3, 1}, {3, 2, 7, 1},
        {11, 12, 7, 1}, {7, 6, 11, 1}, {13, 14, 9, 1}, {9, 8, 13, 1},
        {15, 16, 11, 1}, {11, 10, 15, 1}, {17, 18, 13, 1}, {13, 12, 17, 1},
        {21, 22, 17, 1}, {17, 16, 21, 1}, {23, 24, 19, 1}, {19, 18, 23, 1}};//up

    double tileCenters[19][4] = {{-1.5*s,0,-1.5*s,1}, {-0.5*s,0,-1.5*s,1}, {0.5*s,0,-1.5*s,1}, {1.5*s,0,-1.5*s,1},
        {-1.5*s,0,-0.5*s,1}, {-0.5*s,0,-0.5*s,1}, {0.5*s,0,-0.5*s,1}, {1.5*s,0,-0.5*s,1},
        {-1.5*s,0,0.5*s,1}, {-0.5*s,0,0.5*s,1}, {0.5*s,0,0.5*s,1}, {1.5*s,0,0.5*s,1},
        {-1.5*s,0,1.5*s,1}, {-0.5*s,0,1.5*s,1}, {0.5*s,0,1.5*s,1}, {1.5*s,0,1.5*s,1}};

    /*this.facesColour = new Array(41);
    for(int i = 0; i < 8; i+=1) {
        this.facesColour[i] = 'white';//'black';
    }
    this.facesColour[8] ='#00aaaa';//'tan';
    this.facesColour[9] ='#00aaaa';//'tan';
    for(int i = 10; i < 26; i+=1) {
        this.facesColour[i] = '#004848';//'saddlebrown';
    }*/
}

/*
isValidPosition = function(x, y) {
    var p, padding, padding2, xBoard, dBoard,
        i = 0,
        j = 0;

    if(x > this.points[4][0]) {
        return -1;
    }

    if( x > this.points[3][0]) {
        i = 3;
    }
    else if( x > this.points[2][0]) {
        i = 2;
    }
    else if( x > this.points[1][0]) {
        i = 1;
    }
    else if( x > this.points[0][0]) {
        i = 0;
    }
    else {
        return -1;
    }

    if(y > this.points[20][2]) {
        return -1;
    }

    if(y > this.points[15][2]) {
        j = 15;
    }
    else if(y > this.points[10][2]) {
        j = 10;
    }
    else if(y > this.points[5][2]) {
        j = 5;
    }
    else if(y > this.points[0][2]) {
        j = 0;
    }
    else {
        return -1;
    }
    p = i + j;
    padding = this.tileSize / 4;
    padding2 = this.tileSize - padding;
    xBoard = this.points[p][0];
    dBoard = this.points[p][2];

    if(x < (xBoard + padding) || y < (dBoard + padding)) {
        return -1;
    }
    if(x > (xBoard + padding2) || y > (dBoard + padding2)) {
        return -1;
    }
    return p;
};*/

