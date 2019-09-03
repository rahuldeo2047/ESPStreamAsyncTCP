#ifndef COMMON_DEF_H
#define COMMON_DEF_H

#include "config_json_stream_parser.h"

#include "config.h"


void wifimanager_setup();

// void syslog_debug(char* msg){};
// void syslog_info(char* msg){};
// void syslog_warn(char* msg){};
// void syslog_error(char* msg){};
void syslog_debug(char* msg);
void syslog_info(char* msg);
void syslog_warn(char* msg);
void syslog_error(char* msg);

bool mpu_setup();
bool mpu_scan();
void mpu_loop();
float mpu_getTempFiltered();
float mpu_getTemp();
float mpu_getAccelFftMagFiltered();
float mpu_getAccelTwiceFftMagFiltered();
float mpu_getAccelFftMag();
void mpu_resetSampleTimer();

void Irms_setup();
void Irms_resetSampleTimer();
bool Irms_loop(); // It is not measuring status
float Irms_getCurr();
float Irms_getFilteredCurr();

bool setup_php_server();

void setup_OTA();

bool loop_OTA();

void mpu_loop();

bool loop_php_server(unsigned long _php_sr, unsigned long _php_uptm, float _php_temp_f, float _php_temp_r, float _php_current_f, float _php_current_r, float _php_accel_f, float _php_accel_r);

ConfigListener * getJsonConfigListenerPtr();

bool updateCodeUpdateStatus(void);
bool processConfig();

void setDeviceMacStr();
char * getDeviceMacStr();

void sendGraphDate(char* _device_id, char *message);

bool notifier_ledNotifierSetup();
void notifier_setNotifierState(NOTIFIER_STATES _state);
void notifier_ledNotifierLoop();


void loop_server_connection();
bool is_data_available();
char read_data();
void setup_server_connection();

#endif //COMMON_DEF_H