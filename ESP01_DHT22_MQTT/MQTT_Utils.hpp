void sendMessage(char* topic, char* message) {
  client.connect(mqtt_clientid, mqtt_username, mqtt_password);
  client.publish(topic, message);
  Serial.print("Mensaje enviado:");
  Serial.print(topic);
  Serial.println(message);
}
