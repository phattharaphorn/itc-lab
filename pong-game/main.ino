#include <LedControl.h>
const int PIN_CS  = 10;
const int PIN_CLK = 13;
const int PIN_DIN = 11;
const int MODULES =  4;

const int FristPlayer_BUTTON1 = 5;
const int FristPlayer_BUTTON2 = 4;
const int SecondPlayer_BUTTON1 = 3;
const int SecondPlayer_BUTTON2 = 2;

const int BUZZER = 12;

const int Screen_WIDTH = 8;// ขนาดจอ
const int Screen_HEIGHT = 32;

LedControl led_control = LedControl(PIN_DIN, PIN_CLK, PIN_CS, MODULES);

void Sound(int frequency, int delay_amount) { // set ลำโพง
  tone(BUZZER, frequency);
  delay(delay_amount);
  noTone(BUZZER);
}

bool isButtonPressed(int pin) { 
  delay(10);
  if (digitalRead(pin) == LOW) return true;
  return false;
}

void reDisplay() {
  for (int n = 0; n < 4; n++) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        led_control.setLed(n, i, j, false);
      }
    }
  }
}

void drawP(int x, int y) {
  for (int n = 0; n < 4; n++) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (i == x && j + (8 * n) == y) led_control.setLed((3 - n), 7 - i, j, true);
      }
    }
  }
}

void deleteP(int x, int y) {
  for (int n = 0; n < 4; n++) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (i == x && j + (8 * n) == y) led_control.setLed((3 - n), 7 - i, j, false);
      }
    }
  }
}

int NUMBER_STRUCT[16] = {1, 2, 8, 11, 16, 19, 24, 25, 26, 27, 32, 35, 40, 43, 49, 50};
int NUMBER_PATTERN[10][16] = {
  {1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0},
  {1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1},
  {1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1},
  {0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0},
  {1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1},
  {1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1},
};

void clear_Numb(int x, int y) {
  for (int i = 0; i < 16; i++) {
    int offset_y = NUMBER_STRUCT[i] / 8;
    int offset_x = NUMBER_STRUCT[i] - (offset_y * 8);
    deleteP(x + offset_x, y + offset_y);
  }
}

void show_Numb(int x, int y, int number) {
  for (int i = 0; i < 16; i++) {
    int offset_y = NUMBER_STRUCT[i] / 8;
    int offset_x = NUMBER_STRUCT[i] - (offset_y * 8);
    if (NUMBER_PATTERN[number][i]) {
      drawP(x + offset_x, y + offset_y);
    } else deleteP(x + offset_x, y + offset_y);
  }
}

void blinkScore(int score_1, int score_2, int winner) {
  int player_1_x = 2, player_1_y = 7;
  show_Numb(player_1_x, player_1_y, score_1);

  int player_2_x = 2, player_2_y = 18;
  show_Numb(player_2_x, player_2_y, score_2);
  delay(500);

  if (winner == 1) {
    score_1++;
    clear_Numb(player_1_x, player_1_y);
    delay(400);
    show_Numb(player_1_x, player_1_y, score_1);
    Sound(262, 100);
    Sound(349, 100);
    Sound(392, 100);
    Sound(330, 100);
    clear_Numb(player_1_x, player_1_y);
    Sound(880, 100);
    Sound(1047, 100);
    delay(200);
    show_Numb(player_1_x, player_1_y, score_1);
    delay(500);
  }
  else if (winner == 2) {
    score_2++;
    clear_Numb(player_2_x, player_2_y);
    delay(400);
    show_Numb(player_2_x, player_2_y, score_2);
    Sound(262, 100);
    Sound(349, 100);
    Sound(392, 100);
    Sound(330, 100);
    clear_Numb(player_2_x, player_2_y);
    Sound(880, 100);
    Sound(1047, 100);
    delay(200);
    show_Numb(player_2_x, player_2_y, score_2);
    delay(800);
  }
  clear_Numb(player_1_x, player_1_y);
  clear_Numb(player_2_x, player_2_y);
  delay(600);
}

class Paddle {
  public:
    int x;
    int y;
    int direction;
    int size;

    Paddle(int x, int y) {
      this->x = x;
      this->y = y;
      this->direction = 0;
      this->size = 4;
    }

    void setDirection(int direction) {
      this->direction = direction;
    }

    void draw() {
      if (this->direction != 0) {
        for (int i = 0; i < this->size; i++) {
          deleteP(this->x + i, this->y);
        }
        this->x += this->direction;
        if (this->x > 8 - this->size) this->x = 8 - this->size;
        if (this->x < 0) this->x = 0;
      }

      for (int i = 0; i < this->size; i++) {
        drawP(this->x + i, this->y);
      }
    }
};

class Puck {
  public:
    float x;
    float y;
    float x_previous;
    float y_previous;
    float x_speed;
    float y_speed;

    Puck(float x, float y) {
      this->x = x;
      this->y = y;
      this->x_previous = x;
      this->y_previous = y;
      this->x_speed = 1;
      this->y_speed = -1;
    }
    void update() {
      if (this->x + this->x_speed >= Screen_WIDTH || this->x + this->x_speed < 0) {
        this->x_speed *= -1;
        Sound(131, 15);
        Sound(33, 15);
      }
      this->x += this->x_speed;
      this->y += this->y_speed;
    }

    int getWinner() {
      if (this->y + this->y_speed >= Screen_HEIGHT) return 1;
      if (this->y + this->y_speed < 0) return 2;
      return 0;
    }
    bool collisionCheck(Paddle paddle, int direction) {
      if (direction < 0) {
        if (this->y < paddle.y + 2 && this->y > paddle.y && this->x >= paddle.x - 1 && this->x < paddle.x + paddle.size + 0.5) {
          this->y_speed *= -1;
          return true;
        }
      }
      else if (direction > 0) {
        if (this->y > paddle.y - 2 && this->y < paddle.y && this->x >= paddle.x - 1 && this->x < paddle.x + paddle.size + 0.5) {
          this->y_speed *= -1;
          return true;
        }
      }
      return false;
    }

    void blink() {
      drawP(this->x, this->y);
      Sound(208, 50);
      Sound(156, 50);
      deleteP(this->x, this->y);
      Sound(104, 50);
      delay(100);
      drawP(this->x, this->y);
      delay(200);
      deleteP(this->x, this->y);
      delay(200);
      drawP(this->x, this->y);
      delay(200);
      deleteP(this->x, this->y);
      delay(800);
    }

    void reset() {
      this->x = Screen_WIDTH / 2;
      this->y = Screen_HEIGHT / 2;
      draw();
      Sound(277, 100);
      Sound(164, 100);

      deleteP(this->x, this->y);
      delay(200);
      drawP(this->x, this->y);
      delay(200);
      deleteP(this->x, this->y);
      delay(200);
      drawP(this->x, this->y);
      delay(400);
    }

    void draw() {
      if (round(this->x) != round(this->x_previous)
          || round(this->y) != round(this->y_previous)) {
        deleteP(this->x_previous, this->y_previous);
        this->x_previous = this->x;
        this->y_previous = this->y;
        drawP(this->x, this->y);
      }
    }
};

enum GameState {
  WAITING,
  PLAYGAME,
  ENDGAME,
};
int game_state = PLAYGAME;

Paddle player_1(2, 2);
Paddle player_2(2, 29);
int player_1_score = 0;
int player_2_score = 0;

Puck puck(Screen_WIDTH / 2, Screen_HEIGHT / 2);

void Waiting() {
  if (game_state != WAITING) return;
}

void PlayGame() {
  if (game_state != PLAYGAME) return;

  // Winner
  if (puck.getWinner() == 1) { // Player 1 Wins
    puck.blink();
    blinkScore(player_1_score, player_2_score, 1);
    player_1_score++;
    puck.reset();
  }
  else if (puck.getWinner() == 2) { // Player 2 Wins
    puck.blink();
    blinkScore(player_1_score, player_2_score, 2);
    player_2_score++;
    puck.reset();
  }

  // Player 1 Paddle Move
  player_1.setDirection(0);
  if (isButtonPressed(FristPlayer_BUTTON1)) {
    player_1.setDirection(-1);
  }
  if (isButtonPressed(FristPlayer_BUTTON2)) {
    player_1.setDirection(1);
  }

  // Player 2 Paddle Move
  player_2.setDirection(0);
  if (isButtonPressed(SecondPlayer_BUTTON1)) {
    player_2.setDirection(-1);
  }
  if (isButtonPressed(SecondPlayer_BUTTON2)) {
    player_2.setDirection(1);
  }

  // Puck Update
  if (puck.collisionCheck(player_1, -1)
      || puck.collisionCheck(player_2, 1)) {
    Sound(1047, 20);
    Sound(349, 20);
  }
  puck.update();

  // Draw Player's Paddles
  player_1.draw();
  player_2.draw();

  // Draw Puck
  puck.draw();
}

void EndGame() {
  if (game_state != ENDGAME) return;
}

void setup() {
  //LedMatrix Setup
  int device_count = led_control.getDeviceCount();
  for (int address = 0; address < device_count; address++) {
    led_control.shutdown(address, false);
    led_control.setIntensity(address, 2);
    led_control.clearDisplay(address);
  }

  //Buzzer Setup
  pinMode(BUZZER, OUTPUT);

  //Button Setup
  bool is_button_pressed[4] = {false, false, false, false};
  pinMode(FristPlayer_BUTTON1, INPUT_PULLUP);
  pinMode(FristPlayer_BUTTON2, INPUT_PULLUP);
  pinMode(SecondPlayer_BUTTON1, INPUT_PULLUP);
  pinMode(SecondPlayer_BUTTON1, INPUT_PULLUP);

}
void loop() {
  Waiting();
  PlayGame();
  EndGame();
}
