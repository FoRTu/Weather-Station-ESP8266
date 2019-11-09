void ConnectWiFi_STA(bool useStaticIP = true)
{
	Serial.println("");
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	if(useStaticIP) WiFi.config(ip, gateway, subnet);
	while (WiFi.status() != WL_CONNECTED) 
	{ 
		delay(100);  
		Serial.print('.'); 
	}

	Serial.println("");
	Serial.print("Iniciado STA:\t");
	Serial.println(ssid);
	Serial.print("IP address:\t");
	Serial.println(WiFi.localIP());
}
