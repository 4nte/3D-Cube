#include <IRremote.h>

#define remote_digit_0 16738455
#define remote_digit_1 16724175
#define remote_digit_2 16718055
#define remote_digit_3 16743045



int TKD2 = 3;
int RECV_PIN = 3;
IRrecv irrecv(RECV_PIN);
decode_results results;

int DS_pin = 0;
int STCP_pin = 1;
int SHCP_pin = 2;

byte pointer[3] = {0,0,0};
byte pointer_dimension = 0;

const byte level[4] = {0, 1, 2, 3}; // baze tranzistora

const byte node[16] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19}; //pinovi ledica

boolean UI_drawing = false;
boolean cube[4][4][4];
boolean originalCubeState[4][4][4];
boolean pointerOverride = false;

unsigned int i, j;

unsigned long previousMillis = 0;   
const long UI_drawing_interval = 5;   



void setup()//declaring the pins as output.
{

  
  pinMode(DS_pin, OUTPUT);
  pinMode(STCP_pin, OUTPUT);
  pinMode(SHCP_pin, OUTPUT);
  writereg();
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      for (int z = 0; z < 4; z++) {
        cube[x][y][z] = false;
        if (cube[x][y][z]) {

        }
      }
    }
  }

  for (i = 0; i < 4; i++)
  {
    pinMode(level[i], OUTPUT);
  }
  for (i = 0; i < 16; i++)
  {
    pinMode(node[i], OUTPUT);
  }

  irrecv.enableIRIn();
  cube_main();
}
boolean registers[8];

void writereg() {
  digitalWrite(STCP_pin, LOW);

  for (int i = 7; i >= 0; i--) {
    digitalWrite(SHCP_pin, LOW);
    digitalWrite(DS_pin, registers[i] );
    digitalWrite(SHCP_pin, HIGH);
  }
  digitalWrite(STCP_pin, HIGH);
}



int getNodeNum(int x, int y) {
  if (y == 0)
    return x;
  if (y == 1)
    return 7 - x;
  if (y == 2)
    return 8 + x;
  if (y == 3)
    return 15 - x;
}




void animation_bridovi() {
  byte x = 0;
  while (x < 4) {

    cube[x][0][0] = true;
    cube[x][3][0] = true;
    cube[x][3][3] = true;
    cube[x][0][3] = true;

    cube[3][x][3] = true;
    cube[3][0][x] = true;
    cube[3][3][x] = true;
    cube[3][x][0] = true;

    cube[0][x][0] = true;
    cube[0][0][x] = true;
    cube[0][3][x] = true;
    cube[0][x][3] = true;
    x++;
  }


}

void _delay(unsigned long ms_interval)
{
  unsigned long previousMillis = millis();
  unsigned long currentMillis = millis();
  while (1)
  {
    renderCube();

    currentMillis = millis();
    if (currentMillis -  previousMillis >= ms_interval)
    {
      return;
    }
  }
}
void cube_main() {

}
void loop() {

  //cube[0][2][3] = true;
  //drawObject(13);


  /*
  drawObject(5);
  drawObject(6);
  drawObject(7);
  drawObject(8);
  drawObject(9);
  drawObject(10);
  drawObject(11);
  drawObject(12);
  */
  renderCube();
  recieveInput();
}



void resetCube() {
  for (int z = 0; z < 4; z++) {
    for (int x = 0; x < 4; x++) {
      for (int y = 0; y < 4; y++) {
        cube[x][y][z] = false;
      }
    }
  }

}
void nodesOff() {
  for (byte c = 0; c < 16; c++)
    digitalWrite(node[c], LOW);
}

void drawNode(byte dimension, byte val){

}

void clearPointer(){
  //TODO: provjeri jeli node na kojem je bio pointer vec prije dolaska pointera bio upaljen. Ako jest, onda ga nemoj gasiti.
  if(!pointerOverride)
    cube[pointer[0]][pointer[1]][pointer[2]] = false;
}
void drawPointer(){
  cube[pointer[0]][pointer[1]][pointer[2]] = true;
}
void spawnPointer(byte x, byte y, byte z){
  if(cube[x][y][z])
    pointerOverride = true;
  pointer[0] = x;
  pointer[1] = y;
  pointer[2] = z;
  pointer_dimension = 0;
  drawPointer();
  
}
void setPointer(byte dimension, byte val){
  clearPointer();
  pointer[dimension] = val;
  if(cube[pointer[0]][pointer[1]][pointer[2]])
    pointerOverride = true;
  else
    pointerOverride = false;
  drawPointer();
}
void pointerDraw(){
  cube[pointer[0]][pointer[1]][pointer[2]] = true;
}
void recieveInput() {
  if (irrecv.decode(&results)) {
    switch(results.value){
      case remote_digit_0:
        setPointer(pointer_dimension++,0);
       break;
       case remote_digit_1:
        setPointer(pointer_dimension++,1);
       break;
       case remote_digit_2:
        setPointer(pointer_dimension++,2);
       break;
       case remote_digit_3:
        setPointer(pointer_dimension++,3);
       break;
      case 16769055: // Minus
        clearPointer();
        spawnPointer(0,0,0);
      break;
      case 16754775: // Plus
        pointerDraw();
        spawnPointer(0,0,0);
      break;
      case 16748655: // Eq (ok)

      break;
     case 16720605: // Previous
      pointer_dimension--;
     break;
     case 16712445: // Next
      pointer_dimension++;
     break;

    }
    irrecv.resume(); // Receive the next value
  }

  if(pointer_dimension > 2){// resest pointer
    pointer_dimension = 0;
  }
  else if(pointer_dimension < 0){// resest pointer
    pointer_dimension = 2;
  }
    
}
void renderCube() {

 unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 120) {
    UI_drawing = true;
    previousMillis = currentMillis;
    memcpy(originalCubeState, cube, 64);
    if(pointer_dimension == 0){

      cube[0][pointer[1]][pointer[2]] = true;
       cube[1][pointer[1]][pointer[2]] = true;
        cube[2][pointer[1]][pointer[2]] = true;
         cube[3][pointer[1]][pointer[2]] = true;
    }
    if(pointer_dimension == 1){

      cube[pointer[0]][0][pointer[2]] = true;
       cube[pointer[0]][1][pointer[2]] = true;
        cube[pointer[0]][2][pointer[2]] = true;
         cube[pointer[0]][3][pointer[2]] = true;
    }
    if(pointer_dimension == 2){

      cube[pointer[0]][pointer[1]][0] = true;
       cube[pointer[0]][pointer[1]][1] = true;
        cube[pointer[0]][pointer[1]][2] = true;
         cube[pointer[0]][pointer[1]][3] = true;
    }
    
  }
  for (int z = 0; z < 4; z++) {
    for (int x = 0; x < 4; x++) {
      for (int y = 0; y < 4; y++) {
        if (cube[x][y][z]) {
          digitalWrite(node[getNodeNum(x, y)], HIGH);
        }
      }

    }

    registers[z+1] = HIGH;
    writereg();
    nodesOff();
    registers[z+1] = LOW;
    writereg();
    
    }
    if(UI_drawing){
      memcpy(cube, originalCubeState,64);
      UI_drawing = false;
  }
}


void drawObject(byte obj) {
  switch (obj) {
    case 1: // Dijagonala #1
      cube[0][0][0] = true;
      cube[1][1][1] = true;
      cube[2][2][2] = true;
      cube[3][3][3] = true;
      break;

    case 2: // Dijagonala #2
      cube[3][0][0] = true;
      cube[2][1][1] = true;
      cube[1][2][2] = true;
      cube[0][3][3] = true;
      break;

    case 3: // Dijagonala #3
      cube[0][3][0] = true;
      cube[1][1][1] = true;
      cube[2][2][2] = true;
      cube[3][0][3] = true;
      break;

    case 4: // Dijagonala #4
      cube[0][0][3] = true;
      cube[1][1][2] = true;
      cube[2][2][1] = true;
      cube[3][3][0] = true;
      break;

    case 5: // brid #1
      cube[0][0][0] = true;
      cube[1][0][0] = true;
      cube[2][0][0] = true;
      cube[3][0][0] = true;
      break;

    case 6: // brid #2
      cube[0][3][0] = true;
      cube[1][3][0] = true;
      cube[2][3][0] = true;
      cube[3][3][0] = true;
      break;

    case 7: // brid #3
      cube[0][0][0] = true;
      cube[0][1][0] = true;
      cube[0][2][0] = true;
      cube[0][3][0] = true;
      break;

    case 8: // brid #4
      cube[3][0][0] = true;
      cube[3][1][0] = true;
      cube[3][2][0] = true;
      cube[3][3][0] = true;
      break;

    case 9: // brid #4
      cube[0][0][0] = true;
      cube[0][0][1] = true;
      cube[0][0][2] = true;
      cube[0][0][3] = true;
      break;

    case 10: // brid #4
      cube[3][0][0] = true;
      cube[3][0][1] = true;
      cube[3][0][2] = true;
      cube[3][0][3] = true;
      break;

    case 11: // brid #4
      cube[0][3][0] = true;
      cube[0][3][1] = true;
      cube[0][3][2] = true;
      cube[0][3][3] = true;
      break;
    case 12: // brid #4
      cube[3][3][0] = true;
      cube[3][3][1] = true;
      cube[3][3][2] = true;
      cube[3][3][3] = true;
      break;

    case 13: // brid #4
      cube[0][0][0] = true;
      cube[1][0][0] = true;
      cube[2][0][0] = true;
      cube[3][0][0] = true;
      break;



  }
}





