byte get_remote_controller_signal(){
    byte result_signal = 0;
    byte first_signal = 0;
    byte second_signal = 0;
    if (ir.available())  first_signal = ir.readCommand();
    if (ir2.available()) second_signal = ir2.readCommand();
        if(first_signal == 0x0){ result_signal = second_signal;
        }else{result_signal = first_signal;}
    return result_signal;
}

void remote_controller_dispatcher(byte result_signal){
    remote_controler_changes = true;
    switch(result_signal){
        case LINE1_CH_1:
            change_curr_mode_left();
            Serial.println("left");
        break;  
        case LINE1_CH_2:
            change_curr_mode_right();
            Serial.println("right");
        break;  
        case LINE2_MOVE_1:
            change_value(DECREASE,MIN_BRIGHTNESS,MAX_BRIGHTNESS,REMOTE_CONTROLL_STEP,&(currentMode.brightnes), true);
        break;  
        case LINE2_MOVE_2:
            change_value(INCREASE,MIN_BRIGHTNESS,MAX_BRIGHTNESS,REMOTE_CONTROLL_STEP,&(currentMode.brightnes), true);
        break;    
        case LINE3_1:
            change_value(DECREASE,0,255,REMOTE_CONTROLL_STEP,&(currentMode.color), false);
        break;
        case LINE3_2:
            change_value(INCREASE,0,255,REMOTE_CONTROLL_STEP,&(currentMode.color), false);
        break;
        case LINE4_1:
            change_value(DECREASE,0,255,REMOTE_CONTROLL_STEP,&(currentMode.effect_speed), true);
        break;
        case LINE4_2:
            change_value(INCREASE,0,255,REMOTE_CONTROLL_STEP,&(currentMode.effect_speed), true);
        break;
    }
    if(effect_general_status != START_STATUS) effect_general_status = PARAMETERS_CHANGED_STATUS;
}
