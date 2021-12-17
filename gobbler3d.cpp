#include "gobbler3d.h"

gobbler3d::gobbler3d() : Shape3d(400.0, 300.0)
{
    double tileSize = 80;
    double w = (tileSize - 5.0) / 2;
    double h = (tileSize * 1.3) / 2;
    double d = w;

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
    this->points = {
        {w, h, d, 1},
        {-w, h, d, 1},
        {-w, h, -d, 1},
        {w, h, -d, 1},
        {w/2, -h, d/2, 1},
        {-w/2, -h, d/2, 1},
        {-w/2, -h, -d/2, 1},
        {w/2, -h, -d/2, 1}
    };

    /**
     * matrix constructed with the center of the cube
     * @property matrix
     * @type object
     */
    this->matrix = matrix;

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
        {1, 0, 4, 0}, // face0_b1
        {4, 5, 1, 0}, // face1_b2

        {2, 1, 5, 0}, // face4_r1
        {5, 6, 2, 0}, // face5_r2

        {7, 6, 5, 0}, // face6_u1
        {5, 4, 7, 0}, // face7_u2

        {0, 3, 7, 0}, // face8_l1
        {7, 4, 0, 0}, // face9_l2

        {3, 2, 6, 1}, // face10_f1
        {6, 7, 3, 1} // face11_f2
    };
}
