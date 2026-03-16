#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...

// the font height for font size 1 is 8
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const unsigned char enitioLogo[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x40, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x60, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf1, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf2, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe4, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x88, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x90, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf8, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf9, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x80, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x41, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x80, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x80, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xfc, 0x00, 0x01, 0xfc, 0x9e, 0x3f, 0xff, 0xff, 0xe7, 0xf9, 0x3f, 0xff, 0xe0,
    0x1f, 0xff, 0xff, 0xff, 0x00, 0x03, 0xfd, 0xfe, 0xff, 0xff, 0xff, 0xef, 0xf1, 0xff, 0xff, 0xfc,
    0x7f, 0xff, 0xff, 0x9f, 0xc0, 0x03, 0xfd, 0xfe, 0xff, 0xff, 0xff, 0x9f, 0xe7, 0xff, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0x1f, 0xf0, 0x03, 0xfd, 0xfe, 0xff, 0xff, 0xff, 0x3f, 0xef, 0xff, 0xff, 0xff,
    0x7f, 0xff, 0xfc, 0x3f, 0xfc, 0x03, 0xfd, 0xfe, 0xff, 0xff, 0xfc, 0x7f, 0xcf, 0xff, 0xff, 0xff,
    0x7f, 0x00, 0x00, 0xbf, 0xff, 0x03, 0xfd, 0xfe, 0x40, 0x7f, 0x01, 0x7f, 0x9f, 0xe0, 0x00, 0x7f,
    0x7f, 0x10, 0x00, 0x1f, 0xff, 0x83, 0xfd, 0xfe, 0x00, 0xff, 0x40, 0xff, 0x9f, 0xe8, 0x01, 0x7f,
    0x7f, 0xff, 0xfc, 0x1f, 0xff, 0xe3, 0xfd, 0xfe, 0x00, 0xff, 0x40, 0xff, 0x2f, 0xe8, 0x01, 0x7f,
    0xff, 0xff, 0xf8, 0x9f, 0xff, 0xfb, 0xfd, 0xfe, 0x00, 0xff, 0x41, 0xfe, 0x47, 0xe8, 0x01, 0x7f,
    0xff, 0xff, 0xf8, 0x9f, 0xcf, 0xff, 0xfd, 0xfe, 0x00, 0xff, 0x43, 0xfc, 0x47, 0xe8, 0x01, 0x7f,
    0xff, 0x00, 0x00, 0x9f, 0xc7, 0xff, 0xfd, 0xfe, 0x00, 0xff, 0x43, 0xfc, 0x87, 0xe0, 0x01, 0x7f,
    0x7f, 0xff, 0xf8, 0x9f, 0xc5, 0xff, 0xfd, 0xfe, 0x00, 0xff, 0x47, 0xf9, 0x07, 0xf7, 0xff, 0x7f,
    0xff, 0xff, 0xfe, 0x9f, 0xc4, 0x7f, 0xfd, 0xfe, 0x40, 0xff, 0x4f, 0xf1, 0x07, 0xff, 0xff, 0xff,
    0x7f, 0xff, 0xff, 0xbf, 0xc4, 0x3f, 0xfd, 0xf8, 0x80, 0xff, 0x5f, 0xf2, 0x07, 0xff, 0xff, 0xff,
    0x3f, 0xff, 0xff, 0xff, 0xc4, 0x0f, 0xfd, 0xe3, 0x00, 0xff, 0x1f, 0xe4, 0x03, 0xff, 0xff, 0xf8,
    0x07, 0xff, 0xff, 0xff, 0xc4, 0x03, 0xfd, 0x84, 0x00, 0xff, 0x7f, 0xc4, 0x00, 0xff, 0xff, 0xe4,
    0x00, 0x00, 0x00, 0x08, 0x0c, 0x00, 0xfc, 0x90, 0x00, 0xff, 0x7f, 0xc8, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x80, 0x00, 0xfe, 0xff, 0x90, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x80, 0x00, 0xfc, 0xff, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0xf3, 0xff, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc7, 0xfe, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3f, 0x00, 0xfc, 0x07, 0x60, 0x38, 0x13, 0xfc, 0x9f, 0xc0, 0xee, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x37, 0x00, 0xd8, 0x07, 0x60, 0x18, 0x07, 0xf8, 0x96, 0x80, 0x6e, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0c, 0x01, 0xf0, 0x07, 0xe0, 0x18, 0x0f, 0xf9, 0x06, 0x00, 0xfc, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x39, 0x00, 0xcc, 0x06, 0xe0, 0x18, 0x07, 0xf2, 0x06, 0x00, 0xec, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3f, 0x80, 0xfc, 0x06, 0x60, 0x38, 0x07, 0xc6, 0x07, 0x00, 0xee, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// down nav arrow, 13x4px
const unsigned char downNavArrow[] PROGMEM = {
    0xff, 0xf8, 0x3f, 0xe0, 0x0f, 0x80, 0x02, 0x00};

const int SetGLFunction = 0;
const int FactoryResetFunction = 1;
const int ToggleServerConnectivityFunction = 2;
const int OverwritePlayerIDFunction = 3;
const int CheckScore = 4;
const int setGameEnded = 5;
const int ExitFunction = 6;

void StartUpDisplay()  //ENITIO logo screen
{
  display.clearDisplay();
  display.setTextSize(1); //draw size

  display.setTextColor(SSD1306_WHITE);

  display.drawBitmap(0, 4, enitioLogo, 128, 44, WHITE);

  display.setCursor(0, 56);
  display.println("   Please wait ...   ");
  display.display();
}

void CreditsDisplay(const char *const nameList[], int start_index, int end_index, bool isLastPage)  //credits screen
{
  display.clearDisplay();
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //draw 'inverse' text
  display.setCursor(0, 0);
  display.println(F("   ENITIO Tech Team  "));

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1); //draw size

  int name_y_coord = 10;
  for (int i = start_index; i <= end_index; ++i)
  {
    display.setCursor(0, name_y_coord);
    display.println(nameList[i]);
    name_y_coord += 9;
  }

  if (isLastPage)
  {
    display.setCursor(10, 56);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.println("(Press to return)");
  }
  else
  {
    display.drawBitmap(58, 58, downNavArrow, 13, 4, WHITE);
  }

  display.display();
}

void MACAddressDisplay()  //mac address screen
{
  display.clearDisplay();
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //draw 'inverse' text
  display.setCursor(0, 0);
  display.println(F(" Device MAC Address "));
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1); //draw size
  display.setCursor(0, 22);
  // char mac_str[18];
  // sprintf(mac_str, "MAC: %02X:%02X:%02X:%02X:%02X:%02X", my_MAC_address[0], my_MAC_address[1], my_MAC_address[2], my_MAC_address[3], my_MAC_address[4], my_MAC_address[5]);
  display.println(my_MAC_address);
  display.setCursor(10, 56);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.println("(Press to return)");
  display.display();
}

class Admin_OLED
{
private:
public:
  void WrongPwdDisplay()  //wrong password screen
  {
    display.clearDisplay();
    display.setTextSize(1); //draw size

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 28);

    display.println("    Wrong Password   ");
    display.display();
  }

  void RejectRegisterGLDisplay()  //rej register screen
  {
    display.clearDisplay();
    display.setTextSize(1); //draw size

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 22);

    display.println("Unable to Register GL");

    display.setCursor(0, 34);
    display.println("  after game started ");
    display.display();
  }

  void ConfirmSettingGLDisplay(int role)  //choose role
  {
    display.clearDisplay();
    display.setTextSize(1); //draw size

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 12);

    display.println("    Registered as:     ");

    display.setCursor(0, 32);
    switch (role)
    {
    case freshman:
      display.println("       Freshman      ");
      break;

    case groupLeader:
      display.println("     Group Leader    ");
      break;

    case sigma:
      display.println("        SIGMA       ");

    default:
      break;
    }

    display.display();
  }

  void ConfirmPlayerIDDisplay(int clan, int currentIDnum)  //choose clan
  {
    display.clearDisplay();
    display.setTextSize(1); //draw size

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 20);
    display.println("    Registered as   ");

    display.setCursor(0, 30);
    switch (clan)
    {
    case INVICTA:
      display.println("    CLAN: Invicta   ");
      break;
    case DYNARI:
      display.println("    CLAN: Dynari    ");
      break;
    case EPHILIA:
      display.println("    CLAN: Ephilia   ");
      break;
    case AKRONA:
      display.println("    CLAN: Akrona    ");
      break;
    case SOLARIS:
      display.println("    CLAN: Solaris   ");
      break;
    }

    display.setCursor(0, 40);
    display.printf("        ID %d      ", currentIDnum);

    display.display();
  }

  void ConfirmConnectivityToggle(int connectivityState)  //choose online/offline mode
  {
    display.clearDisplay();
    display.setTextSize(1); //draw size

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 22);

    display.println(" Set WANDERER Mode: ");

    display.setCursor(0, 34);
    if (connectivityState)
      display.println("       Online       ");
    else
      display.println("       Offline      ");
    display.display();
  }

  void display_gameEnded(int gameEndedState)  
  {
    display.clearDisplay();
    display.setTextSize(1); //draw size

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 22);
    Serial.println(gameEndedState);
    display.println(" Set Game State: ");

    display.setCursor(0, 34);
    if (gameEndedState)
      display.println("       Game Ended       ");
    else
      display.println("      Game Started      ");
    display.display();
  }

  void display_EnteringPwd(int currentDigitIndex, int currentDigit, int enteringPwdNav, int *user_key_in_pwd)  // password
  {
    display.clearDisplay();

    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //draw 'inverse' text
    display.setCursor(0, 0);
    display.println(F("Please Enter Password"));

    int i;
    for (i = 0; i < 6; i++)
    {
      display.setCursor(5 + 15 * (i + 1), 28);
      if (currentDigitIndex < i)
      {
        display.setTextColor(SSD1306_WHITE);
        display.print("*");
      }
      else if (currentDigitIndex == i)
      {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        display.print(currentDigit);
      }
      else
      {
        display.setTextColor(SSD1306_WHITE);
        display.print(user_key_in_pwd[i]);
      }
    }

    display.setCursor(0, 56);

    switch (enteringPwdNav)  //password screen
    {
    case 0:
      display.setTextColor(SSD1306_WHITE);
      display.print(" ");
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print("Back to Main Menu");
      display.setTextColor(SSD1306_WHITE);
      display.print(" >");
      break;

    case 1:
      display.setTextColor(SSD1306_WHITE);
      display.print(" <      Enter      > ");
      break;

    case 2:
      display.setTextColor(SSD1306_WHITE);
      display.print(" <          ");
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print("Backspace");
      break;

    default:
      break;
    }

    display.display();
  }

  void display_MainAdmin(int FunctionNav)  //admin screen
  {
    display.clearDisplay();
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //draw 'inverse' text
    display.setCursor(0, 0);
    display.println(F("        Admin        "));

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 8);
    display.println("Register Role");
    display.setCursor(10, 16);
    display.println("Factory Reset");

    display.setCursor(10, 24);
    if (EEPROM.read(ONLINE_mode_add) == 0)  //currently offline
    {
      display.println("Set Online Mode");
    }
    else
      display.println("Set Offline Mode");

    display.setCursor(10, 32);
    display.println("Overwrite ID");

    display.setCursor(10, 40);
    display.println("Check Scores");

    display.setCursor(10, 48);
    if (EEPROM.read(gameEnd_add) == 0)
    {
      display.println("Game Start");
    }
    else
    {
      display.println("Game Ended");
    }

    display.setCursor(10, 56);
    display.println("Back to Main Menu");

    switch (FunctionNav)
    {
    case SetGLFunction:
      display.setCursor(2, 8);
      display.println(">");
      break;

    case FactoryResetFunction:
      display.setCursor(2, 16);
      display.println(">");
      break;

    case ToggleServerConnectivityFunction:
      display.setCursor(2, 24);
      display.println(">");
      break;

    case OverwritePlayerIDFunction:
      display.setCursor(2, 32);
      display.println(">");
      break;

    case CheckScore:
      display.setCursor(2, 40);
      display.println(">");
      break;

    case setGameEnded:
      display.setCursor(2, 48);
      display.println(">");
      break;

    case ExitFunction:
      display.setCursor(2, 56);
      display.println(">");
      break;

    default:
      break;
    }

    // display.setCursor(14, 56);
    // display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    // display.println("(Press to choose)");

    display.display();
  }

  void display_ConfirmingReset(int ConfirmingResetNav)  //reset screen
  {
    display.clearDisplay();
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //draw 'inverse' text
    display.setCursor(0, 0);
    display.println(F(" Confirm to Reset?   "));
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 12);
    display.println("NO");
    display.setCursor(10, 22);
    display.println("YES, CLEAR ID");
    display.setCursor(10, 32);
    display.println("YES, KEEP ID");

    switch (ConfirmingResetNav)
    {
    case 0:
      display.setCursor(2, 12);
      display.println(">");
      break;

    case 1:
      display.setCursor(2, 22);
      display.println(">");
      break;

    case 2:
      display.setCursor(2, 32);
      display.println(">");
      break;

    default:
      break;
    }

    display.setCursor(14, 56);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.println("(Press to choose)");

    display.display();
  }

  void display_SettingGL(int isGLNav)  //role screen
  {
    display.clearDisplay();
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //draw 'inverse' text
    display.setCursor(0, 0);
    display.println(F(" Choose your Role... "));
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 12);
    display.println("Freshman (default)");
    display.setCursor(10, 22);
    display.println("Group Leader");
    display.setCursor(10, 32);
    display.println("Sigma");

    switch (isGLNav)
    {
    case 0:
      display.setCursor(2, 12);
      display.println(">");
      break;

    case 1:
      display.setCursor(2, 22);
      display.println(">");
      break;

    case 2:
      display.setCursor(2, 32);
      display.println(">");
      break;

    default:
      break;
    }

    display.setCursor(14, 56);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.println("(Press to choose)");

    display.display();
  }

  void display_SettingID(int previousID, int currentIDnum)  //player id screen
  {
    display.clearDisplay();
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //draw 'inverse' text
    display.setCursor(0, 0);
    display.println(F(" Setting Player ID "));
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 12);
    display.print("Old ID: ");
    display.println(previousID);

    display.setCursor(0, 22);
    display.print("New ID: ");
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.println(currentIDnum);

    display.setCursor(14, 56);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.println("(Press to choose)");

    display.display();
  }

  void display_Checkingscore(int invicta_score, int dynari_score, int ephilia_score,
                             int akrona_score, int solaris_score)  //score screen
  {
    display.clearDisplay();
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //draw 'inverse' text
    display.setCursor(0, 0);
    display.println(F("     Clan Scores     "));

    display.setTextColor(SSD1306_WHITE); //draw 'inverse' text
    display.setCursor(5, 10);
    display.print("Invicta: "), display.println(invicta_score);
    display.setCursor(5, 19);
    display.print("Dynari: "), display.println(dynari_score);
    display.setCursor(5, 28);
    display.print("Ephilia: "), display.println(ephilia_score);
    display.setCursor(5, 37);
    display.print("Akrona: "), display.println(akrona_score);
    display.setCursor(5, 46);
    display.print("Solaris: "), display.println(solaris_score);

    display.setCursor(10, 55);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.println("(Press to return)");
    display.display();
  }
};

Admin_OLED Admin_OLED;

class MainMenu_OLED
{
private:
public:
  void display_menu(int ProcessNav)  //main menu
  {
    display.clearDisplay();
    display.setTextSize(1); //draw size

    display.setTextColor(SSD1306_WHITE);

    display.drawBitmap(0, 4, enitioLogo, 128, 44, WHITE);

    switch (ProcessNav)
    {
    case MainMenuProcess:
      display.setCursor(26, 56);
      display.println(F("< Main Menu >"));
      break;

    case ProfileProcess:
      display.setCursor(34, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Profile"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case TreasureHuntProcess:
      display.setCursor(15, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Treasure Hunt"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case AdminProcess:
      display.setCursor(40, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Admin"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case CreditProcess:
      display.setCursor(37, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Credit"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case MACAddressProcess:
      display.setCursor(17, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("MAC Address"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;
    }
    display.display();
  }
};

MainMenu_OLED MainMenu_OLED;

class Profile_OLED
{
private:
public:
  void display_CompleteProfilePage(int CLAN, int role)  //profile screen
  {
    display.clearDisplay();
    display.setTextSize(1);                             //draw size
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //draw 'inverse' text
    display.setCursor(0, 0);
    display.println(F("     Your Profile    "));

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 12);
    display.println("ENITIO 2025");

    display.setCursor(0, 24);

    switch (role)
    {
    case freshman:
      display.println("Freshman");
      break;

    case groupLeader:
      display.println("Group Leader");
      break;

    case sigma:
      display.println("SIGMA");

    default:
      break;
    }

    display.setCursor(0, 36);
    switch (CLAN)
    {
    case INVICTA:
      display.println("CLAN: Invicta ");
      break;
    case DYNARI:
      display.println("CLAN: Dynari ");
      break;
    case EPHILIA:
      display.println("CLAN: Ephilia ");
      break;
    case AKRONA:
      display.println("CLAN: Akrona ");
      break;
    case SOLARIS:
      display.println("CLAN: Solaris ");
      break;
    }

    display.setCursor(10, 56);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.println("(Press to return)");
    display.display();
  }

  void display_CLANregisteringPage(int CLAN_pointer)  //register clan screen (after reset)
  {
    display.clearDisplay();
    // change text display here
    display.setTextSize(1);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);

    display.setCursor(0, 0);
    // menu title
    display.println(" Choose your CLAN... ");
    //---------------------------------

    display.setTextColor(SSD1306_WHITE);

    display.setCursor(10, 10);

    display.println("INVICTA");

    display.setCursor(10, 19);

    display.println("DYNARI");

    display.setCursor(10, 28);

    display.println("EPHILIA");

    display.setCursor(10, 37);

    display.println("AKRONA");

    display.setCursor(10, 46);

    display.println("SOLARIS");

    // prints the cursor to highlight menu items
    display.setCursor(2, (CLAN_pointer * 9) + 10);
    display.println(">");

    display.setCursor(14, 56);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.println("(Press to choose)");

    display.display();
  }

  void interim_registering_display()
  {
    display.clearDisplay();
    display.setTextSize(1);                             //draw size
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //draw 'inverse' text
    display.setCursor(0, 0);
    display.println(F("     Your Profile    "));

    display.setCursor(0, 28);
    display.setTextColor(SSD1306_WHITE);
    display.println(F("   Please wait ...   "));
    display.display();
  }
};

Profile_OLED Profile_OLED;

//main treasure hunt
const int mainPage = 0;
const int treasurePage = 1;
const int powerupPage = 2;
const int infoPage = 3;
const int statsPage = 4; // for k/d
const int scorePage = 5; // for shards
const int exitPage = 6;
const int transferPage = 7; // choose stones to transfer
const int mergePage = 8; // merge stones

class TreasureHunt_OLED
{
private:
  String lastNoti;


public:
  void display_mainPage(int HP, int MaxHP, int EN, int MaxEN, String noti, int pageNav, int numDeath, int numKilled)  //main page
  {
    display.clearDisplay();
    display.setTextSize(1); //draw size
    if (noti.length() == 0) //no noti
    {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //draw 'inverse' text
      display.setCursor(0, 0);
      display.println(F("  Treasure Hunt Game "));
    }
    else  //noti
    {
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println(noti);
    }

    display.setCursor(0, 12);
    display.setTextColor(SSD1306_WHITE); //draw white text

    display.setTextSize(2);

  if (HP>0)
  {
    if (HP <= 12 && MaxHP <= 12 )  //not max HP
    {
      display.println("HP ");
      for (int i = 0; i < HP; i++)
      {
        display.fillRect(34 + i * 8, 12, 4, 16, SSD1306_WHITE);
      }
    }
    else  //max HP
    {
      display.printf("HP %d", HP);
      display.setTextSize(1);
      display.setCursor(60, 19);
      display.printf("/%d", MaxHP);
    }

    display.setCursor(0, 32);

    display.setTextSize(2);

    if (EN <= 12 && MaxEN <= 12)  //not max EN
    {
      display.println("EN ");
      for (int i = 0; i < EN; i++)
      {
        display.fillRect(34 + i * 8, 32, 4, 16, SSD1306_WHITE);
      }
    }
    else  //max EN
    {
      display.setTextSize(2);
      display.printf("EN %d", EN);
      display.setTextSize(1);
      display.setCursor(60, 39);
      display.printf("/%d", MaxEN);
    }
  }

  else
  {
    // Death Page
    display.clearDisplay();
    display.setTextSize(1); //draw size
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(noti);

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 16);
    display.println("   You died!  ");
    display.setCursor(0, 28);
    display.printf("   Kills:%d", numKilled);
    display.setCursor(0, 40);
    display.printf("   Death:%d", numDeath);
  }
    display.setTextSize(1);
  

    switch (pageNav)
    {
    case mainPage:
      display.setCursor(44, 56);
      display.println(F("< Main >"));
      break;

    case treasurePage:
      display.setCursor(37, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Shards"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case powerupPage:
      display.setCursor(37, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Stones"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case infoPage:
      display.setCursor(44, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Info"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case statsPage:
      display.setCursor(40, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Stats"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;      

    case scorePage:
      display.setCursor(40, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Score"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case exitPage:
      display.setCursor(44, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Exit"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;
    }
    display.display();

  }

  void display_noClanPage()  //screen after reset (if directly go to treasurehunt)
  {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(20, 12);
    display.print(F("NO CLAN"));
    display.setTextSize(1);
    display.setCursor(8, 32);
    display.print(F("Set it in <Profile>"));

    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.setCursor(10, 56);
    display.println("(Press to return)");
    display.display();
  }
  
  void display_treasurePage(int numL1Treasure, String noti, int pageNav)  //treasure page
  {
    display.clearDisplay();
    display.setTextSize(1); //draw size
    if (noti.length() == 0) //no noti
    {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //draw 'inverse' text
      display.setCursor(0, 0);
      display.println(F("  Treasure Hunt Game "));
    }
    else //noti
    {
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println(noti);
    }

    display.setCursor(0, 10);
    display.setTextSize(1);              //normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); //draw white text

    display.setCursor(20, 16);
    display.print("You are holding");
    display.setCursor(60, 28);
    display.print(numL1Treasure);
    display.setCursor(34, 40);
    display.print("  Shards");

    switch (pageNav)
    {
    case mainPage:
      display.setCursor(44, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Main"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case treasurePage:
      display.setCursor(37, 56);
      display.print(F("< Shards >"));
      break;

    case powerupPage:
      display.setCursor(37, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Stones"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case infoPage:
      display.setCursor(44, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Info"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case statsPage:
      display.setCursor(40, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Stats"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;      

    case scorePage:
      display.setCursor(40, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Score"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case exitPage:
      display.setCursor(44, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Exit"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;
    }
    display.display();
  }

  #include "ENITIO_enums.h"
  void display_powerupPage(int num_blue,
                           int num_yellow,
                           int num_red,
                           int num_purple,
                           int num_green,
                           int num_orange,
                           int num_bomb,
                           Bomb_State bomb_state,
                           String noti,
                           int pageNav,
                           int PowerUpNav,
                           bool blueActive, bool yellowActive, bool redActive, bool purpleActive, bool greenActive, bool orangeActive
                           ) //powerup page
  {
    display.clearDisplay();
    display.setTextSize(1); //draw size
    if (noti.length() == 0)
    {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //draw 'inverse' text
      display.setCursor(0, 0);
      display.println(F("  Treasure Hunt Game "));
    }
    else
    {
      display.setTextColor(SSD1306_WHITE); //draw 'inverse' text
      display.setCursor(0, 0);
      display.println(noti);
    }

    display.setCursor(0, 10);
    display.setTextSize(1);              //normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); //draw white text

    switch (pageNav)
    {
    case mainPage:
      display.setCursor(44, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Main"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case treasurePage:
      display.setCursor(37, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Shards"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case powerupPage:
      display.setCursor(37, 56);
      display.print(F("< Stones >"));
      break;

    case infoPage:
      display.setCursor(44, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Info"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case statsPage:
      display.setCursor(40, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Stats"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;      

    case scorePage:
      display.setCursor(40, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Score"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case exitPage:
      display.setCursor(44, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Exit"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;
    }

    switch (PowerUpNav)  //diff infinity stones
    {
    case blue:
      display.setCursor(0,12);
      display.println("   2x Shard Points");
      display.setCursor(36,24);
      display.println("Blue Stone");
      display.setCursor(0, 40);
      display.print((" You have "));
      display.print(num_blue);
      display.println((" of this!"));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setCursor(0, 56);
      if(blueActive)
        display.println("      Activated      ");
      else if (!blueActive && num_blue > 0)
        display.println("   (Press to use!!)    ");
      else
        display.println("   (Return to Game)    ");
      break;

    case yellow:
      display.setCursor(0,12);
      display.println(" Invincible for 15s");
      display.setCursor(28,24);
      display.println("Yellow Stone");
      display.setCursor(0, 40);
      display.print((" You have "));
      display.print(num_yellow);
      display.println((" of this!"));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setCursor(0, 56);
      if(yellowActive)
        display.println("      Activated      ");
      else if (!yellowActive && num_yellow > 0)
        display.println("   (Press to use!!)    ");
      else
        display.println("   (Return to Game)    ");
      break;

    case red:
      display.setCursor(0,12);
      display.println("    Poison Damage");
      display.setCursor(36,24);
      display.println("Red Stone");
      display.setCursor(0, 40);
      display.print((" You have "));
      display.print(num_red);
      display.println((" of this!"));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setCursor(0, 56);
      if(redActive)
        display.println("      Activated      ");
      else if (!redActive && num_red > 0)
        display.println("   (Press to use!!)    ");
      else
        display.println("   (Return to Game)    ");
      break;

    case purple:
      display.setCursor(0,12);
      display.println("      2x Damage");
      display.setCursor(28,24);
      display.println("Purple Stone");
      display.setCursor(0, 40);
      display.print((" You have "));
      display.print(num_purple);
      display.println((" of this!"));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setCursor(0, 56);
      if(purpleActive)
        display.println("      Activated      ");
      else if (!purpleActive && num_purple > 0)
        display.println("   (Press to use!!)    ");
      else
        display.println("   (Return to Game)    ");
      break;

    case green:
      display.setCursor(0,12);
      display.println("   Regen to Max EN");
      display.setCursor(30,24);
      display.println("Green Stone");
      display.setCursor(0, 40);
      display.print((" You have "));
      display.print(num_green);
      display.println((" of this!"));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setCursor(0, 56);
      if (greenActive)
        display.println("      Activated      ");
      else if (!greenActive && num_green > 0)
        display.println("   (Press to use!!)    ");
      else
        display.println("   (Return to Game)    ");
      break;

    case orange:
      display.setCursor(0,12);
      display.println("     Auto Revive");
      display.setCursor(28,24);
      display.println("Orange Stone    ");
      display.setCursor(0, 40);
      display.print((" You have "));
      display.print(num_orange);
      display.println((" of this!"));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setCursor(0, 56);
      if (orangeActive)
        display.println("      Activated      ");
      else if (!orangeActive && num_orange > 0)
        display.println("   (Press to use!!)    ");
      else
        display.println("   (Return to Game)    ");
      break;

    case transferstone: // transfer stone
      display.setCursor(24, 12);
      display.println("Transfer Stone");
      display.setCursor(24, 24);
      display.println(" Send to ally");
      display.setCursor(0, 40);
      display.print(("  You have "));
      display.print(num_blue + num_yellow + num_red + num_purple + num_green + num_orange);
      display.println((" stones!"));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setCursor(0, 56);
      if ((num_blue + num_yellow + num_red + num_purple + num_green + num_orange) > 0)
        display.println(" (Press to go select)    ");
      else
        display.println("   (Return to Game)    ");
      break;

    case mergestone: // merge stone
      display.setCursor(24, 12);
      display.println(" Merge Stone");
      display.setCursor(20, 24);
      display.println("-1 each = bomb");
      display.setCursor(0, 40);
      display.print(("  You have "));
      display.print(num_bomb);
      display.println((" bombs!"));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setCursor(0, 56);
      if ((num_blue > 0) && (num_yellow > 0) && (num_red > 0) && (num_purple > 0) && (num_green > 0) && (num_orange > 0) && (num_bomb == 0) && (bomb_state == waiting))
      {
        display.println("   (Press to merge!)    ");
      }
      else
      {
        if (num_bomb > 0)
        {
          display.println("   (Press to use!!)    ");
        }
        else
        {
          display.println("   (Return to Game)    ");
        }
      }
      break;

    case 0:
      display.setCursor(10, 16);
      display.println(("Any Infinity Stones"));
      display.setCursor(34, 28);
      display.println(("Collected"));
      display.setCursor(22, 40);  
      display.println(("Will Appear Here"));
      break;

    default:
      break;
    }
    display.display();
  }

  void display_infoPage(int CLAN, int ID, int MULTIPLIER, int MaxEN, String noti, int pageNav)  //info page
  {
    display.clearDisplay();
    display.setTextSize(1); //draw size
    if (noti.length() == 0)
    {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //draw 'inverse' text
      display.setCursor(0, 0);
      display.println(F("  Treasure Hunt Game "));
    }
    else
    {
      display.setTextColor(SSD1306_WHITE); //draw 'inverse' text
      display.setCursor(0, 0);
      display.println(noti);
    }

    display.setCursor(0, 10);
    display.setTextSize(1);              //normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); //draw white text

    switch (CLAN)
    {
    case INVICTA:
      display.println("CLAN: Invicta ");
      break;
    case DYNARI:
      display.println("CLAN: Dynari ");
      break;
    case EPHILIA:
      display.println("CLAN: Ephilia ");
      break;
    case AKRONA:
      display.println("CLAN: Akrona ");
      break;
    case SOLARIS:
      display.println("CLAN: Solaris ");
      break;
    }

    display.setCursor(0, 20);
    display.print("ID: ");
    display.println(ID);

    display.setCursor(0, 30);
    display.print("MULTIPLIER: ");
    display.println(MULTIPLIER);

    display.setCursor(0, 40);
    display.print("Max EN: ");
    display.println(MaxEN);

    switch (pageNav)
    {
    case mainPage:
      display.setCursor(44, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Main"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case treasurePage:
      display.setCursor(37, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Shards"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case powerupPage:
      display.setCursor(37, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Stones"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case infoPage:
      display.setCursor(44, 56);
      display.println(F("< Info >"));
      break;

    case statsPage:
      display.setCursor(40, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Stats"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;      

    case scorePage:
      display.setCursor(40, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Score"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case exitPage:
      display.setCursor(44, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Exit"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;
    }
    display.display();
  }

  void display_statsPage(int numDeath, int numKilled, int scoreInvicta,
                                   int scoreDynari, int scoreEphilia, int scoreAkrona, int scoreSolaris, String noti, int pageNav)  //stats page
  {
    display.clearDisplay();
    display.setTextSize(1); // Draw SIZE
    if (noti.length() == 0) // no noti
    {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
      display.setCursor(0, 0);
      display.println(F("  Treasure Hunt Game "));
    }
    else // noti
    {
      display.setTextColor(SSD1306_WHITE); // Draw 'inverse' text
      display.setCursor(0, 0);
      display.println(noti);
    }

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(2, 13);
    display.printf("Kills:%d", numKilled);

    // death display adjustment
    if (numDeath >= 10)
    {
      display.setCursor(66, 13);
      display.printf("Death:%d", numDeath);
    }
    else
    {
      display.setCursor(72, 13);
      display.printf("Death:%d", numDeath);
    }

    display.setCursor(0, 29);
    display.print(" I   D   E   A   S   ");
    display.setCursor(0, 40);
    // killed by which clan
    display.printf(" %-3d %-3d %-3d %-3d %-3d\n", scoreInvicta, scoreDynari, scoreEphilia, scoreAkrona, scoreSolaris);

    // draw horizontal lines
    display.drawLine(2, 27, 122, 27, SSD1306_WHITE);
    display.drawLine(2, 38, 122, 38, SSD1306_WHITE);
    display.drawLine(2, 49, 122, 49, SSD1306_WHITE);

    // draw vertical lines
    int x_coord = 0;
    for (int i = 0; i <= 5; ++i)
    {
      x_coord = 2 + (24 * i);
      display.drawLine(x_coord, 27, x_coord, 49, SSD1306_WHITE); // display.drawLine(x_start, y_start, x_end, y_end, colour)
    }

    switch (pageNav)
    {
    case mainPage:
      display.setCursor(44, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Main"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case treasurePage:
      display.setCursor(37, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Shards"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case powerupPage:
      display.setCursor(37, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Stones"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case infoPage:
      display.setCursor(44, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Info"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case statsPage:
      display.setCursor(40, 56);
      display.println(F("< Stats >"));
      break;

    case scorePage:
      display.setCursor(40, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Score"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case exitPage:
      display.setCursor(44, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Exit"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;
    }

    display.display();
  }

  void display_scorePage(int num_col, int totalTreasure, String noti, int pageNav)  //score page
  {
    display.clearDisplay();
    display.setTextSize(1); // Draw SIZE
    if (noti.length() == 0) // no noti
    {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
      display.setCursor(0, 0);
      display.println(F("  Treasure Hunt Game "));
    }
    else // noti
    {
      display.setTextColor(SSD1306_WHITE); // Draw 'inverse' text
      display.setCursor(0, 0);
      display.println(noti);
    }

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 13);
    display.printf("Infinity Shards");

    display.setCursor(20, 26);
    display.printf("Collected:%d", num_col);

    display.setCursor(20, 38);
    display.printf("Deposited:%d", totalTreasure);

    switch (pageNav)
    {
    case mainPage:
      display.setCursor(44, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Main"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case treasurePage:
      display.setCursor(37, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Shards"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case powerupPage:
      display.setCursor(37, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Stones"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case infoPage:
      display.setCursor(44, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Info"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;

    case statsPage:
      display.setCursor(40, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Stats"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;      

    case scorePage:
      display.setCursor(40, 56);
      display.print(F("< Score >"));
      break;

    case exitPage:
      display.setCursor(44, 56);
      display.print(F("< "));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(F("Exit"));
      display.setTextColor(SSD1306_WHITE);
      display.print(F(" >"));
      break;
    }

    display.display();
  }

  void display_transferPage(int num_blue,
                          int num_yellow,
                          int num_red,
                          int num_purple,
                          int num_green,
                          int num_orange,
                          String noti,
                          int transferNav
                          )
  {
    display.clearDisplay();
    display.setTextSize(1); //draw size
    if (noti.length() == 0)
    {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //draw 'inverse' text
      display.setCursor(0, 0);
      display.println(F("  Treasure Hunt Game "));
    }
    else
    {
      display.setTextColor(SSD1306_WHITE); //draw 'inverse' text
      display.setCursor(0, 0);
      display.println(noti);
    }

    display.setCursor(0, 10);
    display.setTextSize(1);              //normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); //draw white text

    switch (transferNav)  //diff infinity stones
    { 
    case blue:
      display.setCursor(0,12);
      display.println("      Transfer  ");
      display.setCursor(32,24);
      display.println("Blue Stone");
      display.setCursor(0, 40);
      display.print((" You have "));
      display.print(num_blue);
      display.println((" of this!"));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setCursor(0, 56);
      display.println(" (Press to transfer!)    ");
      break;

    case yellow:
      display.setCursor(0,12);
      display.println("      Transfer  ");
      display.setCursor(28,24);
      display.println("Yellow Stone");
      display.setCursor(0, 40);
      display.print((" You have "));
      display.print(num_yellow);
      display.println((" of this!"));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setCursor(0, 56);
      display.println(" (Press to transfer!)    ");
      break;

    case red:
      display.setCursor(0,12);
      display.println("      Transfer  ");
      display.setCursor(34,24);
      display.println("Red Stone");
      display.setCursor(0, 40);
      display.print((" You have "));
      display.print(num_red);
      display.println((" of this!"));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setCursor(0, 56);
      display.println(" (Press to transfer!)    ");
      break;

    case purple:
      display.setCursor(0,12);
      display.println("      Transfer  ");
      display.setCursor(28,24);
      display.println("Purple Stone");
      display.setCursor(0, 40);
      display.print((" You have "));
      display.print(num_purple);
      display.println((" of this!"));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setCursor(0, 56);
      display.println(" (Press to transfer!)    ");
      break;

    case green:
      display.setCursor(0,12);
      display.println("      Transfer  ");
      display.setCursor(30,24);
      display.println("Green Stone");
      display.setCursor(0, 40);
      display.print((" You have "));
      display.print(num_green);
      display.println((" of this!"));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setCursor(0, 56);
      display.println(" (Press to transfer!)    ");
      break;

    case orange:
      display.setCursor(0,12);
      display.println("      Transfer  ");
      display.setCursor(28,24);
      display.println("Orange Stone    ");
      display.setCursor(0, 40);
      display.print((" You have "));
      display.print(num_orange);
      display.println((" of this!"));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setCursor(0, 56);
      display.println(" (Press to transfer!)    ");
      break;

    case 0:
      display.setCursor(10, 16);
      display.println(("Any Infinity Stones"));
      display.setCursor(10, 28);
      display.println((" To Be Transferred"));
      display.setCursor(20, 40);  
      display.println(("Will Appear Here"));
      display.setCursor(0, 56);
      display.println(" < Stone Selection > ");
      break;

    default:
      break;
    }

    display.display();
  }

  // void display_mergePage(int num_bomb, String noti)
  // {
  //   display.clearDisplay();
  //   display.setTextSize(1);
  //   if (noti.length() == 0)
  //   {
  //     display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //draw 'inverse' text
  //     display.setCursor(0, 0);
  //     display.println(F("  Treasure Hunt Game "));
  //   }
  //   else
  //   {
  //     display.setTextColor(SSD1306_WHITE); //draw 'inverse' text
  //     display.setCursor(0, 0);
  //     display.println(noti);
  //   }

  //   display.setTextColor(SSD1306_WHITE);
  //   display.setCursor(20, 16);
  //   display.print("You are holding");
  //   display.setCursor(60, 28);
  //   display.print(num_bomb);
  //   display.setCursor(34, 40);
  //   display.print("  Bombs");

  //   display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  //   display.setCursor(0, 56);
  //   display.println("   (Press to use!!)    ");
  //   display.display();
  // }


  void display_WaitingPage() // waiting page
  {
    display.clearDisplay();
    display.setTextSize(1); //draw SIZE

    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //draw 'inverse' text
    display.setCursor(0, 0);
    display.println(F("  Treasure Hunt Game "));

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 24);
    display.println("   The game has not  ");
    display.println("      started yet    ");

    display.setCursor(10, 56);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.println("(Press to return)");
    display.display();
  }
};

TreasureHunt_OLED TreasureHunt_OLED;
