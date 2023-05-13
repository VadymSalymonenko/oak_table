void clearStrip(){
   /* for(int i = 0; i < TABLE_HEIGHT; i++){
        for(int j = 0; j < TABLE_WIDTH; j++){
            leds_matrix_brightness[i][j] = 0;
        }
    }*/
    strip.clear();
    strip_show();
    delay(4);
}
void clear_mode_switching(){
    for(int i = 0; i < TABLE_HEIGHT; i++){
        for(int j = 0; j < TABLE_WIDTH; j++){
            cell_light_status[i][j] = 0;
        }
    }
    newcell_timestamp = 0;
    add_colour_timestamp = 0;
    strip.clear();
    strip_show();
    delay(4);
}

void readLine(int line, int nextLine){
    current_line = line;
    static boolean current_line_power_status;
    if(current_line_power_status == false){digitalWrite(tranzistors_line[line],HIGH); delay(3);}
    for(int i = 0; i < TABLE_WIDTH; i++){
        buttons_matrix_status[line][i] = !digitalRead(ports_vertical_lines[i]);
    }
    digitalWrite(tranzistors_line[line],LOW);
    if(nextLine >= 0 && nextLine < TABLE_HEIGHT){ 
        digitalWrite(tranzistors_line[nextLine],HIGH);
        current_line_power_status = true;
    }else{current_line_power_status = false;}
}

void printMatrix(){
    for(int i = TABLE_HEIGHT - 1; i >= 0; i--){
        for(int j = 0; j < TABLE_WIDTH; j++){
            Serial.print(buttons_matrix_status[i][j]);
            Serial.print(", ");
        } 
        Serial.println("");
    }
    Serial.println("------------------------------------------");
}
void set_strip_cell(byte line, byte cell_in_line, mData led_info){
    if(line%2 == 0){ 
        strip.set(line * TABLE_WIDTH + cell_in_line, led_info);
    }else{ 
        strip.set((line+1) * TABLE_WIDTH - cell_in_line - 1, led_info);
    }
}
void strip_show(){
    Timer2.disableISR(CHANNEL_A);
    strip.show();
    Timer2.enableISR(); 
}


void mods_dispatcher(){
    effects_array[(currentMode.effect_num)]();

}


void init_start_mode(){
    read_EEPROM_mod_values(START_MODE_OF_TABLE, &currentMode);
}
void set_default_mods_parameters(){
    effect_info new_mod_info = {0,155,255,180,150,150};

    for(int i = 0; i < sizeof(effects_array)/sizeof(effects_array[0]); i++){
        update_default_mods_parameters(&new_mod_info);
        new_mod_info.effect_num++;
    }
    

    
}
void update_default_mods_parameters(effect_info* mod_info){
    byte adress = (mod_info->effect_num * sizeof(mod_info));
    EEPROM.update(adress++, mod_info->color);
    EEPROM.update(adress++, mod_info->saturation);
    EEPROM.update(adress++, mod_info->brightnes);
    EEPROM.update(adress++, mod_info->effect_speed);
    EEPROM.update(adress++, mod_info->effect_scale);
}

void read_EEPROM_mod_values(byte mod_num, effect_info* curr_effect){
    byte adress = (mod_num * sizeof(curr_effect));
    curr_effect->effect_num = mod_num;
    curr_effect->color = EEPROM.read(adress++);
    curr_effect->saturation = EEPROM.read(adress++);
    curr_effect->brightnes = EEPROM.read(adress++);
    curr_effect->effect_speed = EEPROM.read(adress++);
    curr_effect->effect_scale = EEPROM.read(adress++);
}

void print_EEPROM_mods(){
    for(int i = 0; i < sizeof(effects_array)/sizeof(effects_array[0]); i++){
        byte adress = (i * sizeof(currentMode));
        Serial.print("EEPROM mode №");
        Serial.println(i);     
        Serial.println(EEPROM.read(adress++));
        Serial.println(EEPROM.read(adress++));
        Serial.println(EEPROM.read(adress++));
        Serial.println(EEPROM.read(adress++));
        Serial.println(EEPROM.read(adress++));
    }
    
}
void change_curr_mode_left(){
    int new_mode_num = currentMode.effect_num - 1;  
    if(new_mode_num < 0)new_mode_num = (sizeof(effects_array)/sizeof(effects_array[0])) - 1;
    set_new_curr_mode(new_mode_num);
}

void change_curr_mode_right(){
    int new_mode_num = currentMode.effect_num + 1;  
    if(new_mode_num >= (sizeof(effects_array)/sizeof(effects_array[0]))) new_mode_num = 0;
    set_new_curr_mode(new_mode_num);
}

void set_new_curr_mode(byte mod_num){
    if(mod_num < 0 || mod_num >= (sizeof(effects_array)/sizeof(effects_array[0]))) return;
    
    update_default_mods_parameters(&currentMode);
    read_EEPROM_mod_values(mod_num,&currentMode);
    clear_mode_switching();
    effect_general_status = START_STATUS;
}

void change_value(boolean action,byte min1,byte max1,byte step1, byte* val,boolean border){
    if(action == INCREASE){
        if(border && *val >= max1 - step1){
            *val = max1; return;
        }
        *val += step1;
    }else{
        if(border && *val <= min1 + step1){
            *val= min1 ; return;
        }
        *val -= step1;
    }
}
