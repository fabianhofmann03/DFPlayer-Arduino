#include "dfplayerarduino.h"
extern "C" {
#include <dfplayercore.h>
}

dfplayer::dfplayer(HardwareSerial *new_stream) {
    this->hardws = true;
    this->stream = new_stream;
}

dfplayer::dfplayer(SoftwareSerial *new_stream) {
    this->hardws = false;
    this->stream = new_stream;
}

/**
 * Starts the Serial port of the dfplayer object.
 */
void dfplayer::setup() {
    if (this->hardws) {
        static_cast<HardwareSerial*>(this->stream)->begin(9600);
    } else {
        static_cast<SoftwareSerial*>(this->stream)->begin(9600);
    }
}

/**
 * Set a function that will be called every time the module sends a response. The function will not be called if the checksum is incorrect.
 * 
 * @param ResponseHandler The function that will be called when a response is read. It must be a void and have following parameters: uint8_t version, uint8_t length, uint8_t command, uint8_t feedback, uint16_t parameters.
*/
void dfplayer::setResponseHandler(void (*respHand) (uint8_t vers, uint8_t len, uint8_t cmd, uint8_t feedb, uint16_t param)) {
    this->respHand = respHand;
}

/**
 * Remove the response handler.
*/
void dfplayer::removeResponseHandler() {
    this->respHand = NULL;
}

/**
 * Set the default response handler. The default response handler analyzes the command and parameters and prints a description in the normal Serial port.
*/
void dfplayer::setDefaultResponseHandler() {
    this->respHand = &(dfplayer::printResponse);
}

/**
 * Reads the responses from the module. If you don't add this function the response handler won't be called. If this function is used, DO NOT read the serial port you set for the module.
*/
void dfplayer::update() {
    while (stream->available()) {
        uint8_t data = (*stream).read();
        static uint8_t message_buffer[10] = {0,0,0,0,0,0,0,0,0,0};
        
        ::dfp_update(data, message_buffer, this->respHand);
    }
}

void dfplayer::sendCommand() {
    uint8_t* response = ::getResult();
    (*stream).write((char*)(response + 1), *response);
}

/**
 * Starts playing the next file.
 */
void dfplayer::playNext() {
    ::playNext();
    dfplayer::sendCommand();
}

/**
 * Starts playing the previouse file.
 */
void dfplayer::playPrev() {
    ::playPrev();
    dfplayer::sendCommand();
}

/**
 * Starts playing the specified track from the root directory.
 * 
 * @param Number Number of the track that should be played. Must be between 1 and 3000.
 */
void dfplayer::specifyTrack(uint16_t num) {
    ::specifyTrack(num);
    dfplayer::sendCommand();
}

/**
 * Increases the volume.
 */
void dfplayer::increaseVolume() {
    ::increaseVolume();
    dfplayer::sendCommand();
}

/**
 * Decreases the volume.
 */
void dfplayer::decreaseVolume() {
    ::decreaseVolume();
    dfplayer::sendCommand();
}

/**
 * Sets a specific volume.
 * 
 * @param Volume Volume with which the audio should be played back. 
 */
void dfplayer::specifyVolume(uint16_t vol) {
    ::specifyVolume(vol);
    dfplayer::sendCommand();
}

/**
 * Sets specific equalizer settings.
 * 
 * @param Equalizer Equalizer settings that should be used. Usable settings are Normal, Pop, Rock, Jazz, Classic, Bass.
 * @example player.specifyEQ(player.Bass);
 */
void dfplayer::specifyEQ(uint8_t eq) {
    ::specifyEQ(eq);
    dfplayer::sendCommand();
}

/**
 * Starts playing a specific song in loop.
 * 
 * @param Number Number of the song that should be played in loop.
 */
void dfplayer::specifySingleRepeat(uint16_t num) {
    ::specifySingleRepeat(num);
    dfplayer::sendCommand();
}

/**
 * Selects a device to play music from.
 * 
 * @param Device Device that should be used. Usable settings are SD and USB.
 * @example player.specifyDevice(player.USB);
 */
void dfplayer::specifyDevice(uint8_t device) {
    ::specifyDevice(device);
    dfplayer::sendCommand();
}

/**
 * Puts module in sleep mode.
 */
void dfplayer::sleep() {
    ::sleep();
    dfplayer::sendCommand();
}

/**
 * Resets ... something?
 */
void dfplayer::reset() {
    ::reset();
    dfplayer::sendCommand();
}

/**
 * Plays the current song.
 */
void dfplayer::play() {
    ::play();
    dfplayer::sendCommand();
}

/**
 * Pauses the current song.
 */
void dfplayer::pause() {
    ::pause();
    dfplayer::sendCommand();
}

/**
 * Plays a song in a certain folder.
 * 
 * @param Folder The folder number where the song is contained. Can range from 1 to 99.
 * @param Number The number of the song in the selected folder. Can range from 1 to 255.
 */
void dfplayer::specifyTackInFolder(uint8_t folder, uint8_t num) {
    ::specifyTackInFolder(folder, num);
    dfplayer::sendCommand();
}

/**
 * Sets the audio amplification settings.
 * 
 * @param On If the amplification should be turned on or not.
 * @param Gain For the gains 💪. Can range from 0 to 31.
 */
void dfplayer::setAudioAmp(bool on, uint8_t gain) {
    ::setAudioAmp(on, gain);
    dfplayer::sendCommand();
}

void dfplayer::setAudioAmp(bool on) {
    ::setAudioAmp(on, 0);
    dfplayer::sendCommand();
}

/**
 * Sets if all the songs in the root directory should repeat. Aka once the last song is finished the first starts again.
 * 
 * @param On If the root directory should repeat.
 */
void dfplayer::setAllRepeatPlayback(bool on) {
    ::setAllRepeatPlayback(on);
    dfplayer::sendCommand();
}

/**
 * Plays specific audio file in the MP3 directory. The MP3 directory allows 65536 audios to be stored, but its reccommended to store a max of 3000 tracks.
 * 
 * @param Number Number of the sound in the MP3 directory that should be played. Can range from 1 to 65536.
 */
void dfplayer::specifyTrackInMP3(uint16_t num) {
    ::specifyTrackInMP3(num);
    dfplayer::sendCommand();
}

/**
 * Pauses the current track and starts playing the selected track from the ADVERT folder. Once it's finished, it continues with the paused song.
 * 
 * @param Number Number of the sound in the ADVERT directory that should be played. Can range from 1 to 65536 ... I think.
 */
void dfplayer::insetAd(uint16_t num) {
    ::insetAd(num);
    dfplayer::sendCommand();
}

/**
 * Plays a specific song from a bigger folder. Allows to have fewer folders with more tracks.
 *
 * @param Folder The big-folder number where the song is contained. Can range from 1 to 15.
 * @param Number The number of the song in the selected big-folder. Can range from 1 to 3000.
 */
void dfplayer::specifyTackInBigFolder(uint8_t folder, uint16_t num) {
    ::specifyTackInBigFolder(folder, num);
    dfplayer::sendCommand();
}

/**
 * Stops the track from the ADVERT folder and continues with the paused track.
 */
void dfplayer::stopAd() {
    ::stopAd();
    dfplayer::sendCommand();
}

/**
 * Stops all playback and stops decoding the audio files.
 */
void dfplayer::stop() {
    ::stop();
    dfplayer::sendCommand();
}

/**
 * Repeats all songs in a certain folder. Aka once the last song is finished the first starts again.
 * 
 * @param Folder The folder number where the tracks are contained.
 */
void dfplayer::repeatSpecificFolder(uint8_t folder) {
    ::repeatSpecificFolder(folder);
    dfplayer::sendCommand();
}

/**
 * I don't get the explaination from the datasheet, but here it is: This command is used to randomly play sound files in the storage device according to physical sequence and no matter if there is a folder or not in the device. The first sound file that is conducted to be played is the first one in the device.
 */
void dfplayer::setRandom() {
    ::setRandom();
    dfplayer::sendCommand();
}

/**
 * Repeats the track thats currently playing.
 * 
 * @param On If the repeat-mode should be on.
 */
void dfplayer::repeatCurrent(bool on) {
    ::repeatCurrent(on);
    dfplayer::sendCommand();
}

/**
 * Turns the DAC (Digital-Analog-Converter) on or off.
 * 
 * @param On If the DAC should be on.
 */
void dfplayer::setDAC(bool on) {
    ::setDAC(on);
    dfplayer::sendCommand();
}

void dfplayer::printResponse(uint8_t vers, uint8_t len, uint8_t cmd, uint8_t feedb, uint16_t param) {
  switch (cmd) {
    case 0x3F:
      Serial.print("Module turned on. ");
      switch(param) {
        case 1:
          Serial.println("USB flash drive online.");
          break;
        case 2:
          Serial.println("SD card online");
          break;
        case 3:
          Serial.println("SD card and USB flash drive online.");
          break;
        case 4:
          Serial.println("PC online.");
          break;
      }
      break;
    
    case 0x3D:
      Serial.println("Track number " + String(param) + " finished playing from the SD card.");
      break;
    case 0x3C:
      Serial.println("Track number " + String(param) + " finished playing from the USB flash drive.");
      break;
    
    case 0x41:
      Serial.println("Acknowledge returned.");
      break;

    case 0x40:
      Serial.print("ERROR: ");
      switch(param) {
        case 1:
          Serial.println("Module is still initializing.");
          break;
        case 2:
          Serial.println("Module is in sleep mode.");
          break;
        case 3:
          Serial.println("A frame has not been received completly yet.");
          break;
        case 4:
          Serial.println("The checksum is incorrect.");
          break;
        case 5:
          Serial.println("The specified track is outside the current track scope.");
          break;
        case 6:
          Serial.println("The specified track was not found.");
          break;
        case 7:
          Serial.println("Insertion error.");
          break;
        case 8:
          Serial.println("SD card reading failed");
          break;
        case 10:
          Serial.println("Entered into sleepmode.");
      }
      break;

    case 0x3A:
      switch(param) {
        case 1:
          Serial.print("USB flash drive ");
          break;
        case 2:
          Serial.print("SD card ");
          break;
        case 3:
          Serial.print("USB cable connected to PC ");
      }
      Serial.println(" is plugged in.");
      break;
    case 0x3B:
      switch(param) {
        case 1:
          Serial.print("USB flash drive ");
          break;
        case 2:
          Serial.print("SD card ");
          break;
        case 3:
          Serial.print("USB cable connected to PC ");
      }
      Serial.println("is pulled out.");
  }
}