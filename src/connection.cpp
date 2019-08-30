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

AsyncClient *client = new AsyncClient;
SyncClient sclient;

static void replyToServer(void *arg)
{
	AsyncClient *client = reinterpret_cast<AsyncClient *>(arg);

	// send reply
	if (client->space() > 32 && client->canSend())
	{
		char message[32];
		sprintf(message, "this is from %s", WiFi.localIP().toString().c_str());
		client->add(message, strlen(message));
		client->send();
	}
}

static void sendData(String _msg)
{

	// send data
	if (client->space() > _msg.length() && client->canSend())
	{
		client->add(_msg.c_str(), _msg.length());
		if (client->send())
		{
			sprintf_P(getPrintBuffer(), "\n data sent\n");
		}
		else
		{
			sprintf_P(getPrintBuffer(), "\n data send failed\n");
		}

		Serial.printf(getPrintBuffer());
		syslog_info(getPrintBuffer());
	}
}

/* event callbacks */
static void handleData(void *arg, AsyncClient *client, void *data, size_t len)
{
	//Serial.printf("\n data received from %s \n", client->remoteIP().toString().c_str());
	//Serial.write((uint8_t*)data, len);

	sprintf_P(getPrintBuffer(), "\n data received from %s , (%d) %s", client->remoteIP().toString().c_str(), len, (char *)data);
	Serial.printf(getPrintBuffer());
	syslog_info(getPrintBuffer());

	//os_timer_arm(&intervalTimer, 2000, true); // schedule for reply to server at next 2s
}

void onConnect(void *arg, AsyncClient *client)
{
	//Serial.printf("\n client has been connected to %s on port %d \n", php_server, php_server_port);

	sprintf_P(getPrintBuffer(), "\n client has been connected to %s on port %d \n", php_server, php_server_port);
	Serial.printf(getPrintBuffer());
	syslog_info(getPrintBuffer());
	//replyToServer(client);

	String mac_str = (WiFi.macAddress());
	mac_str.replace(":", "");

	String data_str = "device_code_type=" + String(DEVICE_DEVELOPMENT_TYPE) + "&config_id=" + String(config_id) + "&config_type=l" // long or short
					  + "&device_code_version=" + _VER_ + "&Device_mac_id_str=" + mac_str;

	String getStr = "GET " + String(php_config_server_file_target) + data_str + " HTTP/1.1\r\nHost: " + String(php_server) + "\r\n" + "Connection: keep-alive\r\n"
					//+ "Content-Length: " + data_str.length() + "\r\n" +
					+ "\r\n\r\n"
		//+ data_str
		;

	// String request = String("POST ") + url + "HTTP/1.0 \r\n" +
	//                  "Host: " + host + "\r\n" +
	//                  "Accept: *" + "/" + "*\r\n" +
	//                  "Content-Length: " + data.length() + "\r\n" +
	//                  "Content-Type: aplication/x-www-form-urlencoded\r\n" +
	//                  "\r\n" + data;
	// Serial.print(request);
	// client.print(request);

	sprintf_P(getPrintBuffer(), "\n sending data %s\n", getStr.c_str());
	Serial.printf(getPrintBuffer());
	syslog_info(getPrintBuffer());

	sendData(getStr);
}

void onDisconnect(void *arg, AsyncClient *client)
{
	sprintf_P(getPrintBuffer(), "\n client has been disconnected from %s on port %d \n", php_server, php_server_port);
	Serial.printf(getPrintBuffer());
	syslog_info(getPrintBuffer());
}

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

bool is_data_available()
{
	return sclient.available();
 
}

char read_data()
{ 
	return sclient.read(); // returns -1 if nothing received yet
}

void loop_server_connection()
{

	if (server_connect())
	{
		sclient.setTimeout(2);
		 
		String getStr = create_query();

		if(sclient.printf(getStr.c_str())>0)
		{
			// sent
						Serial.println("Send sent\n");

		}

		// if (sclient.printf(getStr.c_str()) > 0)
		// {
		// 	while (sclient.connected() && sclient.available() == 0)
		// 	{
		// 		delay(1);
		// 	}
		// 	while (sclient.available())
		// 	{
		// 		Serial.write(sclient.read());
		// 	}
		// 	if (sclient.connected())
		// 	{
		// 		sclient.stop();
		// 	}
		// }
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