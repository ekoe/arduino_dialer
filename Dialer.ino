  //define outputs
int output_1 = 3;
int output_2 = 5;

//define inputs
int input_dialing  = 11;
int input_click  = 9;

//this should actually wake the arduino from sleep
//int input_receiver = 5;

//testing and blinking
int delay_time = 500;
int debounce_time = 8;

//control globals
unsigned long time = millis();
unsigned long last_state_time = time;

//from phone
int dialing = 0;
int count = 0;

//state stack
int last_state = 1;
int state = 1;
int true_state = 1;

void setup () {
   pinMode(output_1, OUTPUT); 
   pinMode(output_2, OUTPUT); 
   pinMode(input_dialing, INPUT);
   pinMode(input_click, INPUT);
   Serial.begin(9600);
   
   //turn light on
   Serial.write(17);
   
   //FF
   Serial.write(12); 
   delay(5); //rtfm
 }

void loop() {
  //check to see if we've started dialing
  //only start listening if user is actively dialing  
  dialing = digitalRead(input_dialing);

  if (dialing) { //check for dialer to start
     green();
     
     time = millis();
     state = digitalRead(input_click);
      
     if (last_state_time + debounce_time <= time) { //check on interval
         //two in a row is good enough
         if (last_state == state) {
            //compare to true state
            if (true_state != state) {  //it changed!
               true_state = state;
               if (!true_state) {  //check for value to go low for "click"
                 count++;
               }
            }
         }

        last_state = state;
        last_state_time = time;
     }
    }    
    else { //stopped dialing
      red();
      delay(delay_time);
      
      if (count) {       
        Serial.println(count+0);
        
        int sound = count+219;

       Serial.write(sound);
        }
        
        //reset count
        count = 0;
      }
}

void red() {
  clear_led();
  digitalWrite(output_1, HIGH);
}

void clear_led() {
   digitalWrite(output_1, LOW); 
   digitalWrite(output_2, LOW);
}

void green() {
  clear_led();
  digitalWrite(output_2, HIGH);
}

void amber() {
  clear_led();
  digitalWrite(output_1, HIGH); 
  digitalWrite(output_2, HIGH);
}

/*
void note(pitch, duration, scale) {
    
}
*/
