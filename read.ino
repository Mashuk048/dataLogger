bool mountSPIFFS(){
    // Mount the filesystem
  bool result = SPIFFS.begin();
//  Serial.println("SPIFFS opened: " + result);


  Serial.println("SPIFFS started. Contents:");
  
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {                      // List the file system contents
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("\tFS File: %s, size: %s\r\n", fileName.c_str(), String(fileSize).c_str());
//      Serial.printf("\tFS File: %s ", fileName.c_str());
      if(fileSize > 0){
        if(SPIFFS.remove(LOG_FILE)){ Serial.println("File removed");   }
      }
    }


    
    Serial.printf("\n");
  
  
  return result;
}


bool readFile() {

  // this opens the config file in read-mode
  File file = SPIFFS.open(LOG_FILE, "r");
  
  if (!file) {
    P_R_I_N_T("File not found");
    return false;
  } else {
    // we could open the file
    size_t size = file.size();
    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);

      // Allocate a buffer of size(bytes)
//      char* buf = (char*)malloc(size);
  
      if (buf != NULL) {
        Serial.println("Buffer allocated successfully");
        // Use the buffer for processing
            // Read and store file contents in buf
        file.readBytes(buf.get(), size);
   
        // Closing file
        file.close();
        
        Serial.println("File Content:");
        payload = buf.get();
        Serial.println(payload);
        // Free the buffer when done
//        free(buf);
      } else {
        Serial.println("Failed to allocate buffer");
      }


    

    //publish data
//    mqttPublish(payload);
  }
  P_R_I_N_T("\nFile was successfully parsed");
  return true;
}
