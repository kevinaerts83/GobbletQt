/**
 * Shape3D class
 * @class Shape3D
 * @constructor
 * @namespace GOBBLET
 */
function Shape3D() {
    this.points = [];
    //cached points are rotated and scaled
    this.cache = [];
    this.aHashMap = { 0:1, 1:2, 2:4, 3:8, 5:16, 6:32, 7:64, 8:128, 10:256, 11:512, 12:1024, 13:2048, 15:4096, 16:8192, 17:16384, 18:32768};
};

/**
 * Rotate all the points of the shape
 * @method rotate
 * @namespace GOBBLET
 * @param {array} rotationMatrix to rotate points
 */
Shape3D.prototype.rotate = function(rotationMatrix) {
    var i, l, rotatedPoints = [];
    for (i = 0, l = this.points.length; i < l; i+=1) {
        rotatedPoints[i] = this.matrix.multiplyPointAndMatrix(this.points[i], rotationMatrix);
    }
    this.cache[0] = rotatedPoints;
};

/**
 * Make smaller cubes
 * @method zoom
 * @namespace GOBBLET
 * @param {array} scalingMatrix to zoom points
 */
Shape3D.prototype.zoom = function(scalingMatrix, size) {
    var i, l, scaledPoints = [];
    for (i = 0, l = this.points.length; i < l; i+=1) {
        scaledPoints[i] = this.matrix.multiplyPointAndMatrix(this.cache[(size ? size - 1: 0)][i], scalingMatrix);
    }
    this.cache[size] = scaledPoints;
};

/**
 * Cube class
 * @class Cube
 * @namespace GOBBLET
 * @param {int} s tileSize size of one board tile
 * @param {array} matrix Our matrix object with defined center
 */
function Board3D(s, matrix) {
    this.matrix = matrix;
    this.tileSize = s;
    this.tileCenters = [];
    /*this.w = s*4;
    this.h = 10;*/

    /**
     * define points of the board
     * @property points
     * @type array
    */
    this.points[0] = [-2*s,0,-2*s,1];
    this.points[1] = [-s,0,-2*s,1];
    this.points[2] = [0,0,-2*s,1];
    this.points[3] = [s,0,-2*s,1];
    this.points[4] = [2*s,0,-2*s,1];

    this.points[5] = [-2*s,0,-s,1];
    this.points[6] = [-s,0,-s,1];
    this.points[7] = [0,0,-s,1];
    this.points[8] = [s,0,-s,1];
    this.points[9] = [2*s,0,-s,1];

    this.points[10] = [-2*s,0,0,1];
    this.points[11] = [-s,0,0,1];
    this.points[12] = [0,0,0,1];
    this.points[13] = [s,0,0,1];
    this.points[14] = [2*s,0,0,1];

    this.points[15] = [-2*s,0,s,1];
    this.points[16] = [-s,0,s,1];
    this.points[17] = [0,0,s,1];
    this.points[18] = [s,0,s,1];
    this.points[19] = [2*s,0,s,1];

    this.points[20] = [-2*s,0,2*s,1];
    this.points[21] = [-s,0,2*s,1];
    this.points[22] = [0,0,2*s,1];
    this.points[23] = [s,0,2*s,1];
    this.points[24] = [2*s,0,2*s,1];

    this.points[25] = [-2*s,10,-2*s,1];
    this.points[26] = [2*s,10,-2*s,1];
    this.points[27] = [-2*s,10,2*s,1];
    this.points[28] = [2*s,10,2*s,1];

    /**
     * define 12 faces of the cube
     * @property faces
     * @type array
     */
    this.faces = [];

    //back
    this.faces[0] = [4, 26, 25, 0];
    this.faces[1] = [25, 0, 4, 0];
    //right
    this.faces[2] = [28, 26, 4, 0];
    this.faces[3] = [4, 24, 28, 0];
    //left
    this.faces[4] = [0, 25, 27, 0];
    this.faces[5] = [27, 20, 0, 0];
    //front
    this.faces[6] = [24, 20, 27, 0];
    this.faces[7] = [27, 28, 24, 0];
    //down
    /*this.faces[36] = [28, 24, 23];
    this.faces[37] = [23, 27, 28];*/

    //up
    this.faces[8] = [20, 24, 4, 1];
    this.faces[9] = [4, 0, 20, 1];

    this.faces[10] = [5 ,6, 1, 1];
    this.faces[11] = [1, 0, 5, 1];
    this.faces[12] = [7, 8, 3, 1];
    this.faces[13] = [3, 2, 7, 1];

    this.faces[14] = [11, 12, 7, 1];
    this.faces[15] = [7, 6, 11, 1];
    this.faces[16] = [13, 14, 9, 1];
    this.faces[17] = [9, 8, 13, 1];

    this.faces[18] = [15, 16, 11, 1];
    this.faces[19] = [11, 10, 15, 1];
    this.faces[20] = [17, 18, 13, 1];
    this.faces[21] = [13, 12, 17, 1];

    this.faces[22] = [21, 22, 17, 1];
    this.faces[23] = [17, 16, 21, 1];
    this.faces[24] = [23, 24, 19, 1];
    this.faces[25] = [19, 18, 23, 1];

    /**
     * store tileCenters to position pawns
     * @property tileCenters
     * @type array
     */
    this.tileCenters[0] = [-1.5*s,0,-1.5*s,1];
    this.tileCenters[1] = [-0.5*s,0,-1.5*s,1];
    this.tileCenters[2] = [0.5*s,0,-1.5*s,1];
    this.tileCenters[3] = [1.5*s,0,-1.5*s,1];

    this.tileCenters[5] = [-1.5*s,0,-0.5*s,1];
    this.tileCenters[6] = [-0.5*s,0,-0.5*s,1];
    this.tileCenters[7] = [0.5*s,0,-0.5*s,1];
    this.tileCenters[8] = [1.5*s,0,-0.5*s,1];

    this.tileCenters[10] = [-1.5*s,0,0.5*s,1];
    this.tileCenters[11] = [-0.5*s,0,0.5*s,1];
    this.tileCenters[12] = [0.5*s,0,0.5*s,1];
    this.tileCenters[13] = [1.5*s,0,0.5*s,1];

    this.tileCenters[15] = [-1.5*s,0,1.5*s,1];
    this.tileCenters[16] = [-0.5*s,0,1.5*s,1];
    this.tileCenters[17] = [0.5*s,0,1.5*s,1];
    this.tileCenters[18] = [1.5*s,0,1.5*s,1];

    /**
     * define a colour for every face
     * @property facesColour
     * @type array
     */
    var i = 0;
    this.facesColour = new Array(41);
    for(i = 0; i < 8; i+=1) {
        this.facesColour[i] = 'white';//'black';
    }
    this.facesColour[8] ='#00aaaa';//'tan';
    this.facesColour[9] ='#00aaaa';//'tan';
    for(i = 10; i < 26; i+=1) {
        this.facesColour[i] = '#004848';//'saddlebrown';
    }
};

/**
 * inherit from Shape3D class
 * @method prototype
 */
Board3D.prototype = new Shape3D();

/**
* Pawn wants to move to a new position on the board
* check if this is a valid position
* @method isValidPosition
* @return the index of the chosen board box
*/
Board3D.prototype.isValidPosition = function(x, y) {
    var p, padding, padding2, xBoard, dBoard,
        i = 0,
        j = 0;
    /*
    #0##1##2##3 ##4
    ########### ###
    #5##6##7##8 ##9
    ########### ###
    10#11#12#13 #14
    ########### ###
    15#16#17#18 #19

    ########### ###
    20#21#22#23 #24
    */
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
};

/**
 * Cube class
 * @class Cube
 * @param {int} tileSize size of one board plane
 * @param {array} matrix Our matrix object with defined center
 */
function Cube(tileSize, matrix) {
    var face0_b1, face1_b2, face4_r1, face5_r2, face6_u1, face7_u2, face8_l1, face9_l2, face10_f1, face11_f2;
    var w = (((tileSize - 5) / 2) >> 0), h = (((tileSize * 1.3) / 2) >> 0), d = w;

    this.w = w;
    this.h = h;
    this.d = d;

    /**
     * define 8 points of the cube
     * @property points
     * @type array
     *
     *       	4#######5
     *       	##      -#
     *       	# #     - #
     *       	#  7#######6
     *       	#  #    -  #
     *       	#  #    -  #
     *       	#  #    -  #
     *       	0--#----1  #
     *       	 # #     - #
     *       	  ##      -#
     *       	   3#######2
    */
    this.points[0] = [w,h,d,1];
    this.points[1] = [-w,h,d,1];
    this.points[2] = [-w,h,-d,1];
    this.points[3] = [w,h,-d,1];
    this.points[4] = [w/2,-h,d/2,1];
    this.points[5] = [-w/2,-h,d/2,1];
    this.points[6] = [-w/2,-h,-d/2,1];
    this.points[7] = [w/2,-h,-d/2,1];

    /**
     * matrix constructed with the center of the cube
     * @property matrix
     * @type object
     */
    this.matrix = matrix;

    /*
     * Combine all points with triangles faces so a cube will be formed.
     * If the dot product of the 2 vectors of this triangular is smaller than zero
     * (points away from the center) the face is visible.
     * To make this work:
     * - Move around the shape and look at triangular when it is visible.
     * - Now add the points to the array of the shape counterclockwise.
     * - To calculate the dot vector take point 0 of the triangular and draw a vector to point 1
     *   this vector will point counterclockwise. The second vector is from point 0 to point 2 and will point clockwise.
     * EXAMPLE:
     * the front face: points 2 - 7 - 3 (counterclockwise). vector1= 2-7; vector2 = 2-3
     * the back face: points 1 - 0 - 4 (counterclockwise) look at it from the back. vector1= 1-0; vector2 = 1-4
     * Now the right hand rule says (vector1 = index finger, vector2 = middle finger; dot product = thumb)
     * With the front face the thumb points away from the center, and with the back face the thumb points to the center.
     */
    //back
    face0_b1 = [1, 0, 4, 0];
    face1_b2 = [4, 5, 1, 0];
    //down
    //  var face2_d1 = [2, 3, 0];
    //  var face3_d2 = [0, 1, 2];
    //right
    face4_r1 = [2, 1, 5, 0];
    face5_r2 = [5, 6, 2, 0];
    //up
    face6_u1 = [7, 6, 5, 0];
    face7_u2 = [5, 4, 7, 0];
    //left
    face8_l1 = [0, 3, 7, 0];
    face9_l2 = [7, 4, 0, 0];
    //front
    face10_f1 = [3, 2, 6, 1];
    face11_f2 = [6, 7, 3, 1];

    this.faces = [face0_b1, face1_b2, face4_r1, face5_r2, face6_u1, face7_u2, face8_l1, face9_l2, face10_f1, face11_f2];//, face2_d1, face3_d2
};

/**
 * inherit from Shape3D class
 * @method prototype
 */
Cube.prototype = new Shape3D();

/**
 * Shape class
 * @class Shape
 * @constructor
 * @namespace GOBBLET
 */
function Shape() {
    this.percent = 0;
};

/**
* Translate 3d shape to it's 2d position and shape
* @method translate
*/
Shape.prototype.translateAndProject = function() {
    this.points_2d = [];
    var i, l, translation,
        isPawn = false,
        c = 0;
    if(this.size !== undefined) {
        isPawn = true;
        c = this.size;
    }
    if(!isPawn || this.visible === true) {
        translation = this.matrix.getTranslation(this.centerPoint2D[0], this.centerPoint2D[1], this.centerPoint2D[2]);
        for (i = 0, l = this.shape3D.cache[c].length; i < l; i+=1) {
            this.points_2d[i] = this.matrix.projectPoint(this.matrix.multiplyPointAndMatrix(this.shape3D.cache[c][i], translation));
            if(isPawn) {
                this.setBoundaries(this.points_2d[i], i===0);
            }
        }
    }
};

/**
 * Darker color
 * @method darkerColor
 * @namespace GOBBLET
 */
Shape.prototype.darkerColor = function(color, prc) {
    if(prc !== 0) {
        var num = parseInt(color.slice(1),16),
            amt = Math.round(2.55 * prc),
            R = (num >> 16) + amt,
            G = (num >> 8 & 0x00FF) + amt,
            B = (num & 0x0000FF) + amt;
        return "#" + (0x1000000 + (R<255?R<1?0:R:255)*0x10000 + (G<255?G<1?0:G:255)*0x100 + (B<255?B<1?0:B:255)).toString(16).slice(1);
    }
    return color;
};

/**
 * Draw shape
 * @method draw
 * @namespace GOBBLET
 * @param {2d context} ctx The context
 */
Shape.prototype.draw = function(ctx, isSelection) {
    ctx.save();
    if(ctx && this.points_2d.length > 0 && (this.visible === undefined || this.visible === true)) {
        var i, l, face, colourLength = this.facesColour.length-1;
        ctx.lineWidth = 1;
        if(isSelection) {
            ctx.strokeStyle = this.darkerColor(this.facesColour[0], this.percent*2);
        } else {
            ctx.strokeStyle = (this.facesColour.length > 1) ? '#000' : this.darkerColor(this.facesColour[0], this.percent);
        }
        ctx.lineWidth= (this.facesColour.length > 1) ? 1 : 2;
        for (i = 0, l = this.shape3D.faces.length; i < l; i+=1) {
            face = this.shape3D.faces[i];

            face[3] = this.dotProduct(face, this.points_2d) ? 1 : 0;
            if(face[3] === 1) {
                ctx.fillStyle = this.facesColour[i > colourLength ? colourLength : i];
                ctx.beginPath();
                ctx.moveTo(this.points_2d[face[0]][0],this.points_2d[face[0]][1]);
                ctx.lineTo(this.points_2d[face[1]][0],this.points_2d[face[1]][1]);
                ctx.lineTo(this.points_2d[face[2]][0],this.points_2d[face[2]][1]);
                ctx.stroke();

                //SELECTION
                /*ctx.shadowColor = '#ffffff';
                ctx.shadowBlur = 40;
                ctx.shadowOffsetX = 0;
                ctx.shadowOffsetY = 0;*/

                ctx.fill();
            }
        }

        /*REMOVE
        ctx.rect(this.minX, this.minY, this.maxX-this.minX, this.maxY-this.minY);
        ctx.stroke();

        ctx.fillStyle = '#000';
        ctx.font = 'italic bold 10px sans-serif';
        for(var i = 0, l = this.points_2d.length; i < l; i++) {
            ctx.fillText(i, this.points_2d[i][0], this.points_2d[i][1]);
        }
        //END*/
    }
    ctx.restore();
};

/**
 * Calculate the dot product of the face
 * dot product => determine how similar two vectors point to the same point (same direction = 1;  90 degrees = 0; opposite direction = -1)
 * or simply put the cosine between two vectors
 * @method dotProduct
 * @namespace GOBBLET
 * @param {array} points The 2d points of the shape
 * @param {array} face An array with indexes of the points of the face
 */
Shape.prototype.dotProduct = function(face, p) {
    var a1 = p[face[1]][0] - p[face[0]][0],
        a2 = p[face[1]][1] - p[face[0]][1],
        b1 = p[face[2]][0] - p[face[0]][0],
        b2 = p[face[2]][1] - p[face[0]][1];

    return (a1*b2 - a2*b1) < 0;
};

/**
 * Determine the boundaries of a shape
 * @method contains
 */
Shape.prototype.setBoundaries = function(point2d, first) {
    if(first) {
        this.minX = point2d[0];
        this.minY = point2d[1];
        this.maxX = point2d[0];
        this.maxY = point2d[1];
    }
    else {
        if(point2d[0] < this.minX) {
            this.minX = point2d[0];
        }
        if(point2d[0] > this.maxX) {
            this.maxX = point2d[0];
        }
        if(point2d[1] < this.minY) {
            this.minY = point2d[1];
        }
        if(point2d[1] > this.maxY) {
            this.maxY = point2d[1];
        }
    }
};

/**
 * Determine if a point is inside the shape's bounds
 * @method contains
 */
Shape.prototype.contains = function(mx, my) {
  // All we have to do is make sure the Mouse X,Y fall in the area between
  // the shape's X and (X + Height) and its Y and (Y + Height)
  if((this.minX <= mx) && (this.maxX >= mx) && (this.minY <= my) && (this.maxY >= my)) {
      this.mouseX = mx;
      this.mouseY = my;
      return true;
  }

  this.mouseX = -1;
  this.mouseY = -1;
  return false;
};

/**
 * Pawn class
 * @class Pawn
 * @constructor
 * @namespace GOBBLET
 * @param {matrix} matrix Our matrix object with defined center
 * @param {float} x x-axis value
 * @param {float} y y-axis value
 * @param {float} z z-axis value
 * @param {float} d depth of the pawn
 * @param {int} size 0 = biggest pawn, 1 = normal pawn, ...
 */
function Pawn(matrix, x, y, z, h, d, size, isTurn, n) {
    this.matrix = matrix;
    this.depth = d;
    this.pHeight = Math.floor(h * Math.pow(0.7, size));
    this.size = size;
    this.visible = (size % 4 === 0);
    this.setInvisible = false;
    this.onTheBoard = false;
    this.endX = x;
    this.endZ = z;
    this.enable = true;
    this.isTurn = isTurn;
    this.tile = -1;

    y -= this.pHeight;
    this.centerPoint = [x, y, z, 1];
    this.centerPoint2D = [x, y, z, 1];

    this.mouseX = 0;
    this.mouseY = 0;
    this.z_index = z + d / 2;
    this.id = n;
};

/**
 * inherit from Shape class
 * @method prototype
 */
Pawn.prototype = new Shape();

/**
 * scale and rotate the center point of the shape
 * @method zoomAndRotate
 * @namespace GOBBLET
 */
Pawn.prototype.zoomAndRotate = function(scale, rotMatrix) {
    if(this.visible) {
        var p = [this.centerPoint[0], this.centerPoint[1], this.centerPoint[2], 1];
        if(rotMatrix) {
            p = this.matrix.multiplyPointAndMatrix(p, this.matrix.translation);
            p = this.matrix.multiplyPointAndMatrix(p, this.matrix.getScaling(scale /100.0));
            p = this.matrix.multiplyPointAndMatrix(p, rotMatrix);
            p = this.matrix.multiplyPointAndMatrix(p, this.matrix.inverseTranslation);
        }
        this.z_index = p[2] + (this.depth / 2);
        this.centerPoint2D = p;
    }
};

/**
* Move selected pawn according to the mouse movement
* @method translate
*/
Pawn.prototype.move = function(newX, newY, angle) {
     var sine = Math.round(Math.sin(Math.PI * angle/180)),
     cosine = Math.round(Math.cos(Math.PI * angle/180)),
     tx = (newX - this.mouseX),
     ty = (newY - this.mouseY),
     x = tx*cosine - ty*sine,
     y = tx*sine + ty*cosine;

    this.centerPoint[0] += x;
    this.centerPoint[2] += y;

    this.mouseX = newX;
    this.mouseY = newY;
};

/**
* Lift the pawn on mouse up, drop on mouse down
* @method lift
*/
Pawn.prototype.lift = function(up) {
    this.centerPoint[1] += up;
};
/**
* Place the pawn onto the board
* @method endMove
*/
Pawn.prototype.endMove = function(h) {
    this.centerPoint[1] = h - this.pHeight;
};

/**
* Validate after another pawn has moved
* onDown mouse => isDown = false; isDownValid = true; isOnBoard = true;
* onUp mouse (Invalid move) => isDown = true; isDownValid = false; isOnBoard = check if selected pawn came from stack or board;
* movePawn (valid move) => isDown = true; isDownValid = true; isOnBoard = true;
* @method notification
*/
Pawn.prototype.notification = function(args) {
    var i,
        isDown = args[0],
        isDownValid = args[1],
        bState = args[2],
        isOnBoard = args[3],
        found = false;
    if(this.tile > -1) {
        var t = this.shape3D.aHashMap[this.tile];
        for(i=0; i < this.size; i++) {
            if((bState[i] & t) === t) {
                found = true;
                this.setInvisible = true;
                break;
            }
        }
        if(!found) {
            this.visible = true;
        }
    }
    if(isDown) {
        if(isDownValid) {
            this.isTurn ^= 1;
        }
        if(isOnBoard) {
            this.enable = isDownValid;
        }
    }
};
/**
* Finish event settings
* @method endEvent
*/
Pawn.prototype.endEvent = function(args) {
    if(this.setInvisible) {
        this.visible = false;
        this.setInvisible = false;
    }
};

/**
 * Board class
 * @class Board
 * @namespace GOBBLET
 * @param {array} matrix Our matrix object with defined center
 */
function Board(matrix, x, y, z) {
    this.matrix = matrix;
    this.centerPoint = [x, y, z, 1];
    this.centerPoint2D = [x, y, z, 1];
};

/**
 * inherit from Shape class
 * @method prototype
 */
Board.prototype = new Shape();
