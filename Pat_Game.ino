#include <U8glib.h>


#include "SegaController.h"
#include "Wizard_Sprite_Walk.h"
#include "Enemy_Sprite.h"
#include "Level.h"
#include "Tiles.h"
#include "Music.h"
#include "Logo.h"
#include "Playtune.h"


#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

//Melee Combat
//Teleportation Item
//Upgrade Health
//Upgrade Damage ????
//Add Spells?
//Movable objects
//Levers to change other things
//floor press switches to change things
//Tiles can convert to other tiles, but cannot otherwise be manipulated
//Tiles that deteriate, and break apart
//There are 15 baddies + 8 bosses in Super Mario
//SMB is 12 screens wide, each screen would take 1 second to run accross
//Midi convertor https://github.com/lenshustek/miditones


U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_FAST); // Fast I2C / TWI 


#define Version .08
#define DeathMon 3
#define Max_BadGuy 41
#define Max_Arrows 30





byte face_right,Char_Pos;
float Player_X,Player_Y,Player_Acc_X,Player_Acc_Y,Player_Speed_X,Player_Speed_Y;
byte World_X,World_Y;
int Speed;
byte Player_State,Player_HP;

// Controller states
word currentState = 0;
word lastState = 0;
uint8_t Current_Level[40][16];
bool Game_Started;



//MUSIC Playtune pt;

struct MissleObj
{
  int Pos_X;
  byte Pos_Y;
  byte Image;
  byte MType;//Also determines who/what is affected
  bool face_right;
  int Speed;
};

struct EnemyObj
{
  byte HP;
  int Pos_X;
  byte Pos_Y;
  byte Image;
  byte Behavior;
  bool face_right;
  byte MType;
  byte a,b;  //used for different things depending on MType, as as starting x,y
} ;


EnemyObj BadGuy[Max_BadGuy];
MissleObj Arrows[Max_Arrows];

struct TouchObj
{
  byte Blocks[9];
};



SegaController controller(5, 7, 4, 3, 2, 8, 6);
//SegaController(byte db9_pin_7, byte db9_pin_1, byte db9_pin_2, byte db9_pin_3, byte db9_pin_4, byte db9_pin_6, byte db9_pin_9);
//SegaController(SELC db9_pin_7, UP   db9_pin_1, DOWN db9_pin_2, LEFT db9_pin_3, RIGH db9_pin_4, A/B  db9_pin_6, C/ST db9_pin_9);
//Pinout
//1 : 5v 2: Right 3 : Left 4 : Down  5 : Up  6: GND  7: Select  8: B

// Controller DB9 pins (looking face-on to the end of the plug):
//
// 5 4 3 2 1
//  9 8 7 6
//
// Connect pin 5 to +5V and pin 8 to GND
// Connect the remaining pins to digital I/O pins (see below)

// Specify the Arduino pins that are connected to
// DB9 Pin 7, DB9 Pin 1, DB9 Pin 2, DB9 Pin 3, DB9 Pin 4, DB9 Pin 6, DB9 Pin 9


void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
for ( int x = -1; x < 16; x++ ) 
for ( int y = -1; y < 9; y++ ) 
  if ((x+World_X<40)and(y+World_Y<16)and(Current_Level[x+World_X][y+World_Y] > 0))
    u8g.drawXBMP( x*8, y*8, 8, 8, Tile[(Current_Level[x+World_X][y+World_Y])-0x20]); 
//    u8g.drawXBMP( x*8, y*8, 8, 8, Tile[pgm_read_byte(&Level[0][x+World_X][y+World_Y])-0x20]); 
    
  if (face_right)
      u8g.drawXBMP( truncf(Player_X), truncf(Player_Y), 16, 16, wizard_walk[Char_Pos]);
      else Draw_Mirror( truncf(Player_X), truncf(Player_Y), 16, 16, wizard_walk[Char_Pos]);
for ( int x = 0; x < Max_Arrows; x++ ) 
 
  if ((Arrows[x].Pos_X < 140)and(Arrows[x].MType != 0))
    u8g.drawXBMP( Arrows[x].Pos_X, Arrows[x].Pos_Y, 8, 8, fireball[Arrows[x].Image]);

    
for ( int x = 0; x < Max_BadGuy; x++ ) {
  if ((BadGuy[x].HP > 0)){
     if ((BadGuy[x].Pos_X-World_X*8)>-17)
        if ((BadGuy[x].Pos_X-World_X*8)<128)
           if ((BadGuy[x].Pos_Y-World_Y*8)>-17)
            if ((BadGuy[x].Pos_Y-World_Y*8)<65)
              switch (BadGuy[x].MType) {
              case 0 : drawXBMPl( BadGuy[x].Pos_X-World_X*8, BadGuy[x].Pos_Y-World_Y*8, pgm_read_byte(&sprite_size[BadGuy[x].MType][0] ), pgm_read_byte(&sprite_size[BadGuy[x].MType][1] ), enemy_walk[BadGuy[x].Image],BadGuy[x].face_right);
              break;
              case 1 : drawXBMPl( BadGuy[x].Pos_X-World_X*8, BadGuy[x].Pos_Y-World_Y*8, pgm_read_byte(&sprite_size[BadGuy[x].MType][0] ), pgm_read_byte(&sprite_size[BadGuy[x].MType][1] ), spider_walk[BadGuy[x].Image],BadGuy[x].face_right);
                        u8g.drawLine(BadGuy[x].Pos_X-World_X*8+8, BadGuy[x].a-World_Y*8,BadGuy[x].Pos_X-World_X*8+8, BadGuy[x].Pos_Y-World_Y*8+8);
              break;
              case 2 : drawXBMPl( BadGuy[x].Pos_X-World_X*8, BadGuy[x].Pos_Y-World_Y*8, pgm_read_byte(&sprite_size[BadGuy[x].MType][0] ), pgm_read_byte(&sprite_size[BadGuy[x].MType][1] ), skeleton_walk[BadGuy[x].Image],BadGuy[x].face_right);
              break;
              }}
              else if (BadGuy[x].MType == DeathMon)
                drawXBMPl( BadGuy[x].Pos_X-World_X*8, BadGuy[x].Pos_Y-World_Y*8,16,16, monster_death[BadGuy[x].Image],true);
  
              
              }
enum {BufSize=5}; // If a is short use a smaller number, eg 5 or 6 
char buf[BufSize];
snprintf (buf, BufSize, "%d", 10);
u8g.drawStr(0, 0, buf);
u8g.drawXBMP( 10, 10, 8, 8, 1);

}



void drawXBMPl( u8g_uint_t Pos_X,u8g_uint_t Pos_Y,u8g_uint_t Size_X,u8g_uint_t Size_Y, const u8g_pgm_uint8_t *bitmap,bool FaceLeft){
  if (FaceLeft)
  u8g.drawXBMP(Pos_X, Pos_Y, Size_X, Size_Y, bitmap);
  else Draw_Mirror(Pos_X, Pos_Y, Size_X, Size_Y, bitmap);
  
}

void Draw_Mirror_Line(u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t bitmap[16]){
  u8g_uint_t Temp;
  
    for (byte Position=0;Position<(w/16);Position++){
          Temp = bitmap[Position];
          bitmap[Position] = Bit_Reverse(bitmap[(w/8)-Position-1]);
          bitmap[(w/8)-Position-1] = Bit_Reverse(Temp);
          }
    u8g.drawXBM( x, y, w, 1, bitmap);
}

void Draw_Mirror(u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h, const u8g_pgm_uint8_t *bitmap){
  u8g_uint_t oneline[16];


  for (byte VPosition=0;VPosition<h;VPosition++){
    for (byte Position=0;Position<(w/8);Position++){
      oneline[Position] = pgm_read_byte(&bitmap[VPosition*(w/8)+Position]);
      }
      Draw_Mirror_Line(x,y+VPosition,w,oneline);
    }
}
u8g_uint_t Bit_Reverse( u8g_uint_t x ) 
{ 
   x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa); 
   x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc); 
   x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0); 
   return x;    
}
void setup(void) {
  resetup();
  Game_Started = false;
  

   
  }

void resetup(void) {
  bool test;
  int bgnum;
  u8g.setRot180();
   pinMode(13, OUTPUT);           
  digitalWrite(13, HIGH);  
  Char_Pos = 0;
  Player_X = 24;
  Player_Y = 10;
  Player_Acc_X = 0;
  Player_Acc_Y = 0;
  Player_Speed_X = 0;
  Player_Speed_Y = 0;
  Speed = 9;
  face_right = 1;
  Player_State = Stand;
  World_X = 0;
  World_Y = 0;
  Player_HP = 1;
  Game_Started = true;
  for ( int x = 0; x < Max_Arrows; x++ ) 
      Arrows[x].MType = 0;
  for ( int x = 0; x < Max_BadGuy; x++ ) {
  BadGuy[x].HP = 0;
 BadGuy[x].Pos_X = 128;
  BadGuy[x].Pos_Y = 128;
  BadGuy[x].Image = 0;
  BadGuy[x].Behavior = 0;
  BadGuy[x].MType = 0;

   
  }

//  pt.tune_initchan (10); //If all three are used, lose access to genesis controller
//MUSIC   pt.tune_initchan (11);
//MUSIC   pt.tune_initchan (12);

bgnum = 0;  
  for ( int x = 0; x < 40; x++ ) 
    for ( int y = 0; y < 16; y++ ) {
      Current_Level[x][y] = pgm_read_byte(&Level[0][x][y]);
      if (Current_Level[x][y] == 0x01){
        Player_X = x*8;
        World_X = x - 2;
        Player_Y = y*8;
        World_Y = y -6;
        Current_Level[x][y] = 0;
      }
      else if ((Current_Level[x][y] < 0x20)and(Current_Level[x][y] > 0x00)){
    
    //  test = Insert_Monster(x,y,Current_Level[x][y]);
    BadGuy[bgnum].Pos_X = x*8;//Bad Guy is X pixels from left
    BadGuy[bgnum].Pos_Y = y*8;//Bad Guy is Y pixels from Top
    BadGuy[bgnum].a = BadGuy[bgnum].Pos_Y;
    BadGuy[bgnum].HP = 1;
    BadGuy[bgnum].Image = 1;
    BadGuy[bgnum].MType = Current_Level[x][y]-2;
    BadGuy[bgnum].Behavior = BadGuy[bgnum].MType;
    bgnum++;
      Current_Level[x][y] = 0x00;
      
      }
         
    } 
}




void Player_Controls(void){
  byte Temp_x;


  currentState = controller.getState();
if (currentState != lastState){ 
  
  if ((!(lastState & SC_BTN_A))and(currentState & SC_BTN_A)and !(currentState & SC_BTN_DOWN)and OnGround(Player_X,Player_Y)){
    Player_Acc_Y = -10;
    Player_State = Slide;  
  }
  
  if ((!(lastState & SC_BTN_B))and(currentState & SC_BTN_B)and(Player_State!=Slide)){
      Player_State = Melee;
      Player_Acc_X = face_right*6-3;
  }
  if ((!(lastState & SC_BTN_C))and(currentState & SC_BTN_C)){
      Temp_x = 0;
      Player_State = Shoot;
      do {Temp_x++;}while (  Arrows[Temp_x].MType != 0);
      Arrows[Temp_x].Pos_X = Player_X;
      Arrows[Temp_x].Pos_Y = Player_Y;
      Arrows[Temp_x].Speed = 12-(1-face_right)*24;
      Arrows[Temp_x].face_right = face_right;
      Arrows[Temp_x].MType = 1;
  }

  if (currentState & SC_BTN_UP){

    Activate(Touching ((int)(truncf((Player_X)/8)+World_X),(int)(truncf((Player_Y)/8)+World_Y)), (int)(truncf((Player_X)/8)+World_X),(int)(truncf((Player_Y)/8)+World_Y));
    }




//if ((Player_State==Slide) and (Player_Speed_X<1)and (Player_Speed_X>-1))
    //Player_State = Stand;
  
  

    
  }
    
 
  if ((currentState & SC_BTN_UP)and OnLadder(Player_X,Player_Y)){
    Player_Acc_Y = -1;
    Player_State = Climb;
    Char_Pos = animation_stop[Player_State];
  }
    

  if ((currentState & SC_BTN_DOWN)and(currentState & SC_BTN_A)and((Player_State!=Slide))){
    Player_Acc_X = -(8-face_right*16);
    Player_State = Slide;  
  }
  if (currentState & SC_BTN_LEFT){
    Player_Acc_X = -2*(1-(Player_State==Slide));
    face_right = false;
    if (!Slide){
        Player_State = Walk;  
        }
  }
  if (currentState & SC_BTN_RIGHT){
    Player_Acc_X = 2*(1-(Player_State==Slide));
    face_right = true;
    if (!Slide){
      Player_State = Walk;  
        }
      
  }

  if ((Player_State == Climb)  and !OnLadder(Player_X,Player_Y)){
        Player_State = Stand;
        

  }
        
  if (currentState & SC_BTN_START){
      setup();  
      }
 

    
}
TouchObj Touching(int x,int y){
TouchObj TB; 

  TB.Blocks[0] = Current_Level[x][y];
  TB.Blocks[1] = Current_Level[x+1][y];
  TB.Blocks[2] = Current_Level[x+2][y];
  TB.Blocks[3] = Current_Level[x][y+1];
  TB.Blocks[4] = Current_Level[x+1][y+1];
  TB.Blocks[5] = Current_Level[x+2][y+1];
  TB.Blocks[6] = Current_Level[x][y+2];
  TB.Blocks[7] = Current_Level[x+1][y+2];
  TB.Blocks[8] = Current_Level[x+2][y+2];

return (TB);
}

bool Activate(TouchObj TPoints,int x,int y){
int ax,ay;
byte op;
op = 2;
for (byte TX=x-2; TX < x+1;TX++){
for (byte TY=y-2; TY < y+1;TY++){
if (Current_Level[TX][TY] == 0x41){
op = 0;
ax = TX;
ay = TY;}
if (Current_Level[TX][TY] == 0x47){
op = 1;
ax = TX;
ay = TY;}

}
}    

if (op == 0){
        Current_Level[ax][ay] = 0x47;
        Current_Level[ax+1][ay] = 0x45;
        Current_Level[ax][ay+1] = 0x48;
        Current_Level[ax+1][ay+1] = 0x46;
return (true);

}
if (op == 1){
        Current_Level[ax][ay] = 0x41;
        Current_Level[ax+1][ay] = 0x43;
        Current_Level[ax][ay+1] = 0x42;
        Current_Level[ax+1][ay+1] = 0x44;
return (true);
}
if (op == 2)
  return false;  
}

int rightadjust(int x1,int x2,int size1, int size2){
if (x1<x2)
  return (size1);
  else return (size2);
}

bool BoxCollision(byte Enemy_Num){
  for (int Box_N=0;Box_N<Max_Arrows;Box_N++){
      if ((Arrows[Box_N].MType > 0) and
        (Arrows[Box_N].Pos_X < 128) and 
        (BadGuy[Enemy_Num].HP > 0) and 
         (abs(BadGuy[Enemy_Num].Pos_X-(World_X)*8-(Arrows[Box_N].Pos_X)<(rightadjust((Arrows[Box_N].Pos_X),(BadGuy[Enemy_Num].Pos_X-(World_X)*8),8,(pgm_read_byte(&sprite_size[BadGuy[Enemy_Num].MType][0]))))) and
             (abs(BadGuy[Enemy_Num].Pos_Y-(World_Y)*8-Arrows[Box_N].Pos_Y)<(rightadjust(BadGuy[Enemy_Num].Pos_Y-(World_Y)*8,Arrows[Box_N].Pos_Y,(pgm_read_byte(&sprite_size[BadGuy[Enemy_Num].MType][1])),8)))))
                  return true;
  
}
return false;
}

bool Collision(byte x){
 unsigned short Temp_Player,Temp_BadGuy;
 byte X_Offset,Y_Offset,TPX,TBX;
      if (BadGuy[x].HP > 0)
        if ((abs(BadGuy[x].Pos_X-(World_X)*8-Player_X)) < (rightadjust(((World_X)*8-Player_X),(BadGuy[x].Pos_X),(16),(pgm_read_byte(&sprite_size[BadGuy[x].MType][0]))))){
        
      //      if (abs(BadGuy[x].Pos_Y-(World_Y-1)*8-Player_Y)<(16 pgm_read_byte(&+sprite_size[x][1]))){
              //u8g.drawXBMP( BadGuy[x].Pos_X-World_X*8, BadGuy[x].Pos_Y-World_Y*8, 16, 16, enemy_walk[BadGuy[x].Image]);
              //u8g.drawXBMP( truncf(Player_X), truncf(Player_Y), 16, 16, wizard_walk[Char_Pos]);
              //pgm_read_byte(&Level[0][x][y]);

              
              X_Offset = truncf(Player_X) -  (BadGuy[x].Pos_X-World_X*8);
              Y_Offset = truncf(Player_Y) -  (BadGuy[x].Pos_Y-World_Y*8);

              for (byte i=0;i<(16-abs(Y_Offset));i++){
              if (Y_Offset> 0)
              {//Badguy is higher on screen than player
                //CHECK_BIT(temp, 3)
              Temp_Player = pgm_read_byte(&wizard_walk[Char_Pos]+i*2-Y_Offset*2)+pgm_read_byte(&wizard_walk[Char_Pos]+i*2+1-Y_Offset*2);
              Temp_BadGuy = pgm_read_byte(&enemy_walk[BadGuy[x].Image]+i*2)+pgm_read_byte(&enemy_walk[BadGuy[x].Image]+i*2+1);
              TPX = -1;
              do{
              TPX++;  
              //truncf(Player_Y) + Y_Offset + i
              //truncf(Player_X) + X_Offset + TPX x is tricky because have to start at play_x and look at xth bit
              }while ((TPX<15)and(!CHECK_BIT(Temp_Player, TPX)));
             TBX = 16;
              do{
              TBX--;  
              //truncf(Player_Y) + Y_Offset + i
              //truncf(Player_X) + X_Offset + TPX x is tricky because have to start at play_x and look at xth bit
              }while ((TBX>0)and(!CHECK_BIT(Temp_BadGuy, TBX)));

              if (TPX <= TBX+X_Offset-8)
                return true;  
              if (TPX+X_Offset-8 >= TBX)
                return true;                  
                }
              else {//Player is higher on screen than badguy
                //CHECK_BIT(temp, 3)
              Temp_Player = pgm_read_byte(&wizard_walk[Char_Pos]+i*2)+pgm_read_byte(&wizard_walk[Char_Pos]+i*2+1);
              Temp_BadGuy = pgm_read_byte(&enemy_walk[BadGuy[x].Image]+i*2-Y_Offset*2)+pgm_read_byte(&enemy_walk[BadGuy[x].Image]+i*2+1-Y_Offset*2);
              TPX = 16;
              do{
              TPX--;  
              //truncf(Player_Y) + Y_Offset + i
              //truncf(Player_X) + X_Offset + TPX x is tricky because have to start at play_x and look at xth bit
              }while ((TPX>0)and(!CHECK_BIT(Temp_Player, TPX)));
             TBX = -1;
              do{
              TBX++;  
              //truncf(Player_Y) + Y_Offset + i
              //truncf(Player_X) + X_Offset + TPX x is tricky because have to start at play_x and look at xth bit
              }while ((TBX<15)and(!CHECK_BIT(Temp_BadGuy, TBX)));
              if (TPX+X_Offset-8 >= TBX)
                return true;   
              if (TPX <= TBX+X_Offset-8)
                return true;  
              }
              }
              
            }


  return false;
}

void BadGuy_Move(byte id_num){
  BadGuy[id_num].Image++;
  if (BadGuy[id_num].Image > pgm_read_byte(&sprite_size[BadGuy[id_num].MType][2] ))  
      BadGuy[id_num].Image = 0;



    if (OnScreen(BadGuy[id_num].Pos_X,BadGuy[id_num].Pos_Y,pgm_read_byte(&sprite_size[BadGuy[id_num].MType][0] ),pgm_read_byte(&sprite_size[BadGuy[id_num].MType][1] )))
    switch (BadGuy[id_num].Behavior) {
      case 0 : if (BadGuy[id_num].Pos_X < Player_X+World_X*8){//Standard Bad Guy
                    BadGuy[id_num].Pos_X = BadGuy[id_num].Pos_X + 1;
                    BadGuy[id_num].face_right = true;
                    }
                  else {
                    BadGuy[id_num].face_right = false;
                    BadGuy[id_num].Pos_X = BadGuy[id_num].Pos_X - 1;
                  }
               
      break;
      case 1 : if (BadGuy[id_num].Pos_Y < Player_Y+World_Y*8)//Spider
                  BadGuy[id_num].Pos_Y = BadGuy[id_num].Pos_Y + 1;
                  else if (BadGuy[id_num].Pos_Y>BadGuy[id_num].a)
                          BadGuy[id_num].Pos_Y = BadGuy[id_num].Pos_Y - 1;
      break;
      default : 
      break;
    }
    
    

}

boolean OnScreen(int Pos_X, byte Pos_Y,byte size_x, byte size_y){
       if ((Pos_X-World_X*8)>-(1+size_x))
        if ((Pos_X-World_X*8)<128)
           if ((Pos_Y-World_Y*8)>-(1+size_x))
              if ((Pos_Y-World_Y*8)<65)
                return true;
return false;
}

void Player_Physics(void){
    float Player_New_X,Player_New_Y;
    
  Player_New_X = Player_X;
  Player_New_Y = Player_Y;
  
    Player_Speed_X = Player_Speed_X / 2;
    Player_Speed_Y = Player_Speed_Y / 2;
    
    Player_Speed_X = Player_Speed_X + Player_Acc_X;
    Player_Acc_X = Player_Acc_X * 0.5;
    
    Player_Speed_Y = Player_Speed_Y + Player_Acc_Y;
    Player_Acc_Y = Player_Acc_Y * 0.5;
    
    Player_New_X = Player_New_X + Player_Speed_X;
    Player_New_Y = Player_New_Y + Player_Speed_Y;

  if (HitWall(Player_New_X,Player_New_Y)){
    Player_Acc_X = 0;    
    Player_Speed_X = 0;   
    Player_New_X = Player_X;
  }
        
            

  if (Player_Acc_X > 0)
      Player_Acc_X = Player_Acc_X- 1;
  if (Player_Acc_X < 0)
      Player_Acc_X = Player_Acc_X+ 1;
  if (Player_Acc_Y > 0)
      Player_Acc_Y = Player_Acc_Y- 1;
  if (Player_Acc_Y < 0)
      Player_Acc_Y = Player_Acc_Y+ 1;


  if (Player_Acc_X > 5)
      Player_Acc_X = 5;
  if (Player_Acc_X < -5)
      Player_Acc_X = -5;
  if (Player_Acc_Y > 5)
      Player_Acc_Y = 5;
  if (Player_Acc_Y < -5)
      Player_Acc_Y = -5;

    
if (!(OnGround(Player_New_X,Player_New_Y)))
    Player_Acc_Y = Player_Acc_Y + 2;
    else {
      if (OnGround(Player_X,Player_Y)){
            Player_Acc_Y = 0;
            Player_Speed_Y = 0;}
          else {
            if (!((truncf(Player_New_Y/8)*8) == Player_New_Y)){
            Player_New_Y = truncf(Player_New_Y);
            Player_Acc_Y = 0;
            Player_Speed_Y = 0;}
          }
        
    }

    
    Player_X = Player_New_X;
    Player_Y = Player_New_Y;  


if ((InGround(Player_X,Player_Y))){
      Player_Acc_Y = -5;}
  
    
//Move World -------------------------
  if (Player_X> 100){
      Player_X = 90;
      World_X = World_X + 1;
  }
  if (Player_X< 10){
      Player_X = 16;
      World_X = World_X - 1;
  }

  if (Player_Y> 42){
      Player_Y = 38;
      World_Y = World_Y + 1;
  }
  if (Player_Y< 8){
      Player_Y = 16;
      World_Y = World_Y - 1;
  }

  if (World_X > 140)
    World_X = 0;
  if (World_Y > 15)
    World_Y = 0;
//Move World -------------------------

if (Player_State == MeleeTop){
    Player_State = Stand;  
 
}  

  for ( int x = 0; x < Max_Arrows; x++ ) {
    if ((Arrows[x].MType != 0)and (Arrows[x].Pos_X < 128)and (Arrows[x].Pos_X > -17)){
    Arrows[x].Pos_X = Arrows[x].Pos_X + Arrows[x].Speed;
    if (Arrows[x].Image > 3)
      Arrows[x].Image = 0;
    }
    else Arrows[x].MType = 0;
}

  for ( int x = 0; x < Max_BadGuy; x++ ) {
  if (BoxCollision(x)){
        BadGuy[x].HP = 0;
        BadGuy[x].Image = 0;
        BadGuy[x].MType = DeathMon;
        
        }  
    if (Collision(x)){
    if ((Player_State==Slide) or ((Player_State>=Melee)and(Player_State<=MeleeTop))){
        BadGuy[x].HP = 0;
        BadGuy[x].Image = 0;
        BadGuy[x].MType = DeathMon;
        
        }
      //  else Player_HP = 0;
        
     }}
  

lastState = currentState;    
}
 


boolean OnLadder(float Player_X, float Player_Y){
if (((Current_Level[(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y+1)]) == 0x40)or((Current_Level[(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y)]) == 0x40))
  return true;
  
if ((((Current_Level[(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y+1)]) == 0x40))and((Current_Level[(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y)]) == 0x40))
  return true;
return false;
}

boolean OnGround(float Player_X, float Player_Y){//maybe iterate, and only move one pixel at a time
if ((((Current_Level[(byte)(truncf((Player_X)/8)+World_X)][(byte)(truncf((Player_Y)/8)+World_Y+2)]) > 0x1F))and((Current_Level[(byte)(truncf((Player_X)/8)+World_X)][(byte)(truncf((Player_Y)/8)+World_Y+2)]) < 0x2E))
  return true;
  
if ((((Current_Level[(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y+2)]) > 0x1F))and((Current_Level[(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y+2)]) < 0x2E))
  return true;
  
  
return false;
}

boolean InGround(float Player_X, float Player_Y){

  if (OnGround(Player_X,Player_Y)){
    if ((truncf(Player_Y/8)*8)== Player_Y){
        if (((
          (Current_Level[(byte)(truncf((Player_X)/8)+World_X)][(byte)(truncf((Player_Y)/8)+World_Y+1)]) > 0x1F)  
          and  
          (Current_Level[(byte)(truncf((Player_X)/8)+World_X)][(byte)(truncf((Player_Y)/8)+World_Y+1)]) < 0x2E) ) 
          return true;
        if (((
          (Current_Level[(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y+1)]) > 0x1F)  
          and  
          (Current_Level[(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y+1)]) < 0x2E) )  
                    
           return true;
        
return false;
     }
      
      else return false;
  }

else return false;
  
  
}


boolean HitWall(float Player_X, float Player_Y){
if (((Current_Level[(byte)(truncf((Player_X)/8)+World_X)][(byte)(truncf((Player_Y)/8)+World_Y)]) > 0x1F)and(Current_Level[(byte)(truncf((Player_X)/8)+World_X)][(byte)(truncf((Player_Y)/8)+World_Y)]) < 0x2E)
  return true;
if (((Current_Level[(byte)(truncf((Player_X)/8)+World_X)][(byte)(truncf((Player_Y)/8)+World_Y)+1]) > 0x1F)and(Current_Level[(byte)(truncf((Player_X)/8)+World_X)][(byte)(truncf((Player_Y)/8)+World_Y)+1]) < 0x2E)
  return true;
if (((Current_Level[(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y)]) > 0x1F)and(Current_Level[(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y)]) < 0x2E)
  return true;
if (((Current_Level[(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y+1)]) > 0x1F)and(Current_Level[(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y)+1]) < 0x2E)
  return true;
return false;
}




boolean OnLadder_(float Player_X, float Player_Y){
if (((pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y+1)]) == 0x40))or((pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y)]) == 0x40)))
  return true;
  
if (((pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y+1)]) == 0x40))and((pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y)]) == 0x40)))
  return true;
return false;
}

boolean OnGround_(float Player_X, float Player_Y){//maybe iterate, and only move one pixel at a time
if (((pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)][(byte)(truncf((Player_Y)/8)+World_Y+2)]) > 0x1F))and((pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)][(byte)(truncf((Player_Y)/8)+World_Y+2)]) < 0x2E)))
  return true;
  
if (((pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y+2)]) > 0x1F))and((pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y+2)]) < 0x2E)))
  return true;
  
  
return false;
}

boolean InGround_(float Player_X, float Player_Y){

  if (OnGround(Player_X,Player_Y)){
    if ((truncf(Player_Y/8)*8)== Player_Y){
        if (((pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)][(byte)(truncf((Player_Y)/8)+World_Y+1)]) > 0x1F))and((pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)][(byte)(truncf((Player_Y)/8)+World_Y+1)]) < 0x2E)))
          return true;
        if (((pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y+1)]) > 0x1F))and((pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y+1)]) < 0x2E)))
          return true;
        
        return false;
      }
      
      else return false;
  }
  else return false;
  
  
}


boolean HitWall_(float Player_X, float Player_Y){
if ((pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)][(byte)(truncf((Player_Y)/8)+World_Y)]) > 0x1F)and(pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)][(byte)(truncf((Player_Y)/8)+World_Y)]) < 0x2E))
  return true;
if ((pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)][(byte)(truncf((Player_Y)/8)+World_Y)+1]) > 0x1F)and(pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)][(byte)(truncf((Player_Y)/8)+World_Y)+1]) < 0x2E))
  return true;
if ((pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y)]) > 0x1F)and(pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y)]) < 0x2E))
  return true;
if ((pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y+1)]) > 0x1F)and(pgm_read_byte(&Level[0][(byte)(truncf((Player_X)/8)+World_X)+1][(byte)(truncf((Player_Y)/8)+World_Y)+1]) < 0x2E))
  return true;
return false;
}


void Player_Graphics(void){

  if (Player_HP > 0){

  Char_Pos ++ ;
  
  if (Char_Pos > pgm_read_byte(&animation_stop[Player_State]))
      Char_Pos = pgm_read_byte(&animation_stop[Player_State-1]);
  if (Char_Pos < pgm_read_byte(&animation_stop[Player_State-1]))
      Char_Pos = pgm_read_byte(&animation_stop[Player_State]);
      

  /*
  if ((Player_State >= Melee)and(Player_State < MeleeTop)){
    Player_State++;
    if (Char_Pos<MeleeTop)
      Char_Pos = Melee;
      else {
        Player_State ++;
        switch(Player_State){
          case 3 : Char_Pos = Melee;
            break;
          case 4 : Char_Pos = MeleeTop - 1;
            break;
          case 5 : Char_Pos = MeleeTop;
            break;
          case 6 : Char_Pos = MeleeTop;
            break;
          case 7 : Char_Pos = MeleeTop;
            break;
        }
      }
  }

  */
  if ((truncf(Player_Speed_X) == 0)&&(truncf(Player_Speed_Y) == 0)){
    Char_Pos = 0;  
    Player_State = Stand;
  }
    

  if ((Player_Speed_Y>0)and(OnGround(Player_X,Player_Y)==false))
      Player_State = Jump;
  if ((Player_Speed_Y<0)and(OnGround(Player_X,Player_Y)==false))
      Player_State = Fall;
  }


}


void loop(void) {
  // main loop
    Char_Pos = 0;
  if (!Game_Started){


    
  u8g.firstPage();  
  do {
 /*   u8g.drawXBMP( 0, 0, 16, 16, wizard_walk[0]);
    u8g.drawXBMP( 16, 0, 16, 16, wizard_walk[1]);
    u8g.drawXBMP( 32, 0, 16, 16, wizard_walk[2]);
    u8g.drawXBMP( 48, 0, 16, 16, wizard_walk[3]);
    u8g.drawXBMP( 64, 0, 16, 16, wizard_walk[4]);
    u8g.drawXBMP( 80, 0, 16, 16, wizard_walk[5]);
    u8g.drawXBMP( 96, 0, 16, 16, wizard_walk[6]);
    u8g.drawXBMP( 112, 0, 16, 16, wizard_walk[7]);

/*
    u8g.drawXBMP( 0, 0, 16, 16, wizard_walk[24]);
    u8g.drawXBMP( 16, 0, 16, 16, wizard_walk[25]);
    u8g.drawXBMP( 32, 0, 16, 16, wizard_walk[26]);
    u8g.drawXBMP( 48, 0, 16, 16, wizard_walk[27]);
    u8g.drawXBMP( 64, 0, 16, 16, wizard_walk[28]);
    u8g.drawXBMP( 80, 0, 16, 16, wizard_walk[29]);
    u8g.drawXBMP( 96, 0, 16, 16, wizard_walk[31]);
    u8g.drawXBMP( 112, 0, 16, 16, wizard_walk[32]);
*/
    u8g.drawXBMP( 0, 16, 16, 16, wizard_walk[8]);
    u8g.drawXBMP( 16, 16, 16, 16, wizard_walk[9]);
    u8g.drawXBMP( 32,16, 16, 16, wizard_walk[10]);
    u8g.drawXBMP( 48, 16, 16, 16, wizard_walk[11]);
    u8g.drawXBMP( 64, 16, 16, 16, wizard_walk[12]);
    u8g.drawXBMP( 80, 16, 16, 16, wizard_walk[13]);
    u8g.drawXBMP( 96, 16, 16, 16, wizard_walk[14]);
    u8g.drawXBMP( 112, 16, 16, 16, wizard_walk[15]);

    u8g.drawXBMP( 0, 32, 16, 16, wizard_walk[16]);
    u8g.drawXBMP( 16, 32, 16, 16, wizard_walk[17]);
    u8g.drawXBMP( 32, 32, 16, 16, wizard_walk[18]);
    u8g.drawXBMP( 48, 32, 16, 16, wizard_walk[19]);
    u8g.drawXBMP( 64, 32, 16, 16, wizard_walk[20]);
    u8g.drawXBMP( 80, 32, 16, 16, wizard_walk[21]);
    u8g.drawXBMP( 96, 32, 16, 16, wizard_walk[22]);
    u8g.drawXBMP( 112, 32, 16, 16, wizard_walk[23]);
    

    
 // u8g.drawXBMP( 0, 0, 128, 64, Logo);
} while( u8g.nextPage() );

do {
//MUSIC   if (!(pt.tune_playing) )
//MUSIC       pt.tune_playscore (score);    

  Player_Controls();
} while(!(currentState & SC_BTN_A)and!(currentState & SC_BTN_B)and!(currentState & SC_BTN_C));
  Game_Started = true;
  }
  else {
  if (Player_HP > 0)
      Player_Physics();
  Player_Controls();
  Player_Graphics();
  for (byte x=0;x<Max_BadGuy;x++){
    if ((BadGuy[x].HP > 0)){
     BadGuy_Move(x);}
    if (BadGuy[x].MType == DeathMon){
      BadGuy[x].Image ++;
      if (BadGuy[x].Image > pgm_read_byte(&sprite_size[BadGuy[x].MType][2] )) 
       BadGuy[x].MType = 0;
    }
     
}
              
    
  
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
//MUSIC   if (!(pt.tune_playing) )
//MUSIC       pt.tune_playscore (score);    
 
  // rebuild the picture after some delay
  delay(Speed);}
}

