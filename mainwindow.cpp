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

#include "mainwindow.h"
#include "scribblearea.h"

//! [0]
MainWindow::MainWindow()
{
    scribbleArea = new ScribbleArea;
    setCentralWidget(scribbleArea);

    createActions();
    createMenus();

    setWindowTitle(tr("Label Tool"));
    resize(500, 500);
}
//! [0]

//! [1]
void MainWindow::closeEvent(QCloseEvent *event)
//! [1] //! [2]
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}
//! [2]

//! [3]
void MainWindow::open()
//! [3] //! [4]
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
        {
            scribbleArea->openImage(fileName);

            resetClassState();
        }
    }
}
//! [4]

//! [5]
void MainWindow::save()
//! [5] //! [6]
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}
//! [6]


void MainWindow::saveMask()
//! [5] //! [6]
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveMaskFile(fileFormat);
}

//! [7]
void MainWindow::penColor()
//! [7] //! [8]
{
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());
    if (newColor.isValid())
        scribbleArea->setPenColor(newColor);
}
//! [8]

//! [9]
void MainWindow::penWidth()
//! [9] //! [10]
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Label Tool"),
                                        tr("Select pen width:"),
                                        scribbleArea->penWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        scribbleArea->setPenWidth(newWidth);
}
//! [10]

//! [11]
void MainWindow::about()
//! [11] //! [12]
{
    QMessageBox::about(this, tr("About Label Tool"),
            tr("<p>The <b>Label Tool</b> is a Qt-based tool for anotation. "
               "The tool use the Scribble example as a starting point.</p>"
               ));
}
//! [12]

//! [13]
void MainWindow::createActions()
//! [13] //! [14]
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveAsActs.append(action);
    }

    //save mask file
    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(saveMask()));
        saveMaskAsActs.append(action);
    }



    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, SIGNAL(triggered()), scribbleArea, SLOT(print()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    //Create a new class
    newClassAct = new QAction(tr("&New Class"), this);
    newClassAct->setShortcut(tr("Ctrl+N"));
    connect(newClassAct, SIGNAL(triggered()),
            this, SLOT(createNewClass()));


    //Delete current class
    deleteClassAct = new QAction(tr("&Delte Class"), this);
    deleteClassAct->setShortcut(tr("Ctrl+D"));
    connect(deleteClassAct, SIGNAL(triggered()),
            this, SLOT(deleteCurrentClass()));

    //Clear current class
    clearClassAct = new QAction(tr("&Clear Class"), this);
    clearClassAct->setShortcut(tr("Ctrl+C"));
    connect(clearClassAct, SIGNAL(triggered()),
            this, SLOT(clearCurrentClass()));


    QPixmap oPixmap(20,20);
    oPixmap.fill(scribbleArea->mask_colors[0]);
    QAction * oAct = new QAction(oPixmap, QString("Class %1").arg(scribbleArea->getActiveClassIdx()), this);
    oAct->setCheckable(true);
    oAct->setChecked(true);

    classActionList.push_back(oAct);


    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+S"));
    connect(clearScreenAct, SIGNAL(triggered()),
            this, SLOT(clearImage()));

    //clear mask only
    clearMaskAct = new QAction(tr("&Clear Mask"), this);
    clearMaskAct->setShortcut(tr("Ctrl+M"));
    connect(clearMaskAct, SIGNAL(triggered()),
            this, SLOT(clearMask()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));


    //notify main window to update size
    connect(scribbleArea, SIGNAL(sizeChanged(QSize)), this, SLOT(resizeWindow(QSize)));



    activeClassComboBox = new QComboBox;
    activeClassComboBox->addItem(oPixmap, QString("Class %1").arg(scribbleArea->getActiveClassIdx()));
    activeClassComboBox->setCurrentIndex(scribbleArea->getActiveClassIdx());

    //notify to update active class index
    connect(activeClassComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setActiveClassIdx(int)));
}
//! [14]

//! [15]
void MainWindow::createMenus()
//! [15] //! [16]
{
    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach (QAction *action, saveAsActs)
        saveAsMenu->addAction(action);

    //save mask file
    saveMaskAsMenu = new QMenu(tr("&Save Mask As"), this);
    foreach (QAction *action, saveMaskAsActs)
        saveMaskAsMenu->addAction(action);

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addMenu(saveMaskAsMenu);//save mask file
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(newClassAct);
    optionMenu->addAction(clearClassAct);
    optionMenu->addAction(deleteClassAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);
    optionMenu->addAction(clearMaskAct);//clear mask

    //Class menu
    classMenu = new QMenu(tr("&Classes"), this);
    createClassMenus();


    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    //helpMenu->addAction(aboutQtAct);


    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(classMenu);//Class menu
    menuBar()->addMenu(helpMenu);





    toolBar = addToolBar("main toolbar");
    QLabel * oLabel = new QLabel(tr("Active Class: "));
    toolBar->addWidget(oLabel);
    toolBar->addWidget(activeClassComboBox);
}

void MainWindow::createClassMenus()
{
    classMenu->clear();
    //Class menu

    //classMenu->addSeparator();
    for(int i =0; i<classActionList.size(); i++)
    {
        classMenu->addAction(classActionList[i]);
    }

    //update class visibility
    connect(classMenu, SIGNAL(triggered(QAction*)), this, SLOT(updateClassVisibility(QAction*)));
}

void MainWindow::createClassCombobox()
{
    activeClassComboBox->clear();
    //Class combobox
    for(int i =0; i<scribbleArea->mask_colors.size(); i++)
    {
        QPixmap oPixmap(20,20);
        oPixmap.fill(scribbleArea->mask_colors[i]);

        activeClassComboBox->addItem(oPixmap, QString("Class %1").arg(i));
    }
    //notify to update active class index
    connect(activeClassComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setActiveClassIdx(int)));
}
//! [16]

//! [17]
bool MainWindow::maybeSave()
//! [17] //! [18]
{
    if (scribbleArea->isModified()) {
       QMessageBox::StandardButton ret;
       ret = QMessageBox::warning(this, tr("Label Tool"),
                          tr("The image has been modified.\n"
                             "Do you want to save your changes?"),
                          QMessageBox::Save | QMessageBox::Discard
                          | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            return saveMaskFile("png");
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}
//! [18]

//! [19]
bool MainWindow::saveFile(const QByteArray &fileFormat)
//! [19] //! [20]
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty()) {
        return false;
    } else {
        return scribbleArea->saveImage(fileName, fileFormat.constData());
    }
}
//! [20]

bool MainWindow::saveMaskFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Mask As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty()) {
        return false;
    } else {
        return scribbleArea->saveMaskImage(fileName, fileFormat.constData());
    }
}


void MainWindow::resizeWindow(QSize oSize)
{
    //resize the main window to image size
    this->resize(oSize);
}

void MainWindow::createNewClass()
{
    scribbleArea->createNewClass();

    QPixmap oPixmap(20,20);
    oPixmap.fill(scribbleArea->mask_colors.back());
    QAction * oAct = new QAction(oPixmap, QString("Class %1").arg(scribbleArea->mask_colors.size()-1), this);
    oAct->setCheckable(scribbleArea->mask_class_vis.back());
    oAct->setChecked(scribbleArea->mask_class_vis.back());
    classActionList.push_back(oAct);



    //classMenu = new QMenu(tr("&Classes"), this);
    createClassMenus();

    activeClassComboBox->addItem(oPixmap, QString("Class %1").arg(scribbleArea->mask_colors.size()-1));
    //set new class to be active class
    activeClassComboBox->setCurrentIndex(int(scribbleArea->mask_colors.size())-1);
}

void MainWindow::deleteCurrentClass()
{
    //if it is the last class, only clear it
    if(scribbleArea->mask_class_vis.size()<=1)
    {
        scribbleArea->clearCurrentClass();
        return;
    }

    //erase current class from action list
    classActionList.erase(classActionList.begin() + scribbleArea->getActiveClassIdx());
    //update class index
    for(int i =0; i<classActionList.size(); i++)
    {
        classActionList[i]->setText(QString("Class %1").arg(i));
    }

    scribbleArea->deleteCurrentClass();

    //delete class and update current index in combobox
    createClassCombobox();
    activeClassComboBox->setCurrentIndex(scribbleArea->getActiveClassIdx());


    createClassMenus();
}

void MainWindow::clearCurrentClass()
{
    scribbleArea->clearCurrentClass();
}



void MainWindow::setActiveClassIdx(int oIdx)
{
    scribbleArea->setActiveClassIdx(oIdx);
}

void MainWindow::updateClassVisibility(QAction * oAction)
{
    QString oString = oAction->iconText();
    //remove "Calss ", leave only number
    oString.replace(0,6, "");
    int oIdx = oString.toInt();
    scribbleArea->setClassVisibility(oIdx, oAction->isChecked());
}

void MainWindow::clearImage()
{
    scribbleArea->clearImage();


    resetClassState();
}

void MainWindow::clearMask()
{
    scribbleArea->clearMask();

    resetClassState();

}

void MainWindow::resetClassState()
{
    classActionList.clear();

    QPixmap oPixmap(20,20);
    oPixmap.fill(scribbleArea->mask_colors[0]);
    QAction * oAct = new QAction(oPixmap, QString("Class %1").arg(0), this);
    oAct->setCheckable(true);
    oAct->setChecked(true);

    classActionList.push_back(oAct);
    createClassMenus();


    activeClassComboBox->clear();
    activeClassComboBox->addItem(oPixmap, QString("Class %1").arg(0));
    activeClassComboBox->setCurrentIndex(0);
}
