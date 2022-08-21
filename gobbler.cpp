#include "gobbler.h"

//![0]
Gobbler::Gobbler(QQuickItem *parent) : QQuickPaintedItem(parent), Shape()
{
    this->model = *new gobbler3d();
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

QColor Gobbler::color() const
{
    return m_color;
}

void Gobbler::setColor(const QColor &color)
{
    m_color = color;
}

double Gobbler::tx() const
{
    return m_tx;
}

void Gobbler::setTx(const double &tx)
{
    m_tx = tx;
}

double Gobbler::ty() const
{
    return m_ty;
}

void Gobbler::setTy(const double &ty)
{
    m_ty = ty;
}

double Gobbler::tz() const
{
    return m_tz;
}

void Gobbler::setTz(const double &tz)
{
    m_tz = tz;
}

double Gobbler::getX() {
    return m_tx;
}

double Gobbler::getY() {
    return m_ty;
}

double Gobbler::getZ() {
    return m_tz;
}

void Gobbler::paint(QPainter *painter)
{
    //QPen pen(m_color, 1);
    //painter->setPen(pen);
    model.Rotate(m_matrix);
    painter->setBrush(m_color);
    Shape::paint(m_matrix, model, painter);
}

/*
void Gobbler::on_clicked(){
    this->setXangle(this->xangle() + 10);
    this->setColor(QColor("red"));
    //paint(painter);
}*/
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
