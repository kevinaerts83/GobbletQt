import QtQuick

/*
Reusable BluetoothIcon component
*/
Item {
    id: root
    // Public properties
    property color btColor: "#0A64D6"

    // Canvas draws the icon
    Canvas {
        id: canvas
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d");
            var w = width;
            var h = height;

            // Clear canvas
            ctx.reset();
            ctx.clearRect(0, 0, w, h);
            ctx.imageSmoothingEnabled = true;

            // Center and sizing
            var cx = w / 2;
            var cy = h / 2;
            var minSide = Math.min(w, h);
            // Draw bluetooth symbol in blue.
            // Coordinates defined relative to radius to scale nicely.
            var symScale = ((minSide / 2) - minSide * 0.08) * 1.05;        // overall symbol scale
            var barWidth = Math.max(2, symScale * 0.18);   // vertical bar thickness
            var symHeight = symScale * 1.6;      // vertical extent of symbol

            // central vertical bar
            ctx.beginPath();
            ctx.rect(cx - (barWidth/2), cy - (symHeight/2), barWidth, symHeight);
            ctx.fillStyle = btColor;
            ctx.fill();

            // Function to draw a "rune" triangle shape (upper and lower) that creates the Bluetooth rune look
            function drawRune(yOffsetSign) {
                // yOffsetSign: -1 for upper rune, +1 for lower rune
                var top = cy + (yOffsetSign * -symHeight * 0.47);
                var mid = cy + (yOffsetSign * symHeight * 0.25);
                var outerX = cx + symScale * 0.42 * (yOffsetSign === -1 ? 1 : 1); // sticks to the right
                var innerX = cx - symScale * 0.42;

                ctx.beginPath();
                ctx.moveTo(cx, top - (2 * yOffsetSign));
                ctx.lineTo(outerX, cy + (yOffsetSign * -symHeight * 0.25));
                ctx.lineTo(innerX, mid);
                ctx.strokeStyle = btColor;
                ctx.lineWidth = barWidth - 1;
                ctx.stroke();
            }

            // Upper rune
            drawRune(-1);

            // Lower rune
            drawRune(1);
        }

        // Request paint on resize
        onWidthChanged: requestPaint()
        onHeightChanged: requestPaint()
        Component.onCompleted: requestPaint()
    }

    // Ensure the Canvas repaints when properties change
    onBtColorChanged: canvas.requestPaint()
}
