#include "defines.h"
#include "command_actions.h"
#include "comport_functions.h"
#include "remote.h"
#include "effects.h"

#include "GyverTimers.h"


typedef void (*action_function)(command*);
action_function function_array[]{
    action_get_status,
    action_get_second
};

typedef void (*effect_function)();
effect_function effects_array[]{
    fading_touch,
    drawing_mode,
    one_color_noise,
    multicolor_noise,
    separate_colors_noise,
    tap_game_1_mode
};


void setup() {
    Serial.begin(115200);             // opens serial port, sets data rate to 115200 baud

    attachInterrupt(0, irIsr1, FALLING);
    attachInterrupt(1, irIsr2, FALLING);
    attachInterrupt(5, powerOff, FALLING);

    strip.setBrightness(200);
    strip.oneLedIdle = 240; // сила тока в холостом режиме (мкА)
    strip.oneLedMax = 28;   // сила тока в рабочем режиме (мА)
    
    for(int i = 0; i < TABLE_HEIGHT; i++){ 
        pinMode(tranzistors_line[i],OUTPUT);
        digitalWrite(tranzistors_line[i],LOW);
    }
    for(int i = 0; i < TABLE_WIDTH; i++){ pinMode(ports_vertical_lines[i],INPUT_PULLUP);}
     
    #if set_default_settings_EEPROM==1
        set_default_mods_parameters();
    #endif
    
    delay(200);
    for(int i = 0; i < TABLE_HEIGHT; i++){ 
          if(i < TABLE_HEIGHT-1)readLine(i,i+1);
          if(i >= TABLE_HEIGHT-1)readLine(i,0);
    }
    strip.clear();
    strip.show(); 

    
    commands_list_init();
    is_command_execution_allowed = 1;
    init_start_mode();
    
    Timer2.setPeriod(1000);           
    Timer2.enableISR();    

   print_EEPROM_mods();
   color_palette = malloc(sizeof(mData)*COLOR_PALETTE_SIZE);
}


void loop() {
    byte comand_number = get_remote_controller_signal();
    if(comand_number != 0) remote_controller_dispatcher(comand_number);

    
    for(int i = 0; i < TABLE_HEIGHT; i ++){
        if(i < TABLE_HEIGHT - 1)readLine(i,i+1);
        if(i == TABLE_HEIGHT - 1)readLine(i,0);
        mods_dispatcher();
        #ifdef DEBUGMODE 
            count_circles++;
        #endif
    }


    
    #ifdef DEBUGMODE  
            count_frames++;
        if(millis() - lastTimeDebug >= 1000){
            lastTimeDebug = millis();
            Serial.print(count_frames);
            Serial.print(" fps; ");
            Serial.print(count_circles);
            Serial.println(" lps; "); 
            count_frames = 0;
            count_circles = 0;
        }
    #endif
    
} 
void irIsr1() {
  ir.tick();
}
void irIsr2() {
  ir2.tick();
}

void powerOff() {
    update_default_mods_parameters(&currentMode);
}

ISR(TIMER2_A) {
      Timer2.disableISR(CHANNEL_A);
      Timer2.stop();
      if(Serial.available()) {interrupts_count +=1;}  
      if(interrupts_count == (decode_buffer_size/12 + 1)){
          interrupts_count = 0;
          
          volatile byte result = 0;
          boolean overflow_marker = false;
          byte input_command_lenth = 0;
          char* input_buf = malloc(input_buffer_size);
          
          clean_buffer(input_buf,input_buffer_size);
          read_input_data(input_buf, input_command_lenth);
           
          byte decode_command_lenth = 0;
          char* decode_buf = malloc(input_command_lenth);
          char* decode_request_number = malloc(command_number_size);
          char* decode_arguments_str = malloc(arguments_str_size);


          clean_buffer(decode_buf,input_command_lenth);
          decode_command_lenth = decode_command(input_buf, decode_buf, input_command_lenth, decode_request_number, decode_arguments_str);
          free(input_buf); 

          
           
          result = get_command_number(decode_buf,decode_command_lenth);
                

          if(result != 255)overflow_marker = add_command_to_list(decode_request_number, result, decode_arguments_str);  
          print_general_answer(result,overflow_marker, decode_request_number);

          free(decode_buf); 
          if(result == 255 || overflow_marker){
              free(decode_request_number);
              free(decode_arguments_str);
              
          }  
      }
      if(command_list[0]->embedded_command_number != 255 && is_command_execution_allowed == 1){
          commands_dispatcher();
          commands_list_update();  
      }
    Timer2.resume();
    Timer2.enableISR();               
    
}
