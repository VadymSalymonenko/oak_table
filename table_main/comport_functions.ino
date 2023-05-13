//
void read_input_data(char* input_buf, byte &command_size){
    boolean continue_permit = 1;
    byte i = 0;
    char current_symbol = ' ';
  //  delay(decode_buffer_size/12 + 1); // минимальное время получения 48 бит -- 4. 1 миллисекунда = 12 символов +-. 
    while (Serial.available() > 0) { // цикл записи данных в буфер 
      if(i == input_buffer_size) break;
      current_symbol = Serial.read();  // считывание
      if(current_symbol == '<') {
        while (Serial.available() > 0) {
            current_symbol = Serial.read();   
        }
        return;
      }
      if(current_symbol == '>') {continue_permit = 0; continue;}
      if(continue_permit == 1){
          continue;
      }else{
          if( current_symbol == '\n') {break;} // current_symbol == ' ' ||
          input_buf[i] = current_symbol;
          command_size++;
          i++; 
      }
       
  } 
}

int decode_data(char* data, int len) {
  int i;
    int key;

  for (i = 0; i < len; i++) {
        if (i%2 == 0) {
            char ch = data[i];
            char swap_ch = swap_tens_ones(ch);

            data[i] = swap_ch;
            if(data[i] < 0)
                data[i] = (-1*data[i]) + 100;
            key = data[i] % 20;
        }
        else {
            char ch = data[i];
            char decoded_ch = ch + key;
            char decoded_swap_ch = swap_tens_ones(decoded_ch);

            data[i] = decoded_swap_ch;
            if(data[i] < 0)
                data[i] = (-1*data[i]) + 100;
        }
        data[i] += ' ';
  }

  return 0;
}

byte decode_command(char* input, char* decode_buf, byte input_size, char* decode_request_number, char* decode_arguments_str){

    #if code_info==1
        decode_data(input, input_size+1);
    #endif
    clean_buffer(decode_request_number,command_number_size);
    clean_buffer(decode_arguments_str,arguments_str_size);
    byte i = 0;
    byte size_main_buf = 0;
    for(; i < command_number_size; i++){ decode_request_number[i] = input[i];}
    if(input[i] == command_number_separator) i++;
    for(byte b = 0; i < input_size; i++, b++){ 
        if(input[i] == arguments_separator) break;
        decode_buf[b] = input[i];
        size_main_buf++;
    }
    if(i < input_size){
        for(byte b = 0; i < input_size; i++, b++) decode_arguments_str[b] = input[i];
    }
    return size_main_buf;
}
void clean_buffer(char* buf, byte size_buf){
    for(byte i =0; i < size_buf; i++){
        buf[i] = ' ';
    }
}

boolean cmp_buf_to_memory(char* decode_buf, char* command_to_compare, byte buf_size){
    if(buf_size == strlen(command_to_compare)){
        for(byte i = 0; i < strlen(command_to_compare); i++){
            if(decode_buf[i] != command_to_compare[i]){return false;}
        }
        return true;
    }
      return false;
}
int get_command_number(char* decode_buf, byte buf_size){
    for(int i = 0; i < sizeof(list_of_all_commands)/sizeof(list_of_all_commands[0]); i++){
        if(cmp_buf_to_memory(decode_buf, list_of_all_commands[i], buf_size))return i; 
    } 
    return 255;

}
void print_general_answer(byte result, boolean overflow_marker, char* decode_request_number){
    if(result == 255) { 
      char temp_buf[12];
        for(byte i = 0; i < command_number_size; i++)temp_buf[i] = decode_request_number[i];
        temp_buf[3] = command_number_separator;   
        for(byte i = 4; i < 4 + sizeof(error_command)-1; i++)temp_buf[i] = error_command[i-4];
        code_and_print(temp_buf,(4 + sizeof(error_command)-1));//Serial.println();

    }else{
        if(overflow_marker){ 
        code_and_print(overflow_command, (sizeof(overflow_command)-1)); return;}
        //print_normal_work_message(); 
        //Serial.println();           
    }      
}
