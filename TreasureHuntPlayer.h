#define ACTION_RECV_WAIT 150 // [ms]
#define TEMP_NOTI_WAIT 6000  //including being Attacked, Healed, Successfully Attack and Successfully Collect
#define NOTI_SOUND_DURATION 200
#define TEMP_NOTI_BLINKING 300  //blinking duration of temp noti
#define FEEDBACK_WAIT 5000  //feedback duration

#define LUCKY_NOT_INFECTED_DURATION 20000 // [ms]

#define x2_EN_Regen_bonus_duration 300000 // [ms]

// For treasure buzzer
#define BUZZ_INTERVAL 60  //interval between beeps in milliseconds
#define BEEP_CYCLES 5     //number of beep cycles
#define BEEPS_PER_CYCLE 4 //number of beeps per cycle

int game_started_buffer = 1;

uint8_t newMACAddress_AP[] = {4, 8, 1, 255, 255, 0};
uint8_t newMACAddress_STA[] = {4, 8, 1, 255, 255, 1};


class TreasureHuntPlayer
{
private:
  int ID;
  int CLAN;
  int ROLE;
  int HP;
  int MaxHP;
  int EN; // energy
  int MaxEN;
  int Multiplier;

  unsigned long last_hp_recover = 0;

  action_id action;
  Bomb_State bomb_state;

  //infinity stones not active yet
  bool blueActive = false;
  bool yellowActive = false;
  bool redActive = false;
  bool purpleActive = false;
  bool greenActive = false;
  bool orangeActive = false;

  //countdown timer not active yet
  bool isYellowCountdownActive = false;
  bool isGreenCountdownActive = false;
  bool isBombCountdownActive = false;

  unsigned long yellowStartTime = 0;
  unsigned long greenStartTime = 0;
  unsigned long bombStartTime = 0;

  const int YELLOW_DURATION = 15000; // 15 seconds in ms
  const int GREEN_DURATION = 15000; // 15 seconds in ms
  const int BOMB_DURATION = 120000; //2min

  // attacked by red stone
  bool isPoisoned = false;
  int poisonTicksRemaining = 0;
  unsigned long lastPoisonTick = 0;
  
  unsigned long lastActionReceived = 0;

  int currentPage = 0;
  int lastPageNav = 0;

  String tempNoti = "";
  String permNoti = "";

  unsigned long Nav_start = 0;
  unsigned long tempNoti_start = 0;
  unsigned long last_max_en_decay = 0;
  unsigned long last_en_recover = 0;
  unsigned long last_hp_decay = 0;
  unsigned long last_received_heal = 0;
  unsigned long last_lucky_not_infected = 0;
  unsigned long start_x2_en_regen = 0;
  unsigned long last_update_kills_to_server = 0;

  unsigned long start_receiving_feedback = 0;

  unsigned long previousMillis = 0;
  unsigned long interval = 0;

  int numL1Treasure = 0;
  int numL2Treasure = 0;
  int totalTreasure = 0;
  int currentTreasure = 0;
  int num_col = 0;
  // int num_dep = 0;

  // num of respective stones
  int num_blue = 1;
  int num_yellow = 1;
  int num_red = 1;
  int num_purple = 1;
  int num_green = 1;
  int num_orange = 1;
  int num_bomb = 1;

  bool is_x2EnRegen = false;

  int PowerUpNav = 0;
  bool choosingPowerUp = false;
  int transferNav = 0;
  int selectedStone = 0;
  bool choosingTransfer = false;
  bool isTransferred = false;
  bool isTransferPage = false;

  bool isInstantKilled = false;

  int temp_bomb_attacked = 0;
  int temp_bomb_killed = 0;

  bool onCooldown = false;
  unsigned long timeOfDeath = 0;

  bool ledIsOn = false;
  unsigned long ledActivationTime;
  unsigned long ledMaxActivationDuration = 3000;

  // for achievement & death page
  int scoreInvicta = 0;
  int scoreDynari = 0;
  int scoreEphilia = 0;
  int scoreAkrona = 0;
  int scoreSolaris = 0;
  int numKilled = 0;
  int numDeath = 0;

  // for treasure buzzer
  bool buzzerFirstRun = true;
  bool beeping = false;
  bool isBuzzerOn = false;
  int beepCount = 0;
  int beepCycleCount = 0;
  unsigned long randomLongPause = 0;
  unsigned long buzzerEndTime = 0;

public:
  bool gameStarted = 0;
  bool deviceReady = 0;

  void setup_initial_state(int id, int clan, int role)  //setup
  {
    ID = id;
    CLAN = clan;
    ROLE = role;

    newMACAddress_AP[3] = CLAN;
    newMACAddress_AP[4] = ID;

    newMACAddress_STA[3] = CLAN;
    newMACAddress_STA[4] = ID;

    esp_wifi_set_mac(WIFI_IF_AP, &newMACAddress_AP[0]);
    esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress_STA[0]);

    Serial.print("[NEW] ESP32 Board MAC Address:  ");
    Serial.println(WiFi.macAddress());

    if (EEPROM.read(PLAYER_enable_add) == 0)
    {
      Serial.print("Initializing EEPROM...");

      switch (ROLE)  //diff role HP & EN
      {
      case freshman:
        HP = PARTICIPANT_MaxHP;
        EN = PARTICIPANT_MaxEN;
        MaxHP = PARTICIPANT_MaxHP;
        MaxEN = PARTICIPANT_MaxEN;
        break;

      case groupLeader:
        HP = GL_MaxHP;
        EN = GL_MaxEN;
        MaxHP = GL_MaxHP;
        MaxEN = GL_MaxEN;
        break;

      case sigma:
        HP = SIGMA_MaxHP;
        EN = SIGMA_MaxEN;
        MaxHP = SIGMA_MaxHP;
        MaxEN = SIGMA_MaxEN;
        break;

      default:
        HP = 404;
        EN = 404;
        MaxHP = 404;
        MaxEN = 404;
        break;
      }
      Multiplier = INITIAL_MULTIPLIER; //multiplier 1

      EEPROM.write(PLAYER_enable_add, 1);
      EEPROM.write(PLAYER_HP_add, HP);
      EEPROM.write(PLAYER_EN_add, EN);
      EEPROM.write(PLAYER_MaxHP_add, MaxHP);
      EEPROM.write(PLAYER_MaxEN_add, MaxEN);
      EEPROM.write(PLAYER_MULTIPLIER_add, Multiplier);
      EEPROM.write(POINT_INVICTA_add, 0);
      EEPROM.write(POINT_DYNARI_add, 0);
      EEPROM.write(POINT_EPHILIA_add, 0);
      EEPROM.write(POINT_AKRONA_add, 0);
      EEPROM.write(POINT_SOLARIS_add, 0);
      EEPROM.write(PLAYER_totalTreasure_add, 0);
      EEPROM.write(PLAYER_numDeath_add, 0);
      EEPROM.write(PLAYER_num_bomb_add, 0);
      EEPROM.write(PLAYER_num_col_add, 0);
      // EEPROM.write(PLAYER_num_dep_add, 0);   
      EEPROM.commit();
    }

    else
    {
      Serial.println("Reading Player State from EEPROM...");
      HP = EEPROM.read(PLAYER_HP_add);
      EN = EEPROM.read(PLAYER_EN_add);
      MaxHP = EEPROM.read(PLAYER_MaxHP_add);
      MaxEN = EEPROM.read(PLAYER_MaxEN_add);
      Multiplier = EEPROM.read(PLAYER_MULTIPLIER_add);
      numKilled = EEPROM.read(PLAYER_numKilled_add);
      numL1Treasure = EEPROM.read(PLAYER_numL1Treasure_add);
      numL2Treasure = EEPROM.read(PLAYER_numL2Treasure_add);
      num_blue = EEPROM.read(PLAYER_num_blue_add);
      num_yellow = EEPROM.read(PLAYER_num_yellow_add);
      num_red = EEPROM.read(PLAYER_num_red_add);
      num_purple = EEPROM.read(PLAYER_num_purple_add);
      num_green = EEPROM.read(PLAYER_num_green_add);
      num_orange  = EEPROM.read(PLAYER_num_orange_add);
      scoreInvicta = EEPROM.read(POINT_INVICTA_add);
      scoreDynari = EEPROM.read(POINT_DYNARI_add);
      scoreEphilia = EEPROM.read(POINT_EPHILIA_add);
      scoreAkrona = EEPROM.read(POINT_AKRONA_add);
      scoreSolaris = EEPROM.read(POINT_SOLARIS_add);
      totalTreasure = EEPROM.read(PLAYER_totalTreasure_add);
      numDeath = EEPROM.read(PLAYER_numDeath_add);
      num_bomb = EEPROM.read(PLAYER_num_bomb_add);
      num_col = EEPROM.read(PLAYER_num_col_add);
      // num_dep = EEPROM.read(PLAYER_num_dep_add);      
    }

    if (WIFI_ON)
    {
      WiFi.disconnect();
      // change to correct channel, according to clan
      int clan_channel = dbc.getClanWiFiChannel(CLAN);
      Serial.print("[INITIALISE] Changing to WiFi ");
      Serial.println(clan_channel);
      dbc.changeWiFiChannel(clan_channel);
      Player_EspNOW.enable();
    }
  }

  void sendAction() //actions sent out
  {
    // format of the IR signal (16-bit hexadecimal, i.e. 4 digits)
    // address: 0x0<CLAN><ID - 2 bit>  (ID is 2 bits as there maybe more than 16 people in one CLAN)
    // command: 0x0<Current WiFi Channel><MULTIPLIER><Action>
    if ((action != do_nothing) && (EN > 0) && HP > 0)
    {
      uint16_hex_digits address_digits, command_digits;

      address_digits.digit0 = ID % 16;
      address_digits.digit1 = ID / 16;
      address_digits.digit2 = CLAN;

      command_digits.digit2 = WiFi.channel();
      command_digits.digit0 = action;

      int this_action_multiplier;

      switch (action)
      {
      case attack:
        Player_Buzzer.Blaster();
        //this_action_multiplier = min(Multiplier, MAX_ATTACK_MULTIPLIER);
        this_action_multiplier = 1;
        if(purpleActive) //purple stone 2 dmg
          this_action_multiplier = 2;
        if (ROLE == sigma) // sigma deals 6 dmg
          this_action_multiplier = 6;
        command_digits.digit3 = ROLE;
        break;

      case poison:
        Player_Buzzer.Blaster();
        this_action_multiplier = 1;
        command_digits.digit3 = ROLE;
        redActive = false; //use up red stone
        break;

      case collect:
        this_action_multiplier = 1;    
        command_digits.digit3 = numL1Treasure; //collect shard
        Serial.println("Collect action sent!");
        break;
      
      case heal:
        this_action_multiplier = HEAL_MULTIPLIER; //player heals through pushing down of joystick
        command_digits.digit3 = ROLE;
        Serial.println("Heal action sent!");
        break;

      case transfer:
        this_action_multiplier = selectedStone;
        command_digits.digit3 = ROLE;
        break;

      case revive:
        this_action_multiplier = 4;
        break;

      case depo_kda:
        this_action_multiplier = numKilled;
        command_digits.digit3 = numDeath;
        break;

      case bomb:
        Player_Buzzer.Blaster();
        this_action_multiplier = 1;
        break;

      default:
        this_action_multiplier = 0;
        break;
      }

      command_digits.digit1 = this_action_multiplier;

      ir_signal send_signal;
      send_signal.address = address_digits;
      send_signal.command = command_digits;

      Serial.printf("SEND %d %d %d %d | %d %d %d %d \n", address_digits.digit3, address_digits.digit2, address_digits.digit1, address_digits.digit0,
                    command_digits.digit3, command_digits.digit2, command_digits.digit1, command_digits.digit0);
        Player_IR.send(send_signal, 1);   

      start_receiving_feedback = millis();

      //when GL uses heal they lose HP in exchange
      if ((EEPROM.read(ROLE_add) == groupLeader) && (action == heal))
      {
        HP--;
        EEPROM.write(PLAYER_HP_add, HP);
      }

      EN--;
      EEPROM.write(PLAYER_EN_add, EN);
      Serial.print("CURRENT MAC ADDRESS: ");
      Serial.println(WiFi.macAddress());
    }

    else if ((HP <= 0) && (action == revive))
    {

      uint16_hex_digits address_digits, command_digits;

      address_digits.digit0 = ID % 16;
      address_digits.digit1 = ID / 16;
      address_digits.digit2 = CLAN;

      command_digits.digit2 = WiFi.channel();
      command_digits.digit0 = action;

      int this_action_multiplier;

      command_digits.digit1 = this_action_multiplier;

      ir_signal send_signal;
      send_signal.address = address_digits;
      send_signal.command = command_digits;

      Serial.printf("SEND %d %d %d %d | %d %d %d %d \n", address_digits.digit3, address_digits.digit2, address_digits.digit1, address_digits.digit0,
                    command_digits.digit3, command_digits.digit2, command_digits.digit1, command_digits.digit0);
      Player_IR.send(send_signal, 1);

      start_receiving_feedback = millis();

      Serial.print("CURRENT MAC ADDRESS: ");
      Serial.println(WiFi.macAddress());
    }
  };

  void receiveAction()
  {
    int CLAN_, ID_, EN_, MULTIPLIER_, action_, channel_, role_;  //underscore denotes details of IR signal sender
    unsigned long currTime = millis();
    if (Player_IR.available())
    {
      ir_signal IRsignal_ = Player_IR.read();
      Serial.printf("SIG RECV ON %d %d %d\n", currTime, lastActionReceived, ACTION_RECV_WAIT);
      if (currTime - lastActionReceived > ACTION_RECV_WAIT)
      {
        role_ = IRsignal_.command.digit3;
        channel_ = IRsignal_.command.digit2;
        MULTIPLIER_ = IRsignal_.command.digit1;
        action_ = IRsignal_.command.digit0;

        CLAN_ = (role_ == sigma) ? 255 : IRsignal_.address.digit2;
        ID_ = IRsignal_.address.digit0 + (IRsignal_.address.digit1 << 4);

        Serial.printf("RECV %d %d %d %d | %d %d %d %d \n", IRsignal_.address.digit3, IRsignal_.address.digit2, IRsignal_.address.digit1, IRsignal_.address.digit0, IRsignal_.command.digit3, IRsignal_.command.digit2, IRsignal_.command.digit1, IRsignal_.command.digit0);

        lastActionReceived = currTime;

        if (
            ((CLAN_ != CLAN) && ((action_ == attack) || (action_ == poison) || (action_ == bomb))) ||
            ((ROLE == sigma) && ((action_ == attack) || (action_ == poison) || (action_ == bomb))) ||// enable players to shoot Sigma regardless of clan
            ((action_ == heal) && (CLAN_ == CLAN) && (ID_ != ID)) ||
            ((action_ == heal) && (CLAN_ != CLAN)) ||
            ((action_ == revive) && (CLAN_ == CLAN))||
            ((action_ == transfer) && (CLAN_ == CLAN) && (ID_ !=ID)) ||
            ((action_ == deposit) && (CLAN_ == CLAN) && (ID_ == ID)) ||
            ((action_ == depo_kda) && (CLAN_ == CLAN) && (ID_ == ID)))
        {
          handleAction(CLAN_, ID_, action_, MULTIPLIER_, channel_, role_);
        }
      }
    }
  }

  void handle_respawn()  //for revive
  {
    unsigned long currTime = millis();

    if (!onCooldown) //init
    {
      onCooldown = true;
      timeOfDeath = millis();

      //drop collected treasure(s)
      numL1Treasure = 0;
      EEPROM.write(PLAYER_numL1Treasure_add, numL1Treasure);
    }

    unsigned long elapsedTime = currTime - timeOfDeath;  //duration after death
    int currentCooldown = MAX_COOLDOWN - (elapsedTime / 1000);  //countdown timer shown on death screen

    if (isInstantKilled)
    {
      currentCooldown += (TEMP_NOTI_WAIT / 1000);
    }

    if (currentCooldown > 0)  //still under countdown timer
    {
      if ((!isInstantKilled) || (isInstantKilled && (elapsedTime > TEMP_NOTI_WAIT)))
      {
        permNoti = "    Respawn in " + String(currentCooldown) + "s     ";
      }
    }

    else
    {  //back alive
      orangeActive = false;
      isInstantKilled = false;
      onCooldown = false;
      timeOfDeath = 0;
      HP = MaxHP;
      EEPROM.write(PLAYER_HP_add, HP);

      //light up LED (Green)
      ledIsOn = true;
      ledActivationTime = currTime;
      Player_NeoPixel.displayRGB_FRONT(R_ALIVE, G_ALIVE, B_ALIVE);
      Player_NeoPixel.displayRGB_TOP(R_ALIVE, G_ALIVE, B_ALIVE);
    }
  }

  void update_player_state()
  {
    String noti_to_display;

    unsigned long currTime = millis();

    //respawn if HP is 0
    if ((HP <= 0) && (orangeActive))
    {
      //light up LED (Pink)
      ledIsOn = true;
      ledActivationTime = currTime;
      Player_NeoPixel.displayRGB_FRONT(R_REVIVE, G_REVIVE, B_REVIVE);
      Player_NeoPixel.displayRGB_TOP(R_REVIVE, G_REVIVE, B_REVIVE);

      handle_respawn();

      EN = 0;
      EEPROM.write(PLAYER_EN_add, EN);

      if (Multiplier > 1)
      {
        Multiplier = 1;
        EEPROM.write(PLAYER_MULTIPLIER_add, Multiplier);
      }
    }

    else if ((HP <= 0) && (currentPage == mainPage))
    {
      TreasureHunt_OLED.display_mainPage(HP,  MaxHP, EN,  MaxEN, noti_to_display,  lastPageNav, numDeath, numKilled);
    }

    else
    {
      //turn off LED after a pre-determined duration
      if (ledIsOn && (currTime - ledActivationTime >= ledMaxActivationDuration))
      {
        ledIsOn = false;
        Player_NeoPixel.off_FRONT();
        Player_NeoPixel.off_TOP();
      }

      permNoti = "";
      last_max_en_decay = currTime;

      //EN regen
      if (EN >= MaxEN)
      {
        last_en_recover = currTime;
      }
      else
      {
        int en_rcv = EN_RECOVER_DURATION;
        if (is_x2EnRegen)
        {
          if (currTime - start_x2_en_regen < x2_EN_Regen_bonus_duration)
          {
            en_rcv /= 2;
          }
          else
          {
            is_x2EnRegen = false;
          }
        }
        if (currTime - last_en_recover >= en_rcv)
        {
          EN++;
          EEPROM.write(PLAYER_EN_add, EN);
          last_en_recover = currTime;
        }
      }
    }

    if (yellowActive && isYellowCountdownActive) 
    {
      unsigned long elapsed =  currTime - yellowStartTime;
      Serial.print("Elapsed: ");
      Serial.println(elapsed);
      if (elapsed >= YELLOW_DURATION) 
        {
          yellowActive = false;
          isYellowCountdownActive = false;
          Serial.println("Yellow stone expired");
          tempNoti = "  Yellow Stone Ended  ";
          tempNoti_start = currTime;
          
        }
    }

    if (greenActive)
    {
      EEPROM.write(PLAYER_EN_add, PARTICIPANT_MaxEN);

      if (isGreenCountdownActive) 
      {
        unsigned long elapsed =  currTime - greenStartTime;
        Serial.print("Elapsed: ");
        Serial.println(elapsed);

        if (elapsed >= GREEN_DURATION) 
          {
            greenActive = false;
            isGreenCountdownActive = false;
            Serial.println("Green stone expired");
            tempNoti = "  Green Stone Ended  ";
            tempNoti_start = currTime;
          
          }
      }
    }

    if (isPoisoned)
    {
      unsigned long currTime = millis();
      if (currTime - lastPoisonTick >= 2000 && poisonTicksRemaining > 0)
        {
          lastPoisonTick = currTime;
          poisonTicksRemaining--;

          if (HP > 0)
            {
              HP -= 1;
              if (HP < 0) HP = 0;
              EEPROM.write(PLAYER_HP_add, HP);
              Serial.print("Poison damage. HP: ");
              Serial.println(HP);
            }

        if (poisonTicksRemaining == 0)
          { 
            isPoisoned = false;
            tempNoti = " Poison effect ended ";
            tempNoti_start = currTime;
          }
        }
    }

    if ((isBombCountdownActive) && (bomb_state == active))
    {
      unsigned long elapsed =  currTime - bombStartTime;
      Serial.print("Elapsed: ");
      Serial.println(elapsed);

      if (elapsed >= BOMB_DURATION) 
        {
          isBombCountdownActive = false;
          bomb_state = waiting;
          Serial.println("Bomb expired");
          tempNoti = "    Bomb Ended   ";
          tempNoti_start = currTime;

          ledIsOn = true;
          ledActivationTime = currTime;
          Player_NeoPixel.displayRGB_FRONT(R_PURPLE, G_PURPLE, B_PURPLE);
          Player_NeoPixel.displayRGB_TOP(R_PURPLE, G_PURPLE, B_PURPLE);
        }
    }

    //HP recovery for SIGMA ONLY
    if (ROLE != sigma)
      return;

    if (HP >= MaxHP)
    {
      last_hp_recover = currTime;
    }
    else
    {
      if (currTime - last_hp_recover >= HP_RECOVERY_RATE)
      {
        HP++;
        EEPROM.write(PLAYER_HP_add, HP);
        last_hp_recover = currTime;
      }
    }

    EEPROM.commit();
  }

  void sync_state()  //ensure limit to max HP & EN
  {
    ID = EEPROM.read(ID_add);
    ROLE = EEPROM.read(ROLE_add);

    MaxHP = EEPROM.read(PLAYER_MaxHP_add);
    MaxEN = EEPROM.read(PLAYER_MaxEN_add);
    HP = EEPROM.read(PLAYER_HP_add);
    EN = EEPROM.read(PLAYER_EN_add);

    if (HP > MaxHP)
      HP = MaxHP;
    if (EN > MaxEN)
      EN = MaxEN;
  }

  void handleJoystick_waiting()  //joystick actions when waiting
  {
    joystick_pos joystick_pos = Player_joystick.read_Joystick();
    if (Player_joystick.get_state() == 0)
    {
      switch (joystick_pos)
      {
      case button:
        currentProcess = MainMenuProcess;
        Player_joystick.set_state();
        break;

      case idle:
        break;

      default:
        Player_joystick.set_state();
        break;
      }
    }
  }

  void handleJoystickInGame()  //joystick actions when playing
  {
    joystick_pos joystick_pos = Player_joystick.read_Joystick();
    if (Player_joystick.get_state() == 0)
    {
      switch (joystick_pos)
      {
      case up:
        if (bomb_state == active)
        {
          action = bomb;
        }
        else if (redActive)  //red stone
        {
          action = poison;
        }
        else
        {
          action = attack;
        }
        Player_joystick.set_state();
        break;

      case down:
        if (ROLE == freshman)
        {
          if ((HP > 0) && !GAME_END_ON)
          {
            action = collect;  //collect treasure
          }
          else if (GAME_END_ON)
          {
            action = depo_kda;
          }
          else
          {
            action = revive;
          }
        }
        else
        {
            action = heal;
        }
        Player_joystick.set_state();
        break;

      case left:
        if (!choosingPowerUp)
        {
          lastPageNav--;
          if (lastPageNav > 6)
            lastPageNav -= 7;
          if (lastPageNav < 0)
            lastPageNav += 7;
          Nav_start = millis();
        }
        else
        {
          PowerUpNav--;
          if (PowerUpNav > 8)
            PowerUpNav -= 9;
          if (PowerUpNav < 0)
            PowerUpNav += 9;
        }

        if (choosingTransfer)
        {
          transferNav--;
          if (transferNav > 6)
            transferNav -= 7;
          if (transferNav < 0)
            transferNav += 7;
        }
        Player_joystick.set_state();
        break;

      case right:
        if (!choosingPowerUp)
        {
          lastPageNav++;
          if (lastPageNav > 6)
            lastPageNav -= 7;
          if (lastPageNav < 0)
            lastPageNav += 7;
          Nav_start = millis();
        }
        else
        {
          PowerUpNav++;
          if (PowerUpNav > 8)
            PowerUpNav -= 9;
          if (PowerUpNav < 0)
            PowerUpNav += 9;
        }

        if (choosingTransfer)
        {
          transferNav++;
          if (transferNav > 6)
            transferNav -= 7;
          if (transferNav < 0)
            transferNav += 7;
        }
        Player_joystick.set_state();
        break;

      case button:
        if (EEPROM.read(CLAN_add) == 255)
        {
          runExitRoutine();

          currentProcess = MainMenuProcess;
          currentPage = mainPage; //reset current page
          lastPageNav = currentPage;
        }

        if ((currentPage != transferPage) && !choosingPowerUp && !choosingTransfer/*&& (currentPage != mergePage)*/)
        {
          if (lastPageNav != currentPage)
            currentPage = lastPageNav;
        }

        if (currentPage == powerupPage)
        {
          if (choosingPowerUp)
          {
            handlePowerUp(PowerUpNav);
            choosingPowerUp = false;

            if (isTransferPage)
            {
              currentPage = transferPage;
              lastPageNav = transferPage;
            }
            else
            {
              choosingTransfer = false;
              currentPage = mainPage;
              lastPageNav = mainPage;
            }
          }

          else
          {
            choosingPowerUp = true;
            PowerUpNav = 0;
          }
        }
        
        if (currentPage == transferPage)
        {
          if (choosingTransfer)
          {
            handleTransfer(transferNav);
            action = transfer;
            Player_joystick.set_state();

            choosingTransfer = false;
            isTransferPage = false;
            currentPage = mainPage;
            lastPageNav = mainPage;
          }
          else
          {
            choosingTransfer = true;
            transferNav = 0;
          }
        }

        if (currentPage == exitPage)
        {
          runExitRoutine();
          currentProcess = MainMenuProcess;
          currentPage = mainPage; // reset current page
          lastPageNav = currentPage;
        }

        Player_joystick.set_state();
        break;

      default:
        action = do_nothing;
        if (lastPageNav != currentPage)
        {
          unsigned long currTime = millis();
          if (currTime - Nav_start > NAV_WAIT)
          {
            lastPageNav = currentPage;
          }
        }
        break;
      }
    }
    else
    {
      action = do_nothing;
      if (lastPageNav != currentPage)
      {
        unsigned long currTime = millis();
        if (currTime - Nav_start > NAV_WAIT)
          lastPageNav = currentPage;
      }
    }
  }

  void runExitRoutine()
  {
    //turn off LED & Buzzer
    Player_Buzzer.end_sound();
    Player_NeoPixel.off_FRONT();
    Player_NeoPixel.off_TOP();
    ledIsOn = false;
  }


  void handlePowerUp(int power_up)
  {
    #include "ENITIO_enums.h"
    unsigned long currTime = millis();
    if (HP > 0)
    {
      String noti_to_display;
      switch (power_up)
      {
      case blue:
        isTransferPage = false; 
        if (num_blue > 0)
        {
          num_blue--;
          EEPROM.write(PLAYER_num_blue_add, num_blue);
          blueActive = true;
          Serial.println("Blue Active!");
          ledIsOn = true;
          ledActivationTime = currTime;
          Player_NeoPixel.displayRGB_FRONT(R_BLUE, G_BLUE, B_BLUE);
          Player_NeoPixel.displayRGB_TOP(R_BLUE, G_BLUE, B_BLUE);
        }
        break;

      case yellow:
        isTransferPage = false;
        if (num_yellow > 0)
        {
          num_yellow--;
          EEPROM.write(PLAYER_num_yellow_add, num_yellow);
          yellowActive = true;

          isYellowCountdownActive = true;
          yellowStartTime = millis();
          Serial.println("Yellow Active!");
          Serial.print("Start time: ");
          Serial.println(millis());

          ledIsOn = true;
          ledActivationTime = currTime;
          Player_NeoPixel.displayRGB_FRONT(R_YELLOW, G_YELLOW, B_YELLOW);
          Player_NeoPixel.displayRGB_TOP(R_YELLOW, G_YELLOW, B_YELLOW);          
        }
        break;

      case red:
        isTransferPage = false;
        if (num_red > 0)
        {
          num_red--;
          EEPROM.write(PLAYER_num_red_add, num_red);
          redActive = true;
          Serial.println("Red Active!");

          ledIsOn = true;
          ledActivationTime = currTime;
          Player_NeoPixel.displayRGB_FRONT(R_RED, G_RED, B_RED);
          Player_NeoPixel.displayRGB_TOP(R_RED, G_RED, B_RED);          
        }
        break;

      case purple:
        isTransferPage = false;
        if (num_purple > 0)
        {
          num_purple--;
          EEPROM.write(PLAYER_num_purple_add, num_purple);
          purpleActive = true;
          Serial.println("Purple Active!");

          ledIsOn = true;
          ledActivationTime = currTime;
          Player_NeoPixel.displayRGB_FRONT(R_PURPLE, G_PURPLE, B_PURPLE);
          Player_NeoPixel.displayRGB_TOP(R_PURPLE, G_PURPLE, B_PURPLE);          
        }
        break;

      case green:
        isTransferPage = false;    
        if (num_green > 0)
        {
          num_green--;
          EEPROM.write(PLAYER_num_green_add, num_green);
          greenActive = true;
          
          isGreenCountdownActive = true;
          greenStartTime = millis();
          Serial.println("Green Active!");
          Serial.print("Start time: ");
          Serial.println(millis());

          ledIsOn = true;
          ledActivationTime = currTime;
          Player_NeoPixel.displayRGB_FRONT(R_GREEN, G_GREEN, B_GREEN);
          Player_NeoPixel.displayRGB_TOP(R_GREEN, G_GREEN, B_GREEN);          
        }
        break;

      case orange:
        isTransferPage = false;
        if (num_orange > 0)
        {
          num_orange--;
          EEPROM.write(PLAYER_num_orange_add, num_orange);
          orangeActive = true;
          Serial.println("Orange Active!");

          ledIsOn = true;
          ledActivationTime = currTime;
          Player_NeoPixel.displayRGB_FRONT(R_ORANGE, G_ORANGE, B_ORANGE);
          Player_NeoPixel.displayRGB_TOP(R_ORANGE, G_ORANGE, B_ORANGE);          
        }
        break;

      case transferstone:
        if ((num_blue + num_yellow + num_red + num_purple + num_green + num_orange) > 0)
        {
          isTransferPage = true;
        }
        else
        {
          isTransferPage = false;
        }
        break;
      
      case mergestone:
        isTransferPage = false;
        if ((num_blue > 0) && (num_yellow > 0) && (num_red > 0) && (num_purple > 0) && (num_green > 0) && (num_orange > 0) && (num_bomb == 0) && (bomb_state == waiting))
        {
          num_blue--;
          EEPROM.write(PLAYER_num_blue_add, num_blue); 
          num_yellow--;
          EEPROM.write(PLAYER_num_yellow_add, num_yellow);          
          num_red--;
          EEPROM.write(PLAYER_num_red_add, num_red);          
          num_purple--;
          EEPROM.write(PLAYER_num_purple_add, num_purple);          
          num_green--;
          EEPROM.write(PLAYER_num_green_add, num_green);          
          num_orange--;
          EEPROM.write(PLAYER_num_orange_add, num_orange);

          num_bomb++;
          EEPROM.write(PLAYER_num_bomb_add, num_bomb);

          tempNoti = "    Stones Merged!  ";
          tempNoti_start = millis();
          bomb_state = use;
        }
        else
        {
          if (num_bomb > 0)
          {
            num_bomb--;
            EEPROM.write(PLAYER_num_bomb_add, num_bomb);

            tempNoti = "     Bomb Active!    ";
            tempNoti_start = millis();
            isBombCountdownActive = true;
            
            ledIsOn = true;
            ledActivationTime = currTime;
            Player_NeoPixel.displayRGB_FRONT(R_BLUE, G_BLUE, B_BLUE);
            Player_NeoPixel.displayRGB_TOP(R_BLUE, G_BLUE, B_BLUE);

            delay(500);

            Player_NeoPixel.displayRGB_FRONT(R_YELLOW, G_YELLOW, B_YELLOW);
            Player_NeoPixel.displayRGB_TOP(R_YELLOW, G_YELLOW, B_YELLOW);

            delay(500);

            Player_NeoPixel.displayRGB_FRONT(R_RED, G_RED, B_RED);
            Player_NeoPixel.displayRGB_TOP(R_RED, G_RED, B_RED);

            delay(500);

            Player_NeoPixel.displayRGB_FRONT(R_PURPLE, G_PURPLE, B_PURPLE);
            Player_NeoPixel.displayRGB_TOP(R_PURPLE, G_PURPLE, B_PURPLE);

            delay(500);

            Player_NeoPixel.displayRGB_FRONT(R_GREEN, G_GREEN, B_GREEN);
            Player_NeoPixel.displayRGB_TOP(R_GREEN, G_GREEN, B_GREEN);

            delay(500);

            Player_NeoPixel.displayRGB_FRONT(R_ORANGE, G_ORANGE, B_ORANGE);
            Player_NeoPixel.displayRGB_TOP(R_ORANGE, G_ORANGE, B_ORANGE);

            bomb_state = active;
            bombStartTime = millis();
          }
        }
        break;

      default:
        isTransferPage = false;
        break;
      }

        PowerUpNav = 0;
    }
  }

  void handleTransfer(int transfer_nav)
  {
    
    //choose which stone to be transferred
    if (HP > 0)
    {
      switch(transfer_nav)
      {
      case 1: if (num_blue   > 0) {selectedStone = 1;} else {selectedStone = 0;} break;
      case 2: if (num_yellow > 0) {selectedStone = 2;} else {selectedStone = 0;} break;
      case 3: if (num_red    > 0) {selectedStone = 3;} else {selectedStone = 0;} break;
      case 4: if (num_purple > 0) {selectedStone = 4;} else {selectedStone = 0;} break;
      case 5: if (num_green  > 0) {selectedStone = 5;} else {selectedStone = 0;} break;
      case 6: if (num_orange > 0) {selectedStone = 6;} else {selectedStone = 0;} break;
      default: selectedStone = 0; break;
      }
      
      transferNav = 0;
      isTransferPage = false;
    }
  }

  void handleTreasureBuzzer()  //buzzer sounds
  {
    unsigned long currentMillis = millis();

    //if numL1Treasure is 0, stop any ongoing buzz and reset buzzerFirstRun
    if (numL1Treasure == 0)
    {
      if (buzzerEndTime != 0 && currentMillis - tempNoti_start >= NOTI_SOUND_DURATION)
      {
        Player_Buzzer.end_sound();
        buzzerEndTime = 0;
        Serial.println("Buzzer stopped due to no treasure");
      }
      buzzerFirstRun = true;
      return;
    }

    //handle first run delay when numL1Treasure becomes > 0
    if (buzzerFirstRun)
    {
      buzzerFirstRun = false;
      previousMillis = currentMillis;
      randomLongPause = random(45000, 60000);
      buzzerEndTime = currentMillis + randomLongPause;
      Serial.println("First run delay completed. Next buzz in " + String(randomLongPause) + "ms");

      return;
    }

    if (beeping)
    {
      //handle the beeping sequence
      if (currentMillis - previousMillis >= BUZZ_INTERVAL)
      {
        previousMillis = currentMillis;
        if (!isBuzzerOn)
        {
          isBuzzerOn = true;
          Player_Buzzer.sound(NOTE_E4);
          // Serial.println("Buzz");
        }
        else
        {
          isBuzzerOn = false;
          Player_Buzzer.end_sound();
          ++beepCount;
          // Serial.println("End");
        }

        // After n beeps, switch to pause state
        if (beepCount >= BEEPS_PER_CYCLE)
        {
          beeping = false;
          beepCount = 0;
          beepCycleCount++;
          Player_Buzzer.end_sound();

          if (beepCycleCount >= BEEP_CYCLES)
          {
            beepCycleCount = 0;
            randomLongPause = random(3000, 18000);
            buzzerEndTime = currentMillis + randomLongPause;
            Serial.println("Treasure Buzzer paused. Next buzz in " + String(randomLongPause) + "ms");
          }
          else
          {
            buzzerEndTime = currentMillis + 1000;
            // Serial.println("Beep cycle " + String(beepCycleCount) + " completed");
          }
        }
      }
    }
    else
    {
      //during pause, wait for the random interval to elapse
      if (currentMillis >= buzzerEndTime)
      {
        beeping = true;
        previousMillis = currentMillis;
        // Serial.println("Treasure Buzzer resumed");
      }
    }
  }

  void handleAction(int CLAN_, int ID_, int action_, int MULTIPLIER_, int channel_, int role_)  //handle received actions
  {
    Serial.print("Action ID: ");
    Serial.println(action_);
    unsigned long currTime = millis();
    switch (action_)
    {
    case attack:  //being attacked
      if (HP > 0)
        {
          if (yellowActive)  //yellow stone block dmg
            {
              tempNoti = "       Blocked      ";
              tempNoti_start = millis();
            }
          else
          {
            // HP = (role_ == sigma) ? 0 : max(HP - MULTIPLIER_, 0);
            HP = max(HP-MULTIPLIER_,0);
            if (HP <= 0)  //died --> give score to opp
              {
                switch (CLAN_)
                {
                  case INVICTA:
                    scoreInvicta += (ROLE == sigma) ? SIGMA_KILLED_SCORE : 1;
                    EEPROM.write(POINT_INVICTA_add, scoreInvicta);
                    Serial.println("INVICTA Scores!");
                    break;

                  case DYNARI:
                    scoreDynari += (ROLE == sigma) ? SIGMA_KILLED_SCORE : 1;
                    EEPROM.write(POINT_DYNARI_add, scoreDynari);
                    Serial.println("DYNARI Scores!");
                    break;

                  case EPHILIA:
                    scoreEphilia += (ROLE == sigma) ? SIGMA_KILLED_SCORE : 1;
                    EEPROM.write(POINT_EPHILIA_add, scoreEphilia);
                    Serial.println("EPHILIA Scores!");
                    break;

                  case AKRONA:
                    scoreAkrona += (ROLE == sigma) ? SIGMA_KILLED_SCORE : 1;
                    EEPROM.write(POINT_AKRONA_add, scoreAkrona);
                    Serial.println("AKRONA Scores!");
                    break;

                  case SOLARIS:
                    scoreSolaris += (ROLE == sigma) ? SIGMA_KILLED_SCORE : 1;
                    EEPROM.write(POINT_SOLARIS_add, scoreSolaris);
                    Serial.println("SOLARIS Scores!");
                    break;

                  default:
                    Serial.println("Attacked by SIGMA!");
                    break;
                }

                ++numDeath;
                EEPROM.write(PLAYER_numDeath_add, numDeath);

                //play death SFX
                xTaskCreate(
                    playDeathSoundTask, // Task function
                    "PlayDeathSound",   // Name of the task
                    2048,               // Stack size (in words)
                    NULL,               // Task input parameter
                    1,                  // Priority of the task
                    NULL                // Task handle
                );
              }

            EEPROM.write(PLAYER_HP_add, HP);
            Serial.printf("Attacked. Current HP: %d \n", HP);

            if (role_ == sigma)
            {
              tempNoti = "    Attacked by Sigma     ";
              // isInstantKilled = true;
            }
            else
            {
              tempNoti = "       Attacked      ";
            }

            tempNoti_start = millis();
            feedback_attack(CLAN_, ID_, channel_);
          }
        }
      break;

    case heal:  //heal only when alive
      if (HP > 0)
      {
        HP = min(HP + MULTIPLIER_, MaxHP);
        EEPROM.write(PLAYER_HP_add, HP);
        tempNoti = "        Healed       ";
        tempNoti_start = millis();
        last_received_heal = tempNoti_start;
      }
      break;

    case deposit:
      if (HP > 0 && numL1Treasure > 0)
      {
        Serial.printf("Shards deposited %d\n", numL1Treasure);
        tempNoti = "   Deposited " + String(numL1Treasure) + " Shards.";
        tempNoti_start = millis();
        Player_Buzzer.sound(NOTE_E3);

        updateTotalTreasure(numL1Treasure); //update the total treasure collected by player (achievement)

        //clear treasure from inventory
        numL1Treasure = 0;
        EEPROM.write(PLAYER_numL1Treasure_add, numL1Treasure);
      }
      break;

    case depo_kda:
      if (GAME_END_ON && ((numKilled > 0) || (numDeath > 0)))
      {
        Serial.printf("KDA deposited K: %d, D: %d\n", numKilled, numDeath);
        tempNoti = "    Deposited KDA   ";
        tempNoti_start = millis();
        Player_Buzzer.sound(NOTE_E3);

        //clear kda
        numKilled = 0;
        numDeath = 0;
        EEPROM.write(PLAYER_numKilled_add, numKilled);
        EEPROM.write(PLAYER_numDeath_add, numDeath);

      }
      break;

    case poison:
      if (HP > 0)
      {
        if (yellowActive)  //yellow stone block dmg
          {
            tempNoti = "       Blocked      ";
            tempNoti_start = millis();
          }
        else
        {
          Serial.printf("Attacked. Current HP: %d \n", HP);
          tempNoti = "       Poisoned      ";
          tempNoti_start = millis();
          Player_Buzzer.sound(NOTE_E3);

          isPoisoned = true;
          poisonTicksRemaining = 3;
          lastPoisonTick = millis();
        }
      }
      break;

    case transfer:
      if (HP > 0)
      {
        if(MULTIPLIER_ == 0) //stone not transferred //MULTIPLIER_ is the selectedStone
        {
          tempNoti = "   Stone Not Transferred  ";
          tempNoti_start = millis();
          Player_Buzzer.sound(NOTE_E3);
        }
        else
        {
          Serial.printf("Stone Transferred: %d", MULTIPLIER_);
          tempNoti = "   Stone Transferred  ";
          tempNoti_start = millis();
          Player_Buzzer.sound(NOTE_E3);

          isTransferred = true;

          switch(MULTIPLIER_)
          {
              case blue:
                num_blue++;
                blueActive = false;
                EEPROM.write(PLAYER_num_blue_add, num_blue);
                tempNoti = "  Blue Received!  ";
                tempNoti_start = millis();

                ledIsOn = true;
                ledActivationTime = currTime;
                Player_NeoPixel.displayRGB_FRONT(R_BLUE, G_BLUE, B_BLUE);
                Player_NeoPixel.displayRGB_TOP(R_BLUE, G_BLUE, B_BLUE);                
                break;
              
              case yellow:
                num_yellow++;
                yellowActive = false;
                EEPROM.write(PLAYER_num_yellow_add, num_yellow);
                tempNoti = "  Yellow Received!  ";
                tempNoti_start = millis();

                ledIsOn = true;
                ledActivationTime = currTime;
                Player_NeoPixel.displayRGB_FRONT(R_YELLOW, G_YELLOW, B_YELLOW);
                Player_NeoPixel.displayRGB_TOP(R_YELLOW, G_YELLOW, B_YELLOW);                    
                break;

              case red:
                num_red++;
                redActive = false;
                EEPROM.write(PLAYER_num_red_add, num_red);
                tempNoti = "  Red Received!  ";
                tempNoti_start = millis();

                ledIsOn = true;
                ledActivationTime = currTime;
                Player_NeoPixel.displayRGB_FRONT(R_RED, G_RED, B_RED);
                Player_NeoPixel.displayRGB_TOP(R_RED, G_RED, B_RED);                  
                break;
              
              case purple:
                num_purple++;
                purpleActive = false;
                EEPROM.write(PLAYER_num_purple_add, num_purple);
                tempNoti = "  Purple Received!  ";
                tempNoti_start = millis();

                ledIsOn = true;
                ledActivationTime = currTime;
                Player_NeoPixel.displayRGB_FRONT(R_PURPLE, G_PURPLE, B_PURPLE);
                Player_NeoPixel.displayRGB_TOP(R_PURPLE, G_PURPLE, B_PURPLE);                  
                break;

              case green:
                num_green++;
                greenActive = false;
                EEPROM.write(PLAYER_num_green_add, num_green);
                tempNoti = "  Green Received!  ";
                tempNoti_start = millis();

                ledIsOn = true;
                ledActivationTime = currTime;
                Player_NeoPixel.displayRGB_FRONT(R_GREEN, G_GREEN, B_GREEN);
                Player_NeoPixel.displayRGB_TOP(R_GREEN, G_GREEN, B_GREEN);
                break;
              
              case orange:
                num_orange++;
                orangeActive = false;
                EEPROM.write(PLAYER_num_orange_add, num_orange);
                tempNoti = "  Orange Received!  ";
                tempNoti_start = millis();

                ledIsOn = true;
                ledActivationTime = currTime;
                Player_NeoPixel.displayRGB_FRONT(R_ORANGE, G_ORANGE, B_ORANGE);
                Player_NeoPixel.displayRGB_TOP(R_ORANGE, G_ORANGE, B_ORANGE);                    
                break;

              default:
                break;
          }

          feedback_transfer(CLAN_, ID_, channel_, isTransferred);
          isTransferred = false;
        }
      }
      break;

    case bomb:
      if (HP > 0)
      {
        Serial.println("Bomb action received!");
        Serial.print("Before bomb HP: ");
        Serial.println(HP);
        HP = std::max(HP - BOMB_HP_DEDUCTION, 0);
        Serial.print("After bomb HP: ");
        Serial.println(HP);
        EEPROM.write(PLAYER_HP_add, HP);
        EEPROM.commit();
        std::tie(tempNoti, tempNoti_start) = std::make_pair("   You are Bombed!!  ", millis());
        Player_Buzzer.sound(NOTE_E3);
        feedback_bomb(CLAN_, ID_);

        if (HP == 0) // only increase death count if HP is 0
        {
          isInstantKilled = true;
          ++numDeath;
          EEPROM.write(PLAYER_numDeath_add, numDeath);
          EEPROM.commit();

          // Play death SFX
          xTaskCreate(
              playDeathSoundTask, // Task function
              "PlayDeathSound",   // Name of the task
              2048,               // Stack size (in words)
              NULL,               // Task input parameter
              1,                  // Priority of the task
              NULL                // Task handle
          );
        }
      }
      break;


    default:
      break;
    }
  }

  void updateTotalTreasure(int num_of_treasure_deposited)
  {
    totalTreasure = EEPROM.read(PLAYER_totalTreasure_add); //read the current treasure
    totalTreasure += num_of_treasure_deposited;            //update the total treasure
    EEPROM.write(PLAYER_totalTreasure_add, totalTreasure); //write the updated treasure to EEPROM
    Serial.printf("Total shards collected %d\n", totalTreasure);
  }

  void feedback_attack(int CLAN_, int ID_, int channel_)
  {
    bool killed = (HP == 0);
    Player_EspNOW.send_data(1, 1, CLAN_, ID_, CLAN, killed, channel_);
  }

  void feedback_transfer(int CLAN_, int ID_, int channel_, bool isTransferred)
  {
    Player_EspNOW.send_data(1, 5, CLAN_, ID_, CLAN, isTransferred, channel_);
  }

  void feedback_bomb(int CLAN_, int ID_)
  {
    bool killed = (HP == 0);
    Player_EspNOW.send_data(1, 6, CLAN_, ID_, CLAN, killed, 1); //TODO: specify WiFi channel
  }

  void handleFeedbackMsg(feedback_message feedbackData)  //feedback message
  {
    switch (feedbackData.attackee_type)
    {
    //from wanderers
    case 1:
      if ((feedbackData.attacker_CLAN == CLAN) && (feedbackData.attacker_ID == ID) && (HP > 0))
      {
        // Player_Buzzer.Blaster();
        if (feedbackData.is_attackee_killed == true)
        {
          tempNoti = "    Player Killed! ";
          tempNoti_start = millis();
          Multiplier++;
          numKilled++;
          EEPROM.write(PLAYER_MULTIPLIER_add, Multiplier);
          EEPROM.write(PLAYER_numKilled_add, numKilled);
        }
        else
        {  //opponents lose HP but alive
          tempNoti = " Attack Successfully! ";
          tempNoti_start = millis();
        }
      }
      break;

   //from treasure level 1 & v2
    case 2:
      if ((feedbackData.attacker_CLAN == CLAN) && (feedbackData.attacker_ID == ID) && (HP > 0))
      {
        Serial.println(" Shards Collected! ");
        if(blueActive)
        {
          numL1Treasure += 2;
        }
        else
        {
          numL1Treasure++;
        }
        EEPROM.write(PLAYER_numL1Treasure_add, numL1Treasure);

        Player_Buzzer.sound(NOTE_E3);
        tempNoti = "   Shards Collected! ";
        tempNoti_start = millis();

        num_col += numL1Treasure;
        EEPROM.write(PLAYER_num_col_add, num_col);
        EEPROM.commit();
      }
      break;

    //from treasure level 2, ie powerups
    case 3:
      if ((feedbackData.attacker_CLAN == CLAN) && (feedbackData.attacker_ID == ID) && (HP > 0))
      {
        if (feedbackData.is_attackee_killed == true)
        {
          tempNoti = "  Stone Collected! ";
          tempNoti_start = millis();
          numL2Treasure++;
          unsigned long currTime = millis();
          switch (feedbackData.powerup_received)
          {
            case 1:
              num_blue++;
              blueActive = false;
              EEPROM.write(PLAYER_num_blue_add, num_blue);
              tempNoti = "   Blue Stone!  ";
              tempNoti_start = millis();

              ledIsOn = true;
              ledActivationTime = currTime;
              Player_NeoPixel.displayRGB_FRONT(R_BLUE, G_BLUE, B_BLUE);
              Player_NeoPixel.displayRGB_TOP(R_BLUE, G_BLUE, B_BLUE);
              break;
            
            case 2:
              num_yellow++;
              yellowActive = false;
              EEPROM.write(PLAYER_num_yellow_add, num_yellow);
              tempNoti = "   Yellow Stone!  ";
              tempNoti_start = millis();

              ledIsOn = true;
              ledActivationTime = currTime;
              Player_NeoPixel.displayRGB_FRONT(R_YELLOW, G_YELLOW, B_YELLOW);
              Player_NeoPixel.displayRGB_TOP(R_YELLOW, G_YELLOW, B_YELLOW);
              break;

            case 3:
              num_red++;
              redActive = false;
              EEPROM.write(PLAYER_num_red_add, num_red);
              tempNoti = "   Red Stone!  ";
              tempNoti_start = millis();

              ledIsOn = true;
              ledActivationTime = currTime;
              Player_NeoPixel.displayRGB_FRONT(R_RED, G_RED, B_RED);
              Player_NeoPixel.displayRGB_TOP(R_RED, G_RED, B_RED);
              break;
            
            case 4:
              num_purple++;
              purpleActive = false;
              EEPROM.write(PLAYER_num_purple_add, num_purple);
              tempNoti = "   Purple Stone!  ";
              tempNoti_start = millis();

              ledIsOn = true;
              ledActivationTime = currTime;
              Player_NeoPixel.displayRGB_FRONT(R_PURPLE, G_PURPLE, B_PURPLE);
              Player_NeoPixel.displayRGB_TOP(R_PURPLE, G_PURPLE, B_PURPLE);
              break;

            case 5:
              num_green++;
              greenActive = false;
              EEPROM.write(PLAYER_num_green_add, num_green);
              tempNoti = "   Green Stone!  ";
              tempNoti_start = millis();

              ledIsOn = true;
              ledActivationTime = currTime;
              Player_NeoPixel.displayRGB_FRONT(R_GREEN, G_GREEN, B_GREEN);
              Player_NeoPixel.displayRGB_TOP(R_GREEN, G_GREEN, B_GREEN);
              break;
            
            case 6:
              num_orange++;
              orangeActive = false;
              EEPROM.write(PLAYER_num_orange_add, num_orange);
              tempNoti = "   Orange Stone!  ";
              tempNoti_start = millis();

              ledIsOn = true;
              ledActivationTime = currTime;
              Player_NeoPixel.displayRGB_FRONT(R_ORANGE, G_ORANGE, B_ORANGE);
              Player_NeoPixel.displayRGB_TOP(R_ORANGE, G_ORANGE, B_ORANGE);
              break;
          }
          break;
        }
        else
        {
          tempNoti = " L2 Treasure Damaged ";
          tempNoti_start = millis();
        }
      }
      break;



    //for revive
    case 4:
      if ((feedbackData.attacker_CLAN == CLAN) && (feedbackData.is_attackee_killed == true)) 
      {
        orangeActive = true;
      }
      break;
    
    //feedback when other players receive stone from us
    case 5:
      if (feedbackData.is_attackee_killed == true)
      {
        switch(selectedStone)
        {
          case 1:
            num_blue--;
            EEPROM.write(PLAYER_num_blue_add, num_blue);
            tempNoti = "    Blue Given!  ";
            tempNoti_start = millis();            
            break;

          case 2:
            num_yellow--;
            EEPROM.write(PLAYER_num_yellow_add, num_yellow);
            tempNoti = "    Yellow Given!  ";
            tempNoti_start = millis();                    
            break;

          case 3:
            num_red--;
            EEPROM.write(PLAYER_num_red_add, num_red);
            tempNoti = "    Red Given!  ";
            tempNoti_start = millis();                    
            break;
            
          case 4:
            num_purple--;
            EEPROM.write(PLAYER_num_purple_add, num_purple);
            tempNoti = "    Purple Given!  ";
            tempNoti_start = millis();                
            break;

          case 5:
            num_green--;
            EEPROM.write(PLAYER_num_green_add, num_green);
            tempNoti = "    Green Given!  ";
            tempNoti_start = millis();               
            break;

          case 6:
            num_orange--;
            EEPROM.write(PLAYER_num_orange_add, num_orange);
            tempNoti = "    Orange Given!  ";
            tempNoti_start = millis();               
            break;

          default:
            break;
        }
      }
      break;

    //bomb feedback, source: wanderers and treasure v2
    case 6:
      if ((feedbackData.attacker_CLAN == CLAN) && (feedbackData.attacker_ID == ID) && (bomb_state == active))
      {
        if (feedbackData.is_attackee_killed == true)
        {
          temp_bomb_killed += 1;
          temp_bomb_attacked += 1;
          Multiplier++;
          numKilled++;
          EEPROM.write(PLAYER_MULTIPLIER_add, Multiplier);
          EEPROM.write(PLAYER_numKilled_add, numKilled);
        }
        else
        {
          temp_bomb_attacked += 1;
        }
      }
      break;

    //from wanderers (gl)
    case 7:
      //received a heal feedback
      if (HP != 0)
        EN++;
      HP = MaxHP;
      EEPROM.write(PLAYER_HP_add, HP);
      tempNoti = "        Healed       ";
      tempNoti_start = millis();
      last_received_heal = tempNoti_start;
      break;

    default:
      break;
    }
  }

  // void handleBombed(feedback_message feedbackData)
  // {
  //   if (HP > 0)
  //   {
  //     isInstantKilled = true;
  //     Serial.print("Before bomb HP: ");
  //     Serial.println(HP);
  //     HP = std::max(HP - BOMB_HP_DEDUCTION, 0);
  //     Serial.print("After bomb HP: ");
  //     Serial.println(HP);
  //     std::tie(tempNoti, tempNoti_start) = std::make_pair("   You are Bombed!!  ", millis());
  //     Player_Buzzer.sound(NOTE_E3);
  //     feedback_bomb(feedbackData.attacker_CLAN, feedbackData.attacker_ID);

  //     if (HP == 0) // only increase death count if HP is 0
  //     {
  //       ++numDeath;
  //       EEPROM.write(PLAYER_numDeath_add, numDeath);
  //       EEPROM.commit();

  //       // Play death SFX
  //       xTaskCreate(
  //           playDeathSoundTask, // Task function
  //           "PlayDeathSound",   // Name of the task
  //           2048,               // Stack size (in words)
  //           NULL,               // Task input parameter
  //           1,                  // Priority of the task
  //           NULL                // Task handle
  //       );
  //     }
  //   }
  // }

  void receiveEspNOW() //handle EspNOW
  {
    if (EspNOW_received >= 1)
    {
      int i;
      for (i = 0; i < EspNOW_received; i++)
      {
        feedback_message feedbackData = Player_EspNOW.get_feedback_received();
        switch (feedbackData.msg_type)
        {
        case 1:
          handleFeedbackMsg(feedbackData);
          break;

        // case 2:
        //   handleBombed(feedbackData);
        //   break;

        default:
          break;
        }
        EspNOW_received--;
      }

      // if (temp_bomb_attacked == Player_EspNOW.num_bombed)
      // {
        // Serial.print("Bombed ");
        // Serial.println(temp_bomb_attacked);
        // String num_attack_noti = String(" Bombed ");
        // num_attack_noti.concat(temp_bomb_attacked);
        // String num_killed_noti = String(" Killed ");
        // num_killed_noti.concat(temp_bomb_killed);
        // num_attack_noti += num_killed_noti;
        // tempNoti = num_attack_noti;
        // tempNoti_start = millis();
      //   temp_bomb_attacked = 0;
      //   temp_bomb_killed = 0;
      // }
    }
  }

  void update_display_waiting()  //triggers waiting page
  {
    TreasureHunt_OLED.display_WaitingPage();
  }

  void update_display()
  {
    String noti_to_display;
    if (permNoti.length() == 0)
    {
      if (tempNoti.length() != 0)
      {
        unsigned long currTime = millis();
        if (currTime - tempNoti_start <= TEMP_NOTI_WAIT)
        {
          if (((currTime - tempNoti_start) / TEMP_NOTI_BLINKING) % 2 == 0)
            noti_to_display = tempNoti;
          else
            noti_to_display = "";
        }
        else
        {
          noti_to_display = "";
          tempNoti = "";
        }
      }
    }
    else
    {
      noti_to_display = permNoti;
    }

    switch (currentPage)
    {
    case mainPage:
      if (EEPROM.read(CLAN_add) != 255)
      {
        TreasureHunt_OLED.display_mainPage(HP, MaxHP, EN, MaxEN, noti_to_display, lastPageNav, numDeath, numKilled);
      }
      else
      {
        TreasureHunt_OLED.display_noClanPage();
      }

      break;
    case infoPage:
      TreasureHunt_OLED.display_infoPage(CLAN, ID, Multiplier, MaxEN, noti_to_display, lastPageNav);
      break;

    case treasurePage:
      TreasureHunt_OLED.display_treasurePage(numL1Treasure, noti_to_display, lastPageNav);
      break;

    case powerupPage:
      TreasureHunt_OLED.display_powerupPage(num_blue,
                                            num_yellow,
                                            num_red,
                                            num_purple,
                                            num_green,
                                            num_orange,
                                            num_bomb,
                                            bomb_state,
                                            noti_to_display,
                                            lastPageNav,
                                            PowerUpNav,
                                            blueActive, yellowActive, redActive, purpleActive, greenActive, orangeActive
                                            );
      break;

    case statsPage:
      TreasureHunt_OLED.display_statsPage(numDeath,
                                          numKilled,
                                          scoreInvicta,
                                          scoreDynari,
                                          scoreEphilia,
                                          scoreAkrona,
                                          scoreSolaris,
                                          noti_to_display,
                                          lastPageNav);
      break;

    case scorePage:
      TreasureHunt_OLED.display_scorePage(num_col, totalTreasure, noti_to_display, lastPageNav);
      break;

    case transferPage:
      TreasureHunt_OLED.display_transferPage(num_blue,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
                                            num_yellow,
                                            num_red,
                                            num_purple,
                                            num_green,
                                            num_orange,
                                            noti_to_display,
                                            transferNav
                                            );
      break;


    default:
      lastPageNav = mainPage;
      currentPage = mainPage;
    }
  }

  void update_sound()
  {
    unsigned long currTime = millis();
    if (currTime - tempNoti_start >= NOTI_SOUND_DURATION && !isBuzzerOn && !isPlayingDeathSFX)
    {
      Player_Buzzer.end_sound();
    }
  }

  int get_game_state()
  {
    //retrieve game state from server
    //0 means game did not start
    //1 means in game
    //once the game has started no need to check anymore
    if (!gameStarted)
    {
      gameStarted = game_started_buffer;
      if (gameStarted)
      {
        Serial.println("Game has started! Starting initialisation process...");
        int CLAN = EEPROM.read(CLAN_add);
        int role = EEPROM.read(ROLE_add);
        int id;

        if (WIFI_ON)
        {
          // first check if wanderer has been registered
          while (EEPROM.read(PLAYER_registered_online_add) != 1)
          {
            Serial.println("Attempting to register WANDERER...");
            EEPROM.write(PLAYER_registered_online_add, dbc.registerWanderer(CLAN, my_MAC_address));
          }

          String id_constants_json = dbc.getPlayerIDandGameConstantsJSON(CLAN, my_MAC_address);
          id = dbc.getPlayerIDFromJSON(id_constants_json);
          EEPROM.write(ID_add, id);

          GAME_CONSTANTS game_consts = dbc.retrieveGameConstantsFromJSONArray(id_constants_json);
          HTTP_TIMEOUT = game_consts.HTTP_TIMEOUT;
          EN_RECOVER_DURATION = game_consts.EN_RECOVER_DURATION;
          POISON_DECAY_DURATION = game_consts.POISON_DECAY_DURATION;
          /*POISON_IMMUNITY_DURATION = game_consts.POISON_IMMUNITY_DURATION;
          POISON_INFECTION_PROBABILITY = game_consts.POISON_INFECTION_PROBABILITY;*/
          PARTICIPANT_MaxHP = game_consts.PARTICIPANT_MaxHP;
          GL_MaxHP = game_consts.GL_MaxHP;
          PARTICIPANT_MaxEN = game_consts.PARTICIPANT_MaxEN;
          GL_MaxEN = game_consts.GL_MaxEN;
          INITIAL_MULTIPLIER = game_consts.INITIAL_MULTIPLIER;
          HEAL_MULTIPLIER = game_consts.HEAL_MULTIPLIER;
          MAX_ATTACK_MULTIPLIER = game_consts.MAX_ATTACK_MULTIPLIER;
          MAX_COLLECT_MULTIPLIER = game_consts.MAX_COLLECT_MULTIPLIER;
          BOMB_HP_DEDUCTION = game_consts.BOMB_HP_DEDUCTION;
          KILL_UPDATE_SERVER_INTERVAL = game_consts.KILL_UPDATE_SERVER_INTERVAL;
        }
        else
        {
          Serial.println("WIFI MODE DISABLED, retrieving ID and constants from hardcoded memory");
          id = EEPROM.read(ID_add);
        }

        Serial.printf("[INITIALISE] Current CLAN: %d ID %d\n", CLAN, EEPROM.read(ID_add));
        setup_initial_state(id, CLAN, role); //initialize Player
        deviceReady = 1;
      }
      return gameStarted;
    }
    else if (CLAN == 255)
    {
      CLAN = EEPROM.read(CLAN_add);
    }
    return 1;
  }

  void gameMainLoop()
  {
    if (!get_game_state())
    {
      handleJoystick_waiting();
      update_display_waiting();
    }
    else
    {
      sync_state(); //sync internal variables with EEPROM state
      handleJoystickInGame();
      sendAction();
      receiveAction();
      receiveEspNOW();
      update_player_state();
      update_display();
      update_sound();
      handleTreasureBuzzer();
      EEPROM.commit();
    }
  };


  void gameBackgroundProcess()
  {
    if (deviceReady == 1)
    {
      unsigned long currTime = millis();
      /*
         Considering 250 devices on at the same time, and each request takes 2-3s for roundtrip.
         If we were to stagger the upload periods of all devices, that will take 250 * 3 = 750s === 12 mins
         We can accept a small room of overlap, hence we set a upload period of 10 mins.
      */
      if (currTime - last_update_kills_to_server > KILL_UPDATE_SERVER_INTERVAL)
      {
        //first make sure player has been registered on server
        dbc.connectToWiFi();
        if (EEPROM.read(PLAYER_registered_online_add) != 1)
        {
          //register first
          EEPROM.write(PLAYER_registered_online_add, dbc.registerWanderer(CLAN, my_MAC_address));
        }

        Serial.println("Sending Game Statistics to Server...");
        FailedFeedbackStatistics this_stats;
        this_stats = dbc.sendGameStatistics(CLAN, ID, numKilled, numL1Treasure, numL2Treasure);
        int unrecognized_kills = this_stats.num_kills;
        int unrecognized_powerups = this_stats.num_powerups;
        Multiplier += unrecognized_kills;
        EEPROM.write(PLAYER_MULTIPLIER_add, Multiplier);
        tempNoti = "   Kills Retrieved   ";
        tempNoti_start = millis();
        numL1Treasure += unrecognized_powerups;
        EEPROM.write(PLAYER_numL1Treasure_add, numL1Treasure);
        
        int i;
        for (i = 0; i < unrecognized_powerups; i++)
        {
          int new_PowerUp = random(1, 6);
          switch (new_PowerUp)
          {
           case blue:
             num_blue++;
             EEPROM.write(PLAYER_num_blue_add, num_blue);
             break;

           case yellow:
             num_yellow++;
             EEPROM.write(PLAYER_num_yellow_add, num_yellow);
             break;

           case red:
             num_red++;
             EEPROM.write(PLAYER_num_red_add, num_red);
             break;

           case purple:
             num_purple++;
             EEPROM.write(PLAYER_num_purple_add, num_purple);
             break;

           case green:
             num_green++;
             EEPROM.write(PLAYER_num_green_add, num_green);
             break;

           case orange:
             num_orange++;
             EEPROM.write(PLAYER_num_orange_add, num_orange);
            break;

           default:
            break;
          }
          //tempNoti = " Powerup Retrieved ";
          tempNoti_start = millis();
        }

        last_update_kills_to_server = currTime;
        EEPROM.commit();
        if (failed_kill_feedback > 0)
        {
          int i;
          for (i = 0; i < failed_kill_feedback; i++)
          {
            int this_ID = failed_kill_ID[current_failed_read_pointer];
            int this_CLAN = failed_kill_CLAN[current_failed_read_pointer];
            dbc.uploadFailedFeedback(this_CLAN, this_ID);
            current_failed_read_pointer++;
            if (current_failed_read_pointer > 50)
              current_failed_read_pointer -= 50;
            failed_kill_feedback--;
            Serial.println("Upload Done");
          }
        }

        else
          delay(50);

        WiFi.disconnect();
        dbc.changeWiFiChannel(dbc.getClanWiFiChannel(CLAN));
      }

      else
      {
        delay(50);
      }
    }
  }
};

TreasureHuntPlayer PLAYER;