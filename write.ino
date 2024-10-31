

bool writeFile(String data) {

//  StaticJsonDocument<32> jsondoc;  
//  // JSONify local configuration parameters
//  jsondoc["t"] = t;//Write the new ARM status sent OTA
//  jsondoc["p"] = p;//Write the new ARM status sent OTA


  // Open file for writing
  File f = SPIFFS.open(LOG_FILE, "a");
  if (!f) {
    P_R_I_N_T("Failed to open config file for writing");
    return false;
  }


  // Stringify and write the json string to file and close it
//  char buffer[32];
//  size_t n = serializeJson(jsondoc, buffer);
//  Serial.println("written data: ");
//  Serial.println(buffer);
  f.println(data);
  f.close();

  P_R_I_N_T("\nFile successfully saved");
  return true;
}
