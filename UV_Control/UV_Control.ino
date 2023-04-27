#define DEBOUNCE 500
#define FLASHMS 250
#define TRUE 1
#define FALSE 0
//#define NANO
#ifdef NANO
  const int btnled = 13; //0
#else
  const int btnled = 0; //0
#endif
uint32_t cycleMillis=0;
bool ProgramMode=0;
typedef struct btn_S{
  int pinNum;
  uint32_t dbTimer;
  bool triggered;
}btn_T;

uint32_t ledFlasherTimer=0;
int ledFlasherState=0;
int uvMinutes=0;
int uvHalfMin=0;

typedef struct uv_S{
  int pinNum;
  bool OnCmd;
  uint32_t On_Timer;
  uint32_t uvSecSetpoint;
}uv_T;

btn_T TopBtn;
btn_T SideBtn;
uv_T UVLight;

// the setup routine runs once when you press reset:
void setup() {
  //
  #ifdef NANO
    Serial.begin(115200);
  #endif
  UVLight.pinNum=4;//4;
  TopBtn.pinNum=1;//1;
  SideBtn.pinNum=2;//2;
  UVLight.OnCmd=FALSE;
  UVLight.uvSecSetpoint=420000;  //7 Min
  UVLight.On_Timer=0;
  TopBtn.dbTimer=0;
  TopBtn.triggered=0;
  SideBtn.dbTimer=0;
  SideBtn.triggered=0;
  // initialize the digital pin as an output.
  pinMode(btnled, OUTPUT);
  pinMode(UVLight.pinNum, OUTPUT);
  pinMode(SideBtn.pinNum, INPUT_PULLUP);
  pinMode(TopBtn.pinNum, INPUT_PULLUP);
  digitalWrite(UVLight.pinNum,  LOW); // turn the UV Off (HIGH is the voltage level)   
  digitalWrite(btnled,  HIGH); // turn the LED on (HIGH is the voltage level)  
  delay(1000);
}

void btnTriggered(btn_T *btn){
  bool currentRead=!digitalRead(btn->pinNum);  //Inverted because gnded is active
  if(cycleMillis>btn->dbTimer){
    if(currentRead){
      btn->dbTimer=cycleMillis+DEBOUNCE; //Lockout for debounce setpoint
      btn->triggered=TRUE;
      #ifdef NANO
      Serial.print("Triggered Until: ");
      Serial.print(btn->dbTimer);
      Serial.print("- ");
      Serial.println(currentRead);
      #endif
    }
  }
}

void statusFlashHandler(void){
    if(cycleMillis>=ledFlasherTimer){
      //keep led off before restarting sequence
      if((uvMinutes==(ledFlasherState/2)) && uvHalfMin){
        digitalWrite(btnled, LOW); // turn the LED on (HIGH is the voltage level)
        ledFlasherTimer=cycleMillis+FLASHMS/2;
        ledFlasherState+=2; 
      }else if(uvMinutes>=((ledFlasherState/2)+1)){
        if(ledFlasherState%2==0){
           digitalWrite(btnled, LOW); // turn the LED on (HIGH is the voltage level)
        }else{
           digitalWrite(btnled,  HIGH); // turn the LED on (HIGH is the voltage level)
        }
        ledFlasherTimer=cycleMillis+FLASHMS;
        ledFlasherState++;
      }else{
        digitalWrite(btnled,  HIGH); // turn the LED on (HIGH is the voltage level)
        if((ledFlasherState/2)>(uvMinutes+2)){
          ledFlasherState=0;
        }else{
          ledFlasherState++;
        }
        ledFlasherTimer=cycleMillis+FLASHMS;
      }
    }
}

// the loop routine runs over and over again forever
void loop() {
  static bool oldUVCmd=FALSE;
  uvMinutes=UVLight.uvSecSetpoint/60000;
  uvHalfMin=UVLight.uvSecSetpoint%60000;
  
  cycleMillis=millis();
  btnTriggered(&SideBtn);  //Check btn status
  btnTriggered(&TopBtn);   //Check btn status


  if(SideBtn.triggered){
    SideBtn.triggered=FALSE;
    ProgramMode=!ProgramMode;
    //Serial.print(ProgramMode);
    //Serial.println( " : ProgMode");
    digitalWrite(btnled,  HIGH); // start the mode change with the BTN LED off
    ledFlasherState=0;
  }

  //Programming mode
  if(ProgramMode){
    UVLight.OnCmd=FALSE;
    if(TopBtn.triggered){
      TopBtn.triggered=FALSE;
      UVLight.uvSecSetpoint+=30000;
      //Serial.print(UVLight.uvSecSetpoint);
      //Serial.println( " : Setpoint");
      uvMinutes=UVLight.uvSecSetpoint/60000;
      uvHalfMin=UVLight.uvSecSetpoint%60000;
      //Serial.print(uvMinutes);
      //Serial.print(" : ");
      //Serial.print(uvHalfMin);
      //Serial.println("");
      if(UVLight.uvSecSetpoint>600000){
        UVLight.uvSecSetpoint=30000;  //If greater than 10 min, start over at 1/2 minute
      }
      ledFlasherState=0;  //start indication sequence over again
    }
    statusFlashHandler();
  //Normal Run Mode
  }else{
      //UV Timer Timed out?
      if((UVLight.On_Timer<=cycleMillis) && UVLight.OnCmd){
        UVLight.OnCmd=FALSE;
      }
      //UV start/stop?
      if(TopBtn.triggered){
        TopBtn.triggered=FALSE;
        UVLight.OnCmd=!UVLight.OnCmd;
      }
      
      if(UVLight.OnCmd != oldUVCmd){
        if(UVLight.OnCmd){
          UVLight.On_Timer=cycleMillis + UVLight.uvSecSetpoint; //420000; 
          digitalWrite(UVLight.pinNum, HIGH); // turn the LED on (HIGH is the voltage level)
          digitalWrite(btnled, LOW); // turn the LED on (HIGH is the voltage level) 
        }else{
          UVLight.On_Timer=cycleMillis;
          digitalWrite(btnled,  HIGH); // turn the LED on (HIGH is the voltage level)
          digitalWrite(UVLight.pinNum,  LOW); // turn the LED on (HIGH is the voltage level)   
        }
        #ifdef NANO
          Serial.print(UVLight.OnCmd);
          Serial.print("<<--");
          Serial.print(oldUVCmd);
          Serial.println(""); 
        #endif 
      }
      oldUVCmd=UVLight.OnCmd;
  }  
}
