#include <M5Stack.h>

#include <Screen.h>
#include <ButtonInfoBar.h>
#include <StatusBar.h>
#include <Button.h>
#include <WidgetMosaicComp.h>
#include <string>
#include <time.h>
#include <sys/time.h>
#include <AppScreen.h>
#include <UpDownButton.h>

#include <Logscreen.h>

#include <WiFi.h>
#include <NTPClient.h>

#include "secrets/secrets.h"

bool iniNTP();
static void InitWifi();
static bool isWifiOK();
void debug(String text);

String logText; // Text to be displayed inside M5Stack log widget

const char* ssid     = SSID;
const char* password = SSIDPASS;

// NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// UI declaration

using namespace Codingfield::UI;
AppScreen* screen;
StatusBar* topBar;
ButtonInfoBar* bottomBar;

Codingfield::UI::Button* button0;
Codingfield::UI::Button* button1;
Codingfield::UI::Button* button2;
Codingfield::UI::UpDownButton* set0;
Codingfield::UI::Logscreen* logButt;
Codingfield::UI::UpDownButton* set1;

Codingfield::UI::Button* button02;
Codingfield::UI::Button* button12;
Codingfield::UI::Button* button03;
Codingfield::UI::Button* button13;

WidgetMosaicComp* mosaic;
WidgetMosaicComp* mosaic2;
WidgetMosaicComp* mosaic3;

Widget* focus;

// Application variables to be displayed in GUI

int32_t val0 = 50;
int32_t val1 = 3;
std::string msgSent = "OK";

int32_t inp0 = 0;
int32_t valLog = 0;
int32_t inp1 = 0;


void setupGUI();

void setup() {

    Serial.begin(115200);

    InitWifi();
    iniNTP();

    M5.begin();

    setupGUI();
}

int32_t uptimeMinutes=0;
unsigned long seconds = 0;
unsigned long lastUpdate=0;

void loop() {

    M5.update();
    
    if ((millis() - lastUpdate) > 1000 ){

        seconds++;

        val0 = (val0+random(-2,2)) % 50;
        if (val0<=0) val0=100;
        if (val0>200) val0=100;

        debug(String(val0));
        
        uptimeMinutes = millis() / (60 * 1000);
        topBar->SetUptime(uptimeMinutes);

        topBar->SetDateTime(timeClient.getFormattedTime().c_str());

        if((seconds % 10) == 0) {
            auto rssi = WiFi.RSSI();

            debug("Wifi Sign: " + String(rssi));

            if(rssi >= -55) {
            topBar->SetWifiStatus(StatusBar::WifiStatuses::Full);
            } else if(rssi >= -75) {
            topBar->SetWifiStatus(StatusBar::WifiStatuses::Medium);
            } else if(rssi >= -85) {
            topBar->SetWifiStatus(StatusBar::WifiStatuses::Weak);
            } else {
            topBar->SetWifiStatus(StatusBar::WifiStatuses::No_signal);
            }
        }
        
        lastUpdate = millis();
    }

    screen->Draw(focus);
}


void setupGUI() {

    // TOP BAR ///////////

    topBar = new StatusBar();
    topBar->SetUptime(0);
    topBar->SetWifiStatus(StatusBar::WifiStatuses::No_signal);

    // BOTTOM BAR ///////////

    bottomBar = new ButtonInfoBar();
    bottomBar->SetButtonAText("<");
    bottomBar->SetButtonBText("SELECT");
    bottomBar->SetButtonCText(">");

    // MAIN MOSAIC ///////////

    mosaic = new WidgetMosaicComp(nullptr, 3, 2, topBar, bottomBar, &val0);
    mosaic->SetBackgroundColor(BLUE);
    mosaic->SetTextColor(WHITE);
    mosaic->SetTitle("Main Mosaic");
    mosaic->SetTextSize(2);

    // APP INITIALITZATION ///////////

    screen = new AppScreen(Size(320, 240), BLACK, topBar, bottomBar, mosaic);


    // MAIN MOSAIC WIDGETS (3x2=6) ///////////

    logButt = new Codingfield::UI::Logscreen(mosaic, &valLog, &logText); 
    logButt->SetBackgroundColor(GREEN);
    logButt->SetTextColor(BLACK);
    logButt->SetTitle("Logg");

    button0 = new Codingfield::UI::Button(mosaic, &val0, &seconds);
    button0->SetBackgroundColor(BLUE);
    button0->SetTextColor(WHITE);
    button0->SetTitle("Graph 0");

    button1 = new Codingfield::UI::Button(mosaic, &val1);
    button1->SetBackgroundColor(BLUE);
    button1->SetTextColor(WHITE);
    button1->SetTitle("Value1");

    button2 = new Codingfield::UI::Button(mosaic);
    button2->SetBackgroundColor(GREEN);
    button2->SetTextColor(WHITE);
    button2->SetTitle("SentMsg");
    button2->SetText(msgSent);

    mosaic2 = new WidgetMosaicComp(mosaic, 3, 1, topBar, bottomBar);
    mosaic2->SetBackgroundColor(YELLOW);
    mosaic2->SetTextColor(BLACK);
    mosaic2->SetTitle("Mosaic 2");
    
    set0 = new Codingfield::UI::UpDownButton(mosaic, &inp0); // Up/Down button
    set0->SetBackgroundColor(YELLOW);
    set0->SetTextColor(BLACK);
    set0->SetUnit("V");
    set0->SetTitle("Puls3");


    // MOSAIC 2 WIDGETS (3x1 = 3) ///////////

    button02 = new Codingfield::UI::Button(mosaic2, &val0, &seconds);
    button02->SetBackgroundColor(BLUE);
    button02->SetTextColor(WHITE);
    button02->SetTextSize(1);
    button02->SetTitle("But0-M2");
    
    button12 = new Codingfield::UI::Button(mosaic2, &val1, &seconds);
    button12->SetBackgroundColor(BLUE);
    button12->SetTextColor(WHITE);
    button12->SetTextSize(1);
    button12->SetTitle("But1-M2");

    mosaic3 = new WidgetMosaicComp(mosaic2, 3, 1, topBar, bottomBar, &val0);
    mosaic3->SetBackgroundColor(YELLOW);
    mosaic3->SetTextColor(BLACK);
    mosaic3->SetTitle("Mosaic 3");


    // MOSAIC 3 WIDGETS (3x1 = 3) ///////////

    button03 = new Codingfield::UI::Button(mosaic3, &val0);
    button03->SetBackgroundColor(RED);
    button03->SetTextColor(WHITE);
    button03->SetTextSize(1);
    button03->SetTitle("But 0-M3");

    button13 = new Codingfield::UI::Button(mosaic3, &val1);
    button13->SetBackgroundColor(BLACK);
    button13->SetTextColor(WHITE);
    button13->SetTextSize(1);
    button13->SetTitle("But 1-M3");


    // Give the focus to the main screen
    focus = mosaic;

    // Draw the screen and all its children
    screen->Draw();
}


void debug (String text) {
    String textDebug = String(uptimeMinutes) + " min: " + text;
    Serial.println(textDebug);
    logText = textDebug;
}


bool iniNTP(){

    timeClient.begin();
    // To machine advisor, should be sent UCT+0 to be able to sync different machines world wide.
    timeClient.setUpdateInterval(1000*60*100); // update time every 100 minutes

    bool actualitzada;
    while(true) {
        actualitzada = timeClient.update();
        if (!actualitzada) {
            timeClient.forceUpdate();
            debug("t");
        } else break;
    }
    debug("Connected to NTP. Time = " + timeClient.getFormattedTime());
    
    return(actualitzada);
}


static void InitWifi() {
    debug(" > WiFi");
    debug("Connecting...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    debug("WiFi connected");
    debug("IP address: ");
    debug(String(WiFi.localIP()));
}

static bool isWifiOK(){
    return (WiFi.status() == WL_CONNECTED);
}

