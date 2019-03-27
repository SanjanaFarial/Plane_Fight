# include "iGraphics.h"


#define splashscreen 0
#define menuscreen 1
#define game 2
#define gameover 3
#define highscorescreen 4
#define textinputscreen 5


int currentscreen = 0 ;
int score ;
int lives ;
char playerNames[100][80] ;
int playerScores[100] ;
char name[80];
int nameLength ;

int plane_x , plane_y ;
int enemy_x , enemy_y ;
int bullet_x , bullet_y ;
int obj_x , obj_y ;
int enemyspeed = 10 ;
bool askforname ;

int skyImageIdx ;
int skyMoveTimer ;
int splashtime ;
int enemyMoveTimer ;
int bulletMoveTimer ;
int objMoveTimer ;

//for ipassive mouse
int playFlag,scoreFlag,exitFlag;
char * sky[] = { "s00.bmp" , "s01.bmp" , "s02.bmp" , "s03.bmp" , "s04.bmp" , "s05.bmp" , "s06.bmp" , "s07.bmp" , "s08.bmp" ,
				"s09.bmp" , "s10.bmp" , "s11.bmp" , "s12.bmp" , "s13.bmp" , "s14.bmp" , "s15.bmp" , "s16.bmp" ,
				"s17.bmp" , "s18.bmp" , "s19.bmp" , "s20.bmp" , "s21.bmp" , "s22.bmp" , "s23.bmp" , "s24.bmp" ,
				"s25.bmp" , "s26.bmp" , "s27.bmp" , "s28.bmp" , "s29.bmp" , "s30.bmp" , "s31.bmp" , "s32.bmp" } ;


// dimension of each sky .bmp is the same as game screen
void showSky( void )
{
	iShowBMP( 0 , 0 , sky[ skyImageIdx ] ) ;
}

void showMenu() {

	iShowBMP(0 ,  0 , "fmenu.bmp");
	if(playFlag==1)
		iShowBMP(0 ,  0 , "playafter.bmp");
	if(exitFlag==1)
		iShowBMP(0 ,  0 , "exitafter.bmp");
	if(scoreFlag==1)
		iShowBMP(0 ,  0 , "highscoreafter.bmp");



}

void Sort()
{
	strcpy( playerNames[ 4 ] , name ) ;
	playerScores[ 4 ] = score ;
	for ( int i = 0 ; i < 5 ; i++ ) {
		for ( int j = 0 ; j < 4 ; j++ ) {
			if ( playerScores[ j ] < playerScores[ j + 1 ] ) {
				char temp[ 80 ] ;
				strcpy( temp , playerNames[ j ] ) ;
				strcpy( playerNames[ j ] , playerNames[ j + 1 ] ) ;
				strcpy( playerNames[ j + 1 ] , temp ) ;

				int t ;
				t = playerScores[ j ] ;
				playerScores[ j ] = playerScores[ j + 1 ] ;
				playerScores[ j + 1 ] = t ;
			}
		}
	}
}

void copyFromFile()
{
	FILE *namefile = fopen( "hsname.txt" , "r" ) ;
	FILE *scorefile = fopen( "hsscore.txt" , "r" ) ;
	for ( int i=0 ; i < 5 ; i++ ) {
		//fgets(  playerNames[ i ] , 100 , namefile  ) ;
		fscanf( namefile , "%[^\n]s" , playerNames[ i ] ) ;
		fscanf( namefile , "%*c" ) ;
		fscanf( scorefile , "%d\n" , &playerScores[ i ] ) ;
	}
	fclose( scorefile ) ;
	fclose( namefile ) ;
}

void saveToFile()
{
	FILE *namefile = fopen( "hsname.txt" , "w" ) ;
	FILE *scorefile = fopen( "hsscore.txt" , "w" ) ;
	for ( int i=0 ; i < 5 ; i++ ) {
		//( playerNames[ i ] , namefile ) ;
		fprintf( namefile , "%s\n" , playerNames[ i ] ) ;
		fprintf( scorefile , "%d\n" , playerScores[ i ] ) ;
	}
	fclose( scorefile ) ;
	fclose( namefile ) ;
}

bool checkHighScore()
{
	copyFromFile() ;
	return score > playerScores[ 4 ] ;
}

void textInput()
{

	iShowBMP(0,0,"inputname.bmp");
	iSetColor(255,0,0);
	iText( 270 ,500-305 , name,GLUT_BITMAP_TIMES_ROMAN_24 ) ;
}


void showScore()
{
	iShowBMP( 512 , 0 , "display_stats.bmp" ) ;
	iSetColor( 0 , 0 , 0 ) ;
	char buf[ 80 ] ;
	sprintf( buf , "%d" , lives ) ;
	iText( 512 + 53 , 512 - 112 , buf ) ;
	sprintf( buf , "%5d" , score ) ;
	iText( 512 + 33 , 512 - 285 , buf ) ;
	iSetColor( 255 , 255 , 255 ) ;
}

void showHighScore()
{
	int dyScore = 400 ;
	for (int i = 0 ; i < 5 ; i++ , dyScore -= 400 / 5 ){
		iText( 200 , dyScore , playerNames[ i ] ) ;
		char t[ 80 ] ;
		sprintf( t , "%d" , playerScores[ i ] ) ;
		iText( 400 , dyScore , t ) ;
	}
}


void iDraw()
{
	iClear();

	//int r = rand()%100;

	if ( currentscreen == splashscreen ) {
		iShowBMP( 0 , 0 , "tbird.bmp" ) ;
	}
	else
	if ( currentscreen == menuscreen ) {
		showMenu() ;
	}

	else if ( currentscreen == game ){
		showScore() ;
	    showSky() ;
		iShowBMPWOBG( bullet_x , bullet_y , "bullet.bmp" ) ;
		iShowBMPWOBG( plane_x , plane_y , "new1.bmp" ) ;
		iShowBMPWOBG( enemy_x , enemy_y , "gp.bmp" ) ;
		iShowBMPWOBG( obj_x , obj_y , "bomb.bmp" ) ;

	}

	else if ( currentscreen == highscorescreen ) {
		showHighScore() ;
	}

	else if ( currentscreen == textinputscreen ) {
		textInput() ;
	}

	else if ( currentscreen == gameover ){
		iShowBMP(0,0,"gameover.bmp");
	}

}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
	//place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		if( menuscreen == currentscreen  && mx >= 168 && my >= 512 - 150 && mx <= 576 && my <= 512-91 ){
			score = 0 ;
			lives = 3 ;
			plane_x = 250 , plane_y = 0 ;
			enemy_x = 250 , enemy_y = 500 ;
			obj_x = 300 , obj_y = 500 ;
			currentscreen = game;
		}
		else if ( menuscreen == currentscreen  && mx >= 165 && my >= 512 - 310 && mx <= 585 && my <= 512-250 ) {
			currentscreen = highscorescreen ;
		}
		else if ( menuscreen == currentscreen  && mx >= 81 && my >= 512 - 232 && mx <= 480 && my <= 512-172 ){
			 exit(0);
		}

	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

	}
}
/*iPassiveMouseMove is called to detect and use
the mouse point without pressing any button */

void iPassiveMouseMove(int mx,int my)
{

	//play
	if((mx>=167&&mx<=570)&&(my>=366&&my<=421))
		playFlag=1;
	else
		playFlag=0;
	//exit
	if((mx>=77&&mx<=476)&&(my>=283&&my<=336))
		exitFlag=1;
	else
		exitFlag=0;
	//score
	if((mx>=165&&mx<=582)&&(my>=206&&my<=259))
		scoreFlag=1;
	else
		scoreFlag=0;
}

void fireBullet( void )
{
	bullet_x = plane_x + 56 ;
	bullet_y = plane_y + 128 ;
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
	if(key == 'q')
	{
		//do something with 'q'
	}
	if ( currentscreen == textinputscreen ) {
		if ( isalnum( key ) || key == ' ' ) {
			name[ nameLength++ ] = key ;
			name[ nameLength ] = '\0' ;
		}
		else
            if ( key == '\r' ) {
			Sort() ;
			saveToFile();
			memset( name , 0 , sizeof( name ) ) ;
			nameLength = 0 ;
			currentscreen = highscorescreen ;
		}
		else if ( key == '\b' ) {
			name [ --nameLength ] = '\0' ;
		}
	}
	else if ( currentscreen == gameover && key == '\r' ) {
		if ( askforname ) currentscreen = textinputscreen ;
		else currentscreen = menuscreen ;
	}
	else if ( currentscreen == highscorescreen && key == '\r' ){
		currentscreen = menuscreen ;
	}
	if ( key == ' ' )
	{
		fireBullet() ;
	}
}


/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{

	if(key == GLUT_KEY_END)
	{
		exit(0);
	}
	if (key == GLUT_KEY_UP && plane_y + 10 + 128 <= 512 )
	{
		plane_y += 10 ;
	}
	if(key == GLUT_KEY_DOWN && plane_y - 10 >= 0 )
	{
		plane_y -= 10 ;
	}
	if(key == GLUT_KEY_LEFT && plane_x -10 >= 0 )
	{
		plane_x -= 10 ;
	}
	if(key == GLUT_KEY_RIGHT && plane_x + 10 + 128 <= 512 )
	{
		plane_x += 10 ;
	}

}


void endsplash( void )
{
	currentscreen = menuscreen ;
	iPauseTimer( splashtime ) ;
}

void moveSky( void )
{
	skyImageIdx = ( skyImageIdx + 1 ) % 33 ;
}

void nolife()
{
	askforname = checkHighScore() ;
	currentscreen = gameover ;
}

void moveEnemyOutOfScreen( void )
{
	enemy_y = 512 ;
	enemy_x = rand() % ( 512 - 128l ) ;
}

void enemyCollision( void )
{
	//showExplosion() ;
	lives-- ;
	moveEnemyOutOfScreen() ;
	if ( lives == 0 ) {
		nolife();
	}
}

void enemyMove( void )
{
	enemy_y -= enemyspeed ;
	if ( enemy_y <= 0 ) moveEnemyOutOfScreen() ;
	if ( plane_x + 128 >= enemy_x && plane_x <= enemy_x + 128 && plane_y + 128 >= enemy_y && plane_y <= enemy_y + 128 ) enemyCollision() ;
}

void enemyHit( void )
{
	score += 10 ;
	if ( score % 30 == 0 ) enemyspeed += 5 ;
	bullet_y = 9000 ;
	//showExplosion ;
	moveEnemyOutOfScreen() ;
}

void moveBullet( void )
{
	bullet_y += 5 ;
	if ( bullet_x >= enemy_x && bullet_x <= enemy_x + 128 && bullet_y >= enemy_y && bullet_y <= enemy_y + 128 ) {
		enemyHit() ;
	}
}

void moveObjOutOfScreen()
{
	obj_y = 512 ;
	obj_x = rand() % ( 512 - 128 ) ;
}

void objCollision( void )
{
	//showExplosion() ;
	lives-- ;
	moveObjOutOfScreen() ;
	if ( lives == 0 ) {
		nolife();
	}
}

void moveObj()
{
	obj_y -= enemyspeed / 2 ;
	if ( obj_y <= 0 ) moveObjOutOfScreen() ;
	if ( plane_x + 128 >= obj_x && plane_x <= obj_x + 67 && plane_y + 128 >= obj_y && plane_y <= obj_y + 77 ) objCollision() ;

}

int main()
{
	srand( ( unsigned )( time(NULL) % 1234 ) );
	copyFromFile() ;
	splashtime = iSetTimer( 2000 , endsplash ) ;
	skyMoveTimer = iSetTimer( 50 , moveSky ) ;
	enemyMoveTimer = iSetTimer( 200 , enemyMove ) ;
	bulletMoveTimer = iSetTimer( 50 , moveBullet ) ;
	objMoveTimer = iSetTimer( 50 , moveObj ) ;
	iInitialize(640, 512, "T-Bird");
	return 0;
}
