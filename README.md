CPSC 605 Project ３
===================
Articistic porcelain teapot with cracks
  
Author: Yanxiang Wu
        Qimin Yao

Implemented Features:
---------------------
* Texture mapping
* Environment mapping
* Normal mapping
* Depth of Field
* AntiAlias

How to make:
------------
make 

How to run:
-----------
main_base el.vert bp.frag teapot.obj ground.obj sky.obj
or
sh t.sh

How to control:
---------------
* a to enable Antialiasing, w to toggle between 4, 8, 12 and 16 jitter iteration
* b to enable Depth of Field, z and x to toggle focus
* 1 2 3 to control lights
* n to draw normal for the central model

Extra Feature:
---------------
Since it takes a lot lines of code allow the mouse control of camera, I just took it off.
If you want to try mouse control version. Please run:
	
sh mouse.sh

Statistic of Project:
---------------------
FYI:
Files: 		 12
Blank Lines: 197
Comment:	 218

Code:		 1034
Shader:		 102
Total Code:	 1136



This project was compiled and runned at frog18, if encounter problem restart the machine

Have fun!
