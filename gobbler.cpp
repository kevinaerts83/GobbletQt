#include "gobbler.h"

//![0]
Gobbler::Gobbler(QQuickItem *parent) : QQuickPaintedItem(parent), Shape()
{
    this->model = *new gobbler3d();
}
//![0]

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

double Gobbler::xangle() const
{
    return m_xangle;
}

void Gobbler::setXangle(const double &xangle)
{
    m_xangle = xangle;
    emit xangleChanged(xangle);
    model.Rotate(m_xangle, m_yangle, m_zoom);
    update();
}

double Gobbler::yangle() const
{
    return m_yangle;
}

void Gobbler::setYangle(const double &yangle)
{
    m_yangle = yangle;
    emit yangleChanged(yangle);
    model.Rotate(m_xangle, m_yangle, m_zoom);
    update();
}

double Gobbler::zoom() const
{
    return m_zoom;
}

void Gobbler::setZoom(const double &zoom)
{
    m_zoom = zoom;
    emit zoomChanged(zoom);
    model.Rotate(m_xangle, m_yangle, m_zoom);
    update();
}

//![1]
void Gobbler::paint(QPainter *painter)
{
    //QPen pen(m_color, 1);
    //painter->setPen(pen);
    painter->setBrush(m_color);
    painter->setRenderHints(QPainter::Antialiasing, true);

    QVector<QVector<double>> points2d;

    double translation [4][4];
    model.matrix.getTranslationMatrix(100, 100, 100, translation);

    for (int i = 0; i < 8; i++) {
        points2d.append(model.matrix.ProjectPoint(model.matrix.MultiplyPointAndMatrix(model.cache[i], translation)));
    }
    Shape::paint(points2d, model.faces, painter);
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
