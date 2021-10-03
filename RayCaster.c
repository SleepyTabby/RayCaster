#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>
#define PI 3.1415926535
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.0174533 // one degree

float posX, posY, pdx, pdy, pa;//player pos

void DrawPlayer(){
	glColor3f(1,1,0); // research
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(posX, posY);
	glEnd();
	
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2i(posX, posY);
	glVertex2i(posX+pdx * 5, posY+pdy * 5);
	glEnd();
}

int mapX=8, mapY=8, cubeSize=64;
int map[]= {
1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,1,
1,0,1,1,0,0,0,1,
1,0,1,0,0,1,0,1,
1,0,0,0,1,1,0,1,
1,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,
};

void drawMap2D(){
	int y;
	int x;
	int xCubeOffset;
	int yCubeOffset;
    for(y = 0; y < mapY; y++){
    	for(x = 0; x < mapX; x++){
    		if(map[y * mapX + x] == 1)
    		glColor3f(1,1,1);
    		else
    		glColor3f(0,0,0);
    		
    		xCubeOffset = x * cubeSize; 
    		yCubeOffset = y * cubeSize;
    		glBegin(GL_QUADS);
    		glVertex2i(xCubeOffset + 1, yCubeOffset                       + 1); //base
    		glVertex2i(xCubeOffset + 1, yCubeOffset + cubeSize            - 1); // base + right vert
    		glVertex2i(xCubeOffset - 1 + cubeSize, yCubeOffset + cubeSize - 1); // base + right upper vert
    		glVertex2i(xCubeOffset - 1 + cubeSize, yCubeOffset            + 1); // base + upper vert
    		glEnd();
        }
    }
}

float distance(float ax, float ay, float bx, float by, float angle){
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

//NOTE: >> is the same as /   it devides
void RaycastDrawRays2D(){
	int ray, r_mapX,r_mapY,mapPos,dof/*map width*/; float raysX/*horizontal line x value*/, raysY/*horizontal line y value*/, raysAngle, xOffset, yOffset;
	
	//PI VALUE  means 180 degrees and this means looking down.
	raysAngle=pa-DR * 30;                          //pa = playerAngle.   DR = degree in radian
	
	if(raysAngle<0){
		raysAngle += 2*PI;
	}
	if(raysAngle>2*PI){
	    raysAngle -= 2*PI;
	}
	
	for(ray=0; ray<10; ray++){
		//---Check Horizontal Lines---
		dof=0;
		float distanceHorizontal = 1000000, horizontalX=posX, horizontalY=posY;
		float aTan= -1/tan(raysAngle);
		
		if(raysAngle>PI){
			raysY=(((int)posY>>6) <<6) -0.0001; 
			raysX= (posY-raysY) * aTan+posX; 
			yOffset=-64; 
			xOffset=-yOffset*aTan;//looking up
		}
		
		if(raysAngle<PI){
			raysY=(((int)posY>>6) <<6) +64; 
			raysX= (posY-raysY) * aTan+posX; 
			yOffset= 64; 
			xOffset=-yOffset*aTan;//looking down
		}
		
		if(raysAngle==0 || raysAngle == PI){ //looking straight left or right
			raysX=posX; 
			raysY=posY; 
			dof=8;
		}
		
		while(dof < 8){
			r_mapX = (int) (raysX)>>6; r_mapY = (int)(raysY)>>6; mapPos=r_mapY*mapX+r_mapX;
			if( mapPos > 0 && mapPos<mapX*mapY && map[mapPos] == 1){//you've hit a wall
				horizontalX = raysX;
				horizontalY = raysY;
				distanceHorizontal = distance(posX, posY, horizontalX, horizontalY, raysAngle);
				dof=8;
			}
			else{
				raysX += xOffset; 
				raysY += yOffset; 
				dof+=1;
			}
		}
			//---Check Vertical Lines---
		dof=0;
		float distanceVertical = 1000000, verticalX=posX, verticalY=posY;
		float nTan=-tan(raysAngle);
		
		if(raysAngle>P2 && raysAngle < P3){
			raysX=(((int)posX>>6) <<6) -0.0001; 
			raysY= (posX-raysX) * (nTan+posY); 
			xOffset=-64; 
			yOffset=-xOffset * nTan;//looking left
		}
		
		if(raysAngle<P2 || raysAngle>P3){
			raysX=(((int)posX>>6) <<6) +64;     
			raysY= (posX-raysX) * nTan+posY; 
			xOffset= 64; 
			yOffset=-xOffset * nTan;//looking right
		}
		
		if(raysAngle==0 || raysAngle == PI){ //looking straight up or down
			raysX=posX; 
			raysY=posY; 
			dof=8;
		}
		
		while(dof < 8){
			r_mapX = (int) (raysX)>>6; r_mapY = (int)(raysY)>>6; mapPos=r_mapY*mapX+r_mapX;
			if(mapPos > 0 && mapPos< (mapX*mapY) && map[mapPos] == 1){//you've hit a wall
				verticalX = raysX;
				verticalY = raysY;
				distanceVertical = distance(posX, posY, verticalX, verticalY, raysAngle);
				dof=8;
			}
			else{
				raysX += xOffset; 
				raysY+=yOffset; 
				dof+=1;
			}
		}
		if(distanceVertical < distanceHorizontal)
		{
			raysX = verticalX;
			raysY = verticalY;
			
		}
		if(distanceHorizontal < distanceVertical)
		{
			raysX = horizontalX;
			raysY = horizontalY;
			
		}
		glColor3f(1,0,0);
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex2i(posX , posY );
		glVertex2i(raysX, raysY);
		glEnd();
		
		
		
	
		
		
		raysAngle+=DR;
	if(raysAngle<0){
		raysAngle += 2*PI;
	}
	if(raysAngle>2*PI){
	    raysAngle -= 2*PI;
	}
	}
}

void display()
{
 glClear(GL_COLOR_BUFFER_BIT);
 drawMap2D();
 DrawPlayer();
 RaycastDrawRays2D();
 glutSwapBuffers();
 
}

void Buttons(unsigned char key, int xPos, int yPos){
if(key=='a'){
	//posX-=5;
pa -= 0.1; if(pa < 0     ){pa += 2 * PI;} pdx=cos(pa) * 5; pdy=sin(pa) * 5;	
}
if(key=='d'){
	//posX+=5;
	pa += 0.1; if(pa > 2 * PI){pa -= 2 * PI;} pdx=cos(pa) * 5; pdy=sin(pa) * 5;
}
if(key=='w'){
	posX+=pdx; posY+=pdy;
}
if(key=='s'){
	posX-=pdx; posY-=pdy;
}
glutPostRedisplay();
}

void Init(){
glClearColor(0.3,0.3,0.3,0);
gluOrtho2D(0, 512, 512, 0);
posX= 300; posY = 300; pdx=cos(pa) * 5; pdy=sin(pa) * 5;
}

int main(int argc, char** argv)
{ 
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
 glutInitWindowSize(512, 512);
 glutCreateWindow("Composer - RayCaster");
 Init();
 glutDisplayFunc(display);
 glutKeyboardFunc(Buttons);
 glutMainLoop();
 //return 0;
}
