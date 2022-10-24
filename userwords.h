/* userwords.h 
FJRusso 221024
Provides SPIFFS & SD card suppport
 To simplify calling C functions, you can also refer to elements on the stack with the types positional names:

n10 n9 n8 n7 n6 n5 n4 n3 n2 n1 n0 - Access stack as cell_t integer values
                   c4 c3 c2 c1 c0 - Access stack as char* values
                   b4 b3 b2 b1 b0 - Access stack as uint8_t* byte values
             a6 a5 a4 a3 a2 a1 a0 - Access stack
*/
//
//
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz 
//
void sdcardbeginc(){
      if(!SD.begin()){
        Serial.println("\nCard Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();
    if(cardType == CARD_NONE){
        Serial.println("\nNo SD card attached");
        return;
    }
    Serial.print("\nSD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
    Serial.printf("Bytes Used - %lluMB\n", SD.usedBytes() / (1024 * 1024));
}
//
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    int SubFCount = 0;
    int FCount = 0;
    int TDir = 0;
    Serial.printf("\nListing directory: %s\n", dirname);
    File root = fs.open(dirname);
    if(!root){
        Serial.println("\nFailed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("\nNot a directory");
        return;
    }
    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            if(SubFCount > 0) {Serial.printf("\n Total Files = %u ", SubFCount);}
            Serial.println("\n**************************************************"  );
            Serial.print("\n  DIR : ");
            Serial.print(file.name());
            SubFCount = 0;
            TDir += 1;
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
            SubFCount += 1;
            FCount += 1;
        }
        file = root.openNextFile();
    }
     Serial.println("\n**************************************************");
       Serial.printf("Total Files = %u\tTotal Directories = %u", FCount, TDir);
            Serial.println("\n**************************************************"  );
}
//
int LDir(fs::FS &fs, const char * dirname, uint8_t levels, char * Buff ){
    int SubFCount = 0;
    int FCount = 0;
    int TDir = 0;
    int LnCount = 0;
    sprintf_P(Buff,"Listing directory: %s\n", dirname);
    Buff += 48;
    LnCount += 1;
    File root = fs.open(dirname);
    if(!root){
        Serial.println("\nFailed to open directory");
        return 0;
    }
    if(!root.isDirectory()){
        Serial.println("\nNot a directory");
        return 0;
    }
    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            if(SubFCount > 0) {sprintf_P(Buff, "Total Files = %u\n", SubFCount);}
     Buff += 48;
     LnCount += 1;
     sprintf_P(Buff,"\n**********************************************\n");
     Buff += 48;
     LnCount += 1;
            strcpy_P(Buff,"DIR : ");
            Buff += 48;
            LnCount += 1;
            strcpy_P(Buff,(file.name()));
            Buff += 48;
            LnCount += 1;
            SubFCount = 0;
            TDir += 1;
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            SubFCount += 1;
            FCount += 1;
        }
        file = root.openNextFile();
    }
     sprintf_P(Buff,"\n**********************************************\n");
     Buff += 48;
     LnCount += 1;
     sprintf_P(Buff, "Total Files = %u Total Directories = %u\n", FCount, TDir);
     Buff += 48;
     LnCount += 1;
     sprintf_P(Buff,"\n**********************************************\n");
     Buff += 48;
     LnCount += 1;
     return LnCount;
}
//
void createDir(fs::FS &fs, const char * path){
    Serial.printf("\nCreating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.print("Dir created");
    } else {
        Serial.print("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
    Serial.printf("\nRemoving Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

// Returns the size of the file in bytes (unsigned long)

long filesize(fs::FS &fs, const char * path){
    // Serial.printf("Deleting file: %s\n", path);
    size_t len = 0;
   File file = fs.open(path);
   len = file.size();
    file.close();
    return len;
}
//
void readFilebuffsize(fs::FS &fs, const char * path, char buff[], long len){
    Serial.printf("Reading file: %s\n", path);
    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }
     Serial.print("Read from file: ");
    int i = 0;
    while(file.available()&& i<len){
        buff[i] = file.read(); 
        i++;
    }
    file.close();
}
//
void readFilebuff(fs::FS &fs, const char * path, char buff[], int len){
  readFilebuffsize( fs, path, buff, len);
}
//
void readFilebuffold(fs::FS &fs, const char * path, char buff[], int readbytes){
  int bytesread = 0;
  Serial.printf("\nReading file: %s\n", path);
    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }
     Serial.println("Read from file: ");
       int i = 0;
    while(file.available()&& i<readbytes){
        buff[i] = file.read();
        i++;
        bytesread = i;
    }
    Serial.printf("Bytes Read = %u\nReading completed\n", bytesread);
    file.close();
}

void readFilebuffseek(fs::FS &fs, const char * path, char buff[], long seekpos){
    Serial.printf("Reading file: %s\n", path);
    File file = fs.open(path);  
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }
    file.seek(seekpos);
     Serial.print("Read from file: ");
    int i = 0;
    while(file.available()&& i<1024){
        buff[i] = file.read(); 
        i++;
    }
    file.close();
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("\nReading file: %s\n", path);
    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }
    Serial.println("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    Serial.println("\nCompleted");
    file.close();
}

void writeFilebuff(fs::FS &fs, const char * path, char buff[], int len) {   
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
     Serial.println("Write from Buffer to file: ");
     Serial.printf("File length = %u\n", len);
     // uint8_t i = 0;
     //  while(file.available()&& i<len){
         file.print(buff[0], len);
     //  file.print(buff[i]); 
     //  Serial.println(buff[i]);
     //  Serial.print(buff);
     //  i++;       }
     //  file.write(buff[], len);
     //  file.print(buff[],len);   // file.write(buf, len)
    file.close();
    Serial.println("\nFile written");
}
void writeFile(fs::FS &fs, const char * path, char * message){
    Serial.printf("Writing file: %s\n", path);
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.write(*message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}
//
void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);
//
    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append Failed");
    }
    file.close();
}
//
void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}
//
void testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }
//
    file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
//
    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}
//
#define USER_WORDS \
  X("SPIFFS.LDIR", SPIFFS_LDIR, listDir(SPIFFS, "/", 0)) \
  Y(SPIFFSwritef,  writeFile(SPIFFS, c0, c1); DROPn(2) ) \
  Y(SPIFFSwritefbuff,  writeFilebuff(SPIFFS, c2, c1, n0); DROPn(3) ) \
  Y(SPIFFSreadf,  readFile(SPIFFS, c0); DROP ) \
  Y(SPIFFSreadfbuff, readFilebuff(SPIFFS, c2, c1, n0); DROPn(3) ) \
  Y(SPIFFSreadfbuffsize, readFilebuffsize(SPIFFS, c2, c1, n0); DROPn(3) ) \
  Y(SPIFFSreadfbuffseek, readFilebuffseek(SPIFFS, c2, c1, n0); DROPn(3) ) \
  Y(SPIFFSappendf, appendFile(SPIFFS, c0, c1); DROPn(2) )\
  Y(SPIFFSfilesize, PUSH filesize(SPIFFS,c0); NIP ) \
  Y(SPIFFSdeletef,  deleteFile(SPIFFS, c0); DROP ) \
  Y(SPIFFSrenamef, renameFile(SPIFFS, c1, c0); DROPn(2) ) \
  Y(SDdir, listDir(SD, "/ESP32Forth", 10); ) \
  Y(SDreadf,  readFile(SD, c0); DROP ) \
  X("SD.dir", sd_dir, listDir(SD, "/", 10); ) \
  X("SD.ldir", sd_ldir, listDir(SD, c0, 10); DROP ) \
  X("SD.dir0", sd_dir0, listDir(SD, "/", 0); ) \
  X("SD.dir1", sd_dir1, listDir(SD, "/", n0); DROP ) \
  X("SD.dir2", sd_dir2, n0 = LDir(SD, "/", n0, c1); NIP ) \
  X("SDbegin", sd_begin, sdcardbeginc();) \
  X("SDusedBytes", SD_USED_BYTES2, PUSH SD.usedBytes())\
  X("SDCardSize", SD_CARD_SIZE, PUSH SD.cardSize())
/* 
   X("SD.dir2", sd_dir2, LDir(SD, "/ESP32Forth", n0, c1); DROPn(3) ) \

 -- END OF FILE --
*/