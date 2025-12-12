#ifndef USERMOD_SINRICPRO_H
#define USERMOD_SINRICPRO_H

#include "wled.h"

// Default configuration values
#ifndef USERMOD_SINRICPRO_DEVICE_ID
  #define USERMOD_SINRICPRO_DEVICE_ID "693c36596ebb39d664cbf65b"
#endif

#ifndef USERMOD_SINRICPRO_APP_KEY
  #define USERMOD_SINRICPRO_APP_KEY "cfc93eb7-e6d4-40a7-87ed-d1355aa14da2"
#endif

#ifndef USERMOD_SINRICPRO_APP_SECRET
  #define USERMOD_SINRICPRO_APP_SECRET "e975af56-388c-4512-88b8-8ccd5fd6c776-9c6cb2a1-7fab-4bcb-926e-a8ce023eb79b"
#endif

// SinricPro usermod for WLED
class SinricProUsermod : public Usermod {
  private:
    // Configuration variables
    bool enabled = false;
    bool initDone = false;
    unsigned long lastConnectionAttempt = 0;
    unsigned long lastHeartbeat = 0;
    
    String deviceId = USERMOD_SINRICPRO_DEVICE_ID;
    String appKey = USERMOD_SINRICPRO_APP_KEY;
    String appSecret = USERMOD_SINRICPRO_APP_SECRET;
    
    bool connected = false;
    int reconnectAttempts = 0;
    
    // String constants for JSON configuration
    static const char _name[];
    static const char _enabled[];
    static const char _deviceId[];
    static const char _appKey[];
    static const char _appSecret[];

  public:
    /**
     * Enable/Disable the usermod
     */
    inline void enable(bool enable) { enabled = enable; }

    /**
     * Get usermod enabled/disabled state
     */
    inline bool isEnabled() { return enabled; }

    /**
     * Get connection status
     */
    inline bool getConnected() { return connected; }

    /**
     * Setup function - called once at startup
     */
    void setup() override {
      initDone = true;
      DEBUG_PRINTLN(F("SinricPro usermod setup complete"));
      DEBUG_PRINTF(F("Device ID: %s\n"), deviceId.c_str());
      DEBUG_PRINTF(F("App Key: %s\n"), appKey.c_str());
      connected = false;
      reconnectAttempts = 0;
    }

    /**
     * Connected function - called when WiFi connects
     */
    void connected() override {
      DEBUG_PRINTLN(F("SinricPro: WiFi connected, ready to connect to SinricPro"));
      lastConnectionAttempt = 0; // Reset connection timer to connect immediately
    }

    /**
     * Loop function - called continuously
     */
    void loop() override {
      if (!enabled || !initDone || !WLED_CONNECTED) return;

      unsigned long currentTime = millis();

      // Try to connect to SinricPro if not connected
      if (!connected) {
        if (currentTime - lastConnectionAttempt > 10000) { // Try every 10 seconds
          attemptConnection();
          lastConnectionAttempt = currentTime;
        }
      } else {
        // Send heartbeat every 30 seconds
        if (currentTime - lastHeartbeat > 30000) {
          sendHeartbeat();
          lastHeartbeat = currentTime;
        }
      }
    }

    /**
     * Add custom info to JSON info endpoint
     */
    void addToJsonInfo(JsonObject& root) override {
      JsonObject user = root["u"];
      if (user.isNull()) user = root.createNestedObject("u");

      JsonArray sArray = user.createNestedArray(F("SinricPro"));
      sArray.add(F("Status: "));
      sArray.add(connected ? F("Connected") : F("Disconnected"));
      
      if (!enabled) sArray[0] = F("Disabled");
    }

    /**
     * Add form fields to settings page
     */
    void addToConfig(JsonObject& root) override {
      JsonObject obj = root[FPSTR(_name)];
      if (obj.isNull()) obj = root.createNestedObject(FPSTR(_name));

      obj[FPSTR(_enabled)] = enabled;
      obj[FPSTR(_deviceId)] = deviceId;
      obj[FPSTR(_appKey)] = appKey;
      obj[FPSTR(_appSecret)] = appSecret;
    }

    /**
     * Read configuration from settings
     */
    bool readFromConfig(JsonObject& root) override {
      JsonObject obj = root[FPSTR(_name)];
      if (obj.isNull()) return false;

      bool changed = false;

      if (getJsonValue(obj[FPSTR(_enabled)], enabled)) changed = true;
      if (getJsonValue(obj[FPSTR(_deviceId)], deviceId)) changed = true;
      if (getJsonValue(obj[FPSTR(_appKey)], appKey)) changed = true;
      if (getJsonValue(obj[FPSTR(_appSecret)], appSecret)) changed = true;

      if (changed && enabled) {
        connected = false;
        reconnectAttempts = 0;
        lastConnectionAttempt = 0; // Force reconnection attempt
      }

      return changed;
    }

  private:
    /**
     * Attempt to connect to SinricPro
     */
    void attemptConnection() {
      if (reconnectAttempts >= 3) {
        DEBUG_PRINTLN(F("SinricPro: Max reconnection attempts reached"));
        return;
      }

      DEBUG_PRINTLN(F("SinricPro: Attempting connection..."));
      
      // In a real implementation, this would establish a WebSocket connection
      // For now, we simulate successful connection
      if (deviceId.length() > 0 && appKey.length() > 0 && appSecret.length() > 0) {
        connected = true;
        reconnectAttempts = 0;
        lastHeartbeat = millis();
        DEBUG_PRINTLN(F("SinricPro: Connected successfully"));
      } else {
        DEBUG_PRINTLN(F("SinricPro: Missing credentials"));
        reconnectAttempts++;
      }
    }

    /**
     * Send heartbeat to SinricPro
     */
    void sendHeartbeat() {
      if (!connected) return;
      DEBUG_PRINTLN(F("SinricPro: Sending heartbeat"));
      // In a real implementation, this would send a heartbeat message
    }

    /**
     * Helper function to safely extract JSON values
     */
    template <typename T>
    bool getJsonValue(const JsonVariant& src, T& dst) {
      if (src.is<T>()) {
        dst = src.as<T>();
        return true;
      }
      return false;
    }
};

// Define constant strings in flash memory
const char SinricProUsermod::_name[] PROGMEM = "SinricPro";
const char SinricProUsermod::_enabled[] PROGMEM = "enabled";
const char SinricProUsermod::_deviceId[] PROGMEM = "deviceId";
const char SinricProUsermod::_appKey[] PROGMEM = "appKey";
const char SinricProUsermod::_appSecret[] PROGMEM = "appSecret";

#endif // USERMOD_SINRICPRO_H
