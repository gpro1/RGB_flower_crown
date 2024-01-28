/* RGB LED Flower Crown
 * Attiny85 (Digispark)
 * 
 */

const int LED_RED = 0;
const int LED_GREEN = 1;
const int LED_BLUE = 4;
const int BUTTON = 3;

const unsigned long BUTTON_PERIOD_MS = 100;

#define PINK_BLUE_PULSE         0
#define YELLOW                  1
#define PINK                    2
#define BLUE                    3
#define GREEN                   4
#define PINK_PULSE              5
#define BLUE_PULSE              6

unsigned long periods[7] = {4, 0, 0, 0, 0, 20, 30};
const int LAST_STATE = BLUE_PULSE;
int mode;

//Direction variable for some modes
int dir;

//RGB pwm values for the LEDs
int pwm_value_r;
int pwm_value_g;
int pwm_value_b;

unsigned long time_ms; //Current time
unsigned long mode_time; //Previous time the animation FSM was run
unsigned long button_time; //Previous time the button was checked

int button_state; //Current button state
int previous_state;  //Previous button state

int mode_init;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN,  OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUTTON, INPUT);

  mode = PINK_BLUE_PULSE;
  mode_init = 1;
  mode_time = millis();
  button_time = millis();
}

void loop() {

  time_ms = millis();

  //Run animation if period is up
  if(time_ms - mode_time >= periods[mode])
  {
    mode_time = time_ms;
    
    //Animation
    switch(mode)
    {
      case PINK_BLUE_PULSE:
        if(mode_init == 1) //Initialize values
        {
          pwm_value_g = 100;
          pwm_value_b = 180;
          pwm_value_r = 0;
          dir = 1;
          mode_init = 0;
        }
        else //run animation (4ms period)
        {
          if(dir == 1)
          {
            pwm_value_r = pwm_value_r + 1;
            if(pwm_value_r >= 255)
            {
              dir = 0;
            }
          }
          else
          {
            pwm_value_r = pwm_value_r - 1;
            if(pwm_value_r <= 0)
            {
              dir = 1;
            }
          }      
        }
      break;
    
      case YELLOW:
        pwm_value_g = 165;
        pwm_value_b = 30;
        pwm_value_r = 250;
        mode_init = 0;
      break;
    
      case PINK:
        pwm_value_g = 0;
        pwm_value_b = 30;
        pwm_value_r = 250;
        mode_init = 0;
      break;
    
      case BLUE:
        pwm_value_g = 90;
        pwm_value_b = 250;
        pwm_value_r = 0;
        mode_init = 0;
      break;
    
      case GREEN:
        pwm_value_g = 250;
        pwm_value_b = 30;
        pwm_value_r = 0;
        mode_init = 0;
      break;
    
      case PINK_PULSE:
        if(mode_init == 1) //Initialize values
        {
          pwm_value_g = 90;
          pwm_value_b = 30;
          pwm_value_r = 250;
          dir = 0;
          mode_init = 0;
        }
        else
        {
          if(dir == 1)
          {
            pwm_value_r = pwm_value_r + 7;
            pwm_value_g = pwm_value_g + 3;
            pwm_value_b = pwm_value_b + 1;
            if(pwm_value_b >= 30)
            {
              dir = 0;
            }
            if(pwm_value_r >= 255) pwm_value_r = 255;
            if(pwm_value_g >= 255) pwm_value_g = 255;
          }
          else
          {
            pwm_value_r = pwm_value_r - 7;
            pwm_value_g = pwm_value_g - 3;
            pwm_value_b = pwm_value_b - 1;
            if(pwm_value_b <= 0)
            {
              dir = 1;
            }
            if(pwm_value_r <= 0) pwm_value_r = 0;
            if(pwm_value_g <= 0) pwm_value_g = 0;
          }
        }
      break;
    
      case BLUE_PULSE:
        if(mode_init == 1) //Initialize values
        {
          pwm_value_g = 90;
          pwm_value_b = 250;
          pwm_value_r = 0;
          dir = 0;
          mode_init = 0;
        }
        else
        {
          if(dir == 1)
          {
            pwm_value_g = pwm_value_g + 3;
            pwm_value_b = pwm_value_b + 7;
            if(pwm_value_g >= 90)
            {
              dir = 0;
            }
            if(pwm_value_b >= 255) pwm_value_b = 255;
            if(pwm_value_g >= 255) pwm_value_g = 255;
          }
          else
          {
            pwm_value_g = pwm_value_g - 3;
            pwm_value_b = pwm_value_b - 7;
            if(pwm_value_g <= 0)
            {
              dir = 1;
            }
            if(pwm_value_b <= 0) pwm_value_b = 0;
            if(pwm_value_g <= 0) pwm_value_g = 0;
          }
        }
      break;
    
      default:
      break;
    }
    
    analogWrite(LED_RED, pwm_value_r);
    analogWrite(LED_GREEN, pwm_value_g);
    analogWrite(LED_BLUE, pwm_value_b);
  }
 

  //Check for user input if time is up
  if(time_ms - button_time >= BUTTON_PERIOD_MS)
  {
    button_time = time_ms;
    previous_state = button_state;
    button_state = digitalRead(BUTTON);
    
    if(previous_state == 1 and button_state == 0)
    {
      mode++;
      mode_init = 1;
      if(mode > LAST_STATE)
      {
        mode = PINK_BLUE_PULSE;
      }
    }
  }
}
