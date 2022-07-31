#include <iostream>
#include "windows.h"
#include "time.h"
#include <math.h>
#include <string>
#include <stdio.h>
#include <conio.h>
#include <limits>
using namespace std;

string playerSng;    //single player player name
string playerPvc;    //player vs computer player name
string player1;      //pvp player 1 name
string player2;      //pvp player 2 name

bool upperBoarderCollison = false;       //collision checker for upper boarder

bool win = false;
bool endgame = false;
bool restart = false;

char fullscreenOn = 'Y';
int score = 0;  //score for single player
int x,y;    //Final destination coordinates
int tempX,tempY;  //units striker disk travel (can be positive or negative)

int pvpCount = 1; //moves counter for pvp
int score1 = 0; //player1 score for pvp
int score2 = 0; //player2 score for pvp

int pvcCount = 1; //moves counter for pvc
int score3 = 0; //player score for pvc
int score4 = 0; //computer score for pvc
   
bool firstShot = true; //checking if it is the first shot of game that hits the disk cluster
char UserPermission;
int singleMaxPoints = 17;    //max points for single player
int multiMaxPoints = 11;      //max points for multiplayer
char mode = 'S';   //mode (singleplayer/pvp/pvc)
bool noNames = true;   //check if player names are being set
int dx,dy;             //position coordinates of striker
char Qpos;
int x5,y5;

//y coordinte of firing position is dy
//x coordinate of firing position is dx

//Assume Board surface is frictionless and well powdered
//Assume time taken for any disk to travel to any location is 1s
// s=ut
// s=u*1     s=u

char board[15][41] = {
	"++++++++++++++++++++++++++++++++++++++++",
	"+ 0                                  0 +",
	"+                                      +",
	"+                                      +",
	"+                O @ O                 +",
	"+               @ O @ @                +",
	"+              O @ $ O O               +",
	"+               @ O @ @                +",
	"+                O @ O                 +",
	"+                                      +",
	"+                                      +",
	"+                                      +",
	"+                                      +",
	"+ 0                Q                 0 +",
	"++++++++++++++++++++++++++++++++++++++++"};
	
	
HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE); //setting up handler for font size in console
void fontsize(int, int);
HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE); //setting up handler for font color in console  
	
	//function for fontsize
void fontsize(int a, int b) {
	PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();
	lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
	lpConsoleCurrentFontEx->dwFontSize.X = a;
	lpConsoleCurrentFontEx->dwFontSize.Y = b;
	SetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
}


//function for font colors
VOID WINAPI SetConsoleColors(WORD attribs) {
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFOEX cbi;
	cbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(hOutput, &cbi);
	cbi.wAttributes = attribs;
	SetConsoleScreenBufferInfoEx(hOutput, &cbi);
}

//Initiates the fullscreen function
//Do later, has to fix fullscreen because can't exit fullscreen
void fullscreen(){

   keybd_event(VK_MENU, 0x38, 0, 0);
   keybd_event(VK_RETURN, 0x1c, 0, 0);
    keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);

}

//function to print board
void printBoard(){
	for(int i=0;i<15;i++){
		if(i<10){
			cout<<"                     "<<i;
		}else{
			cout<<"                    "<<i;
		}
		
		for(int j=0;j<41;j++){
		   if(j<40){
		   	   cout<<board[i][j];
		   }else{
		   	   cout<<board[i][j]<<endl;
		   }
		   
		}
	}
	
}

//function for moving animation
void movinganimation()
{
	int i=x+1; int j=y;
  	if(dy>j)
	{
   		for(;dx>i;dx--)
		{	
		
		system("cls");
		board[dx][dy] = ' ';
		board[dx-1][dy-1] = 'Q';
		printBoard();
		
		board[dx-1][dy-1] =' ';
		system("cls");
		printBoard();
		dy--;
		}	
 	}
 	
    if(dy==(j))
    {
    	for(;dx>i;dx--)
    	{
      
		system("cls");
		board[dx][dy]   = ' ';
		board[dx-1][dy] = 'Q';
		printBoard();
		board[dx-1][dy] = ' ';
		  
		}
	}
	
	else
	{
		for(;dx>i;dx--)
	{	
	
		system("cls");
		board[dx][dy]     = ' ';
		board[dx-1][dy+1] = 'Q';
		printBoard();
		
		board[dx-1][dy+1] =' ';
		system("cls");
		printBoard();
		dy++;
	}		
	}
 
 
 	board[13][19] = 'Q';
}


//checking collisions in path to the destination
void pathCollision(){
	
	int i=x+1; int j=y;
	int dx1 = dx;
	int dy1 = dy;
	dx1 = dx1 - 1;
	
	
  	if(dy1>j)
	{
		dy1 = dy1-1;
   		for(;dx1>i;dx1--)
		{	
		
		if(board[dx1][dy1] != ' '){
			
		x = dx1;
		y = dy1;	
			
		}
		
		dy1--;
		}	
 	}
 	
    else if(dy1==(j)){
    	for(;dx1>i;dx1--)
    	{
      
		if(board[dx1][dy1] != ' '){
			
		x = dx1;
		y = dy1;	
			
		}
		  
		}
	}
	
	else{
		dy1 = dy1 + 1;
		for(;dx1>i;dx1--){	
	     
	    if(board[dx1][dy1] != ' '){
			
		x = dx1;
		y = dy1;	
			
		}
		
		dy1++;
	    }		
	}
}
	

//checking collisions after bounce back on border
void bounceBackCollision(int x2,int y2){
	
	switch(board[x-x2][y-y2]){
		case 'O': {
		
		    if(y<dy){
		    	
		    board[x-x2-1][y-y2+1]='O';   //displace the next o
		    	
			}else{
				
			board[x-x2-1][y-y2-1]='O';   //displace the next o
				
			}
		
			break;
		}
		
		case '@':{
		
			if(y<dy){
		    	
		    board[x-x2-1][y-y2+1]='@';   //displace the next o
		    	
			}else{
				
			board[x-x2-1][y-y2-1]='@';   //displace the next o
				
			}
			
			break;
		}
		case '$':{
			
			if(y<dy){
		    	
		    board[x-x2-1][y-y2+1]='$';   //displace the next o
		    	
			}else{
				
			board[x-x2-1][y-y2-1]='$';   //displace the next o
				
			}
			
			break;
		}
			
	}
}

//checking collision with element in the selected destination	
void collisionChecking(int x1,int y1){  

   switch(board[x-x1][y+y1]){
		case 'O': {
		
			//collisionChecking(tempX,tempY);   //checking collision and positioning
			if(y<dy){
				board[x-x1-1][y+y1-1]='O';   //displace the next o
			}else{
				board[x-x1-1][y+y1+1]='O';   //displace the next o
			}
			
			
			break;
		}
		
		
		case '@':{
		
			if(y<dy){
				board[x-x1-1][y+y1-1]='@';   //displace the next @
			}else{
				board[x-x1-1][y+y1+1]='@';   //displace the next @
			}
			
			break;
		}
		case '$':{
			
			if(y<dy){
				board[x-x1-1][y+y1-1]='$';   //displace the next $
			}else{
				board[x-x1-1][y+y1+1]='$';   //displace the next $
			}
			
			break;
		}
		//when the disk fall to hole, 1 score is added 
		//if queen(RED) falls to hole, 5 score is added
		case '0':{    
			      
		    if(board[x][y]=='$'){
		    	score = score + 5;
			}else{
			    score=score+1;	
			}
		    
			break;
		}
		
	}
    
}

//maleeshas part
void startScreen(){
	
	fontsize(40, 40);
		SetConsoleTextAttribute(color, 6); //gold
		cout << " ------------------------------------------------------------------------------\n";
		cout << " |";   
		SetConsoleTextAttribute(color, 12); 
		cout << "                               CARROM GAME                                 ";
		SetConsoleTextAttribute(color, 6); cout << " |\n";
		cout << " ------------------------------------------------------------------------------\n\n";

		//animation 
		int i = 0;
		SetConsoleTextAttribute(color, 1);
		
		char Design[80] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*','*','*','*', '*','*',' ','W','E','L','C', 'O','M','E',' ','T','O',' ','C','A','R','R','O','M',' ','G','A','M','E',' ','*','*', '*','*','*','*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
		while (i < 80)
		{

			cout << Design[i];
			i++;
			Sleep(25);
		}

		cout << endl << endl;

		SetConsoleTextAttribute(color, 11);
		cout << endl;
		
		//fontsize(15, 15);
		//printBoard();
		//fontsize(40, 40);
		cout << "	  Press 'Y'(to start) or 'N'(to exit) : ";
		SetConsoleTextAttribute(color, 12);
		cin >> UserPermission;
	
}

void instruction_page(){
	fontsize(40, 40);
	system("CLS");
	SetConsoleTextAttribute(color, 6);
	cout << " ------------------------------------------------------------------------------\n";
	cout << " |";   
	SetConsoleTextAttribute(color, 12); 
	cout << "                               CARROM GAME                                 ";
	SetConsoleTextAttribute(color, 6); cout << " |\n";
	cout << " ------------------------------------------------------------------------------\n\n";
	
	char Design[100] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ', '-','>',' ','I','N','S','T', 'R','U','C','T','I','O','N','S',' ','F','O','R',' ','T','H','E',' ','G','A','M','E', ' ','<','-',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ', };

	int i = 0;
	while (i < 80)
	{
		SetConsoleTextAttribute(color, 11);
		cout << Design[i];
		i++;
		Sleep(25);
	}
	SetConsoleTextAttribute(color, 4);
	cout << "  \n\n  How to play  \n\n";
	SetConsoleTextAttribute(color, 14);
	cout << "	You can score by putting Disks to net holes\n";
	cout << "	First person to reach the maximum marks will win\n";
	cout << "	Putting a White or Black disk to a net hole gives 1 point\n";
	cout << "	Putting The RED Disk to a net hole gives 5 points\n\n";
	
	cout << "	Striker disk - Q\n";
	cout << "	White disks - O\n";
	cout << "	Black disks - @\n";
	cout << "	RED disk - $\n";
	cout << "	Net hole - 0\n";
	cout << "	Border - +\n";
	SetConsoleTextAttribute(color, 4);
	cout << "  \n  Inputs  \n\n";
	SetConsoleTextAttribute(color, 14);
	cout << "   You can enter the units that disk travels in x and y axis\n";
	cout << "   You should enter units that have equal magnitude ex: x=6,y=6  x=-6,y=6  x=0,y=6\n";
	cout << "   y units should not be zero and x units can be zero (straight shot)\n";
	cout << "   To move in right direction x units should be + values\n";
	cout << "   To move in left direction x units should be - values\n";
	cout << "   Do you want to exit Fullscreen? (Y/N) : ";
	cin>>fullscreenOn;
	
	switch(fullscreenOn){       //This can be deleted after we implement options menu
		case 'y':
		case 'Y':{
			fullscreen();     //exits fullscreen
			break;
		}
		
		case 'n':
		case 'N':{
			                //remain in fullscreen
			break;
		}
	}

	SetConsoleTextAttribute(color, 1);
	cout << "\n\n	Press enter to countine...";
	cin.get();
	cin.get();
}

//spreading of disks after first attack to the cluster
void spreading(){
	
	int i,j;
	
	srand(time(0));
	
	if(((board[x][y]=='O')||(board[x][y]=='@')||(board[x][y]=='$'))&&firstShot){
		firstShot = false;
		
		//spreading row 4
		
		board[4][17]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = 'O';
		
		board[4][19]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = '@';
		
		board[4][21]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = 'O';
		
		//spreading row 5
		
		board[5][16]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = '@';
		
		board[5][18]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = 'O';
		
		board[5][20]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = '@';
		
		board[5][22]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = '@';
		
		//spreading row 6
		
		board[6][15]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = 'O';
		
		board[6][17]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = '@';
		
		board[6][19]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = '$';
		
		board[6][21]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = 'O';
		
		board[6][23]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = 'O';
		
		//spreading row 7
		
		board[7][16]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = '@';
		
		board[7][18]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = 'O';
		
		board[7][20]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = '@';
		
		board[7][22]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = '@';
		
		
		//spreading row 8
		board[8][17]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = 'O';
		
		board[8][19]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = '@';    
		
		board[8][21]=' '; 
		do{
		  i = (rand()%10)+1;
		  j = (rand()%23)+8;	
		} while(board[i][j] != ' ');
		
		board[i][j] = 'O';
		
	}
	
}

//scoreboard for singleplayer
void scoreBoard(){
	
	cout<<"                                   "<<playerSng<<"  SCORE : "<<score<<endl;                 
	
}

//scoreboard for pvp
void scoreBoardPvp(){
	
	cout<<"                            "<<player1<<" SCORE : "<<score1<<endl;                 
	cout<<"                            "<<player2<<" SCORE : "<<score2<<endl; 
}

//scoreboard for pvc
void scoreBoardPvc(){
	
	cout<<"                            "<<playerPvc<<" SCORE   : "<<score3<<endl;                 
	cout<<"                            COMPUTER SCORE : "<<score4<<endl; 
}

//chathura's part
void bounceBack(int vx,int vy){
	
	//use Coefficient of restitution (use restitution)
	//has to bounce back after collision with border
	
	//COR of carrom board and the dice is assumed to be 0.7
	
	if(upperBoarderCollison){
		
	const float COR = 0.7; 
	
	// calculating new velocity
	int newVx = vx*COR; 
	int newVy = vy*COR;

    //assume all disks take 1s to travel
    //tempX1 = newVx*1
    //tempY1 = newVy*1
	int tempX1 = newVx;  
	int tempY1 = newVy;    

	bounceBackCollision(-tempX1,tempY1);	
	board[x+tempX1][y-tempY1]=' ';
	upperBoarderCollison = false;
		
	} 
	else {
		
	const float COR = 0.7; 
	
	// calculating new velocity
	int newVx = vx*COR; 
	int newVy = vy*COR;

    //assume all disks take 1s to travel
    //tempX1 = newVx*1
    //tempY1 = newVy*1
	int tempX1 = newVx;  
	int tempY1 = newVy;    

	bounceBackCollision(tempX1,tempY1);	
	board[x-tempX1][y-tempY1]=' ';	
		
	}
		
}

//function to handle singleplayer gameplay movements
void gameplay(){
	
	if(firstShot){
		
		pathCollision();
		movinganimation();
		spreading();
	
	}
	else{
		
		if(x<0||y<0||y>39){
			
			if((x>0)&&(y<0)){
				y=0;
			}
			
			if((x>0)&&(y>39)){
				y=39;
			}
			
			if((x<0)&&(y>0)&&(y<39)){
				
				//upper boarder collision
				upperBoarderCollison = true;
				x=0;
			}
			
		}
		
		pathCollision();  //fix   //check if collide on path to destination and update final destination
		
		movinganimation();
		
		switch(board[x][y]){
		case 'O': {
			
			board[x][y]=' ';
			
			collisionChecking(tempX,tempY);   //checking collision and positioning
			
			if(board[x-tempX][y+tempY] != '0'){
				
				//can't go outside the map
				x5 = x - tempX;
				y5 = y + tempY;
				
				if(x5<0||y5<0||y5>39){
				
				if((x5>0)&&(y5<0)){
					
				y5=2;
				
			    }
			
			   if((x5>0)&&(y5>39)){
			   	
				y5=37;
				
			    }
			
			    if((x5<0)&&(y5>0)&&(y5<39)){
			    	
				x5=2;
				
			    }
				
				}
				
				board[x5][y5]='O';    
				
			}else{
				board[x-tempX][y+tempY]='0';
			}			
			  		
			break;
		}
		case '+':{
			
			//If firing disk hit the border it has to bounce back
			
			//Assume time taken to travel for all disks is 1s
			//velocity along x axis vx = tempX/1
			//velocity along y axis vy = tempY/1
			
			bounceBack(tempX,tempY); 
			
			break;
		}
		
		case '@':{
				
			board[x][y]=' ';
			
			collisionChecking(tempX,tempY);   //checking collision and positioning
			
			if(board[x-tempX][y+tempY] != '0'){
				
				//can't go outside the map
				x5 = x - tempX;
				y5 = y + tempY;
				
				if(x5<0||y5<0||y5>39){
				
				if((x5>0)&&(y5<0)){
					
				y5=2;
				
			    }
			
			   if((x5>0)&&(y5>39)){
			   	
				y5=37;
				
			    }
			
			    if((x5<0)&&(y5>0)&&(y5<39)){
			    	
				x5=2;
				
			    }
				
				}
				
				board[x5][y5]='@';    
				
			}else{
				board[x-tempX][y+tempY]='0';
			}			
			
			break;
		}
		case '$':{
			
			collisionChecking(tempX,tempY);   //checking collision and positioning
			board[x][y]=' ';
			
			if(board[x-tempX][y+tempY] != '0'){
				
				//can't go outside the map
				x5 = x - tempX;
				y5 = y + tempY;
				
				if(x5<0||y5<0||y5>39){
				
				if((x5>0)&&(y5<0)){
					
				y5=2;
				
			    }
			
			   if((x5>0)&&(y5>39)){
			   	
				y5=37;
				
			    }
			
			    if((x5<0)&&(y5>0)&&(y5<39)){
			    	
				x5=2;
				
			    }
				
				}
				
				board[x5][y5]='$';    
				
			}else{
				board[x-tempX][y+tempY]='0';
			}					
			
			break;
		}
			
	}
	
}
}

//function to handle pvp gameplay movements
void gameplaypvp(bool whoMove){
	
	if(firstShot){
		
		pathCollision();
		movinganimation();
		spreading();
	
	}
	else{
		
		if(x<0||y<0||y>39){
			
			if((x>0)&&(y<0)){
				y=0;
			}
			
			if((x>0)&&(y>39)){
				y=39;
			}
			
			if((x<0)&&(y>0)&&(y<39)){
				
				//upper boarder collision
				upperBoarderCollison = true;
				x=0;
			}
			
		}
		
		pathCollision();    //check if collide on path to destination and update final destination
		
		movinganimation();
		
		switch(board[x][y]){
		case 'O': {
			
			if(!whoMove){
				
				cout<<"Error. "<<player2<<" isn't allowed to aim at o."<<endl;
				system("pause");
				break;
			}
			
			board[x][y]=' ';
			
			collisionChecking(tempX,tempY);   //checking collision and positioning
			
			if(board[x-tempX][y+tempY] != '0'){
				
				//can't go outside the map
				x5 = x - tempX;
				y5 = y + tempY;
				
				if(x5<0||y5<0||y5>39){
				
				if((x5>0)&&(y5<0)){
					
				y5=2;
				
			    }
			
			   if((x5>0)&&(y5>39)){
			   	
				y5=37;
				
			    }
			
			    if((x5<0)&&(y5>0)&&(y5<39)){
			    	
				x5=2;
				
			    }
				
				}
				
				board[x5][y5]='O';    
				
			}else{
				board[x-tempX][y+tempY]='0';
			}			
			  		
			break;
		}
		case '+':{
			
			//If firing disk hit the border it has to bounce back
			
			//Assume time taken to travel for all disks is 1s
			//velocity along x axis vx = tempX/1
			//velocity along y axis vy = tempY/1
			
			bounceBack(tempX,tempY); 
			
			break;
		}
		
		case '@':{
			
			if(whoMove){
				
				cout<<"Error. "<<player1<<" isn't allowed to aim at @."<<endl;
				system("pause");
				break;
			}
				
			board[x][y]=' ';
			
			collisionChecking(tempX,tempY);   //checking collision and positioning
			
			if(board[x-tempX][y+tempY] != '0'){
				
				//can't go outside the map
				x5 = x - tempX;
				y5 = y + tempY;
				
				if(x5<0||y5<0||y5>39){
				
				if((x5>0)&&(y5<0)){
					
				y5=2;
				
			    }
			
			   if((x5>0)&&(y5>39)){
			   	
				y5=37;
				
			    }
			
			    if((x5<0)&&(y5>0)&&(y5<39)){
			    	
				x5=2;
				
			    }
				
				}
				
				board[x5][y5]='@';    
				
			}else{
				board[x-tempX][y+tempY]='0';
			}			
			
			break;
		}
		case '$':{
			
			collisionChecking(tempX,tempY);   //checking collision and positioning
			board[x][y]=' ';
			
			if(board[x-tempX][y+tempY] != '0'){
				
				//can't go outside the map
				x5 = x - tempX;
				y5 = y + tempY;
				
				if(x5<0||y5<0||y5>39){
				
				if((x5>0)&&(y5<0)){
					
				y5=2;
				
			    }
			
			   if((x5>0)&&(y5>39)){
			   	
				y5=37;
				
			    }
			
			    if((x5<0)&&(y5>0)&&(y5<39)){
			    	
				x5=2;
				
			    }
				
				}
				
				board[x5][y5]='$';    
				
			}else{
				board[x-tempX][y+tempY]='0';
			}					
			
			break;
		}
			
	}
	
}
}
	
//function to handle pvc gameplay movements
void gameplaypvc(bool whoMove){
	
	if(firstShot){
		
		pathCollision();
		movinganimation();
		spreading();
	
	}
	else{
		
		if(x<0||y<0||y>39){
			
			if((x>0)&&(y<0)){
				y=0;
			}
			
			if((x>0)&&(y>39)){
				y=39;
			}
			
			if((x<0)&&(y>0)&&(y<39)){
				
				//upper boarder collision
				upperBoarderCollison = true;
				x=0;
			}
			
		}
		
		pathCollision();    //check if collide on path to destination and update final destination
		
		movinganimation();
		
		switch(board[x][y]){
		case 'O': {
			
			if(!whoMove){
				
				cout<<"Error. Computer isn't allowed to aim at o."<<endl;
				system("pause");
				break;
				
			}
			
			board[x][y]=' ';
			
			collisionChecking(tempX,tempY);   //checking collision and positioning
			
			if(board[x-tempX][y+tempY] != '0'){
				
				//can't go outside the map
				x5 = x - tempX;
				y5 = y + tempY;
				
				if(x5<0||y5<0||y5>39){
				
				if((x5>0)&&(y5<0)){
					
				y5=2;
				
			    }
			
			   if((x5>0)&&(y5>39)){
			   	
				y5=37;
				
			    }
			
			    if((x5<0)&&(y5>0)&&(y5<39)){
			    	
				x5=2;
				
			    }
				
				}
				
				board[x5][y5]='O';    
				
			}else{
				board[x-tempX][y+tempY]='0';
			}				
			  		
			break;
		}
		case '+':{
		
			//If firing disk hit the border it has to bounce back
			
			//Assume time taken to travel for all disks is 1s
			//velocity along x axis vx = tempX/1
			//velocity along y axis vy = tempY/1
			
			bounceBack(tempX,tempY); 
			
			break;
		}
		
		case '@':{
			
			if(whoMove){
				
				cout<<"Error. "<<playerPvc<<" isn't allowed to aim at @."<<endl;
				system("pause");
				break;
				
			}
				
			board[x][y]=' ';
			
			collisionChecking(tempX,tempY);   //checking collision and positioning
			
			if(board[x-tempX][y+tempY] != '0'){
				
				//can't go outside the map
				x5 = x - tempX;
				y5 = y + tempY;
				
				if(x5<0||y5<0||y5>39){
				
				if((x5>0)&&(y5<0)){
					
				y5=2;
				
			    }
			
			   if((x5>0)&&(y5>39)){
			   	
				y5=37;
				
			    }
			
			    if((x5<0)&&(y5>0)&&(y5<39)){
			    	
				x5=2;
				
			    }
				
				}
				
				board[x5][y5]='@';    
				
			}else{
				board[x-tempX][y+tempY]='0';
			}			
			
			break;
		}
		case '$':{
			
			board[x][y]=' ';
			
			collisionChecking(tempX,tempY);   //checking collision and positioning
			
			if(board[x-tempX][y+tempY] != '0'){
				
				//can't go outside the map
				x5 = x - tempX;
				y5 = y + tempY;
				
				if(x5<0||y5<0||y5>39){
				
				if((x5>0)&&(y5<0)){
					
				y5=2;
				
			    }
			
			   if((x5>0)&&(y5>39)){
			   	
				y5=37;
				
			    }
			
			    if((x5<0)&&(y5>0)&&(y5<39)){
			    	
				x5=2;
				
			    }
				
				}
				
				board[x5][y5]='$';    
				
			}else{
				board[x-tempX][y+tempY]='0';
			}			
			
			break;
		}
			
	}
	
}
	
}

//singleplayer gameplay
void singlePlayer(){
	
	if(noNames){
		
	system("CLS");
	
	fontsize(40, 40);
		SetConsoleTextAttribute(color, 6); //gold
		cout << " ------------------------------------------------------------------------------\n";
		cout << " |";   
		SetConsoleTextAttribute(color, 12); 
		cout << "                               CARROM GAME                                 ";
		SetConsoleTextAttribute(color, 6); cout << " |\n";
		cout << " ------------------------------------------------------------------------------\n\n";

	int i = 0;
		SetConsoleTextAttribute(color, 3);
		char Design[74] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','`','`','`','`', '`','`',' ',' ','S','I','N','G', 'L','E',' ','P','L','A','Y','E','R',' ','M','O','D','E',' ',' ',' ','`','`', '`','`','`','`',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };

		while (i < 74)
		{

			cout << Design[i];
			i++;
			Sleep(25);
		}

		cout << endl << endl;
	
	string PlayerName;
	
    //getting user data
	SetConsoleTextAttribute(color, 14);
	cout << "     Before we start..... \n\n";
	SetConsoleTextAttribute(color, 7);
	cout << "     Please enter your User Name (maximum 10 characters)   :..........\b\b\b\b\b\b\b\b\b\b";
	SetConsoleTextAttribute(color, 10);
	cin >> PlayerName;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	SetConsoleTextAttribute(color, 6);
	cout << "\n\n\n\n\t\t       Hello, "<<PlayerName<<" Let's start the game.  \n\n";
	playerSng = PlayerName;
	PlayerName.clear();
	cout << "\a";
	Sleep(2500);
    
    system("CLS");
    
    noNames = false;
		
	}
	
	fontsize(30,30);
	//SetConsoleTextAttribute(color, 6); //gold
	SetConsoleTextAttribute(color, 14);
	
	cout<<"                      ";
	cout<<"012345678..............................39"<<endl;
	
	printBoard();
	cout<<endl;
	scoreBoard();
	
		dx = 13;  dy =19;
	
	cout<<"Move the disk left(A) or right(D) "<<endl;
	cout<<"To shoot the disk press E"<<endl;
	
	cin>>Qpos;
	while(Qpos!='E')
	{
	switch(Qpos)
	{
		case 'A':
		case 'a':{ 
		
		board [dx][dy-1] = 'Q';
		board [dx][dy] = ' '; 
		dx = dx;
		dy = dy-1 ;break;	
		
		//ss = 1;	
		}
		
		case 'd':
		case 'D':{ 
	
		board [dx][dy] =' ';
		board[dx][dy+1] = 'Q';
		dx = dx;
		dy = dy+1 ;break;	
		
	//	ss = 2;	
		}		
	}
	
	system("cls");
	printBoard();
	
	cout<<endl;
	scoreBoard();
	
	cout<<"Column number : "<<dy<<endl;
	cout<<"Move the disk left(A) or right(D) "<<endl;
	cout<<"To shoot the disk press E"<<endl;
	cin>>Qpos;
		
	}
	system("cls");
	printBoard();
	
	cout<<endl;
	scoreBoard();
	
	cout<<"Number of x axis units target moves : ";
	cin>>tempY;
	cout<<"Number of y axis units target moves : ";
	cin>>tempX;
	
	x = dx-tempX;
	y = dy+tempY;
	
	gameplay();               //run all gameplay 
	
	if(singleMaxPoints == score){
		
		win = true;
		
		//maleesha part	
		system("CLS");
	fontsize(40, 40);
	//welcome screen
		SetConsoleTextAttribute(color, 6); //gold
		cout << " ------------------------------------------------------------------------------\n";
		cout << " |";   
		SetConsoleTextAttribute(color, 12); 
		cout << "                               CARROM GAME                                 ";
		SetConsoleTextAttribute(color, 6); cout << " |\n";
		cout << " ------------------------------------------------------------------------------\n\n";
	
	Sleep(500);
	SetConsoleTextAttribute(color, 11);
	
	cout<<"\n\n\t\t\tWELL PLAYED! "<<playerSng<<", YOU WON! \n\n";
	cout<<"\a";
	int i = 0;
		SetConsoleTextAttribute(color, 4);
		char Design[77] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','C','O','N', 'G','R','A','T','U','L','A','T','I','O','N','S','!',' ',' ',' ', ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };

		while (i < 77)
		{

			cout << Design[i];
			i++;
			Sleep(25);
		}

		
	cout << endl << endl;
		

	Sleep(2500);
	
	SetConsoleTextAttribute(color, 9);
	cout<<"Enter 'Q' to Quit "<<endl;
	cout<<"Enter 'R' to Restart "<<endl;
	cout<<"Enter 'M' to Main Menu "<<endl;
	SetConsoleTextAttribute(color, 13);
	cout<<">>>>>> ";
	SetConsoleTextAttribute(color, 12);
	char endinput;
	cin>>endinput;
	
	switch(endinput){       
		case 'Q':
		case 'q':{
			endgame = true;   
			break;
		}
		
		case 'R':
		case 'r':{
			singlePlayer();            
			break;
		}
		case 'M':
		case 'm':{
			restart = true;
			break;
		}
	}
		
	}
	
}

//player vs player gameplay
void pvp(){
	
	//player1 name 
	//player 2 name
	//player 1 score
	//player2 score
	
	//maxpoints 11
	
	//add to instructions
	//player 1 is always - o team
	//player 1 is only allowed to directly hit o and $
	//player 2 is always - @ team
	//player 2 is only allowed to directly hit @ and $
	
	//player vs player gameplay
	
	if(noNames){
		
	system("CLS");
	
	fontsize(40, 40);
		SetConsoleTextAttribute(color, 6); //gold
		cout << " ------------------------------------------------------------------------------\n";
		cout << " |";   
		SetConsoleTextAttribute(color, 12); 
		cout << "                               CARROM GAME                                 ";
		SetConsoleTextAttribute(color, 6); cout << " |\n";
		cout << " ------------------------------------------------------------------------------\n\n";

	int i = 0;
		SetConsoleTextAttribute(color, 3);
		char Design[77] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','`','`','`','`', '`',' ',' ',' ','P','L','A','Y','E','R',' ', 'V','S',' ','P','L','A','Y','E','R',' ','M','O','D','E',' ',' ',' ','`','`', '`','`','`','`',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };

		while (i < 77)
		{

			cout << Design[i];
			i++;
			Sleep(25);
		}

		cout << endl << endl;
	
	string PlayerName1;
	string PlayerName2;
    //getting user data
	SetConsoleTextAttribute(color, 14);
	cout << "     Before we start..... \n\n";
	SetConsoleTextAttribute(color, 7);
	cout << "     Please enter Player 1 Name (maximum 10 characters)   :..........\b\b\b\b\b\b\b\b\b\b";
	SetConsoleTextAttribute(color, 10);
	cin >> PlayerName1;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	
	SetConsoleTextAttribute(color, 7);
	cout << "     Please enter Player 2 Name (maximum 10 characters)   :..........\b\b\b\b\b\b\b\b\b\b";
	SetConsoleTextAttribute(color, 10);
	cin >> PlayerName2;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	
	SetConsoleTextAttribute(color, 6);
	cout << "\n\n\n\t      Hello, "<<PlayerName1<<" & "<<PlayerName2<<" Let's start the game.  \n\n";
	player1 = PlayerName1;         //pvp player 1 name
    player2 = PlayerName2;         //pvp player 2 name
	PlayerName1.clear();
	PlayerName2.clear();
	cout << "\a";
	Sleep(2500);
    
    system("CLS");
    
    noNames = false;
		
	}
	
	bool whoMove = (pvpCount%2);
	
	fontsize(30,30);
	//SetConsoleTextAttribute
	SetConsoleTextAttribute(color, 13);
	
	cout<<"                      ";
	cout<<"012345678..............................39"<<endl;
	
	printBoard();
	cout<<endl;
	
	scoreBoardPvp();
	cout<<endl;
	
		dx = 13;  dy =19;
	
	cout<<"Move the disk left(A) or right(D) "<<endl;
	cout<<"To shoot the disk press E"<<endl;
	
	cin>>Qpos;
	while(Qpos!='E')
	{
	switch(Qpos)
	{
		case 'A':
		case 'a':{ 
		
		board [dx][dy-1] = 'Q';
		board [dx][dy] = ' '; 
		dx = dx;
		dy = dy-1 ;break;	
		
		//ss = 1;	
		}
		
		case 'd':
		case 'D':{ 
	
		board [dx][dy] =' ';
		board[dx][dy+1] = 'Q';
		dx = dx;
		dy = dy+1 ;break;	
		
	//	ss = 2;	
		}		
	}
	
	system("cls");
	printBoard();
	
	cout<<endl;
	scoreBoardPvp();
	
	cout<<"Column number : "<<dy<<endl;
	cout<<"Move the disk left(A) or right(D) "<<endl;
	cout<<"To shoot the disk press E"<<endl;
	cin>>Qpos;
		
	}while(Qpos!='E');
	
	system("cls");
	printBoard();
	
	cout<<endl;
	scoreBoardPvp();
	
	if(whoMove){
		cout<<"                           "<<player1<<"'s turn"<<endl;
	}
	else{
		cout<<"                           "<<player2<<"'s turn"<<endl;
	}
	
	cout<<"                            "<<player1<<" only can attack 'o' disks"<<endl;
	cout<<"                            "<<player2<<" only can attack '@' disks"<<endl;
	cout<<"                            Both can attack RED disk '$'      "<<endl;
	
	cout<<"Number of x axis units target moves : ";
	cin>>tempY;
	cout<<"Number of y axis units target moves : ";
	cin>>tempX;
	
	x = dx-tempX;
	y = dy+tempY;
	
	gameplaypvp(whoMove);
	
	if(whoMove){
		score1 = score1+score;
	}
	else{
		score2 = score2+score;
	}
	
	pvpCount=pvpCount+1;
	score = 0;
	
	if(multiMaxPoints == score1){
		
		pvpCount = 1; //use this to end screen
		win = true;
		//-------------
		system("CLS");
	fontsize(40, 40);
	//welcome screen
		SetConsoleTextAttribute(color, 6); //gold
		cout << " ------------------------------------------------------------------------------\n";
		cout << " |";   
		SetConsoleTextAttribute(color, 12); 
		cout << "                               CARROM GAME                                 ";
		SetConsoleTextAttribute(color, 6); cout << " |\n";
		cout << " ------------------------------------------------------------------------------\n\n";
	
	Sleep(500);
	SetConsoleTextAttribute(color, 11);
	
	cout<<"\n\n\t\t\tWELL PLAYED! "<<player1<<", YOU WON! \n\n";
	cout<<"\a";
	int i = 0;
		SetConsoleTextAttribute(color, 4);
		char Design[77] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','C','O','N', 'G','R','A','T','U','L','A','T','I','O','N','S','!',' ',' ',' ', ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };

		while (i < 77)
		{

			cout << Design[i];
			i++;
			Sleep(25);
		}

		
	cout << endl << endl;
		

	Sleep(2500);
	
	SetConsoleTextAttribute(color, 9);
	cout<<"Enter 'Q' to Quit "<<endl;
	cout<<"Enter 'R' to Restart "<<endl;
	cout<<"Enter 'M' to Main Menu "<<endl;
	SetConsoleTextAttribute(color, 13);
	cout<<">>>>>> ";
	SetConsoleTextAttribute(color, 12);
	char endinput;
	cin>>endinput;
	
	switch(endinput){       
		case 'Q':
		case 'q':{
			endgame = true;    
			break;
		}
		
		case 'R':
		case 'r':{
			pvp();          
			break;
		}
		case 'M':
		case 'm':{
			restart = true;
			break;
		}
	}
	
	

		
	}
	else if(multiMaxPoints == score2){
		
		pvpCount = 0; //use this to end screen
		win = true;
		//------------
		system("CLS");
	fontsize(40, 40);
	//welcome screen
		SetConsoleTextAttribute(color, 6); //gold
		cout << " ------------------------------------------------------------------------------\n";
		cout << " |";   
		SetConsoleTextAttribute(color, 12); 
		cout << "                               CARROM GAME                                 ";
		SetConsoleTextAttribute(color, 6); cout << " |\n";
		cout << " ------------------------------------------------------------------------------\n\n";
	
	Sleep(500);
	SetConsoleTextAttribute(color, 11);
	cout<<"\n\n\t\t\tWELL PLAYED! "<<player2<<", YOU WON! \n\n";
	cout<<"\a";
	int i = 0;
		SetConsoleTextAttribute(color, 4);
		char Design[77] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','C','O','N', 'G','R','A','T','U','L','A','T','I','O','N','S','!',' ',' ',' ', ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };

		while (i < 77)
		{

			cout << Design[i];
			i++;
			Sleep(25);
		}

		
	cout << endl << endl;
		

	Sleep(2500);
	
	SetConsoleTextAttribute(color, 9);
	cout<<"Enter 'Q' to Quit "<<endl;
	cout<<"Enter 'R' to Restart "<<endl;
	cout<<"Enter 'M' to Main Menu "<<endl;
	SetConsoleTextAttribute(color, 13);
	cout<<">>>>>> ";
	SetConsoleTextAttribute(color, 12);
	char endinput;
	cin>>endinput;
	
	switch(endinput){       
		case 'Q':
		case 'q':{
			endgame = true;    
			break;
		}
		
		case 'R':
		case 'r':{
			pvp();          
			break;
		}
		case 'M':
		case 'm':{
			restart = true;
			break;
		}
	}
	
	

	}
	
}

//player vs computer gameplay
void pvc(){
	
	//player name
	//player score
	//computer score
	
	//maxpoints 11
	
	//add to instructions
	//player is always - o team
	//player is only allowed to directly hit o and $
	//computer is always - @ team
	//computer is only allowed to directly hit @ and $
	
	//player vs computer gameplay
	
	if(noNames){
		
	system("CLS");
	
	fontsize(40, 40);
		SetConsoleTextAttribute(color, 6); //gold
		cout << " ------------------------------------------------------------------------------\n";
		cout << " |";   
		SetConsoleTextAttribute(color, 12); 
		cout << "                               CARROM GAME                                 ";
		SetConsoleTextAttribute(color, 6); cout << " |\n";
		cout << " ------------------------------------------------------------------------------\n\n";

	int i = 0;
		SetConsoleTextAttribute(color, 3);
		char Design[84] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','`','`','`','`', '`','`',' ',' ',' ','P','L','A','Y','E','R',' ','V','S',' ','C','O','M','P','U','T','E','R',' ','M','O','D','E',' ',' ',' ','`','`', '`','`','`','`',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };

		while (i < 84)
		{

			cout << Design[i];
			i++;
			Sleep(25);
		}

		cout << endl << endl;
	
	string PlayerName;
	
//getting user data
	SetConsoleTextAttribute(color, 14);
	cout << "     Before we start..... \n\n";
	SetConsoleTextAttribute(color, 7);
	cout << "     Please enter your User Name (maximum 10 characters)   :..........\b\b\b\b\b\b\b\b\b\b";
	SetConsoleTextAttribute(color, 10);
	cin >> PlayerName;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	SetConsoleTextAttribute(color, 6);
	cout << "\n\n\n\n\t\t       Hello, "<<PlayerName<<" Let's start the game.  \n\n";
	playerPvc = PlayerName;
	PlayerName.clear();
	cout << "\a";
	Sleep(2500);
	
	system("CLS");
	
	noNames = false;
	
	}
	
	bool whoMove = (pvcCount%2);
	
	fontsize(30,30);
	//SetConsoleTextAttribute
	SetConsoleTextAttribute(color, 13);
	
	cout<<"                      ";
	cout<<"012345678..............................39"<<endl;
	
	printBoard();
	cout<<endl;
	
	scoreBoardPvc();
	cout<<endl;
	
	//computer can't move 'Q' disk
	
	if(whoMove){
		
	dx = 13;  dy =19;
	
	cout<<"Move the disk left(A) or right(D) "<<endl;
	
	cout<<"To shoot the disk press E"<<endl;
	
	cin>>Qpos;
	while(Qpos!='E')
	{
	switch(Qpos)
	{
		case 'A':
		case 'a':{ 
		
		board [dx][dy-1] = 'Q';
		board [dx][dy] = ' '; 
		dx = dx;
		dy = dy-1 ;break;	
		
		//ss = 1;	
		}
		
		case 'd':
		case 'D':{ 
	
		board [dx][dy] =' ';
		board[dx][dy+1] = 'Q';
		dx = dx;
		dy = dy+1 ;break;	
		
	//	ss = 2;	
		}		
	}
	
	system("cls");
	printBoard();
	
	cout<<endl;
	
	scoreBoardPvc();
	
	cout<<"Column number : "<<dy<<endl;
	cout<<"Move the disk left(A) or right(D) "<<endl;
	cout<<"To shoot the disk press E"<<endl;
	cin>>Qpos;
		
	}while(Qpos!='E');	
		
	}
	
	system("cls");
	
	printBoard();
	cout<<endl;
	scoreBoardPvc();
	
	if(whoMove){
		cout<<"                           "<<playerPvc<<"'s turn"<<endl;
	}
	else{
		cout<<"                            COMPUTER's turn"<<endl;
	}
	
	cout<<"                            Player only can attack 'o' disks"<<endl;
	cout<<"                            Computer only can attack '@' disks"<<endl;
	cout<<"                            Both can attack RED disk '$'      "<<endl;
	
	if(whoMove){
	
	//player give data to move
	
	cout<<"Number of x axis units target moves : ";
	cin>>tempY;
	cout<<"Number of y axis units target moves : ";
	cin>>tempX;
	
	x = dx-tempX;
	y = dy+tempY;
		
	}
	else{
		
	//computer randomly gives data
	//program random inputs for tempY,tempX
	
	srand(time(0));  // Initialize random number generator
	
	//dy = (rand() % 38) + 1;
	
	if(dy<20){
		
		tempY = (rand() % 19) + 1;
		
	} else{
		
		tempY = (rand() % 18) + 1;
		
	}
	
	tempX = (rand() % 12) + 1;
	
	cout<<"Number of x axis units target moves : "<<tempY<<endl;
	cout<<"Number of y axis units target moves : "<<tempX<<endl;
	
	Sleep(5000);
		
	}
	x = dx-tempX;
	y = dy+tempY;
		
	gameplaypvc(whoMove);
	
	if(whoMove){
		score3 = score3+score;
	}
	else{
		score4 = score4+score;
	}
	
	pvcCount = pvcCount + 1;
	score = 0;
	
	if(multiMaxPoints == score3){
		
		pvcCount = 1; //use this to end screen
		win = true;
		//-----------
		system("CLS");
	fontsize(40, 40);
	//welcome screen
		SetConsoleTextAttribute(color, 6); //gold
		cout << " ------------------------------------------------------------------------------\n";
		cout << " |";   
		SetConsoleTextAttribute(color, 12); 
		cout << "                               CARROM GAME                                 ";
		SetConsoleTextAttribute(color, 6); cout << " |\n";
		cout << " ------------------------------------------------------------------------------\n\n";
	
	Sleep(500);
	SetConsoleTextAttribute(color, 11);
	
	cout<<"\n\n\t\t\tWELL PLAYED! "<<playerPvc<<", YOU WON! \n\n";
	cout<<"\a";
	int i = 0;
		SetConsoleTextAttribute(color, 4);
		char Design[77] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','C','O','N', 'G','R','A','T','U','L','A','T','I','O','N','S','!',' ',' ',' ', ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };

		while (i < 77)
		{

			cout << Design[i];
			i++;
			Sleep(25);
		}

		
	cout << endl << endl;
		

	Sleep(2500);
	
	SetConsoleTextAttribute(color, 9);
	cout<<"Enter 'Q' to Quit "<<endl;
	cout<<"Enter 'R' to Restart "<<endl;
	cout<<"Enter 'M' to Main Menu "<<endl;
	SetConsoleTextAttribute(color, 13);
	cout<<">>>>>> ";
	SetConsoleTextAttribute(color, 12);
	char endinput;
	cin>>endinput;
	
	switch(endinput){       
		case 'Q':
		case 'q':{
			endgame = true;     
			break;
		}
		
		case 'R':
		case 'r':{
			pvc();            
			break;
		}
		case 'M':
		case 'm':{
			restart = true;
			break;
		}
	}
	
	}
	else if(multiMaxPoints == score4){
		
		pvcCount = 0; //use this to end screen
		win = true;
		//---------
		system("CLS");
	fontsize(40, 40);
	//welcome screen
		SetConsoleTextAttribute(color, 6); //gold
		cout << " ------------------------------------------------------------------------------\n";
		cout << " |";   
		SetConsoleTextAttribute(color, 12); 
		cout << "                               CARROM GAME                                 ";
		SetConsoleTextAttribute(color, 6); cout << " |\n";
		cout << " ------------------------------------------------------------------------------\n\n";
	
	Sleep(500);
	SetConsoleTextAttribute(color, 11);
	
	cout<<"\n\n\t\t\tCOMPUTER WON! , TRY AGAIN! \n\n";
	cout<<"\a";


		
	cout << endl << endl;
		

	Sleep(2500);
	
	SetConsoleTextAttribute(color, 9);
	cout<<"Enter 'Q' to Quit "<<endl;
	cout<<"Enter 'R' to Restart "<<endl;
	cout<<"Enter 'M' to Main Menu "<<endl;
	SetConsoleTextAttribute(color, 13);
	cout<<">>>>>> ";
	SetConsoleTextAttribute(color, 12);
	char endinput;
	cin>>endinput;
	
	switch(endinput){       
		case 'Q':
		case 'q':{
			endgame = true;    
			break;
		}
		
		case 'R':
		case 'r':{
			pvc();           
			break;
		}
		case 'M':
		case 'm':{
			restart = true;
			break;
		}
	}	
	}
	
}


int main(){
	
	//disks can't go outside the map    1   me,maleesha, chathura
	
	fullscreen();
	
	//maleesha's part start===================================================================
	startScreen();
	
	
	switch(UserPermission) {
  	case 'y':
	case 'Y':{
		
		instruction_page();
    
    SetConsoleTextAttribute(color, 6); //gold
		cout << "                    MODES OF THE GAME            "<<endl<<endl;
		cout << "                    SINGLE PlAYER         (S)    "<<endl;
		cout << "                    PlAYER VS PLAYER      (P)    "<<endl;
		cout << "                    PLAYER VS COMPUTER    (C)    "<<endl;
		
		cout<<endl;
		cout << "     Choose a mode (S/P/C) : ";
		cin >> mode;       //store the value on temporary variable
		
		switch(mode){    //check if single player or multiplayer
			case 's':
			case 'S':{
				mode = 'S';     //mode set to singleplayer
				break;
			}
			case 'p':
			case 'P':{
				mode = 'P';     //mode set to multiplayer
				break;
			}
			case 'c':
			case 'C':{
				mode = 'C';     //mode set to multiplayer
				break;
			}
			default:{
				cout << "\n     Invalid input ....";
				cout <<"\n      Game starting again....";
				main();
				break;
			}
		}
		
		break;
	}
    
    
	case 'n':
  	case 'N':{
  		
  	system("CLS");
  	SetConsoleTextAttribute(color, 6);
	cout << " ------------------------------------------------------------------------------\n";
	cout << " |";   
	SetConsoleTextAttribute(color, 12); 
	cout << "                               CARROM GAME                                 ";
	SetConsoleTextAttribute(color, 6); cout << " |\n";
	cout << " ------------------------------------------------------------------------------\n\n";
  	SetConsoleTextAttribute(color, 4); 
	cout<<"\n		  	\tSee You again...\n\n";
	SetConsoleTextAttribute(color, 7);
  	Sleep(2000);
	return 0;
  		
	break;
	}
  	
	default:{
		
	SetConsoleTextAttribute(color, 7);
    cout<<"\n\n	Wrong input! Game will restart in 3 seconds...";
    Sleep(3000);
    system ("CLS");
    main();
		
	break;
	
	}
}
  	
	system ("CLS");
	
	//maleesha's part end====================================================================
	
	continue1:
		
	switch(mode){
		case 's':
		case 'S':{
			singlePlayer();
			break;
		}
		
		case 'p':
		case 'P':{
			pvp();
			break;
		}
		case 'c':
		case 'C':{
			pvc();
			break;
		}
	}
	
	if(restart){
		
	system("CLS");
	main();
	restart = false;
		
	}
	
	if(endgame){
		
	return 0;
		
	}else{
		
	system("CLS");
	goto continue1;
		
	}		
			
}




