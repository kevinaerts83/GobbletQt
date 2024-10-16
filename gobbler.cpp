#include "gobbler.h"

//![0]
Gobbler::Gobbler(QQuickItem *parent, Shape *shape, Gobbler3d model) : QQuickPaintedItem(parent), model(model), m_shape(shape) {

    //if (model) {
    // model = *new Gobbler3d(150);
    //}

    if (!m_shape) {
        m_shape = new Shape();
    }

}
//![0]

//![1]
QString Gobbler::name() const
{
    return m_name;
}

void Gobbler::setName(const QString &name)
{
    m_name = name;
}

bool Gobbler::isWhite() const
{
    return m_isWhite;
}

void Gobbler::setWhite(const bool &isWhite)
{
    m_isWhite = isWhite;
}

int Gobbler::size() const
{
    return m_size;
}

void Gobbler::setSize(const int &size)
{
    m_size = size;
}

int Gobbler::depth() const
{
    return m_depth;
}

void Gobbler::setDepth(const int &depth)
{
    m_depth = depth;
}

double Gobbler::x3d() const
{
    return m_x3d;
}

void Gobbler::setX3d(const double &x3d)
{
    m_x3d = x3d;
}

double Gobbler::y3d() const
{
    return m_y3d;
}

void Gobbler::setY3d(const double &y3d)
{
    m_y3d = y3d;
}

double Gobbler::z3d() const
{
    return m_z3d;
}

void Gobbler::setZ3d(const double &z3d)
{
    m_z3d = z3d;
}

void Gobbler::paint(QPainter *painter)
{
    if (this->depth() == 0) {
        model.Rotate(m_matrix, m_x3d, m_y3d, m_z3d);
        QColor color = m_isWhite ? QColor(QColorConstants::Svg::ivory) : QColor(QColorConstants::Svg::saddlebrown);
        painter->setBrush(color);
        m_shape->paint(m_matrix, model, painter);
    }
}

// Static comparator function to sort by z-index (ascending)
bool Gobbler::compareByZindex(const Gobbler* a, const Gobbler* b) {
    return a->z3d() < b->z3d();
}

//![1]

/*
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

Pawn.prototype = new Shape();

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

Pawn.prototype.lift = function(up) {
    this.centerPoint[1] += up;
};

Pawn.prototype.endMove = function(h) {
    this.centerPoint[1] = h - this.pHeight;
};

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

Pawn.prototype.endEvent = function(args) {
    if(this.setInvisible) {
        this.visible = false;
        this.setInvisible = false;
    }
};
*/
