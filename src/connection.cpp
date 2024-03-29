#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>

extern "C"
{
#include <osapi.h>
#include <os_type.h>
}

#include "ESPAsyncTCP.h"
#include "SyncClient.h"

#include <Arduino.h>
#include "config.h"

#include "JsonStreamingParser.h"
#include "JsonListener.h"
#include "config_json_stream_parser.h"

#include "common_def.h"

//AsyncClient *client = new AsyncClient;
SyncClient sclient;

JsonStreamingParser json_parser;
ConfigListener json_parser_listener;
Device_config * device_config;
  
bool server_connect()
{
	bool status = true;
	if (!sclient.connected())
	{
		if (!sclient.connect(php_server, php_server_port))
		{
			Serial.println("Connect Failed");
			status = false;
		}
	}
	return status;
}

String create_query()
{ 
	String mac_str = (WiFi.macAddress());
	mac_str.replace(":", "");

	String data_str = "device_code_type=" + String(DEVICE_DEVELOPMENT_TYPE) + "&config_id=" + String(config_id) + "&config_type=l" // long or short
					  + "&device_code_version=" + _VER_ + "&Device_mac_id_str=" + mac_str;

	String getStr = "GET " + String(php_config_server_file_target) + data_str + " HTTP/1.1\r\nHost: " + String(php_server) + "\r\n" + "Connection: keep-alive\r\n"
					//+ "Content-Length: " + data_str.length() + "\r\n" +
					+ "\r\n\r\n"
		//+ data_str
		;

	return getStr;
}

bool server_is_data_available()
{
	return sclient.available();
}

char read_data()
{ 
	return sclient.read(); // returns -1 if nothing received yet
}

// call it only when check_for_data() == true
bool server_parse_data()
{
	bool status ;
	json_parser.reset();
	unsigned long ts_wait_for_client = millis();
    while (server_is_data_available())
    {
        json_parser.parse( (char) read_data() );
        //delayMicroseconds(100);

        if (millis() - ts_wait_for_client > 250)
        {
            //Serial.println(" timed out.");
            sprintf(print_buffer, " timed out.");
            //Serial.println();
            Serial.println(print_buffer);
            syslog_warn(print_buffer);
            status = false;                                                                                 
            sclient.stop();     
            return false;
            break;
        }
    } 
    
    // // This can use too much of data on internet download part
 
    sprintf(print_buffer, "JSON parsing ended");
    //Serial.println();
    Serial.println(print_buffer);
    syslog_info(print_buffer);

	return status;
}

bool check_for_response()
{
	// Check HTTP status
	bool status;
    uint8_t status_str[32] = {0};
    const char *status_ptr = (const char *)status_str;
    sclient.setTimeout(1500);

    sclient.readBytesUntil('\r', status_str, sizeof(status_str)); 

	// sprintf(getPrintBuffer(), "\n\nResponse: %s\n\n", status_str);
    //     //Serial.print(F("Unexpected response: "));
	// Serial.println(getPrintBuffer());
	// syslog_warn(getPrintBuffer());

    //notifier_setNotifierState(NOTIFIER_STATES::_2_LED_SERVER_DATA_SENT_RESPONDED);
 
    // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
    if (strcmp(status_ptr + 9, "200 OK") != 0)
    {
        //notifier_setNotifierState(NOTIFIER_STATES::_0_NOTIFIER_CODE_ERROR);
        status = false;
        sprintf(getPrintBuffer(), "\n\nUnexpected response: %s\n\n", status_str);
        //Serial.print(F("Unexpected response: "));
        Serial.println(getPrintBuffer());
        syslog_warn(getPrintBuffer());
		// return if any other flow further needs to be done
    }
	else
	{
		status = true;
	}
	

	return status;


}

bool server_check_for_data()
{
	bool status = check_for_response();
	if(status == true)
	{
		sclient.setTimeout(2);
		char endOfHeaders[] = "\r\n\r\n";
		if (!sclient.find(endOfHeaders))
		{
			status = false;
		}
		else
		{
			status = true;
		}
	}
	
	return status;
}

ConfigListener * getJsonConfigListenerPtr()
{
    return &json_parser_listener;
} 

void setup_server_connection()
{
	json_parser.setListener(&json_parser_listener); 
    device_config = json_parser_listener.getDeviceConfigPtr();
}

void loop_server_connection()
{

	if (server_connect())
	{
		sclient.setTimeout(2);
		 
		String getStr = create_query();

		// send data
		if(sclient.printf(getStr.c_str())>0) // send data
		{
			// sent
			Serial.printf_P("Send sent: %s\n",getStr.c_str());
		}
 
		else
		{
			sclient.stop();
			Serial.println("Send Failed");
			while (sclient.connected())
				delay(0);
		}
	}

	// client->onData(&handleData, client);
	// client->onConnect(&onConnect, client);
	// client->onDisconnect(&onDisconnect, client);
	// client->connect(php_server, php_server_port);// php_server_file_target
}