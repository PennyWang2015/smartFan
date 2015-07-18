/**
 * 人体红外感应 - 控制风扇
 *  v1.0.2 - 一个人体传感器,稳定版
 *    功能点:
 *        - 3秒内有2次以上有人体活动,即开启继电器(Pin7)
 *        - 连续10-20分钟感应不到人体活动, 关闭继电器(Pin7)
 *    接线说明:
 *        - 4号数字针脚, 人体红外
 *        - 6号数字针脚, LED指示灯
 *        - 7号数字针脚, 继电器(风扇)
 * for maiao company
 */
#define DEBUG 0
#define PIN_MOTION_01 4         //人体红外接到数字口
#define PIN_DELAY_FEN 7         //继电器 - 风扇
#define PIN_LED_01 6            //LED

int _haspeople = 0;              //是否有人
int _acts[10] = {0,0,0,0,0,0,0,0,0,0};   //检测有动作的次数, 每分钟1次, 只要数组中有1,就判定有人
int _i_act = 0; //数组计算下标
long _stay_time_tmp = 0;     //临时变量

void setup() {
  pinMode(PIN_MOTION_01, INPUT);
  pinMode(PIN_LED_01, OUTPUT);
  pinMode(PIN_DELAY_FEN, OUTPUT);
  if(DEBUG){
    Serial.begin(9600);
    Serial.println("v1.0.2 is ready.");
  }
}

void loop() {
  if(digitalRead(PIN_DELAY_FEN)==LOW){
      int acts = 0;      
      for(int i=1;i<=3;i++){
        if(digitalRead(PIN_MOTION_01)==HIGH)acts++;
        delay(1000);
      }
      if(DEBUG){Serial.print("delay is off...acts:");Serial.println(acts);}
      if(acts>=2){
        fenON();                                //3秒内有2次以上有人体活动
      }else{
        delay(1000);
      }
  }else{
    //继电器开启状态，检查是否需要关闭
    checkperson();
    if(DEBUG){
      Serial.print("...G");Serial.println(_haspeople);
    }
    if(_haspeople==0){
      fenOFF();
    }
    delay(1000);
  }
}

void fenON(){
  _haspeople = 1;
  if(digitalRead(PIN_LED_01)==LOW){digitalWrite(PIN_LED_01,HIGH);}          //turn on led
  if(digitalRead(PIN_DELAY_FEN)==LOW){digitalWrite(PIN_DELAY_FEN,HIGH);}    //turn on fen
  long keep_power_on = 60000;        //每次开启维持1min,防止频繁开关
  if(DEBUG){Serial.print("keep on...");Serial.println(keep_power_on);}
  delay(keep_power_on);
  if(DEBUG){Serial.print("keep on time is up.");Serial.println(keep_power_on);}
}
void fenOFF(){
  //turn off led
  if(digitalRead(PIN_LED_01)==HIGH){digitalWrite(PIN_LED_01,LOW);}
  //turn off fen
  if(digitalRead(PIN_DELAY_FEN)==HIGH){digitalWrite(PIN_DELAY_FEN,LOW);}
}

void checkperson(){
  if(digitalRead(PIN_MOTION_01)==HIGH){
    //记录有人
    _acts[_i_act] = 1;
  }
  if(DEBUG){
    Serial.print("A");Serial.print(_i_act);
    Serial.print("B");Serial.print(_acts[_i_act]);
  }

  //检查是否满足1分钟的条件
  if(_stay_time_tmp==0)_stay_time_tmp = millis();
  int diff = (millis() - _stay_time_tmp)/1000;   //换算成秒
  if(diff>=60){
    //array +1
    _i_act++;
    _stay_time_tmp=0;
  }
  if(DEBUG){
    Serial.print("C");Serial.print(_i_act);
    Serial.print("D");Serial.print(_stay_time_tmp);
  }

  if(_i_act>=10){
    //record has people?
    //clear array, reset array

    if(DEBUG){Serial.print("H");}
    _haspeople = 0;
    for(int i=0;i<10;i++){
      //分析10个数组中是否有动作,有则判定有人
      if(_acts[i]==1){
        _haspeople = 1;
      }
      if(DEBUG){Serial.print(_acts[i]);}
      _acts[i] = 0;
    }
    _i_act = 0;
    if(DEBUG){
      Serial.print("check array is clear?");
      for(int i=0;i<10;i++){
        Serial.print("E");Serial.print(i);
        Serial.print("F");Serial.print(_acts[i]);
      }
    }
  }
  
  delay(2);
}


