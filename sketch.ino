#include <MD_MAX72xx.h>
#include <ezButton.h>

// matrix controls
const byte data_pin = 11;
const byte chip_select_pin = 10;
const byte clock_pin = 13;
const byte max_devices = 1;

int y = 7;
int x = 0;
int paddle_length = 3, direction = "";

int max_x = 7;

ezButton button(8);

long int current_time, prev_time_random_object = 0, prev_time_player_movement = 0;
int threshold = 400;

int random_object1_position_x = 3;
int random_object_position_y = 8;
int random_object2_position_x = 3;
int random_object3_position_x = 3;


bool gameState = true;
// creating matrix object
MD_MAX72XX matrix = MD_MAX72XX(MD_MAX72XX::PAROLA_HW, chip_select_pin, max_devices);

// define the joystick controls
const byte hpin = A0;
const byte vpin = A1;

void setup() {
  Serial.begin(9600);

  matrix.begin();
  matrix.clear();

  
  button.setDebounceTime(25);
  show_message("WELCOME");
}

void loop() {

  // put your main code here, to run repeatedly:
  if (gameState == true) {
    button.loop();
    current_time = millis();
    check_direction();

    //control the movement the player sprite and draws all the objects
    if (current_time - prev_time_player_movement >= 100) {
      prev_time_player_movement = current_time;

      matrix.clear();

      move_sprite();

      matrix.setPoint(y, x, true);
      matrix.setPoint(random_object_position_y, random_object1_position_x, true);
      matrix.setPoint(random_object_position_y, random_object2_position_x, true);
      matrix.setPoint(random_object_position_y, random_object3_position_x, true);
    }

    //controls the movement of the random_object
    if (current_time - prev_time_random_object >= threshold) {
      prev_time_random_object = current_time;
      random_object_position_y++;
      if (random_object_position_y > 7) {
        generate_random_objects();
        threshold = threshold - 10;
        if (threshold < 150) {
          threshold = 150;
        }
      }
    }

    //game over condition
    if (random_object_position_y == 7) {
      if (random_object1_position_x == x || random_object2_position_x == x || random_object3_position_x == x) {
        gameState = false;

      }
    }
  }

  /* show the game over message and reset the game with
  push button when game is over*/
  if (gameState == false) {
    show_message("GAME OVER");
    while(true){
      button.loop();
      if(button.isPressed()){
        gameState=true;
        break;
      }
    }
  }

}



void check_direction() {
  if (analogRead(hpin) > 512)
    direction = "left";
  else if (analogRead(hpin) < 512)
    direction = "right";
  else
    direction = "";
}

void window_check() {
  if (x > 7 ) x = 7;
  else if (x < 0) x = 0;
}

void move_sprite() {
  if (direction == "left") x++;
  else if (direction == "right") x--;

  window_check();
}

void generate_random_objects() {

  random_object_position_y = 0;

  while (true) {

    random_object1_position_x = random(0, 8);
    random_object2_position_x = random(0, 8);
    random_object3_position_x = random(0, 8);

    if (random_object1_position_x != random_object2_position_x &&
        random_object2_position_x != random_object3_position_x &&
        random_object3_position_x != random_object1_position_x) {
      break;
    }
  }
}

void show_message(char messageToShow[]) {

  all_animate();
  int i = 0;

  while (messageToShow[i] != '\0') {
    matrix.setChar(6, messageToShow[i]);
    delay(500);
    matrix.clear();
    delay(500);
    i++;
  }
}

void all_animate() {
  int i = 0;
  while (i < 3) {
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 8; y++) {
        matrix.setPoint(y, x, true);
      }
    }
    delay(500);
    matrix.clear();
    delay(500);
    i++;
  }
}