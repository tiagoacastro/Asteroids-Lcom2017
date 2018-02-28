#include <string.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdint.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include "graphics.h"
#include "time.h"
#include "vbe.h"
#include "lmlib.h"
#include "timer.h"
#include "i8042.h"
#include "i8254.h"
#include "keyboard.h"
#include "lmlib.h"
#include "video_gr.h"
#include <machine/int86.h>
#include "Bitmap.h"
#include "mouse.h"
#include <unistd.h>
#include "EndGameState.h"
#include "HighscoreState.h"
#include "NewHighscoreState.h"

extern NewHighscoreState* newHighscoreS;
extern int irq_set_mouse,irq_set_keyboard;
extern HighestScore* highestscore;
extern HighestScore* highestscore1;
extern HighestScore* highestscore2;
extern Bitmap* numbers[10];
Bitmap* alphabet[26];
static int cap = 0;
static char name[11] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '\0'};
extern int newHighscoreFlag;

NewHighscoreState* newNewHighscoreState() {
    NewHighscoreState* state;
    state = (NewHighscoreState*) malloc(sizeof(NewHighscoreState));

    state->background = loadBitmap("/home/lcom/Asteroids/res/NewHighScore.bmp");

    loadNumbers();

    loadAlphabet();

    return state;
}

void deleteNewHighscoreState() {
    deleteBitmap(newHighscoreS->background);

    deleteNumbers();

    deleteAlphabet();

    free(newHighscoreS);
}

void drawNewHighscoreStateBackground(){
    drawBitmap(newHighscoreS->background, 0, 0, ALIGN_LEFT);
}

void loadAlphabet()
{
	alphabet[0] = loadBitmap("/home/lcom/Asteroids/res/A.bmp");
	alphabet[1] = loadBitmap("/home/lcom/Asteroids/res/B.bmp");
	alphabet[2] = loadBitmap("/home/lcom/Asteroids/res/C.bmp");
	alphabet[3] = loadBitmap("/home/lcom/Asteroids/res/D.bmp");
	alphabet[4] = loadBitmap("/home/lcom/Asteroids/res/E.bmp");
	alphabet[5] = loadBitmap("/home/lcom/Asteroids/res/F.bmp");
	alphabet[6] = loadBitmap("/home/lcom/Asteroids/res/G.bmp");
	alphabet[7] = loadBitmap("/home/lcom/Asteroids/res/H.bmp");
	alphabet[8] = loadBitmap("/home/lcom/Asteroids/res/I.bmp");
	alphabet[9] = loadBitmap("/home/lcom/Asteroids/res/J.bmp");
	alphabet[10] = loadBitmap("/home/lcom/Asteroids/res/K.bmp");
	alphabet[11] = loadBitmap("/home/lcom/Asteroids/res/L.bmp");
  alphabet[12] = loadBitmap("/home/lcom/Asteroids/res/M.bmp");
  alphabet[13] = loadBitmap("/home/lcom/Asteroids/res/N.bmp");
	alphabet[14] = loadBitmap("/home/lcom/Asteroids/res/O.bmp");
	alphabet[15] = loadBitmap("/home/lcom/Asteroids/res/P.bmp");
	alphabet[16] = loadBitmap("/home/lcom/Asteroids/res/Q.bmp");
	alphabet[17] = loadBitmap("/home/lcom/Asteroids/res/R.bmp");
	alphabet[18] = loadBitmap("/home/lcom/Asteroids/res/S.bmp");
	alphabet[19] = loadBitmap("/home/lcom/Asteroids/res/T.bmp");
	alphabet[20] = loadBitmap("/home/lcom/Asteroids/res/U.bmp");
	alphabet[21] = loadBitmap("/home/lcom/Asteroids/res/V.bmp");
	alphabet[22] = loadBitmap("/home/lcom/Asteroids/res/W.bmp");
	alphabet[23] = loadBitmap("/home/lcom/Asteroids/res/X.bmp");
	alphabet[24] = loadBitmap("/home/lcom/Asteroids/res/Y.bmp");
  alphabet[25] = loadBitmap("/home/lcom/Asteroids/res/Z.bmp");
}

void deleteAlphabet(){
	deleteBitmap(alphabet[0]);
	deleteBitmap(alphabet[1]);
	deleteBitmap(alphabet[2]);
	deleteBitmap(alphabet[3]);
	deleteBitmap(alphabet[4]);
	deleteBitmap(alphabet[5]);
	deleteBitmap(alphabet[6]);
	deleteBitmap(alphabet[7]);
	deleteBitmap(alphabet[8]);
	deleteBitmap(alphabet[9]);
	deleteBitmap(alphabet[10]);
	deleteBitmap(alphabet[11]);
  deleteBitmap(alphabet[12]);
  deleteBitmap(alphabet[13]);
	deleteBitmap(alphabet[14]);
	deleteBitmap(alphabet[15]);
	deleteBitmap(alphabet[16]);
	deleteBitmap(alphabet[17]);
	deleteBitmap(alphabet[18]);
	deleteBitmap(alphabet[19]);
	deleteBitmap(alphabet[20]);
	deleteBitmap(alphabet[21]);
	deleteBitmap(alphabet[22]);
	deleteBitmap(alphabet[23]);
	deleteBitmap(alphabet[24]);
  deleteBitmap(alphabet[25]);
}

int keyHandler(){
  char *string1;
  string1 = malloc(11);
  strcpy(string1, name);
  if(cap <= 9)
  switch(key){
    case Akey:
      string1[cap] = 'A';
      cap++;
      strcpy(name, string1);
      free(string1);
      return 0;
      case Bkey:
        string1[cap] = 'B';
        cap++;
        strcpy(name, string1);
        free(string1);
        return 1;
        case Ckey:
          string1[cap] = 'C';
          cap++;
          strcpy(name, string1);
          free(string1);
          return 2;
          case Dkey:
            string1[cap] = 'D';
            cap++;
            strcpy(name, string1);
            free(string1);
            return 3;
            case Ekey:
              string1[cap] = 'E';
              cap++;
              strcpy(name, string1);
              free(string1);
              return 4;
              case Fkey:
                string1[cap] = 'F';
                cap++;
                strcpy(name, string1);
                free(string1);
                return 5;
                case Gkey:
                  string1[cap] = 'G';
                  cap++;
                  strcpy(name, string1);
                  free(string1);
                  return 6;
                  case Hkey:
                    string1[cap] = 'H';
                    cap++;
                    strcpy(name, string1);
                    free(string1);
                    return 7;
                    case Ikey:
                      string1[cap] = 'I';
                      cap++;
                      strcpy(name, string1);
                      free(string1);
                      return 8;
                      case Jkey:
                        string1[cap] = 'J';
                        cap++;
                        strcpy(name, string1);
                        free(string1);
                        return 9;
                        case Kkey:
                          string1[cap] = 'K';
                          cap++;
                          strcpy(name, string1);
                          free(string1);
                          return 10;
                          case Lkey:
                            string1[cap] = 'L';
                            cap++;
                            strcpy(name, string1);
                            free(string1);
                            return 11;
                            case Mkey:
                              string1[cap] = 'M';
                              cap++;
                              strcpy(name, string1);
                              free(string1);
                              return 12;
                              case Nkey:
                                string1[cap] = 'N';
                                cap++;
                                strcpy(name, string1);
                                free(string1);
                                return 13;
                                case Okey:
                                  string1[cap] = 'O';
                                  cap++;
                                  strcpy(name, string1);
                                  free(string1);
                                  return 14;
                                  case Pkey:
                                    string1[cap] = 'P';
                                    cap++;
                                    strcpy(name, string1);
                                    free(string1);
                                    return 15;
                                    case Qkey:
                                      string1[cap] = 'Q';
                                      cap++;
                                      strcpy(name, string1);
                                      free(string1);
                                      return 16;
                                      case Rkey:
                                        string1[cap] = 'R';
                                        cap++;
                                        strcpy(name, string1);
                                        free(string1);
                                        return 17;
                                        case Skey:
                                          string1[cap] = 'S';
                                          cap++;
                                          strcpy(name, string1);
                                          free(string1);
                                          return 18;
                                          case Tkey:
                                            string1[cap] = 'T';
                                            cap++;
                                            strcpy(name, string1);
                                            free(string1);
                                            return 19;
                                            case Ukey:
                                              string1[cap] = 'U';
                                              cap++;
                                              strcpy(name, string1);
                                              free(string1);
                                              return 20;
                                              case Vkey:
                                                string1[cap] = 'V';
                                                cap++;
                                                strcpy(name, string1);
                                                free(string1);
                                                return 21;
                                                case Wkey:
                                                  string1[cap] = 'W';
                                                  cap++;
                                                  strcpy(name, string1);
                                                  free(string1);
                                                  return 22;
                                                  case Xkey:
                                                    string1[cap] = 'X';
                                                    cap++;
                                                    strcpy(name, string1);
                                                    free(string1);
                                                    return 23;
                                                    case Ykey:
                                                      string1[cap] = 'Y';
                                                      cap++;
                                                      strcpy(name, string1);
                                                      free(string1);
                                                      return 24;
                                                      case Zkey:
                                                        string1[cap] = 'Z';
                                                        cap++;
                                                        strcpy(name, string1);
                                                        free(string1);
                                                        return 25;
                                                        case BACK_SPACE:
                                                          if(cap != 0){
                                                            cap--;
                                                            string1[cap] = '0';
                                                            strcpy(name, string1);
                                                          }
                                                          free(string1);
                                                          return 99;
                                                          default:
                                                            free(string1);
                                                            return -1;
  }
  else
    if(key == BACK_SPACE){
      cap--;
      string1[cap] = '0';
      strcpy(name, string1);
      free(string1);
      return 99;
    }
  free(string1);
  return 100;
}

void printName(int x, int y){
  int c = 0;
  while (name[c] != '0')
  {
    c++;
    if(c == 10)
      break;
  }
  x += 9 * c;
  c--;
  while (c >= 0)
  {
    x -= 18;
    switch(name[c])
    {
    case 'A':
      drawBitmap(alphabet[0], x, y, ALIGN_LEFT);
      break;
    case 'B':
      drawBitmap(alphabet[1], x, y, ALIGN_LEFT);
      break;
    case 'C':
      drawBitmap(alphabet[2], x, y, ALIGN_LEFT);
      break;
    case 'D':
      drawBitmap(alphabet[3], x, y, ALIGN_LEFT);
      break;
    case 'E':
      drawBitmap(alphabet[4], x, y, ALIGN_LEFT);
      break;
    case 'F':
      drawBitmap(alphabet[5], x, y, ALIGN_LEFT);
      break;
    case 'G':
      drawBitmap(alphabet[6], x, y, ALIGN_LEFT);
      break;
    case 'H':
      drawBitmap(alphabet[7], x, y, ALIGN_LEFT);
      break;
    case 'I':
      drawBitmap(alphabet[8], x, y, ALIGN_LEFT);
      break;
    case 'J':
      drawBitmap(alphabet[9], x, y, ALIGN_LEFT);
      break;
    case 'K':
      drawBitmap(alphabet[10], x, y, ALIGN_LEFT);
      break;
    case 'L':
      drawBitmap(alphabet[11], x, y, ALIGN_LEFT);
      break;
    case 'M':
      drawBitmap(alphabet[12], x, y, ALIGN_LEFT);
      break;
    case 'N':
      drawBitmap(alphabet[13], x, y, ALIGN_LEFT);
      break;
    case 'O':
      drawBitmap(alphabet[14], x, y, ALIGN_LEFT);
      break;
    case 'P':
      drawBitmap(alphabet[15], x, y, ALIGN_LEFT);
      break;
    case 'Q':
      drawBitmap(alphabet[16], x, y, ALIGN_LEFT);
      break;
    case 'R':
      drawBitmap(alphabet[17], x, y, ALIGN_LEFT);
      break;
    case 'S':
      drawBitmap(alphabet[18], x, y, ALIGN_LEFT);
      break;
    case 'T':
      drawBitmap(alphabet[19], x, y, ALIGN_LEFT);
      break;
    case 'U':
      drawBitmap(alphabet[20], x, y, ALIGN_LEFT);
      break;
    case 'V':
      drawBitmap(alphabet[21], x, y, ALIGN_LEFT);
      break;
    case 'W':
      drawBitmap(alphabet[22], x, y, ALIGN_LEFT);
      break;
    case 'X':
      drawBitmap(alphabet[23], x, y, ALIGN_LEFT);
      break;
    case 'Y':
      drawBitmap(alphabet[24], x, y, ALIGN_LEFT);
      break;
    case 'Z':
      drawBitmap(alphabet[25], x, y, ALIGN_LEFT);
      break;
    }
    c--;
  }
}

int startNewHighscoreState(){
  srand(time(NULL));
  key = 0x00;
  char namae[11] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '\0'};
  strcpy(name, namae);
	int freq = 2;
	int counter = 0;
  cap = 0;
		int ipc_status, r;
		message msg;

		int irq_set_timer = timer_subscribe_int();

		if (irq_set_timer == ERROR) {
			printf("Error subscribing to timer \n");
			return ERROR;
		}



			while (key != ENTER) {
				if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
					printf("driver_receive failed with: %d", r);
					continue;
				}

				if (is_ipc_notify(ipc_status)) {
					switch (_ENDPOINT_P(msg.m_source)) {
					case HARDWARE:
            if (msg.NOTIFY_ARG & irq_set_keyboard) {
              kbd_read_buffer_asm();
              keyHandler();
            }
						if (msg.NOTIFY_ARG & irq_set_mouse) {
		          updateMouse();
		        }
						if (msg.NOTIFY_ARG & irq_set_timer)
						{

							if(counter % freq ==0)
							{
                drawNewHighscoreStateBackground();
                drawMouseMenu();
                switch(newHighscoreFlag){
                  case 1:
                    drawScore(highestscore->score, 500, 340);
                    break;
                  case 2:
                    drawScore(highestscore1->score, 500, 340);
                    break;
                  case 3:
                    drawScore(highestscore2->score, 500, 340);
                    break;
                }
                printName(500, 640);
                swapBuffer();
							}
							counter++;
						}
					default:
						break;
					}

				}
			}


		if (timer_unsubscribe_int() == ERROR) {
			printf("Error subscribing to timer \n");
			return ERROR;
		}

    switch(newHighscoreFlag){
      case 1:
          strcpy(highestscore->name, name);
        break;
      case 2:
          strcpy(highestscore1->name, name);
        break;
      case 3:
          strcpy(highestscore2->name, name);
        break;
    }

    newHighscoreFlag = 0;



		return 0;
}
