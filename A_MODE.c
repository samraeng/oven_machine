VOID AUTO_(VOID)
{lcd_gotoxy(1,0);
  lcd_putc("                    "); 
  lcd_gotoxy(2,1);
  lcd_putc("                    "); 
  
  //lcd_gotoxy(1,0);
  //lcd_putc("AUTO");
  flg_manual=0;  
  WHILE(!input(sw_auto))
  { // chk_lcd();
  //=============display current time==
     read_time();
     //output_toggle(RELAY1);
     delay_ms(100);
     get_sec(ds1307_regs[0]);
     get_min(ds1307_regs[1]);
     get_hr(ds1307_regs[2]);
     get_date(ds1307_regs[4]);
     get_month(ds1307_regs[5]);
     get_year(ds1307_regs[6]);
     
   // restart_wdt();


      lcd_gotoxy(1,1);
      lcd_putc(NUM[hr10]);
      //lcd_gotoxy(1,5);
      lcd_putc(NUM[hr00]); 
      lcd_putc(":");
      //=====================
      lcd_gotoxy(1,4);
      lcd_putc(num[min10]);
      delay_us(10);
      lcd_gotoxy(1,5);
      lcd_putc(num[min00]);
      lcd_putc(":");
     //=======================
       lcd_gotoxy(1,7);
      lcd_putc(num[sec10]);
      lcd_putc(num[sec00]);  
  //=============set stop time=========
      if(!input(sw_set))
      {
         delay_ms(300);
          if(!input(sw_set))
          {
              while(!input(sw_set)){restart_wdt();}
              flg_set_time=1;
              set_time_auto();
              
          }
      }
  //===========  set high low  temp====
    if(!input(sw_up))
    {
        delay_ms(300);
        if(!input(sw_up))
        {
          while(!input(sw_up)){restart_wdt();}
          flg_settemp=1;
          set_temp();
        }
        
    }
  //==========start process============
      if(!input(sw_prg))
      {
         delay_ms(300);
          if(!input(sw_prg))
          {
              while(!input(sw_prg)){restart_wdt();}
              flg_auto=1;
             
              start_auto();
          }
      }  
 
 
                    
               
             

                  
          
  }// exit while(sw_auto))  STOP= auto mode
                flg_MODE=0;FLG_SET_PRG=0;N_MODE=2;
                LCD_CLR();
                output_low(relay1);output_low(relay2);output_low(relay3);
                output_low(relay4);output_low(relay5);output_low(relay6);  
}
void set_time_auto(void)
{
     read_time();
     //output_toggle(RELAY1);
     delay_ms(100);
     get_sec(ds1307_regs[0]);
     get_min(ds1307_regs[1]);
     get_hr(ds1307_regs[2]);
        
     get_date(ds1307_regs[4]);
     get_month(ds1307_regs[5]);
     get_year(ds1307_regs[6]);
     //========= display current time==
      lcd_gotoxy(1,1);
      lcd_putc(NUM[hr10]);
      //lcd_gotoxy(1,5);
      lcd_putc(NUM[hr00]); 
      lcd_putc(":");
      //=====================
      lcd_gotoxy(1,4);
      lcd_putc(num[min10]);
      delay_us(10);
      //lcd_gotoxy(1,5);
      lcd_putc(num[min00]);
      lcd_putc(":");
     //=======================
       lcd_gotoxy(1,7);
      lcd_putc(num[sec10]);
      lcd_putc(num[sec00]);
      //===========read stop time====
      stop_hr=read_eeprom(4);stop_min=read_eeprom(3);
        lcd_gotoxy(1,11);lcd_putc("to");
      //=========== display previod time===
      
      hex_bcd(stop_hr);
      lcd_gotoxy(1,14);
      lcd_putc(num[a]);
      lcd_putc(num[b]);
      
      lcd_putc(":");//1,16
      
      hex_bcd(stop_min);
      lcd_gotoxy(1,17);
      lcd_putc(num[a]);
      lcd_putc(num[b]);      

  while(flg_set_time)
        {
          //=============set position========
          if(!input(sw_set))
          {  delay_ms(101);
            if(!input(sw_set))
            {            
             while(!input(sw_set)){restart_wdt();}
             
             

               if(set_count==0)
               {
                lcd_gotoxy(1,16);
               }
               if(set_count==1)
               {
                lcd_gotoxy(1,13);
               }
                set_count++;
                if(set_count>1)set_count=0;
            }    
          }
          //=====================set min==============================
                  if(set_count==0 && !input(sw_up) && input(sw_down))
                  {
                    delay_ms(101);
                    if(set_count==0 && !input(sw_up) && input(sw_down))
                    {
                     
                     if(stop_min<60 && stop_min>=0)
                     {
                       stop_min++;                      
                       hex_bcd(stop_min);
                       lcd_gotoxy(1,17);
                       lcd_putc(num[a]);
                       lcd_putc(num[b]);
                       write_eeprom (3,stop_min);
                     }
                    }
                  }
          //=========================        
                  if(set_count==0 && input(sw_up) && !input(sw_down))
                  {
                    delay_ms(101);
                    if(set_count==0 && input(sw_up) && !input(sw_down))
                    {
                     if(stop_min>=1)
                     {
                      stop_min--;
                     
                      hex_bcd(stop_min);
                      lcd_gotoxy(1,17);
                      lcd_putc(num[a]);
                      lcd_putc(num[b]);
                      write_eeprom (3,stop_min);
                     } 
                    
                    }
                  } 
          
          //========================
                  if(set_count==1 && !input(sw_up) && input(sw_down))
                  {
                    delay_ms(101);
                    if(set_count==1 && !input(sw_up) && input(sw_down))
                    {
                     if( stop_hr<24 &&  stop_hr >= 0)
                     {
                       stop_hr++;
                       
                       hex_bcd(stop_hr);
                       lcd_gotoxy(1,14);
                       lcd_putc(num[a]);
                       lcd_putc(num[b]);
                       write_eeprom (4,stop_hr);
                     }
                    
                    }
                  }
          //=========================
                  if(set_count==1 && input(sw_up) && !input(sw_down))
                  {
                    delay_ms(101);
                    if(set_count==1 && input(sw_up) && !input(sw_down))
                    {
                      if( stop_hr>=1)
                      {
                      stop_hr--;
                    
                      hex_bcd(stop_hr);
                      lcd_gotoxy(1,14);
                      lcd_putc(num[a]);
                      lcd_putc(num[b]);
                      write_eeprom (4,stop_hr);
                      }
                    
                    }
                  }
          //===================exit set time on=====================
                 if(!input(sw_up) && !input(sw_down))
                 {
                   delay_ms(100);
                    
                    if(!input(sw_up) && !input(sw_down))
                    {
                     while(!input(sw_up) && !input(sw_down)){restart_wdt();}
                     flg_set_time=0;write_eeprom (2,0);
                    } 

                  
                 }


                        

        }
//=======SEC/MIN/HOURS/DATE/MONT/YEAR        
   //set_time(00,min,hr,28,12,04);write_eeprom (2,0);
  // lcd_gotoxy(2,1);
  // lcd_putc("                    "); 
}  
void start_auto(void)
{
           output_high(relay3);// on value gas
           delay_ms(4000);
           output_high(relay2);// on ignition tr
                    
           delay_ms(4000);
           
           delay_ms(2000);

           output_low(relay2);
           delay_ms(3000);
           output_high(relay1);//on pump    
           output_high(relay4);//rotor  palate on
           flg_lowtemp=1;
           
           FLG_BLOWER=0;
           LOOP_BLOWER=0;        
           flg_tempover=0;
          // flg_rotor_stop=0;
    
set_adc_channel(1);
delay_us(10);
t_break1 = read_adc();

t_break1 *=225;
t_break1 += 15360;
t_break1 /= 1024;

t_break = t_break1;
        
        
        
       stop_min=read_eeprom(3);stop_hr=read_eeprom(4); 
       lowtemp=read_eeprom(5);
       hightemp=read_eeprom(6);
       
      lcd_gotoxy(2,9); lcd_putc("  ");
      
      hex_bcd_temp(lowtemp);

      lcd_gotoxy(2,11);lcd_putc(num[t1]);lcd_putc(num[t2]);
      lcd_putc(num[t3]);
      
      lcd_gotoxy(2,14);lcd_putc("<");
      lcd_gotoxy(2,15);lcd_putc("T");
      lcd_gotoxy(2,16);lcd_putc("<");
      
      hex_bcd_temp(hightemp);

      lcd_gotoxy(2,17);lcd_putc(num[t1]);lcd_putc(num[t2]);
      lcd_putc(num[t3]);
      
      
      flg_lowtemp=0;
      
        set_adc_channel(0);
        delay_us(10);
        read_temp1 = read_adc();    
      
        set_adc_channel(0);
        delay_us(10);
        read_temp1 = read_adc();
        
        set_adc_channel(0);
        delay_us(10);
        read_temp1 = read_adc();  
        
        set_adc_channel(0);
        delay_us(10);
        read_temp1 = read_adc();
        
        set_adc_channel(0);
        delay_us(10);
        read_temp1 = read_adc();        
      
    while(flg_auto && !input(sw_auto))
    {restart_wdt();
//================================ON OFF BLOWER=====

/*IF(FLG_BLOWER==0 && flg_tempover==0)
{
  LOOP_BLOWER++;
  IF(LOOP_BLOWER>500)
  {
      output_high(relay6);//BLOWER ON
      LOOP_BLOWER=0;
      FLG_BLOWER=1;
  }
}

IF(FLG_BLOWER==1 && flg_tempover==0)
{
   LOOP_BLOWER++;
   IF(LOOP_BLOWER>90)
   {
      OUTPUT_LOW(RELAY6);
      LOOP_BLOWER=0;
      FLG_BLOWER=0;
   }
}*/
//////////////////////////////on blower////////////////
       if(!input(sw_blower))
       {
          delay_ms(100);
          {
             if(!input(sw_blower))
             {
               output_high(relay6);// on blower
               
             }
             
          }
       }
////////////////////////////off blower//////////////
  if(input(sw_blower))
    {
      delay_ms(100);
       {
         if(input(sw_blower))
          {
           output_low(relay6);// off blower
                            
          }
       }
    }


//================timer rotate=====
set_adc_channel(1);
delay_us(10);
t_break1 = read_adc();

t_break1 *=225;
t_break1 += 15360;
t_break1 /= 1024;

t_break = t_break1;      
      
      
      
      //===============chk_lcd===============
 
loop2++;
if(loop2>=2)
{
    lcd_gotoxy(1,11);
    lcd_putc("t");
    //----------------
    lcd_gotoxy(1,12);
    lcd_putc("o");
    //----------------
    
    chk_num_lcd=lcd_getc(1,11);
    
    if(chk_num_lcd != "t")
    {
     lcd_init();
    }
    
    //=======================
    
    chk_num_lcd = lcd_getc(1,12);
    
    if(chk_num_lcd != "o")
    {
     lcd_init();
    }
  loop2=0;  
}     
      
      
      //=============display current time=====
     read_time();
     //chk_lcd();
     delay_ms(10);
     get_sec(ds1307_regs[0]);
     get_min(ds1307_regs[1]);
     get_hr(ds1307_regs[2]);
          
     get_date(ds1307_regs[4]);
     get_month(ds1307_regs[5]);
     get_year(ds1307_regs[6]);
    
    //================== clear for not display line 1==== 
    
      lcd_gotoxy(1,0);
      lcd_putc(" ");// 
      
      lcd_gotoxy(1,9);
      lcd_putc("  ");//     
      lcd_gotoxy(1,13);
      lcd_putc(" ");//
      lcd_gotoxy(1,19);
      lcd_putc(" ");//      
    //================== display current time=====
      
      lcd_gotoxy(1,1);
      lcd_putc(NUM[hr10]);
      //lcd_gotoxy(1,5);
      lcd_putc(NUM[hr00]); 
      lcd_putc(":");
      //=====================
      lcd_gotoxy(1,4);
      lcd_putc(num[min10]);
      delay_us(10);
      lcd_gotoxy(1,5);
      lcd_putc(num[min00]);
      lcd_putc(":");
     //=======================
       lcd_gotoxy(1,7);
      lcd_putc(num[sec10]);
      lcd_putc(num[sec00]);
      //====================== to stop time===========
      lcd_gotoxy(1,11);lcd_putc("to");
      
      hex_bcd(stop_hr);
      lcd_gotoxy(1,14);
      lcd_putc(num[a]);
      lcd_putc(num[b]);
      
      lcd_putc(":");//1,16
      
      hex_bcd(stop_min);
      lcd_gotoxy(1,17);
      lcd_putc(num[a]);
      lcd_putc(num[b]);
      
      
      //==================display hitemp  lotemp====
      // lowtemp=read_eeprom(5);
      // hightemp=read_eeprom(6);
      
      lcd_gotoxy(2,9); lcd_putc("  ");
      
      hex_bcd_temp(lowtemp);

      lcd_gotoxy(2,11);lcd_putc(num[t1]);lcd_putc(num[t2]);
      lcd_putc(num[t3]);
      
      lcd_gotoxy(2,14);lcd_putc("<");
      lcd_gotoxy(2,15);lcd_putc("T");
      lcd_gotoxy(2,16);lcd_putc("<");
      
      hex_bcd_temp(hightemp);

      lcd_gotoxy(2,17);lcd_putc(num[t1]);lcd_putc(num[t2]);
      lcd_putc(num[t3]);    
      //=====================ROTATE PAUSE===========
           
          /*if(!input(limit_sw) && flg_rotor_stop==0)
          {
             delay_ms(30);
             if(!input(limit_sw))
             {
                 output_low(relay4);
                 flg_rotor_stop=1;
             }
             
          }*/      
     //============read temperature ===
    // average temp 

     set_adc_channel(0);
     delay_us(10);

     read_temp1 = read_adc(); 
     if(read_temp1>195 && read_temp1<600)
     {
     sum_temp=sum_temp+read_temp1;
     n_readtemp++;
     }
     if(n_readtemp>=5)
     {
     sum_temp/=5;
     read_temp1=sum_temp;n_readtemp=0;
     
     //if(read_temp1>=184 && read_temp1<=553)
     
        read_temp1*=168;     // adc zero = 184 AT  17 C
        read_temp1-=23304;  // adc span = 553  AT  200 C 
        read_temp1/=348;    // temp zero = 17 span = 200
        dis_temp = read_temp1;temp=read_temp1;
        sum_temp=0;
     
     //---------display read temp---
     }
     //---------display read temp---
     
     lcd_gotoxy(2,0);
     lcd_putc("Temp"); 
     
     lcd_gotoxy(2,4);
     lcd_putc("  ");// empty 2 char 
     
     //hex_bcd(dis_temp);
     IF(dis_temp<32 )
     {
       dis_temp=32;
       lcd_gotoxy(2,1);          
       lcd_putc("Temp<");       
     }
     
     hex_bcd_temp(dis_temp);
     
     lcd_gotoxy(2,6);
     lcd_putc(num[t1]);//6
     lcd_putc(num[t2]); lcd_putc(num[t3]);//8     
     
     //===================== temp > set temp================
     if(temp >= hightemp)
     {
         flg_lowtemp=1;

           //output_high(relay2);// on ignition tr
           delay_ms(50);
           output_LOW(relay3);// on value gas
           delay_ms(5000);
           delay_ms(5000);
           delay_ms(5000);
           delay_ms(5000);
           output_low(relay2);
           output_LOW(relay1);//on pump
         
         flg_tempover=1;
         //output_high(relay6);//BLOWER
         
     }
     if(temp <= lowtemp && flg_lowtemp)
     {
           // output_low(relay6);
            output_high(relay3);// on value gas            
            delay_ms(4000);
            output_high(relay2);// on ignition tr
          
          
           delay_ms(5000);
           delay_ms(5000);
           delay_ms(2000);
           delay_ms(2000);
           delay_ms(1000);
           output_low(relay2);
           delay_ms(3000);
           output_high(relay1);//on pump           
                     
           flg_tempover=0;
           flg_lowtemp=0;
    }
      
    //=========== time stop=====
       if(stop_min==cmd_min&&stop_hr==cmd_hr  )
       {
        flg_auto=0;
        
        output_low(relay2); //of ignition
        output_low(relay3); //off value gas
        output_low(relay4); //off  rotate         
        delay_ms(5000);
        delay_ms(5000);
        
        output_low(relay1);// off pump
        output_high(relay5);
       }
    //============manual stop====
            if(!input(sw_up) && !input(sw_down))
           {
              delay_ms(100);
                    
               if(!input(sw_up) && !input(sw_down))
               {
                while(!input(sw_up) && !input(sw_down)){restart_wdt();}
                flg_auto=0;N_MODE=2;
                
                output_low(relay1);
                output_low(relay2);
                output_low(relay3);
                output_low(relay4);
              } 
             

           }        
          //========================== rotate stop and delay start ======
           
          /*if(!input(limit_sw) && flg_rotor_stop==0)
          {
             delay_ms(30);
             if(!input(limit_sw))
             {
                 output_low(relay4);
                 flg_rotor_stop=1;
             }
             
          }*/
          //=============================rotor on again===================
          
          if(flg_rotor_stop)
          {
            loop++;
            if(loop>t_break)
            {
             output_high(relay4);
             delay_ms(100);
             loop=0;flg_rotor_stop=0;
            }
            
          }
          
          

      
    }
}

void set_temp(void)
{set_count=0;
                lcd_gotoxy(2,1);
                lcd_putc("low");
                
                lcd_gotoxy(2,9);
                lcd_putc("hi");
                
                lowtemp=read_eeprom(5);
                hightemp=read_eeprom(6);
                
                       hex_bcd_temp(lowtemp);
                       lcd_gotoxy(2,5);
                       lcd_putc(num[t1]);
                       lcd_putc(num[t2]);
                       lcd_putc(num[t3]);
                
                       hex_bcd_temp(hightemp);
                       lcd_gotoxy(2,12);
                       lcd_putc(num[t1]);
                       lcd_putc(num[t2]);
                       lcd_putc(num[t3]); 
               
                
  while(flg_settemp)
  {restart_wdt();
        //=============set position========
          if(!input(sw_set))
          {  delay_ms(101);
            if(!input(sw_set))
            {            
             while(!input(sw_set)){restart_wdt();}
             
             

               if(set_count==0)
               {
                lcd_gotoxy(2,1);
                lcd_putc("low");lcd_gotoxy(2,5);
               }
               if(set_count==1)
               {
                lcd_gotoxy(2,8);
                lcd_putc("hi");lcd_gotoxy(2,11);
               }
                set_count++;
                if(set_count>1)set_count=0;
            }    
          }
          //=====================set min==============================
                  if(set_count==0 && !input(sw_up) && input(sw_down))
                  {
                    delay_ms(101);
                    if(set_count==0 && !input(sw_up) && input(sw_down))
                    {
                     
                     if(lowtemp<150 && lowtemp>=0)
                     {
                       lowtemp++;                      
                       hex_bcd_temp(lowtemp);
                       lcd_gotoxy(2,5);
                       lcd_putc(num[t1]);
                       lcd_putc(num[t2]);
                       lcd_putc(num[t3]);
                       write_eeprom (5,lowtemp);
                     }
                    }
                  }
          //=========================        
                  if(set_count==0 && input(sw_up) && !input(sw_down))
                  {
                    delay_ms(101);
                    if(set_count==0 && input(sw_up) && !input(sw_down))
                    {
                     if(lowtemp>=1)
                     {
                      lowtemp--;
                     
                      hex_bcd_temp(lowtemp);
                      lcd_gotoxy(2,5);
                       lcd_putc(num[t1]);
                       lcd_putc(num[t2]);
                       lcd_putc(num[t3]);
                      write_eeprom (5,lowtemp);
                     } 
                    
                    }
                  } 
          
          //========================
                  if(set_count==1 && !input(sw_up) && input(sw_down))
                  {
                    delay_ms(101);
                    if(set_count==1 && !input(sw_up) && input(sw_down))
                    {
                     if( hightemp<150 && hightemp >= 0)
                     {
                       hightemp++;
                       
                       hex_bcd_temp(hightemp);
                       lcd_gotoxy(2,12);
                       lcd_putc(num[t1]);
                       lcd_putc(num[t2]);
                       lcd_putc(num[t3]);
                       write_eeprom (6,hightemp);
                     }
                    
                    }
                  }
          //=========================
                  if(set_count==1 && input(sw_up) && !input(sw_down))
                  {
                    delay_ms(101);
                    if(set_count==1 && input(sw_up) && !input(sw_down))
                    {
                      if( hightemp>=1)
                      {
                      hightemp--;
                    
                       hex_bcd_temp(hightemp);         
                       lcd_gotoxy(2,12);
                       lcd_putc(num[t1]);
                       lcd_putc(num[t2]);
                       lcd_putc(num[t3]);
                      write_eeprom (6,hightemp);
                      }
                    
                    }
                  }
          //===================exit set time on=====================
                 if(!input(sw_up) && !input(sw_down))
                 {
                   delay_ms(100);
                    
                    if(!input(sw_up) && !input(sw_down))
                    {
                     while(!input(sw_up) && !input(sw_down)){restart_wdt();}
                     flg_settemp=0;write_eeprom (2,0);
                    } 

                  
                 }


                             
  }
                        lcd_gotoxy(2,1);
                      lcd_putc("                    ");
  
}
void stop_rotate(void)
{
      //DELAY_MS(500);
   if(!flg_rotor_stop)
   {
      output_low(relay4);
      flg_rotor_stop=1;
   }
}
