#include <Arduino.h>
#include <Ticker.h>
#include <elapsedMillis.h>
#include <jled.h>
#include "common_def.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define ev_name(event) (STRINGIFY(event))

//int count = 0;

/* 

1 gpio white led
1 gpio blue led
1 gpio blue led
1 gpio yellow led
1 gpio red led
1 gpio buzzer

enum 
{

_0_LED_CODE_BURN_STARTED, // burn=1 HZ, solid white

_1_LED_WIFI_CHECK, // 1 HZ BLINK
_1_LED_WIFI_CONFIG, // OR ERROR 3-5 HZ BLINK
_1_LED_WIFI_CONNECTED, // SOLID // 0 HZ BLINK
_1_LED_WIFI_OFFLINE_MODE, // OFF OR 5 SEC BLINK

_2_LED_SERVER_CONNECTED, // 1 HZ BLINK
_2_LED_SERVER_CON_FAILD, // 3-5 HZ BLINK
_2_LED_SERVER_DATA_SENDING, // SOLID BLUE
// _2_ ... sendinf failed will be same as _2_LED_SERVER_CON_FAILD
_2_LED_SERVER_DATA_SENT, // OR OFFLINE MODE // BLUE OFF OR 5 SEC BLINK

_3_LED_SENSOR_OK, //  OFF OR 5 TO 10 SEC BLINK
_3_LED_SENSOR_NOTIFY, // 1 HZ BLINK YELLOW
_3_LED_SENSOR_ALERT, // 2 HZ BLINK YELLOW
_4_LED_SENSOR_WARN, // 5 HZ YELLOW BLINK AND 1 HZ RED BLINK
_4_LED_SENSOR_EMERGENCY, // 10 HZ YELLOW BLINK AND 2 HZ RED BLINK

_5_BUZZER_SENSOR_OK, // OFF
_5_BUZZER_SENSOR_NOTIFY, // 100 HZ SOUND with 1 HZ on off 
_5_BUZZER_SENSOR_ALERT, // 200 HZ SOUND with 1 HZ on off
_5_BUZZER_SENSOR_WARN, // 500 HZ SOUND with 2 HZ on off
_5_BUZZER_SENSOR_EMERGENCY // 1000 HZ SOUND with 2 HZ on off
};

enum
{
  IDLE=0,
  POWER_ON,
  
  WIFI_STATE_CHECKING, // NORMAL BLINK BLUE
  WIFI_STATE_CONFIG, // OR FAILED // FAST BLINK BLUE
  WIFI_STATE_CONNECTED, // SOLID BLUE

  SERVER_CONNECTED, // NORMAL BLINK BLUE
  SERVER_CONNECTION_FAILED, // FAST BLINK BLUE
  SERVER_DATA_SENDING, // SOLID BLUE
  SERVER_DATA_SENT, // IDLE // BLUE OFF

  SENSOR_OK, // BUZZER, YELLOW AND RED OFF
  SENSOR_NOTIFY, // YELLOW BLINK AND BUZZER LOW FREQ SOUND
  SENSOR_ALERT, // YELLOW FAST BLINK  AND BUZZER MID FREQ SOUND
  SENSOR_WARN, // RED BLINK AND BUZZER HIG FREQ SOUND
  SENSOR_EMERGENCY, // RED FAST BLINK AND BUZZER VERY HIGH FREQ SOUND



};

*/

// https://github.com/jandelgado/jled
//#include <jled.h>

// breathe LED for 5 times, LED is connected to pin 9 (PWM capable) gpio
//auto led = JLed(D13).Breathe(800).Forever().DelayAfter(200).FadeOn(500).DelayAfter(200).FadeOff(500).DelayAfter(200);

// Ticker flipper
// , flipper_white
// , flipper_blue_1
// , flipper_blue_2
// , flipper_yellow
// , flipper_red
// , flipper_white_off;

// // to verfy as MPU was not connected.
// #define LED_WHITE (D8) // (D8) // I2C gone bad
// #define LED_BLUE_1 (D1) // (D7) // Serial print is good
// #define LED_BLUE_2 (D2) // (D6) // all good
// #define LED_YELLOW (D3) // (D5) // all good
// #define LED_RED (D4) // (D0) // I2C gone bad completely
// #define LED_WHITE_OFF (D4) // (D8)

// struct LED_STATE
// {
//   uint8_t LED_PERI;
//   bool LED_state;
//   Ticker flipper;

// };

// #define WHITE (0)
// #define BLUE_1 (1)
// #define BLUE_2 (2)
// #define YELLOW (3)
// #define RED (4)
// #define WHITE_OFF (5)
// #define LED_ID_MAX (WHITE_OFF+1)

// // This does not work
// // struct LED_STATE led_states[]=
// // {
// //   {flipper_white, LED_WHITE, false},
// //   {flipper_blue_1, LED_BLUE_1, false},
// //   {flipper_blue_2, LED_BLUE_2, false},
// //   {flipper_yellow, LED_YELLOW, false},
// //   {flipper_red, LED_RED, false}
// // };

// // struct padding is a big issue
// struct LED_STATE led_states[LED_ID_MAX]=
// {
//   { LED_WHITE, false, flipper_white},
//   { LED_BLUE_1, false, flipper_blue_1},
//   { LED_BLUE_2, false, flipper_blue_2},
//   { LED_YELLOW, false, flipper_yellow},
//   { LED_RED, false, flipper_red},
//   { LED_WHITE_OFF, false, flipper_white_off},
// };

// auto led_breathe_white_normal = JLed(LED_WHITE).Blink(50, 4950).Forever();//.FadeOff(500).DelayAfter(250).Breathe(500).DelayAfter(200);//.FadeOff(500).DelayAfter(750);
// auto led_breathe_white_fast = JLed(LED_WHITE).Blink(50, 950).Forever();//.FadeOff(500).DelayAfter(250).Breathe(500).DelayAfter(200);//.FadeOff(500).DelayAfter(750);

// enum LED_WHITE_BREATHE_STATE
// {
//   LED_WHITE_BREATHE_STATE__OFFLINE,
//   LED_WHITE_BREATHE_STATE__NORMAL,
//   LED_WHITE_BREATHE_STATE__MID,
//   LED_WHITE_BREATHE_STATE__FAST,
//   LED_WHITE_BREATHE_STATE__VERY_FAST,
//   LED_WHITE_BREATHE_STATE__INVALID
// };

// LED_WHITE_BREATHE_STATE led_white_breahe_state;

// JLed led_breathe_white[LED_WHITE_BREATHE_STATE__INVALID] =
// {
//   JLed(LED_WHITE).Blink(50, 9950).Forever(), // LED_WHITE_BREATHE_STATE__OFFLINE
//   JLed(LED_WHITE).Blink(10, 4950).Forever(), // LED_WHITE_BREATHE_STATE__NORMAL
//   JLed(LED_WHITE).Blink(5, 1950).Forever(), //LED_WHITE_BREATHE_STATE__MID
//   JLed(LED_WHITE).Blink(5, 150).Forever(),  //LED_WHITE_BREATHE_STATE__FAST
//   JLed(LED_WHITE).Blink(5, 50).Forever()//LED_WHITE_BREATHE_STATE__VERY_FAST
// };

// JLed leds[] = {
//     JLed(LED_BLUE_1).FadeOn(1000).Forever(),
//     JLed(LED_BLUE_2).Blink(100, 900).Forever(),
//     JLed(LED_YELLOW).FadeOff(1000).Forever(),
//     JLed(LED_RED).Breathe(1000).Forever(),
//     //JLed(LED_BLUE_1).FadeOn(1000).Forever(),
//     //JLed(LED_RED).Off(),
//     //JLed(LED_BLUE_2).Off()
// };

// JLedSequence sequence(JLedSequence::eMode::PARALLEL, leds);

// JLed white_led[] = {
//     //JLed(LED_WHITE).Breathe(2000).Repeat(1),
//     //JLed(LED_WHITE).FadeOff(500).Repeat(1),
//     JLed(LED_WHITE).Blink(50, 4950).Forever()
// };
// JLedSequence white_sequence(JLedSequence::eMode::SEQUENCE, white_led);


// void flip()
// {
// 	int state = digitalRead(D4);  // get the current state of GPIO1 pin
// 	digitalWrite(D4, !state);     // set pin to the opposite state

// 	// ++count;
// 	// // when the counter reaches a certain value, start blinking like crazy
// 	// if (count == 20)
//   // {
// 	// 	flipper.attach(0.1, flip);
// 	// }
// 	// // when the counter reaches yet another value, stop blinking
// 	// else if (count == 120)
//   // {
// 	// 	flipper.detach();
// 	// }
// }

// void flip_d(const uint8_t led_id)
// {
// 	int pin_state = digitalRead(led_states[led_id].LED_PERI);  // get the current state of GPIO1 pin
//   //bool state = led_states[led_id].LED_state;
//   led_states[led_id].LED_state = pin_state;

// 	digitalWrite(led_states[led_id].LED_PERI, !pin_state);     // set pin to the opposite state

// }

// void flip_dx(const uint8_t led)
// {
// 	int pin_state = digitalRead(led);  // get the current state of GPIO1 pin
//   //bool state = led_states[led_id].LED_state;

// 	digitalWrite( led , !pin_state);     // set pin to the opposite state

// }

// // led_id and freq
// void ledState(int led_id, float freq)
// {
//   float ms = 0;
//   if(freq == 0)
//   {
//     led_states[led_id].flipper.detach();
//   }
//   else
//   {
//     ms = (1000.0f/freq);
//     //led_states[led_id].flipper.attach_ms(ms, std::bind(flip_d, led_id));
//     led_states[led_id].flipper.attach_ms_scheduled(ms, std::bind(flip_d, led_id));

//   }

// }

// white works with D1 mini
void setup()
{

  Serial.begin(115200);
  pinMode(D4 /* D9*/, OUTPUT);
  digitalWrite(D4 /* D9*/, HIGH);

  notifier_ledNotifierSetup();

  wifimanager_setup();

  setup_server_connection(); // can not initiate connection // only variable initiation.

  // pinMode(LED_WHITE, OUTPUT); // always on
  // digitalWrite(LED_WHITE, LOW);

  // pinMode(LED_BLUE_1, OUTPUT);
  // digitalWrite(LED_BLUE_1, HIGH);

  // pinMode(LED_BLUE_2, OUTPUT);
  // digitalWrite(LED_BLUE_2, HIGH);

  // pinMode(LED_YELLOW, OUTPUT);
  // digitalWrite(LED_YELLOW, HIGH);

  // //pinMode(D1, OUTPUT);
  // //digitalWrite(D1, HIGH);

  // pinMode(LED_RED, OUTPUT);
  // digitalWrite(LED_RED, HIGH);

  //pinMode(A0, INPUT);

  //pinMode(D2, OUTPUT);
  //digitalWrite(D2, HIGH);
  //analogWriteFreq(100); // can be useful for buzzer

  // flip the pin every 0.3s
  //flipper.attach(1, flip);
  mpu_setup();
  //flipper_red.attach_ms(300, std::bind(flip_dx, D0));
  //flipper_red_2.attach_ms(300, std::bind(flip_d, D1));
  //flipper_red.attach_ms(300, std::bind(flip_d, RED));
  // flipper_yellow.attach_ms(301, std::bind(flip_d, YELLOW));
  // flipper_blue_1.attach_ms(302, std::bind(flip_d, BLUE_1));
  // flipper_blue_2.attach_ms(303, std::bind(flip_d, BLUE_2));
  // flipper_white.attach_ms(304, std::bind(flip_d, WHITE));

  //ledState(WHITE, 0.25);
  // ledState(BLUE_1, 1.0);
  // ledState(BLUE_2, 0.5);
  // ledState(YELLOW, 0.25);
  // ledState(RED, 0.125);
  //delay(500.0/0.25);
  //ledState(WHITE_OFF, 0.5);
  //flipper.attach(0.6, flip);
}

elapsedMillis elapsedTimeMain, elprint, elapsed_event;

// float inc = 1.0;
// bool trig[LED_ID_MAX] = {false};
// float trig_freq[LED_ID_MAX] = {0.0f};

// char white_led_status = '0';

// const float max_trig_freq = 100.0f ;

// void serialCheck()
// {
//   if(Serial.available())
//   {
//     char c = Serial.read();
//     char d = c;
//     d = d - 0x30;
//     if(isDigit(c) && (d < LED_ID_MAX))
//     {

//       for(int i = 0 ; i < LED_ID_MAX ; i++)
//       {
//         trig[i] = false;
//       }

//       trig[(int)d] = true;
//       Serial.printf_P("LED ID trig[%d] (=%d) selected.\n", d, trig[d]);
//     }
//     else if ( (c=='+') ||  (c=='-') )
//     {
//       TRIG(WHITE)
//       TRIG(BLUE_1)
//       TRIG(BLUE_2)
//       TRIG(YELLOW)
//       TRIG(RED)
//       TRIG(WHITE_OFF)
//     }
//     else if ( (c=='o') ||  (c=='n') ||  (c=='m') ||  (c=='f') ||  (c=='v') )
//     {
//       white_led_status = c;
//       switch (white_led_status)
//       {
//         case ('o'):
//           led_white_breahe_state = LED_WHITE_BREATHE_STATE__OFFLINE;
//         break;
//         case ('n'):
//           led_white_breahe_state = LED_WHITE_BREATHE_STATE__NORMAL;
//         break;
//         case ('m'):
//           led_white_breahe_state = LED_WHITE_BREATHE_STATE__MID;
//         break;
//         case ('f'):
//           led_white_breahe_state = LED_WHITE_BREATHE_STATE__FAST;
//         break;
//         case ('v'):
//           led_white_breahe_state = LED_WHITE_BREATHE_STATE__VERY_FAST;
//         break;
//         default:
//           led_white_breahe_state = LED_WHITE_BREATHE_STATE__INVALID;
//       }

//     }
//     else
//     {
//       Serial.printf_P("Invalid cmd : %d %c\n", c, c);
//     }

//   }
// }

struct TEST_EVENT
{
  enum NOTIFIER_STATES state;
  uint32_t duration_ms;
  char *event_name;
};

#define CODE(event, ms) \
  {                     \
    event, ms, #event   \
  }

#define MAX_TEST_EVENT (20)

struct TEST_EVENT test_events[MAX_TEST_EVENT] =
    {
        CODE(NOTIFIER_STATES::_0_NOTIFIER_HB_OFFLINE_MODE, 10000),
        CODE(NOTIFIER_STATES::_0_NOTIFIER_HB_PING, 10000),
        CODE(NOTIFIER_STATES::_0_NOTIFIER_CODE_ERROR, 5000),
        CODE(NOTIFIER_STATES::_0_NOTIFIER_LOCAL_CODE_BURN_STARTED, 5000),
        CODE(NOTIFIER_STATES::_0_NOTIFIER_REMOTE_CODE_BURN_STARTED, 5000),
        CODE(NOTIFIER_STATES::_1_LED_WIFI_CHECK, 5000),
        CODE(NOTIFIER_STATES::_1_LED_WIFI_CONN_FAILED, 5000),
        CODE(NOTIFIER_STATES::_1_LED_WIFI_CONNECTED, 5000),
        CODE(NOTIFIER_STATES::_1_LED_WIFI_OFFLINE_MODE, 5000),
        CODE(NOTIFIER_STATES::_1_LED_WIFI_CONFIG, 5000),
        CODE(NOTIFIER_STATES::_2_LED_SERVER_CONN_FAILED, 5000),
        CODE(NOTIFIER_STATES::_2_LED_SERVER_CONNECTED, 5000),
        CODE(NOTIFIER_STATES::_2_LED_SERVER_CONNECTING, 5000),
        CODE(NOTIFIER_STATES::_2_LED_SERVER_DATA_SENDING, 5000),
        CODE(NOTIFIER_STATES::_2_LED_SERVER_DATA_SENT, 5000),
        CODE(NOTIFIER_STATES::_3_LED_SENSOR_OK, 5000),
        CODE(NOTIFIER_STATES::_3_LED_SENSOR_NOTIFY, 5000),
        CODE(NOTIFIER_STATES::_3_LED_SENSOR_ALERT, 5000),
        CODE(NOTIFIER_STATES::_4_LED_SENSOR_WARN, 5000),
        CODE(NOTIFIER_STATES::_4_LED_SENSOR_EMERGENCY, 5000)};

// struct TEST_EVENT test_events[MAX_TEST_EVENT]=
// {
//   {NOTIFIER_STATES::_0_NOTIFIER_HB_OFFLINE_MODE , 10000, "_0_NOTIFIER_HB_OFFLINE_MODE"},
//   {NOTIFIER_STATES::_0_NOTIFIER_HB_PING , 10000, "_0_NOTIFIER_HB_PING"},
//   {NOTIFIER_STATES::_0_NOTIFIER_CODE_ERROR , 5000, "_0_NOTIFIER_CODE_ERROR"},
//   {NOTIFIER_STATES::_0_NOTIFIER_LOCAL_CODE_BURN_STARTED , 5000, "_0_NOTIFIER_LOCAL_CODE_BURN_STARTED"},
//   {NOTIFIER_STATES::_0_NOTIFIER_REMOTE_CODE_BURN_STARTED , 5000, "_0_NOTIFIER_REMOTE_CODE_BURN_STARTED"},
//   {NOTIFIER_STATES::_1_LED_WIFI_CHECK , 5000, "_1_LED_WIFI_CHECK"}
// };

int event_num = 0;

int test_index = 0;

bool status_server_response=false;

void loop()
{

  //serialCheck();

  // if(Serial.available())
  // {
  //   char c = Serial.read();
  //   if(c=='r')
  //   {
  //     ESP.reset();
  //   }

  //   if(c=='+')
  //   {
  //     event_num += 1;
  //     notifier_setNotifierState(test_events[((event_num)%MAX_TEST_EVENT)].state);
  //     Serial.printf_P("notifier event: %s\n", test_events[((event_num)%MAX_TEST_EVENT)].event_name);

  //   }
  //   if(c=='-')
  //   {
  //     event_num -= 1;
  //     notifier_setNotifierState(test_events[((event_num)%MAX_TEST_EVENT)].state);
  //     Serial.printf_P("notifier event: %s\n", test_events[((event_num)%MAX_TEST_EVENT)].event_name);

  //   }
  // }
  //led_breathe_white.Update();
  //sequence.Update();

  mpu_loop();
  //notifier_ledNotifierLoop();

  // if(led_white_breahe_state != LED_WHITE_BREATHE_STATE__INVALID)
  // {
  //   led_breathe_white[led_white_breahe_state].Update();
  // }

  // if(elapsed_event >= test_events[((event_num)%MAX_TEST_EVENT)].duration_ms)
  // {
  //   elapsed_event = 0;

  //   notifier_setNotifierState(test_events[((event_num)%MAX_TEST_EVENT)].state);
  //   Serial.printf_P("notifier event: %s\n", test_events[((event_num)%MAX_TEST_EVENT)].event_name);

  // }

  if (status_server_response)
  {
    //Serial.write(read_data());
    Serial.printf_P("|server| %d | parse_data() %d\n",micros(), status_server_response);

    status_server_response = parse_data();

    Serial.printf_P("|server| %d | parsed data() %d\n",micros(), status_server_response);

    status_server_response = false;
  }

  if (is_data_available() && status_server_response==false)
  {
    //Serial.write(read_data());
    status_server_response = check_for_data();
    Serial.printf_P("|server| %d | check_for_data() %d\n",millis(),status_server_response);
  }

  if (elapsedTimeMain >= 10000)
  {
    elapsedTimeMain = 0;
    //sequence.Reset();
    mpu_scan();
    
    Serial.printf_P("|server| %d | send_data() %d\n",millis(), status_server_response);

    loop_server_connection(); // test
    status_server_response =false;

    //led_breathe_white_normal.Reset();
  }
}
