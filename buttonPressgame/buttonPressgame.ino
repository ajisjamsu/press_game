#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // select the pins used on the LCD panel
// define some values used by the panel and buttons
int lcd_key = 0;
int adc_key_in = 0;
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

#define inputPin 0

static const char* names[] = {"AJIS", "EMMA", "KOKO", "KATE", "KATH", "MATT", "SAXE", "SIVI"};
char* hiUser;
int hiScore;
static const int GAME_TIME = 10;

unsigned long tempTime;

// ------
// USER-DEFINED FUNCTIONS
// ------

// read the buttons
int read_LCD_buttons(){ 
  adc_key_in = analogRead(0); // read the value from the sensor
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  // We make this the 1st option for speed reasons since it will be the most likely result
  if (adc_key_in > 1000) return btnNONE;
  // For V1.0 comment the other threshold and use the one below:
  if (adc_key_in < 50) return btnRIGHT;
  if (adc_key_in < 195) return btnUP;
  if (adc_key_in < 380) return btnDOWN;
  if (adc_key_in < 555) return btnLEFT;
  if (adc_key_in < 790) return btnSELECT;
  return btnNONE; // when all others fail, return this.
}

char* user_select() {
  lcd.clear();
  char* username = new char[4]; 
  username = "test";
  // Allow the user to cycle through users and choose one
  lcd.setCursor(0,0); // move to the begining of the first line
  lcd.print("SELECT NAME");

  int nameIndex = 0;
  while(1) { // read the buttons until a name is selected
    lcd_key = read_LCD_buttons(); 
    switch (lcd_key){
      case btnUP:{
        if (nameIndex == 0) {
          nameIndex = 7;
        } else {
          nameIndex--;
        }
        lcd.setCursor(0,1);
        lcd.print(names[nameIndex]);
      }
      case btnDOWN:{
        if (nameIndex == 7) {
          nameIndex = 0;
        } else {
          nameIndex++;
        }
        lcd.setCursor(0,1);
        lcd.print(names[nameIndex]);
      }
      case btnSELECT:{
        username = names[nameIndex];
        break;
      }
      case btnNONE:{
        lcd.setCursor(0,1);
        lcd.print(names[nameIndex]);
      }
    }
    //lcd.setCursor(5,1);
    //lcd.print(nameIndex);
    delay(200);
  }
  return username;
}

int play_game() {
  lcd.clear();
  int start_time = millis();
  int time_remaining;
  int numPresses = 0;
  bool buttonUp = true;
  /* Button press detection: 
  - buttonUp is true iutton is up
  - When button is depressed (READ = LO), buttonUp is set to False
    - numPresses is iterated here
  - When button is let up again (READ = HI), buttonUp is set to True
  - Now, it has another opportunity to be set False again the next
    time the button is detected to be read. 
  */
  while(millis() - start_time < GAME_TIME*1000) {
    lcd.setCursor(0,0); // move to the begining of the first line
    lcd.print("PUSH THE BUTTON!");

    lcd.setCursor(0,1); // move to the begining of the second line
    lcd.print(numPresses);

    if (buttonUp) { // look for when the button is depressed
      if (digitalRead(inputPin) == LOW) {
        buttonUp = false;
        numPresses++;
      }
    } else { // look for when the button is let up again
      if (digitalRead(inputPin) == HIGH) {
        buttonUp = true;
      }
    }
    
    time_remaining = (GAME_TIME - (millis() - start_time)/1000);
    if (time_remaining == GAME_TIME) {
      lcd.setCursor(9,1);
      lcd.print(time_remaining);
    } else {
      lcd.setCursor(9,1);
      lcd.print(" ");
      lcd.print(time_remaining);
    }
  }
  return numPresses;
}

void display_score(int score, char* hiUser, int hiScore) {
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("TIME!!");
  lcd.setCursor(1,1);
  lcd.print("You did great.");
  delay(1000);
  lcd.clear();
  while(digitalRead(inputPin) == HIGH) {
    lcd.setCursor(0,0); // put LCD cursor at the start
    lcd.print("YOUR SCORE: ");
    lcd.setCursor(12,0);
    lcd.print(score);

    lcd.setCursor(0,1);
    lcd.print("HISCORE ");
    //lcd.setCursor(8,1);
    //lcd.print(hiUser);
    lcd.setCursor(13,1);
    lcd.print(hiScore);
  } // leave when the button is pressed
  
}

// ------
// MAIN SETUP AND LOOP FUNCTIONS
// ------

void setup(){
  lcd.begin(16, 2); // start the library
  lcd.setCursor(0,0); // set the LCD cursor position
  lcd.print("Push the buttons"); // print a simple message on the LCD
  pinMode(inputPin, INPUT_PULLUP); // receive external pushbutton input on D0

  hiUser = new char[4];
  hiUser = "    ";
  hiScore = 0;
}

void loop(){
  char* user = new char[4];
  //user = user_select();
  int score = play_game();
  if (score > hiScore) {
    hiUser = user;
    hiScore = score;
  }
  display_score(score, hiUser, hiScore);
}

