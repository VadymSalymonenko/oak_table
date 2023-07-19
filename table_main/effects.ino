void fading_touch(){
    byte delayBetweenCycles;
    if(currentMode.brightnes >= FT_MAX_BRIGHTNESS)currentMode.brightnes = FT_MAX_BRIGHTNESS;
    if(currentMode.brightnes <= FT_MAX_BRIGHTNESS_LOW)currentMode.brightnes = FT_MAX_BRIGHTNESS_LOW;
    if(currentMode.effect_speed <= FT_MIN_SPEED)currentMode.effect_speed = FT_MIN_SPEED;
    decreasing_step = map(currentMode.effect_speed, 0, 255, FT_DECREASING_MAX_STEP, 3);

    delayBetweenCycles = TOTAL_SHINE_TIME / ((currentMode.brightnes - FT_MIN_BRIGHTNESS)*decreasing_step);
    for(int i = 0; i < TABLE_WIDTH; i++){
        if(buttons_matrix_status[current_line][i]){
 
            set_strip_cell(current_line, i, mHSV(currentMode.color,currentMode.saturation,currentMode.brightnes));

            cell_light_status[current_line][i] =  currentMode.brightnes;
        }
        if(buttons_matrix_status[current_line][i] == 0){        
            if(cell_light_status[current_line][i] >= FT_MIN_BRIGHTNESS + decreasing_step){
                cell_light_status[current_line][i] -= decreasing_step;
                set_strip_cell(current_line, i, mHSV(currentMode.color,currentMode.saturation, (cell_light_status[current_line][i])));
            }
            if(cell_light_status[current_line][i] != 0 && cell_light_status[current_line][i] < FT_MIN_BRIGHTNESS + decreasing_step){
                set_strip_cell(current_line, i, mRGB(0, 0,0));
                cell_light_status[current_line][i] = 0;
            }
                      
        }
    }
      if(current_line == TABLE_HEIGHT-1){
          strip_show();
      }
      delayMicroseconds(1800);
      delayMicroseconds((long)1000*(currentMode.effect_speed%(255/FT_DECREASING_MAX_STEP))/(255/FT_DECREASING_MAX_STEP));

}

//void drawing_mode_update();
//void drawing_mode_execute();

void drawing_mode(){
      for(int i = 0; i < TABLE_WIDTH; i++){
        if(cell_light_status[current_line][i] == 0){  
            if(buttons_matrix_status[current_line][i]){ 
                set_strip_cell(current_line, i, mHSV(currentMode.color,currentMode.saturation,currentMode.brightnes));
                cell_light_status[current_line][i] = 1;
            }
        }
        if(cell_light_status[current_line][i] == 1 || cell_light_status[current_line][i] == 2){  
            set_strip_cell(current_line, i, mHSV(currentMode.color,currentMode.saturation,currentMode.brightnes));
        }
        if(buttons_matrix_status[current_line][i] == 0 && cell_light_status[current_line][i] == 1){   
            cell_light_status[current_line][i] = 2;
        }
        if(buttons_matrix_status[current_line][i] && cell_light_status[current_line][i] == 2){  
            set_strip_cell(current_line, i,  mRGB(0, 0,0));
            cell_light_status[current_line][i] = 3; 
        }
        if(buttons_matrix_status[current_line][i] == 0 && cell_light_status[current_line][i] == 3){   
            cell_light_status[current_line][i] = 0;
        }            
    }
      if(current_line == TABLE_HEIGHT-1 ){
          strip.show();
      }
	  if(current_line == TABLE_HEIGHT-1){
		  delay(5); // git merge test;
	  }
      delay(2);
}

//----------------------------------------------\\
//----------------------------------------------\\
//---------------full_screen_noise--------------\\
//----------------------------------------------\\
//----------------------------------------------\\

void one_color_noise(){
    if(currentMode.brightnes >= OCN_MAX_BRIGHTNESS)currentMode.brightnes = OCN_MAX_BRIGHTNESS;
    if(currentMode.brightnes <= OCN_MAX_BRIGHTNESS_LOW)currentMode.brightnes = OCN_MAX_BRIGHTNESS_LOW;  
    full_screen_noise_dispatcher(ONE_COLOR_1);
}
void multicolor_noise(){
    if(currentMode.brightnes >= MN_MAX_BRIGHTNESS)currentMode.brightnes = MN_MAX_BRIGHTNESS;
    if(currentMode.brightnes <= MN_MAX_BRIGHTNESS_LOW)currentMode.brightnes = MN_MAX_BRIGHTNESS_LOW;
    if(currentMode.effect_scale <= MN_MIN_COLOR_RANGE)currentMode.effect_scale = MN_MIN_COLOR_RANGE;
    
    full_screen_noise_dispatcher(MULTICOLOR_1);
}
void separate_colors_noise(){
    if(currentMode.brightnes >= SCN_MAX_BRIGHTNESS)currentMode.brightnes = SCN_MAX_BRIGHTNESS;
    if(currentMode.brightnes <= SCN_MAX_BRIGHTNESS_LOW)currentMode.brightnes = SCN_MAX_BRIGHTNESS_LOW;
    full_screen_noise_dispatcher(SEPARATE_COLORS_1);
}


void full_screen_noise_dispatcher(byte type_palette_generator){
    if(effect_general_status == START_STATUS){
        init_full_screen_noise(type_palette_generator);
        effect_general_status = WORK_STATUS;
    }
    if(effect_general_status == PARAMETERS_CHANGED_STATUS){
        generate_color_palette(type_palette_generator);
        effect_general_status = WORK_STATUS;
    }
    if(effect_general_status == WORK_STATUS){
        full_screen_noise();
    }
}

void full_screen_noise(){
  
    byte counter_step = map(currentMode.effect_speed, 0, 255, FSN_COUNTER_STEP_MAX, FSN_COUNTER_STEP_MIN);

    Serial.print("currentMode.effect_speed == ");
    Serial.println(currentMode.effect_speed);
    Serial.print("counter_step == ");
    Serial.println(counter_step);
    Serial.print("micro ==  ");
    Serial.println((long)FSN_CHANGED_DELAY*(currentMode.effect_speed%(255/FSN_COUNTER_STEP_MAX))/(255/FSN_COUNTER_STEP_MAX));
    
    if(current_line == TABLE_HEIGHT-1 ){
 
    byte min_noise = 255;
    byte max_noise = 0;
    for(int i = 0; i < TABLE_HEIGHT; i++){
        for(int j = 0; j < TABLE_WIDTH; j++){
            byte noise = inoise8(i * 50, j * 50, counter);  
            if(noise < min_noise) min_noise = noise; 
            if(noise > max_noise) max_noise = noise; 
            cell_light_status[i][j] = noise;
            //Serial.print(noise);
           // Serial.print(", ");
        }
      //Serial.println();
    }
    //Serial.println("-------------------------");

    for(int i = 0; i < TABLE_HEIGHT; i++){
        for(int j = 0; j < TABLE_WIDTH; j++){
            byte final_kef = map( cell_light_status[i][j],min_noise, max_noise, 0, 255);
            set_strip_cell(i, j, get_noise_color(final_kef));
        }
    }
    counter+=counter_step;
    strip_show();
    }
    delayMicroseconds(1000 + (long)FSN_CHANGED_DELAY*(currentMode.effect_speed%(255/FSN_COUNTER_STEP_MAX))/(255/FSN_COUNTER_STEP_MAX));
}

mData get_noise_color(int val) {
    return color_palette[map(val,0,255,0,COLOR_PALETTE_SIZE-1)];
}
void init_full_screen_noise(byte type_palette_generator){
    //color_palette = malloc(sizeof(mData)*COLOR_PALETTE_SIZE);
    generate_color_palette(type_palette_generator); 
}
void generate_color_palette(byte type_palette_generator){
    switch(type_palette_generator){
        case ONE_COLOR_1:
            one_color_generator();
            break;  
        case MULTICOLOR_1:
            multicolor_generator();
            break;  
        case SEPARATE_COLORS_1:
            separate_colors_generator();
            break;            
    }
}
void one_color_generator(){
for(byte i = 0; i < COLOR_PALETTE_SIZE; i++){
    //color_palette[i] = mHSV(50+i,255,150+i/2);
    byte temp_brightnes = OCN_MIN_BRIGHTNESS + (MAX_BRIGHTNESS - OCN_MIN_BRIGHTNESS)*i/COLOR_PALETTE_SIZE;
        
    color_palette[i] = mHSV( (currentMode.color + (unsigned int)OCN_COLOR_RANGE*i/COLOR_PALETTE_SIZE),
                              255 - (unsigned int)50*i/COLOR_PALETTE_SIZE,
                              (long)temp_brightnes*currentMode.brightnes/255);
    }
}
void multicolor_generator(){
    for(byte i = 0; i < COLOR_PALETTE_SIZE; i++){
    //color_palette[i] = mHSV(50+i,255,150+i/2);
    byte temp_brightnes = MN_MIN_BRIGHTNESS + (MAX_BRIGHTNESS - MN_MIN_BRIGHTNESS)*i/COLOR_PALETTE_SIZE;
        
    color_palette[i] = mHSV( (currentMode.color + (unsigned int)currentMode.effect_scale*i/COLOR_PALETTE_SIZE),
                              255,
                              (long)temp_brightnes*currentMode.brightnes/255);
    }
}
void separate_colors_generator(){
    for(byte i = 0; i < COLOR_PALETTE_SIZE; i++){
      
        
        color_palette[i] = mHSV( (50 + (unsigned int)140*i/COLOR_PALETTE_SIZE),
                                 currentMode.saturation,
                                currentMode.brightnes);  
                                    
    /*byte temp_brightnes = MN_MIN_BRIGHTNESS + (MAX_BRIGHTNESS - MN_MIN_BRIGHTNESS)*i/COLOR_PALETTE_SIZE;
        
    color_palette[i] = mHSV( 30 + 180/(  ((unsigned int)SCN_COLORS_NUM*i/COLOR_PALETTE_SIZE)+1),
                              255,
                              (long)temp_brightnes*currentMode.brightnes/255);
    */
    }
}


//----------------------------------------------\\
//----------------------------------------------\\
//----------------tap_game_1_mode---------------\\
//----------------------------------------------\\
//----------------------------------------------\\
void tap_game_1_end_game();
void tap_game_1_mode_execute();
long scores = 0;

void tap_game_1_mode(){
    byte buttons_pressed = 0;
    if(!effect_general_status){
        tap_game_1_mode_execute();
    } else {
        for(byte i = 0; i < TABLE_HEIGHT; i++){
            for(byte j = 0; j < TABLE_WIDTH; j++){
                if(buttons_matrix_status[i][j] > 0)
                    buttons_pressed++;
            }
        }
        if(buttons_pressed >= 4){
            strip.clear();
            strip_show();
            effect_general_status = false;
        }
    }
}

void tap_game_1_mode_execute(){
    int newcell_duty_cycle, colour_duty_cycle = 0;
    byte lighted_cells = 0;
   


    //Serial.print("scores -- ");
    //Serial.println(scores);
    delay(2);
    if(buttons_matrix_status[1][4] == 1){
        Serial.println("BBBBBBBBBB");
    }

    

    //set random cell every x seconds.
    if((1500 - scores*30) >= 300)
        newcell_duty_cycle = (1500 - scores*30); // TODO: normal exponential logic.

    if(millis() - newcell_timestamp >= newcell_duty_cycle){
        byte random_cell_in_line = random(0, TABLE_WIDTH-1);
        byte random_line = random(0, TABLE_HEIGHT-1);
        Serial.print(random_line);
        Serial.print(", ");
        Serial.println(random_cell_in_line); 
        mData mhsv = mHSV(120, currentMode.saturation, currentMode.brightnes);
        set_strip_cell(random_line, random_cell_in_line, mhsv);
        cell_light_status[random_line][random_cell_in_line] = 120;
        newcell_timestamp = millis();
    }
    
    // Check if user pinned lighted cell:
    for(byte i = 0; i < TABLE_HEIGHT; i++){
      for(byte j = 0; j < TABLE_WIDTH; j++){
        if(buttons_matrix_status[i][j] == 1){
            if(cell_light_status[i][j] > 0){
                scores += 2; //TODO: logic with different scores depending on colour and total scores.
                set_strip_cell(i, j, mHSV(0, 0, 0)); //turn off light
                cell_light_status[i][j] = -1;
                Serial.print("Pressed ");
                Serial.print(i);
                Serial.print(", ");
                Serial.println(j); 
            }
            if(cell_light_status[i][j] == 0) {  
              Serial.println("aaaaaaaaaaaaaaaaaa");  
                if(scores > 0)scores -= 1; //TODO: logic with different scores depending on colour and total scores.
            }
        } else if(cell_light_status[i][j] < 0) {
              cell_light_status[i][j] = 0;
        }

        if(cell_light_status[i][j] > 0){
            lighted_cells++;
        }

      }
    }
    colour_duty_cycle = 20;
    if(millis() - add_colour_timestamp >= colour_duty_cycle){
            mData mhsv;
            for(byte i = 0; i < TABLE_HEIGHT; i++){
                for(byte j = 0; j < TABLE_WIDTH; j++){
                    if(cell_light_status[i][j] > 1){ 
                        cell_light_status[i][j] -= 1;
                        mhsv = mHSV(cell_light_status[i][j], currentMode.saturation, currentMode.brightnes);
                        set_strip_cell(i, j, mhsv);
                    }
                }
            }
            add_colour_timestamp = millis();
    }


    if(lighted_cells >= tap_game_1_LOOSE_LIM)
        tap_game_1_end_game();


    if(current_line == TABLE_HEIGHT-1){
        strip_show();
    }
}

void tap_game_1_end_game(){
    // TODO: normal animation.
    Serial.println("end");
    for (int i = 0; i < TABLE_WIDTH*TABLE_HEIGHT; i++) {
        strip.set(i, mHSV(1, 255, 200));
    }
    strip_show();
    delay(1000);

    // TODO: show scores.

    effect_general_status = true;
}
