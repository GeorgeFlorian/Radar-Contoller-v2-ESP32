#include <state/state.h>
#include <network_connection.h>
#include <esp_backend.h>
#include <radar_coms.h>

void checkResetBtn()
{
  int q = 0;
  if (digitalRead(RST_BTN) == LOW)
  {
    q++;
    delay(2000);
    if (digitalRead(RST_BTN) == LOW)
    {
      q++;
    }
  }

  if (q == 2)
  {
    logOutput("WARNING: Reset button was pressed !");
    StaticJsonDocument<1024> json = factoryReset();
    if (writeJSONtoFile(json))
    {
      restartSequence(2);
    }
    else
    {
      logOutput("ERROR: Could not factory reset. Please restart the device and try again !");
    }
  }

  q = 0;
}

void setup()
{
  Serial.begin(115200);
  if (!SPIFFS.begin(true))
  {
    Serial.println(F("An Error has occurred while mounting SPIFFS ! Formatting in progress"));
    return;
  }

  setPins();

  checkResetBtn();

  // Read settings from /config.json and update live state
  if (!initializeState())
  {
    logOutput("ERROR: Could not get start-up configuration. Restarting...");
    restartSequence(5);
  }
  // Initialize radar
  sendToRadar();
  // Connect to a network
  startConnection();
  // Start back-end server
  startEspServer();

  Serial.print("WiFi MAC - ");
  Serial.println(WiFi.macAddress());
  Serial.print("ETH MAC - ");
  Serial.println(ETH.macAddress());

  USE_SERIAL1.write("SET output1HoldTime 500\r\n");
}

void loop()
{
  delay(1);
  if (changed_network_config)
  {
    logOutput("Changed NETWORK configuration. Restarting...");
    restartSequence(3);
  }
  if (restart_flag)
  {
    Serial.println("Changed other configuration. Restarting...");
    restartSequence(3);
  }

  radarRoutine();
}