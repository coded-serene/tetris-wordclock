#define WEBSERVER_H
//#include "MyWC12x12_config.h"

//#include <ESP8266WebServer.h>
#include <Arduino.h>
#include <WebServer.h>

#ifdef WEBSERVER_CPP
//
// Webserver für die Konfiguration
//ESP8266WebServer server(80);
WebServer server(80);

#else
	
extern WebServer server;

extern void handleRootPath();
extern void handleClientServer();
extern void startServer();
#endif
