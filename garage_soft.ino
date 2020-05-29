#define d1 3
#define d1_speed 1000
#define s1 4
#define s2 5
#define k1_i A0
#define k1_o 2
#define k2_i A1
#define k2_o 6
#define con 7
#define eng 8
#define p1 9
#define p2 10
#define p3 11
#define c_speed 500
#define opening_time 180000

int closing  = 0;
int opening  = 0;
long d_time = 0;
long c_time = 0;
bool first = true;
bool revert_o = false;
bool revert_c = false;
long open_time = 0;

void setup() {
  Serial.begin(9600);
  pinMode(d1, OUTPUT);
  pinMode(s1, INPUT_PULLUP);
  pinMode(s2, INPUT_PULLUP);
  pinMode(k1_i, INPUT);
  pinMode(k1_o, OUTPUT);
//  pinMode(con, OUTPUT);
  pinMode(p1, OUTPUT);
  pinMode(p2, OUTPUT);
  pinMode(eng, OUTPUT);
  pinMode(p3, OUTPUT);
}

void loop() {
 
    if (digitalRead(s1) == 0){

        if(closing == 0 && opening == 0){
            Serial.println("Zamykanie");
            closing = 1;
        } else if(opening == 1){
            Serial.println("Przerwanie otwierania"); 
            Serial.println("Zamykanie");
            opening = 0;
            revert_o = true;
        }
        while (digitalRead(s1) == 0);
    }
  
    if (digitalRead(s2) == 0){

        if(opening == 0 && closing == 0){
            Serial.println("Otwieranie");
            opening = 1;
        } else if(closing == 1){
            Serial.println("Przerwanie zamykania"); 
            Serial.println("Otwieranie");
            revert_c = true;
            closing = 0;
        }
        while (digitalRead(s2) == 0);
    }
  
    if(closing == 1 || opening == 1){
        if(first){
            digitalWrite(k1_o, HIGH);
            digitalWrite(k2_o, HIGH);
            digitalWrite(eng, HIGH);
            first = false;
        }
        
        if(d_time == 0){
            d_time = millis();
        } else {
            if(millis() - d_time >= d1_speed){
                digitalWrite(d1, HIGH);
            }
            if(millis() - d_time >= d1_speed + 500){
                digitalWrite(d1, LOW);
                d_time = 0;
            }
        }
        if(c_time == 0){
            c_time = millis();
        } else {
            if(millis() - c_time >= c_speed){
                digitalWrite(p3, LOW);
                c_time = 0;
            }
        }
    } else {
        d_time = 0;
        c_time = 0;
        digitalWrite(p3, HIGH);
        digitalWrite(d1, LOW);
        digitalWrite(k1_o, LOW);
        digitalWrite(k2_o, LOW);
        digitalWrite(eng, LOW);
        first = true;
    }

    if(revert_o){
        opening = 0;
        closing = 1;
        revert_o = false;
    }
    if(revert_c){
        opening = 1;
        closing = 0;
        revert_c = false;
    }
        

    if(opening == 1){
        digitalWrite(p1, HIGH);
        digitalWrite(p2, HIGH);
        double anl = analogRead(k1_i);
        if(anl > 1000){
            Serial.println("Otworzono");
            Serial.println("Brama stop");
            opening = 0;
        }
      Serial.println(millis());
      if(open_time == 0){
            open_time = millis();
            Serial.println(open_time);
        } else {
            if(millis() - open_time >= opening_time){
                opening = 0;
                open_time = 0;
            }
        }
    }

    if(closing == 1){
        digitalWrite(p1, LOW);
        digitalWrite(p2, LOW);
        double anl = analogRead(k2_i);
        if(anl > 1000){
            Serial.println("Zamknieto");
            Serial.println("Brama stop");
            closing = 0;
        } 
    }

    delay(50);
}
