#include <stdio.h>
#include <stdlib.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// DEFINES TOUCH
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin
#define MINPRESSURE 5
#define MAXPRESSURE 1000
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940
#define SEM_CLICK 255

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define BOARD_COLOR CYAN
#define X_COLOR BLUE
#define O_COLOR MAGENTA
#define BACKGROUND_COLOR BLACK
#define TAM_X 25

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

typedef struct node
{
  char info[3][3];
  struct node *lado;
  struct node *baixo;
} Arv;

int i, pos = 0;
int contador = 2;
uint8_t multiplayer = 0;
extern TouchScreen ts;
extern Adafruit_TFTLCD tft;

// Definições da árvore
Arv *a;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  tft.reset();
  uint16_t identifier = 0x9341;
  tft.begin(identifier);

  tft.fillScreen(BLACK);

  drawBoard();

  a = cria_vazia();
}

void loop() {

  // put your main code here, to run repeatedly:
  //  do{
  //    pos = getJogadaTouch();
  //    Serial.println(pos);
  //  } while(pos == SEM_CLICK);
  //  if (i % 2 == 0)
  //    drawSelected(pos, 1);
  //  else
  //    drawSelected(pos, 0);
  //  delay(500);
  //  i++;

  while (!jogoacabou(a)) {
    if (contador % 2 == 0) {
      //insere_simb(a, 'X');
      drawSelected(insere_simb(a, 'X'), 0);
      delay(1000);
    } else {
      if (multiplayer) {
        //insere_simb(a, 'O');
        drawSelected(insere_simb(a, 'O'), 1);
      } else {
        drawSelected(vezDoPc(a, 'O'), 1);
      }
    }
    contador++;
    switch (ganhou(a)) {
      case -1:
        tft.setCursor(100, 140);
        tft.fillScreen(BLUE);
        tft.setTextColor(RED);    tft.setTextSize(3);
        tft.println("O Ganhou");
        delay(5000);
        break;
      case 1:
        tft.setCursor(100, 140);
        tft.fillScreen(MAGENTA);
        tft.setTextColor(RED);    tft.setTextSize(3);
        tft.println("X Ganhou");
        delay(5000);
        break;
      case 2:
        tft.setCursor(100, 140);
        tft.fillScreen(YELLOW);
        tft.setTextColor(RED);    tft.setTextSize(3);
        tft.println("Empate");
        delay(5000);
        break;
      default:
        //tft.fillScreen(YELLOW);
        break;
    }
    //delay(100);
    //printJogo(a);
  }
}

void printJogo(Arv* a) {
  int i, j;
  tft.fillScreen(BLACK);
  drawBoard();
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      if (a->info[i][j] == 'X')
        drawSelected(XY_to_pos(i, j), 0);
      else if (a->info[i][j] == '0')
        drawSelected(XY_to_pos(i, j), 1);
    }
  }
}

Arv* cria_vazia() {
  Arv* a = (Arv*) malloc(sizeof(Arv));
  int i, j;
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      a->info[i][j] = 32;
    }
  }
  a->lado = NULL;
  a->baixo = NULL;
  return a;
}

int insere_simb(Arv* a, char simb) {
  int pos;
  do {
    pos = getJogadaTouch();
  } while (pos == SEM_CLICK);
  switch (pos) {
    case 0:
      a->info[0][0] = simb;
      break;
    case 1:
      a->info[0][1] = simb;
      break;
    case 2:
      a->info[0][2] = simb;
      break;
    case 3:
      a->info[1][0] = simb;
      break;
    case 4:
      a->info[1][1] = simb;
      break;
    case 5:
      a->info[1][2] = simb;
      break;
    case 6:
      a->info[2][0] = simb;
      break;
    case 7:
      a->info[2][1] = simb;
      break;
    case 8:
      a->info[2][2] = simb;
      break;
    default:
      a->info[2][2] = simb;
      break;
  }
  // Espera o botão ser desprecionado
  delay(50);
  return pos;
}

int minimax(Arv* a, int jogador) {
  int winner = ganhou(a);
  if (winner != 0) return winner * jogador;
  int l, c;
  int move = -1;
  int score = -2;
  char simb;
  for (l = 0; l < 3; ++l) {
    for (c = 0; c < 3; ++c) { //Para todos movimentos,
      if (a->info[l][c] == 32) { //Se tiver vago,
        if (jogador == -1)
          simb = 'X';
        if (jogador == 1)
          simb = 'O';
        a->info[l][c] = simb; //faz o movimento
        int thisScore = -minimax(a, jogador * -1);
        if (thisScore > score) {
          score = thisScore;
          move = l;
        }//Pega a pior jogada para o oponente
        a->info[l][c] = 32;//Torna-o vago novamente
      }
    }
  }
  if (move == -1) return 0;
  return score;
}

int ganhou(Arv* a) {
  //Retorna -1 se X ganhou,1 se O ganhou,2 se empatou,0 se nao acabou ainda
  int i, l, c, flagocol = 0, flagxcol = 0, flagxlinha = 0, flagolinha = 0, velha = 0, flagxdig = 0, flagodig = 0;
  //Linhas e Colunas
  for (l = 0; l < 3; l++)
  {
    flagocol = flagxcol = flagxlinha = flagolinha = 0;
    for (c = 0; c < 3; c++)
    {
      if (a->info[l][c] == 'x' || a->info[l][c] == 'X') {
        flagxcol++;
        velha++;
      }
      else {
        if (a->info[l][c] == 'o' || a->info[l][c] == 'O') {
          flagocol++;
          velha++;
        }
      }
      if (a->info[c][l] == 'x' || a->info[c][l] == 'X') {
        flagxlinha++;
      }
      else {
        if (a->info[c][l] == 'o' || a->info[c][l] == 'O')
          flagolinha++;
      }
    }
    if (flagxcol == 3 || flagxlinha == 3) {
      velha = 0;
      return -1;
    }
    if (flagocol == 3 || flagolinha == 3) {
      velha = 0;
      return 1;
    }
  }
  if (velha == 9) {
    return 2;
  }
  //Diagonal Principal
  for (l = 0; l < 3; l++) {
    if (a->info[l][l] == 'x' || a->info[l][l] == 'X') {
      flagxdig++;
    } else {
      if (a->info[l][l] == 'o' || a->info[l][l] == 'O')
        flagodig++;
    }
  }

  if (flagodig == 3) {
    return 1;
  } else if (flagxdig == 3) {
    return -1;
  }
  //Diagonal Secundaria
  flagxdig = flagodig = 0;
  if (a->info[0][2] == 'x' || a->info[0][2] == 'X')
    flagxdig++;
  if (a->info[1][1] == 'x' || a->info[1][1] == 'X')
    flagxdig++;
  if (a->info[2][0] == 'x' || a->info[2][0] == 'X')
    flagxdig++;
  if (a->info[0][2] == 'o' || a->info[0][2] == 'O')
    flagodig++;
  if (a->info[1][1] == 'o' || a->info[1][1] == 'O')
    flagodig++;
  if (a->info[2][0] == 'o' || a->info[2][0] == 'O')
    flagodig++;

  if (flagodig == 3) {
    return 1;
  } else if (flagxdig == 3) {
    return -1;
  }
  return 0;
}

int vezDoPc(Arv* a, char simb) {
  int linha = -1, coluna = -1;
  int score = -2;
  int l, c;
  // Para todas as opcoes de jogadas
  for (l = 0; l < 3; l++) {
    for (c = 0; c < 3; c++) {
      // Se estiver vago
      if (a->info[l][c] == 32) {
        // CPU faz a jogada
        a->info[l][c] = 'O';
        // A jogada eh analisada
        int tempScore = -minimax(a, -1);
        // Volta ao estado anterior esvaziando o espaco preenchido
        a->info[l][c] = 32;
        // Se a jogada tiver um score maior que os anteriores
        if (tempScore > score) {
          // Adiciona a jogada como a melhor
          score = tempScore;
          linha = l;
          coluna = c;
        }
      }
    }
  }
  a->info[linha][coluna] = simb;
  Serial.println(linha);
  Serial.println(coluna);
  Serial.println(XY_to_pos(linha, coluna));
  return XY_to_pos(linha, coluna);
}

int jogoacabou(Arv* a) {
  int i, l, c, flagocol = 0, flagxcol = 0, flagxlinha = 0, flagolinha = 0, velha = 0, flagxdig = 0, flagodig = 0;
  //Linhas e Colunas
  for (l = 0; l < 3; l++)
  {
    flagocol = flagxcol = flagxlinha = flagolinha = 0;
    for (c = 0; c < 3; c++)
    {
      if (a->info[l][c] == 'x' || a->info[l][c] == 'X') {
        flagxcol++;
        velha++;
      }
      else {
        if (a->info[l][c] == 'o' || a->info[l][c] == 'O') {
          flagocol++;
          velha++;
        }
      }
      if (a->info[c][l] == 'x' || a->info[c][l] == 'X') {
        flagxlinha++;
      }
      else {
        if (a->info[c][l] == 'o' || a->info[c][l] == 'O')
          flagolinha++;
      }
    }
    if (flagxcol == 3 || flagxlinha == 3) {
      printf("X Ganhou!\n");
      velha = 0;
      return 1;
    }
    if (flagocol == 3 || flagolinha == 3) {
      printf("O Ganhou!\n");
      velha = 0;
      return 1;
    }
  }
  if (velha == 9) {
    printf("O jogo deu velha!\n");
    return 1;
  }
  //Diagonal Principal
  for (l = 0; l < 3; l++)
  {
    if (a->info[l][l] == 'x' || a->info[l][l] == 'X') {
      flagxdig++;
    }
    else {
      if (a->info[l][l] == 'o' || a->info[l][l] == 'O')
        flagodig++;
    }
  }
  if (flagodig == 3) {
    printf("O Ganhou!\n");
    return 1;
  }
  else if (flagxdig == 3) {
    printf("X Ganhou!\n");
    return 1;
  }
  //Diagonal Secundaria
  flagodig = flagxdig = 0;
  if (a->info[0][2] == 'x' || a->info[0][2] == 'X')
    flagxdig++;
  if (a->info[1][1] == 'x' || a->info[1][1] == 'X')
    flagxdig++;
  if (a->info[2][0] == 'x' || a->info[2][0] == 'X')
    flagxdig++;
  if (a->info[0][2] == 'o' || a->info[0][2] == 'O')
    flagodig++;
  if (a->info[1][1] == 'o' || a->info[1][1] == 'O')
    flagodig++;
  if (a->info[2][0] == 'o' || a->info[2][0] == 'O')
    flagodig++;
  if (flagodig == 3) {
    printf("O Ganhou!\n");
    return 1;
  }
  else if (flagxdig == 3) {
    printf("X Ganhou!\n");
    return 1;
  }
  return 0;
}


int XY_to_pos(int x, int y) {
  if (x == 0 && y == 0) return 0;
  else if (x == 0 && y == 1) return 1;
  else if (x == 0 && y == 2) return 2;
  else if (x == 1 && y == 0) return 3;
  else if (x == 1 && y == 1) return 4;
  else if (x == 1 && y == 2) return 5;
  else if (x == 2 && y == 0) return 6;
  else if (x == 2 && y == 1) return 7;
  else if (x == 2 && y == 2) return 8;
}

void drawX(uint16_t x, uint16_t y) {
  tft.drawLine(x - TAM_X, y - TAM_X - 1, x + TAM_X, y + TAM_X - 1, X_COLOR);
  tft.drawLine(x - TAM_X, y - TAM_X    , x + TAM_X, y + TAM_X    , X_COLOR);
  tft.drawLine(x - TAM_X, y - TAM_X + 1, x + TAM_X, y + TAM_X + 1, X_COLOR);

  tft.drawLine(x - TAM_X, y + TAM_X - 1, x + TAM_X, y - TAM_X - 1, X_COLOR);
  tft.drawLine(x - TAM_X, y + TAM_X    , x + TAM_X, y - TAM_X    , X_COLOR);
  tft.drawLine(x - TAM_X, y + TAM_X + 1, x + TAM_X, y - TAM_X + 1, X_COLOR);
}

void drawO(uint16_t x, uint16_t y) {
  tft.fillCircle(x, y, 30, O_COLOR);
  tft.fillCircle(x, y, 27, BACKGROUND_COLOR);
}

void drawSelected(uint8_t pos, uint8_t isBola) {
  uint16_t x, y;
  switch (pos) {
    case 0:
      x = 200;
      y = 80;
      break;
    case 1:
      x = 200;
      y = 160;
      break;
    case 2:
      x = 200;
      y = 240;
      break;
    case 3:
      x = 120;
      y = 80;
      break;
    case 4:
      x = 120;
      y = 160;
      break;
    case 5:
      x = 120;
      y = 240;
      break;
    case 6:
      x = 40;
      y = 80;
      break;
    case 7:
      x = 40;
      y = 160;
      break;
    case 8:
      x = 40;
      y = 240;
      break;
    default:
      return;
  }
  if (isBola)
    drawO(x, y);
  else
    drawX(x, y);
}
void drawBoard() {

  // Desenhando as colunas
  tft.drawLine(20, 119, 220, 119, BOARD_COLOR);
  tft.drawLine(20, 120, 220, 120, BOARD_COLOR);
  tft.drawLine(20, 121, 220, 121, BOARD_COLOR);

  tft.drawLine(20, 199, 220, 199, BOARD_COLOR);
  tft.drawLine(20, 200, 220, 200, BOARD_COLOR);
  tft.drawLine(20, 201, 220, 201, BOARD_COLOR);

  // Desenhando
  tft.drawLine(159, 40, 159, 280, BOARD_COLOR);
  tft.drawLine(160, 40, 160, 280, BOARD_COLOR);
  tft.drawLine(161, 40, 161, 280, BOARD_COLOR);

  tft.drawLine(79, 40, 79, 280, BOARD_COLOR);
  tft.drawLine(80, 40, 80, 280, BOARD_COLOR);
  tft.drawLine(81, 40, 81, 280, BOARD_COLOR);
}

uint8_t getJogadaTouch() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    /*
      Serial.print("X = "); Serial.print(p.x);
      Serial.print("\tY = "); Serial.print(p.y);
      Serial.print("\tPressure = "); Serial.println(p.z);
    */
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);

    return XYToPos(p.x, p.y);
  }
  return SEM_CLICK;
}

uint8_t XYToPos(uint16_t x, uint16_t y) {
  // Quadrante da direita
  if (y > 220) {
    if (x > 160) { // Direita em cima
      return 2;
    } else {
      if (x < 80) { // Direita baixo
        return 8;
      } else { //Direita no meio
        return 5;
      }
    }
    // -----------------------------
    // Quadrante da esquerda
  } else {
    if (y < 120) {
      if (x > 160) { // Esquerda em cima
        return 0;
      } else {
        if (x < 80) { // Esquerda baixo
          return 6;
        } else { //Direita no meio
          return 3;
        }
      }
      // -----------------------------
      // Meio
    } else {
      if (x > 160) { // Meio em cima
        return 1;
      } else {
        if (x < 80) { // Meio baixo
          return 7;
        } else { //Meio no meio
          return 4;
        }
      }
    }
  }
}
