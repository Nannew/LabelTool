/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#include <QPrintDialog>
#endif

#include "scribblearea.h"

//! [0]
ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    scribbling = false;
    myPenWidth = 1;
    //myPenColor = Qt::blue;
    myPenColor = QColor(128, 128, 255, 255);

    image_ori = QImage(500,500, QImage::Format_RGB888);
    image_ori.fill(qRgb(255, 255, 255));


    mask = QImage(500,500, QImage::Format_RGB32);
    mask.fill(qRgb(0, 0, 0));


    //The default/first class use num , background 0
    mask_class_idx = 0;
    std::vector<QPolygon> oPoly;
    polyVec.push_back(oPoly);
    //random color for class
    mask_colors.push_back(QColor(rand() % 255, rand() % 255, rand() % 255, 128));
    bool vis = true;
    mask_class_vis.push_back(vis);
}
//! [0]

//! [1]
bool ScribbleArea::openImage(const QString &fileName)
//! [1] //! [2]
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    //QSize newSize = loadedImage.size().expandedTo(size());
    //resizeImage(&loadedImage, newSize);

    //resize to image size instead
    emit sizeChanged(loadedImage.size());
    //resize(loadedImage.size());
    image = loadedImage;


    //keep a copy for clearing mask
    image_ori = loadedImage;


    //create a black mask for image
    mask = QImage(loadedImage.size(), QImage::Format_RGB32);
    mask.fill(qRgb(0, 0, 0));


    resetState();

    modified = false;
    update();
    return true;
}
//! [2]

//! [3]
bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
//! [3] //! [4]
{
    QImage visibleImage = image;
    //resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}
//! [4]

bool ScribbleArea::saveMaskImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = mask;
    //resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}

//! [5]
void ScribbleArea::setPenColor(const QColor &newColor)
//! [5] //! [6]
{
    myPenColor = newColor;
}
//! [6]

//! [7]
void ScribbleArea::setPenWidth(int newWidth)
//! [7] //! [8]
{
    myPenWidth = newWidth;
}

int ScribbleArea::getActiveClassIdx()
{
    return mask_class_idx;
}
//! [8]

//! [9]
void ScribbleArea::clearImage()
//! [9] //! [10]
{
    image.fill(qRgb(255, 255, 255));

    image_ori = image;

    //clear mask also
    mask.fill(qRgb(0, 0, 0));

    resetState();

    modified = true;
    update();
}
//! [10]


void ScribbleArea::clearMask()
{
    image = image_ori;

    //clear mask also
    mask.fill(qRgb(0, 0, 0));

    resetState();

    modified = true;
    update();
}




void ScribbleArea::createNewClass()
{
    //mask_num = mask_colors.size() + 1;
    mask_colors.push_back(QColor(rand() % 255, rand() % 255, rand() % 255, 128));
    bool vis = true;
    mask_class_vis.push_back(vis);

    std::vector<QPolygon> oPoly;
    polyVec.push_back(oPoly);
    //update();
}


//! [11]
void ScribbleArea::mousePressEvent(QMouseEvent *event)
//! [11] //! [12]
{
    if (event->button() == Qt::LeftButton) {

        if(!scribbling)
        {
            scribbling = true;
            //enable tracking
            setMouseTracking(true);

            //clear poly for new drawing
            //poly.clear();
            tmpPoly.clear();
        }

        // This is what I prefer, when the pos is close to boundary
        QPoint oPos = QPoint( (this->width() - event->x())>10? event->x():this->width(), (this->height() - event->y())>10? event->y():this->height());

        lastPoint = oPos;
        image_old = image;
        mask_old = mask;

        //https://www.bogotobogo.com/Qt/Qt5_QPainterPath_QPolygon.php
        tmpPoly << oPos;

    }

    if(event->button() == Qt::RightButton && scribbling)
    {
        image = image_old;
        mask = mask_old;

        //drawLineTo(event->pos());
        scribbling = false;

        //disable tracking
        setMouseTracking(false);

        //draw poly
        // This is what I prefer, when the pos is close to boundary
        QPoint oPos = QPoint( (this->width() - event->x())>10? event->x():this->width(), (this->height() - event->y())>10? event->y():this->height());

        tmpPoly << oPos;//add last point
        //Now add poly to the vector
        polyVec[mask_class_idx].push_back(tmpPoly);
        drawPoly();
    }

}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    //if ((event->buttons() & Qt::LeftButton) && scribbling)
    if (scribbling)
    {
        image = image_old;
        mask = mask_old;
        drawLineTo(event->pos());
    }
}

//void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
//{
//    if (event->button() == Qt::LeftButton && scribbling) {
//        image = image_old;
//        drawLineTo(event->pos());
//        scribbling = false;
//    }
//}

//! [12] //! [13]
void ScribbleArea::paintEvent(QPaintEvent *event)
//! [13] //! [14]
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}
//! [14]

//! [15]
void ScribbleArea::resizeEvent(QResizeEvent *event)
//! [15] //! [16]
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}
//! [16]

//! [17]
void ScribbleArea::drawLineTo(const QPoint &endPoint)
//! [17] //! [18]
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    //int rad = (myPenWidth / 2) + 2;
    //update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    update();

    //lastPoint = endPoint;
}
//! [18]

//! [19]
void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
//! [19] //! [20]
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB888);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}
//! [20]

//! [21]
void ScribbleArea::print()
{
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog printDialog(&printer, this);
//! [21] //! [22]
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_NO_PRINTER
}
//! [22]




void ScribbleArea::drawPoly()
{
//    if(polyVec.empty())
//    {
//        return;
//    }

    image = image_ori;
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    // Brush
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);



    for(int i = 0; i < polyVec.size(); i++)
    {
        //visibility
        if(!mask_class_vis[i])
        {
            continue;
        }
        for(int j = 0; j < polyVec[i].size(); j++)
        {
            if(polyVec[i][j].empty())
            {
                continue;
            }
            brush.setColor(mask_colors[i]);

            // Fill polygon
            QPainterPath path;
            path.addPolygon(polyVec[i][j]);

            painter.drawPolygon(polyVec[i][j]);
            painter.fillPath(path, brush);
        }

    }



    //also update mask
    mask.fill(qRgb(0, 0, 0));
    QPainter mask_painter(&mask);
    for(int i = 0; i < polyVec.size(); i++)
    {
        for(int j = 0; j < polyVec[i].size(); j++)
        {
            if(polyVec[i][j].empty())
            {
                continue;
            }

            //use color of current class
            mask_painter.setPen(QPen(QColor(i, i, i, 255), myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            //update brush color to current class
            brush.setColor(QColor(i, i, i, 255));

            //now update mask
            mask_painter.drawPolygon(polyVec[i][j]);
            QPainterPath path;
            path.addPolygon(polyVec[i][j]);
            mask_painter.fillPath(path, brush);
        }
    }

    modified = true;

    update();
}



void ScribbleArea::setActiveClassIdx(int oIdx)
{
    mask_class_idx = oIdx;
}

void ScribbleArea::setClassVisibility(int oIdx, bool oVis)
{
    mask_class_vis[oIdx] = oVis;

    //update visibility
    drawPoly();
}

void ScribbleArea::deleteCurrentClass()
{

    mask_class_vis.erase(mask_class_vis.begin() + mask_class_idx);
    mask_colors.erase(mask_colors.begin() + mask_class_idx);

    polyVec[mask_class_idx].clear();
    polyVec.erase(polyVec.begin() + mask_class_idx);

    mask_class_idx = std::min(int(mask_colors.size()), std::max(0, mask_class_idx));

    //update visibility
    drawPoly();
}

void ScribbleArea::clearCurrentClass()
{
    polyVec[mask_class_idx].clear();
    //update visibility
    drawPoly();
}

void ScribbleArea::resetState()
{
    for(int i = 0; i <polyVec.size(); i++)
    {
        polyVec[i].clear();
        polyVec[i].shrink_to_fit();
    }
    polyVec.clear();
    polyVec.shrink_to_fit();

    mask_colors.clear();
    mask_colors.shrink_to_fit();

    mask_class_vis.clear();
    mask_class_vis.shrink_to_fit();

    mask_class_idx = 0;

    std::vector<QPolygon> oPoly;
    polyVec.push_back(oPoly);
    //random color for class
    mask_colors.push_back(QColor(rand() % 255, rand() % 255, rand() % 255, 128));
    bool vis = true;
    mask_class_vis.push_back(vis);

}
