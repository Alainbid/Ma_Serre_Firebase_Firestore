

// Route for root / web page
  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/html", index_html);
  // });

// Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  // server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
  //   String inputMessage1;
  //   String inputMessage2;
  //   // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  //   if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
  //     inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
  //     inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
  //     digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
  //   }
  //   else {
  //     inputMessage1 = "No message sent";
  //     inputMessage2 = "No message sent";
  //   }
  //   Serial.print("frequence = ");
  //   Serial.println(inputMessage1);
  //   Serial.print(" duree = ");
  //   Serial.println(inputMessage2);
  //   request->send(200, "text/plain", "OK");
  // });

  // Start server
 // server.begin();
 //  lireConsigne();
 // ecriredb();