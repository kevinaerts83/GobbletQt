#include "gobbler3d.h"

Gobbler3d::Gobbler3d(double size) : Shape3d()
{
    double tileSize = size;
    double w = (tileSize - 5.0) / 2;
    double h = (tileSize * 1.3);
    double d = w;

    /**
     * define 8 points of the cube
     * @property points
     * @type array
     *
     *       	6#######5
     *       	##      -#
     *       	# #     - #
     *       	#  7#######4
     *       	#  #    -  #
     *       	#  #    -  #
     *       	#  #    -  #
     *       	2--#----1  #
     *       	 # #     - #
     *       	  ##      -#
     *       	   3#######0
    */
    this->points = {
        {w, 0, d / 2, 1},
        {w / 2, 0, d, 1},
        {-w / 2, 0, d, 1},
        {-w, 0, d / 2, 1},
        {-w, 0, -d / 2, 1},
        {-w / 2, 0, -d, 1},
        {w / 2, 0, -d, 1},
        {w, 0, -d / 2, 1},

        {w / 1.5, -h, d / 3, 1},
        {w / 3, -h, d / 1.5, 1},
        {-w / 3, -h, d / 1.5, 1},
        {-w / 1.5, -h, d / 3, 1},
        {-w / 1.5, -h, -d / 3, 1},
        {-w / 3, -h, -d / 1.5, 1},
        {w / 3, -h, -d / 1.5, 1},
        {w / 1.5, -h, -d / 3, 1}
        //0, -(h + 3), 0, 1} // top
    };

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
    this->faces = {
        {1, 0, 8, 0}, // face0_b1
        {8, 9, 1, 0},
        {2, 1, 9, 0}, // face1_b2
        {9, 10, 2, 0},

        {3, 2, 10, 0}, // face4_r1
        {10, 11, 3, 0},
        {4, 3, 11, 0}, // face5_r2
        {11, 12, 4, 0},

        {5, 4, 12, 0}, // face8_l1
        {12, 13, 5, 0},
        {6, 5, 13, 0}, // face9_l2
        {13, 14, 6, 0},

        {7, 6, 14, 1}, // face10_f1
        {14, 15, 7, 1},
        {0, 7, 15, 1}, // face11_f2
        {15, 8, 0, 1}

        /*{8, 16, 9, 0},
        {9, 16, 10, 0},
        {10, 16, 11, 0},
        {11, 16, 12, 0},
        {12, 16, 13, 0},
        {13, 16, 14, 0},
        {14, 16, 15, 0},
        {15, 16, 8, 0}*/
    };
}
