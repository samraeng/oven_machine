 //RELAY1 PIN_E2  pump
// RELAY2 PIN_E1  inintion tr
//RELAY3 PIN_E0  value gas 

//RELAY4 PIN_A5  rotate motor 
//#DEFINE RELAY5 PIN_A4//
//#DEFINE RELAY6 PIN_B4


VOID MANUAL(VOID)
{ lcd_gotoxy(1,0);
  lcd_putc("                    ");
  lcd_gotoxy(2,1);
  lcd_putc("                    "); 
  
  lcd_gotoxy(1,0);
  lcd_putc("MANUAL");
  
  n_readtemp=0;
  flg_manual=1;
  
  
  WHILE(!input(sw_manual))
  { restart_wdt();
  
   //  loop++;
   //  if(loop>=100)
   //  {
    //   output_toggle(pin_c2);
    //   loop=0;chk_lcd();
    // }
//=============================
loop2++;
if(loop2>=3)
{
    lcd_gotoxy(1,0);
    lcd_putc("M");
    //----------------
    lcd_gotoxy(1,1);
    lcd_putc("A");
    //----------------
    
    chk_num_lcd=lcd_getc(1,0);
    
    if(chk_num_lcd != "M")
    {
     lcd_init();
    }
    
    //=======================
    
    chk_num_lcd = lcd_getc(1,1);
    
    if(chk_num_lcd != "A")
    {
     lcd_init();
    }
  loop2=0;  
} 
  //========= START AUTO=====
    //------ start firring process
      IF(!INPUT(SW_PRG))
      {
         delay_ms(100);
         if(!input(sw_prg))
         {
           //output_high(relay1);//on pump
          // delay_ms(5000);
           output_high(relay2);// on ignition tr
           delay_ms(50);
           output_high(relay3);// on value gas
           delay_ms(4000);
          // delay_ms(5000);
          // delay_ms(5000);
           output_low(relay2);
           //delay_ms(5000);
           delay_ms(2000);
           output_high(relay1);//on pump
           
           //output_low(relay2);
         
         }
         
         
      }
      //---- stop firring process
      IF(!INPUT(SW_SET))
      {
         delay_ms(100);
         if(!input(sw_set))
         {
           output_low(pump);//off pump
           delay_ms(200);
           output_low(relay1);// off value gas
           output_low(relay3);
         }
      }
      //------ jog rotate
      IF(!INPUT(SW_UP))
        {
           delay_ms(100);
           if(!input(sw_up))
           {
           output_high(relay4);
           delay_ms(1000);
           while(input(limit_sw));
           //delay_ms(10);
            output_low(relay4);
           }
           while(!input(sw_up)){ restart_wdt();}
            
        }    
      //---------------ON OFF RELAY4------------
           IF(INPUT(SW_UP)&& !INPUT(SW_DOWN))
        {
           delay_ms(100);
            if(!input(sw_down))
            {
             output_high(relay4);
              
                     //  output_high(relay6);
            }
           while(!input(sw_down)){ restart_wdt();} 
        
        }   
     //=================on/off blower==============
       if(!input(sw_blower))
       {
          delay_ms(10);
          {
             if(!input(sw_blower))
             {
               output_toggle(relay6);
               while(!input(sw_blower));
             }
          }
       }
      //===================on/off pump=============
       if(!input(sw_ox))
       {
          delay_ms(10);
          {
             if(!input(sw_ox))
             {
               output_toggle(relay1);
               while(!input(sw_ox));
             }
          }
       }      
      
    // display time & temperature
     read_time();
     
     delay_ms(100);
     get_sec(ds1307_regs[0]);
     get_min(ds1307_regs[1]);
     get_hr(ds1307_regs[2]);
          
     get_date(ds1307_regs[4]);
     get_month(ds1307_regs[5]);
     get_year(ds1307_regs[6]);
    
      lcd_gotoxy(1,7);
      lcd_putc(NUM[hr10]);
      //lcd_gotoxy(1,5);
      lcd_putc(NUM[hr00]); 
      lcd_putc(":");
      //=====================
      lcd_gotoxy(1,10);
      lcd_putc(num[min10]);
      delay_us(10);
      //lcd_gotoxy(1,10);
      lcd_putc(num[min00]);
      lcd_putc(":");
     //=======================
       lcd_gotoxy(1,13);
      lcd_putc(num[sec10]);
      lcd_putc(num[sec00]);
      
       lcd_putc("     ");
      
     //============read temperature ===
     set_adc_channel(0);
     delay_us(10);
     
    // average temp 
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
  
     
     lcd_gotoxy(1,0);
     lcd_putc("MANUAL");     
     lcd_gotoxy(2,1);
     
     
     lcd_putc("Temp");
  
     IF(dis_temp<32 )
     {
       dis_temp=32;
            lcd_gotoxy(2,1);          
     lcd_putc("Temp<");
     }     
     hex_bcd_temp(dis_temp);
     
      lcd_gotoxy(2,0);
      lcd_putc(" ");
     
      lcd_gotoxy(2,5);
      lcd_putc(" ");
     
     lcd_gotoxy(2,6);
     lcd_putc(num[t1]);
     lcd_putc(num[t2]);
     lcd_putc(num[t3]);
     
      lcd_gotoxy(2,9);
      lcd_putc("  ");     
      
      lcd_putc("C"); 
      lcd_putc("        ");      
       
       

    
  }// exit loop
  
                     flg_MODE=0;FLG_SET_PRG=0;N_MODE=2;
                     LCD_CLR();
                     output_low(relay1);output_low(relay2);output_low(relay3);
                     output_low(relay4);output_low(relay5);output_low(relay6); 
  

}

void chk_lcd(void)
{
   /* lcd_gotoxy(2,10);
    lcd_putc("a");
    //----------------
    lcd_gotoxy(2,11);
    lcd_putc("b");
    //----------------
    
    chk_num_lcd=lcd_getc(2,10);
    
    if(chk_num_lcd != "a")
    {
     lcd_init();
    }
    
    //=======================
    
    chk_num_lcd = lcd_getc(2,11);
    
    if(chk_num_lcd != "b")
    {
     lcd_init();
    }*/
    
  //lcd_gotoxy(1,0);
  //lcd_putc("                    "); 
 // lcd_gotoxy(2,1);
 // lcd_putc("                    "); 

}
