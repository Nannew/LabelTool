# LabelTool
**LabelTool** is an simple annotation tool for creating label masks from images. The tool is written in C++ and based on Qt and reuse a few features from Qt Scribble example.

* This tool is rather a framework and lacks other features of a labelling tool.

## The tool support the following features
* Open, save, and print images and masks

<img src="../media/images/1.png?raw=true" width="400" height="400">



* Draw and label objects with line segments
* Recognize boundary point when drawing

*Left click to start. Repeat left click to mark the boundary of object*

<img src="../media/images/6.png?raw=true" width="400" height="400">   <img src="../media/images/7.png?raw=true" width="400" height="400">

*When the cursor is within 10 pixels from the image boundary, clicking with result to the closest point start from the boundary. Right click to end drawing and the tool will then fill the region*

<img src="../media/images/8.png?raw=true" width="400" height="400">   <img src="../media/images/9.png?raw=true" width="400" height="400">



* Reset image (Ctrl+S) and class labels (Ctrl+M)
* Add (Ctrl+N), clear (Ctrl+c), and delete (Ctrl+D) a class label

<img src="../media/images/2.png?raw=true" width="400" height="400">



* Show/hide an class label with checkable items under menu

<img src="../media/images/3.png?raw=true" width="400" height="400">    <img src="../media/images/4.png?raw=true" width="400" height="400">



* Change active class to label

<img src="../media/images/5.png?raw=true" width="400" height="400">
