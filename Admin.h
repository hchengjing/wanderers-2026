void clearEEPROM(){
    int i;
    for (i = 0; i < EEPROM_SIZE; i++)
    {
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
};

class Admin
{
private:
    int admin_pwd[6] = {9, 1, 1, 0, 1, 2};  //to be changed every ENITIO (prevent seniors from editing)
    int user_key_in_pwd[6] = {};
    int currentDigitIndex = 0;
    int currentDigit = 0;
    int enteringPwdNav = 1;
    bool verified = false;
    int FunctionNav = 0;
    bool isConfirmingReset = false;
    int ConfirmingResetNav = 1;
    bool isSettingGL = false;
    int isGLNav = 1;
    bool isOverwritingID = false;
    int currentIDnum = 0;
    bool isCheckingScore = false;

public:    
    void handleJoystickMain()  //main admin screen
    {
        joystick_pos joystick_pos = Player_joystick.read_Joystick();
        if (Player_joystick.get_state() == 0)
        {
            switch (joystick_pos)  //joystick actions when viewing admin
            {
            case up:
                FunctionNav = max(FunctionNav - 1, 0);
                Player_joystick.set_state();
                break;

            case down:
                FunctionNav = min(FunctionNav + 1, 6);
                Player_joystick.set_state();
                break;

            case button:
                switch (FunctionNav)  //diff functions within admin, button means true (select)
                {
                case FactoryResetFunction:
                    isConfirmingReset = true;
                    break;

                case SetGLFunction:
                    isSettingGL = true;
                    break;

                case ToggleServerConnectivityFunction:
                    handleServerConnectivityToggle();
                    break;

                case OverwritePlayerIDFunction:
                    isOverwritingID = true;
                    break;

                case CheckScore:
                    isCheckingScore = true;
                    break;

                case setGameEnded:
                    handleGameEnded();
                    break;

                case ExitFunction:
                    currentProcess = MainMenuProcess;
                    verified = false;
                    FunctionNav = 0;
                    break;

                default:
                    break;
                }
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

    void handleJoystickPwd()  //joystick actions when putting in password
    {
        joystick_pos joystick_pos = Player_joystick.read_Joystick();
        if (Player_joystick.get_state() == 0)
        {
            switch (joystick_pos)
            {
            case up:  //from 0->9
                currentDigit++;
                if (currentDigit > 9)
                    currentDigit -= 10;
                if (currentDigit < 0)
                    currentDigit += 10;
                Player_joystick.set_state();
                break;

            case down:  //from 9->0
                currentDigit--;
                if (currentDigit > 9)
                    currentDigit -= 10;
                if (currentDigit < 0)
                    currentDigit += 10;
                Player_joystick.set_state();
                break;

            case left:  //back to main menu
                enteringPwdNav = max(enteringPwdNav - 1, 0);
                Player_joystick.set_state();
                break;

            case right:  //backspace
                enteringPwdNav = min(enteringPwdNav + 1, 2);
                Player_joystick.set_state();
                break;

            case button:
                switch (enteringPwdNav)
                {
                case 0:
                    currentProcess = MainMenuProcess;
                    enteringPwdNav = 1;
                    currentDigit = 0;
                    currentDigitIndex = 0;
                    break;

                case 1:
                    if (currentDigitIndex < 5)
                    {
                        //once user button, move on to next digit
                        user_key_in_pwd[currentDigitIndex] = currentDigit;
                        currentDigitIndex++;
                        currentDigit = 0;
                    }
                    else
                    {
                        user_key_in_pwd[currentDigitIndex] = currentDigit;
                        currentDigit = 0;
                        currentDigitIndex = 0;
                        int pwd_check = 1;
                        int i;
                        for (i = 0; i < 6; i++)
                        {
                            pwd_check *= (int)(user_key_in_pwd[i] == admin_pwd[i]);
                        }
                        if (pwd_check)
                        {
                            //digit correct
                            verified = true;
                        }
                        else
                        {
                            Admin_OLED.WrongPwdDisplay();
                            Player_Buzzer.sound(NOTE_E4);
                            delay(1500);
                            Player_Buzzer.end_sound();
                        }
                    }
                    break;

                case 2:
                    currentDigitIndex = max(currentDigitIndex - 1, 0);  //exceed max digit
                    currentDigit = 0;
                    enteringPwdNav = 1;
                    break;

                default:
                    break;
                }
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

    void handleJoystickConfirmReset()  //joystick actions once reset id
    {
        int player_id;
        joystick_pos joystick_pos = Player_joystick.read_Joystick();
        if (Player_joystick.get_state() == 0)
        {
            switch (joystick_pos)
            {
            case up:
                ConfirmingResetNav = max(ConfirmingResetNav - 1, 0);
                Player_joystick.set_state();
                break;

            case down:
                ConfirmingResetNav = min(ConfirmingResetNav + 1, 2);
                Player_joystick.set_state();
                break;

            case button:
                switch (ConfirmingResetNav)
                {
                case 0:  //no
                    isConfirmingReset = false;
                    ConfirmingResetNav = 1;
                    break;

                case 1:  //yes, clear id
                    StartUpDisplay();
                    clearEEPROM();
                    EEPROM.write(ONLINE_mode_add, 0); //set default OFFLINE mode
                    EEPROM.write(gameEnd_add, 0);
                    EEPROM.write(CLAN_add, 255);      //255 to set clan to unknown (default 0 is invicta)
                    EEPROM.commit();
                    ESP.restart();
                    break;

                case 2:  //yes, keep id
                    StartUpDisplay();
                    player_id = EEPROM.read(ID_add);
                    clearEEPROM();
                    EEPROM.write(ONLINE_mode_add, 0); //set default OFFLINE mode
                    EEPROM.write(gameEnd_add, 0);                    
                    EEPROM.write(CLAN_add, 255);      //255 to set clan to unknown (default 0 is invicta)
                    EEPROM.write(ID_add, player_id);
                    EEPROM.commit();
                    ESP.restart();
                    break;

                default:
                    break;
                }
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

    void handleJoystickSettingGL()  //choosing role
    {
        joystick_pos joystick_pos = Player_joystick.read_Joystick();
        if (Player_joystick.get_state() == 0)
        {
            switch (joystick_pos)
            {
            case up:
                isGLNav = max(isGLNav - 1, 0);
                Player_joystick.set_state();
                break;

            case down:
                isGLNav = min(isGLNav + 1, 2);
                Player_joystick.set_state();
                break;

            case button:
                EEPROM.write(ROLE_add, isGLNav);
                Admin_OLED.ConfirmSettingGLDisplay(isGLNav);

                // set HP and EN
                switch (isGLNav)
                {
                case freshman:
                    EEPROM.write(PLAYER_HP_add, PARTICIPANT_MaxHP);
                    EEPROM.write(PLAYER_EN_add, PARTICIPANT_MaxEN);
                    EEPROM.write(PLAYER_MaxHP_add, PARTICIPANT_MaxHP);
                    EEPROM.write(PLAYER_MaxEN_add, PARTICIPANT_MaxEN);
                    break;

                case groupLeader:
                    EEPROM.write(PLAYER_HP_add, GL_MaxHP);
                    EEPROM.write(PLAYER_EN_add, GL_MaxEN);
                    EEPROM.write(PLAYER_MaxHP_add, GL_MaxHP);
                    EEPROM.write(PLAYER_MaxEN_add, GL_MaxEN);
                    break;

                case sigma:
                    EEPROM.write(PLAYER_HP_add, SIGMA_MaxHP);
                    EEPROM.write(PLAYER_EN_add, SIGMA_MaxEN);
                    EEPROM.write(PLAYER_MaxHP_add, SIGMA_MaxHP);
                    EEPROM.write(PLAYER_MaxEN_add, SIGMA_MaxEN);
                    break;

                default:
                    break;
                }

                EEPROM.commit();
                isGLNav = 1;
                isSettingGL = false;
                delay(1500);
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

    void handleJoystickOverwriteID()  //change id
    {
        joystick_pos joystick_pos = Player_joystick.read_Joystick();
        if (Player_joystick.get_state() == 0)
        {
            int saved_clan;
            switch (joystick_pos)
            {
            case up:
                currentIDnum = max(currentIDnum - 1, 0);
                Player_joystick.set_state();
                break;

            case down:
                currentIDnum = min(currentIDnum + 1, 255);
                Player_joystick.set_state();
                break;

            case button:
                EEPROM.write(ID_add, currentIDnum);
                saved_clan = EEPROM.read(CLAN_add);
                Admin_OLED.ConfirmPlayerIDDisplay(saved_clan, currentIDnum);
                EEPROM.commit();
                isOverwritingID = false;
                delay(1500);
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

    void handleServerConnectivityToggle()  //online/offline mode
    {
        int newConnectivityMode;
        if (EEPROM.read(ONLINE_mode_add) == 0)
        {
            Serial.println("Switching from offline -> online mode");
            newConnectivityMode = 1;
        }
        else
        {
            Serial.println("Switching from online -> offline mode");
            newConnectivityMode = 0;
        }
        EEPROM.write(ONLINE_mode_add, newConnectivityMode);
        EEPROM.commit();
        WIFI_ON = newConnectivityMode;
        Admin_OLED.ConfirmConnectivityToggle(newConnectivityMode);
        delay(1500);
        Player_joystick.set_state();
    }

    void handleJoystickClanScore()  //check clan score
    {
        joystick_pos joystick_pos = Player_joystick.read_Joystick();
        if (Player_joystick.get_state() == 0)
        {
            switch (joystick_pos)
            {
            case button:
                isCheckingScore = false;
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

    void handleGameEnded()
    {
        int currentState = EEPROM.read(gameEnd_add);
        Serial.print("[DEBUG] Current gameEnd_add: "); Serial.println(currentState);

        int gameEndedState = (currentState == 0) ? 1 : 0;
        Serial.print("[DEBUG] New gameEndedState: "); Serial.println(gameEndedState);

        EEPROM.write(gameEnd_add, gameEndedState);
        bool commitSuccess = EEPROM.commit(); // Check if EEPROM write succeeded
        Serial.print("[DEBUG] EEPROM.commit() success: "); Serial.println(commitSuccess);

        int verifyState = EEPROM.read(gameEnd_add); // Read back to confirm
        Serial.print("[DEBUG] Verified gameEnd_add: "); Serial.println(verifyState);

        GAME_END_ON = gameEndedState;
        Admin_OLED.display_gameEnded(gameEndedState);
        delay(1500);
    }

    void AdminLoop()  //all admin functions
    {
        switch (verified)
        {
        case 0:
            handleJoystickPwd();
            Admin_OLED.display_EnteringPwd(currentDigitIndex, currentDigit, enteringPwdNav, user_key_in_pwd);
            break;

        case 1:
            if (isConfirmingReset)
            {
                handleJoystickConfirmReset();
                Admin_OLED.display_ConfirmingReset(ConfirmingResetNav);
            }
            else if (isSettingGL)
            {
                handleJoystickSettingGL();
                Admin_OLED.display_SettingGL(isGLNav);
            }
            else if (isOverwritingID)
            {
                handleJoystickOverwriteID();
                int previousID = EEPROM.read(ID_add);
                Admin_OLED.display_SettingID(previousID, currentIDnum);
            }
            else if (isCheckingScore)
            {
                handleJoystickClanScore();
                int invicta_score = EEPROM.read(POINT_INVICTA_add);
                int dynari_score = EEPROM.read(POINT_DYNARI_add);
                int ephilia_score = EEPROM.read(POINT_EPHILIA_add);
                int akrona_score = EEPROM.read(POINT_AKRONA_add);
                int solaris_score = EEPROM.read(POINT_SOLARIS_add);

                Admin_OLED.display_Checkingscore(invicta_score, dynari_score, ephilia_score,
                                                 akrona_score, solaris_score);
            }
            else
            {
                handleJoystickMain();
                Admin_OLED.display_MainAdmin(FunctionNav);
            }
            break;

        default:
            break;
        }
    }
};

Admin My_Admin;
