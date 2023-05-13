

void action_get_status(command* command1){
    char temp_buf[] = "   |active";
    for(byte i = 0; i < command_number_size; i++) temp_buf[i] = command1->request_number[i];
    code_and_print(temp_buf, 10);
}


void action_get_second(command* command1){
    char temp_buf[15];
    byte size_of_buf = 4;
    for(byte i = 0; i < command_number_size; i++) temp_buf[i] = command1->request_number[i];
    temp_buf[3] = command_number_separator;
    size_of_buf = str_builder_long(temp_buf, size_of_buf, 0, ( (long)(millis()/1000)));
    code_and_print(temp_buf, size_of_buf);
}









boolean add_command_to_list(char* decode_request_number, byte command_number, char* decode_arguments_str){
    for(int i = 0; i <= command_list_size;i++){
        if(i == command_list_size)return (true);
        if(command_list[i]->embedded_command_number == 255){      
            //command new_command;
            command_list[i]->request_number = decode_request_number;
            command_list[i]->embedded_command_number = command_number;
            command_list[i]->arguments_str = decode_arguments_str;
            delay(1);
            return false;
        }
    }
    return true;
}


void commands_dispatcher(){
    if(is_command_execution_allowed == 1){      
        is_command_execution_allowed = 0;
        function_array[(command_list[0]->embedded_command_number)](command_list[0]);
    
        is_command_execution_allowed = 1;
        free(command_list[0]->request_number);
        free(command_list[0]->arguments_str);
       
        command_list[0]->embedded_command_number = 255;  
    }
}

char swap_tens_ones(char ch) {
    int ones = ch % 10;
    int tens = ((ch % 100) - ones) / 10;
    char fin_ch;

    fin_ch = (ch - tens*10 - ones) + (ones*10 + tens);
    return fin_ch;
}

int code_data(char* data, int len) {
  int i;
    int key;

  for (i = 0; i < len; i++) {
        data[i] -= ' ';
        if (i%2 == 0) key = data[i] % 20;

        if(data[i] > 100)
            data[i] = -1* (data[i] - 100);
        
        if (i%2 == 0) {
            char ch = data[i];
            char swap_ch = swap_tens_ones(ch);

            data[i] = swap_ch;
        }
        else {
            char ch = data[i];
            char swap_ch = swap_tens_ones(ch);
            char coded_swap_ch = swap_ch - key;

            data[i] = coded_swap_ch;
        }
    }
    return 0;
}
void code_and_print(char* str, byte size_str){
    Serial.print('>');
    #if code_info==1
        code_data(str, size_str);
    #endif
    for(byte i = 0; i < size_str; i++){
        Serial.print(str[i]);
    }
    Serial.print('<');
}

byte str_builder_long(char* main_buf, byte start_point, boolean param_symbol, long num){
    byte num_size = 1;
    if(num != 0){
        for(byte i = 9; i >= 0; i--){
            if( (num/((long) pow(10,i))) != 0){ num_size = i+1; break;}
        }
    }
    char number_array[11];
    ltoa(num, number_array, 10);
    if(num < 0){num_size++;}

    byte total_lenth = start_point + num_size;
    if(param_symbol){
        main_buf[start_point] = arguments_separator;  
        total_lenth++;
        start_point++;
    }
    for(int i = 0; i < num_size; i++){
        main_buf[start_point + i] = number_array[i];
    }
    return total_lenth;
}

byte str_builder_double(char* main_buf, byte start_point, boolean param_symbol, double num, byte rest_symbols_num){
    if(rest_symbols_num == 0) rest_symbols_num = 1;
    if(rest_symbols_num > 5) rest_symbols_num = 5;
    long main_part = ( (long) num);
    long rest_part = ((long) ((num-main_part)*pow(10,rest_symbols_num)));

    if(num < 0) rest_part = rest_part * (-1);
    byte total_size = 0;

    total_size = str_builder_long(main_buf, start_point, param_symbol, main_part);
    main_buf[total_size] = double_separator;
    total_size++;
    total_size = str_builder_long(main_buf, total_size, 0, rest_part);
    return total_size;
    
}
void print_general_done_str(command* command1){
    char temp_buf[] = "   |done";
    for(byte i = 0; i < command_number_size; i++) temp_buf[i] = command1->request_number[i];
    code_and_print(temp_buf, 8);
}

void commands_list_update(){        
    for(byte i = 0;i < command_list_size-1; i++){
        if(command_list[i]->embedded_command_number == 255 && command_list[i+1]->embedded_command_number != 255){
            command_list[i] = command_list[i+1]; 
            command_list[i+1] = NULL;
        }       
    }
}
void commands_list_init(){
    for(byte i = 0;i < command_list_size; i++){
          command_list[i] = new command;
          command_list[i]->embedded_command_number = 255;
    }
}
