#include <test_program.h>
// test change file1



#define sw_prg pin_b0
#define sw_set pin_b1
#define sw_up pin_b3
#define sw_down pin_b2

#DEFINE RELAY1 PIN_E2
#DEFINE RELAY2 PIN_E1
#DEFINE RELAY3 PIN_E0

#DEFINE RELAY4 PIN_A5
#DEFINE RELAY5 PIN_A4
#DEFINE RELAY6 PIN_B4


#use i2c(Master,fast,sda=PIN_C4,scl=PIN_C5)

//#include "lcd_sam3_b.c"
#include "lcd_sam3.c"
#define seconds_reg 0
#define minutes_reg 1
#define hours_reg 2
#define day_of_week_reg 3
#define date_reg 4
#define month_reg 5
#define year_reg 6
#define control_reg 7
#define ds1307_nvram_start_address 8
#define control_init_val 0x90
//-------------------------------------------------------

int8 trisa;
#locate trisa = 0xf92

int8 porta;
#locate porta = 0xF80


//--------------------------
int8 trisb;
#locate trisb=0xf93

int8 portb;
#locate portb=0xf81

//#bit relay6//portb.4

//#bit sw_prg=portb.0
//#bit sw_set  = portb.1
//#bit sw_up   = portb.2
//#bit sw_down = portb.3
//----------------------------
int8 trise;
#locate trise = 0xf96

int8 porte;
#locate porte= 0xf84

//-----------------------------

int8 trisc;
#locate trisc = 0xf94
#bit trisc7=trisc.7

int8 portc;
#locate portc=  0xf82
#bit pump = portc.7


#define write_cntrl 0xd0
#define read_cntrl  0xd1

VOID LCD_CLR(VOID)
{
  lcd_gotoxy(1,0);
  lcd_putc("                    "); 
  lcd_gotoxy(2,1);
  lcd_putc("                    "); 
}


void selectmode(void);
char tobcd(char bin_val);
void set_time(char sec,char min,char hour,char dat1,char month1,char year1);
void read_time(void);
void get_sec(char sec);
void get_min(char min);
void get_hr(char hr);
void get_date(char dat);
void get_month(char month);
void get_year(char year);
char read_byte(char addr);
void write_byte(char addr,char value);
void set_TIME(void);
void set_OFF(void);
void display_hours(void);
void display_min(void);
void display_sec(void);
char ds1307_regs[7];
void set_program(void);
VOID SAVE_PRG(VOID);
void delay_min(int8 d_min);

int8 sec00;
int8 sec10;
int8 min00;
int8 min10;
//=========min on_off ==
 int8 min;




int8 hr00;
int8 hr10;
//=========hr_on_off
 int8 hr;



int8 dat00;
int8 dat10;

int8 month00;
int8 month10;

int8 year00;
int8 year10;
int8 cmd_min,cmd_hr;
int8 stop_min,stop_hr;


int1 flg_set_time,flg_set_prg,FLG_MODE;

int1 flg_stop_min,flg_stop_hr;
int1 flg_auto;
//int1 flg_set_oFF;

int8 set_count,N_MODE;
int8 a,b;

//int8 pre_cmd;



INT8 M1,M2,M3,M4;
void hex_bcd(int8 k);
byte const num[12] = {'0','1','2','3','4','5','6','7','8','9'};
//#include "func_settime.c"
#include "ds1307_sam1.c"


void chk_lcd(void);
void active_cmd(void);
VOID SELECT_MODE(VOID);
VOID MANUAL(VOID);
VOID AUTO_(VOID);
void set_time_auto(void);
void start_auto(void);
#INCLUDE "M_MODE.C"
#INCLUDE "A_MODE.C"


void main()
{
   setup_timer_3(T3_DISABLED | T3_DIV_BY_1);
   
      set_count=0;flg_set_prg=0;N_MODE=0;flg_stop_min=flg_stop_hr=0;
      FLG_MODE=0;flg_auto=0;
      M1=M2=M3=M4=0;
trisa=0xcf;
trisb=0xef;
//trise=0xf8;

   OUTPUT_LOW(RELAY1);OUTPUT_LOW(RELAY2);OUTPUT_LOW(RELAY3);
   OUTPUT_LOW(RELAY4);OUTPUT_LOW(RELAY5);OUTPUT_LOW(RELAY6);
   
   

   lcd_init();
   lcd_gotoxy(1,0);
   lcd_putc("                    ");
   lcd_gotoxy(2,1);
   lcd_putc("                    ");  
   
     IF(read_eeprom(2)!=0)
    {
      set_time(15,58,23,28,12,04);   //SEC/MIN/HOURS/DATE/MONT/YEAR
      write_eeprom (3,0);write_eeprom (4,0);
    } 


   while(TRUE)
   {
  
     restart_wdt();
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


      display_hours();
      //=====================
     
      display_min();
     //=======================
      display_sec();

 
 //===============function set time=======
        if(!input(sw_set))
          {
            while(!input(sw_set)){restart_wdt();}
                        
            flg_set_time=1;
            set_TIME();
          
          }
 //===============  SELECT MODE============
        IF(!INPUT(SW_PRG))
        {
          while(!input(sw_PRG)){restart_wdt();}
          flg_set_prg=1;
           SELECT_MODE();
        }
 
 
     }

   
}

void display_hours(void)
{
      lcd_gotoxy(1,4);
      lcd_putc(NUM[hr10]);
      lcd_gotoxy(1,5);
      lcd_putc(NUM[hr00]); 
      lcd_putc(":");
}
void display_min(void)
{
      lcd_gotoxy(1,7);
      lcd_putc(num[min10]);
      delay_us(10);
      lcd_gotoxy(1,8);
      lcd_putc(num[min00]);
      lcd_putc(":");
}
void display_sec(void)
{

     
       lcd_gotoxy(1,10);
      lcd_putc(num[sec10]);
      delay_us(10);
       lcd_gotoxy(1,11);
      lcd_putc(num[sec00]);

}
void hex_bcd(int8 k)
{//int8 t;
 k%=100;
 a=0;b=0;
 a=k/10;
 b=k%10;
}

void set_time(void)
{
         set_count=0;  
         lcd_gotoxy(2,1);
         lcd_putc("SET TIME");
               
        // display_hours();display_min(); display_sec();
      lcd_gotoxy(2,11);
      lcd_putc(NUM[hr10]); lcd_gotoxy(2,12); lcd_putc(NUM[hr00]);lcd_putc(":");
      
      lcd_gotoxy(2,14);
      lcd_putc(num[min10]);delay_us(10);lcd_gotoxy(2,15);lcd_putc(num[min00]);
      lcd_putc(":");
                  
                  
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
                lcd_gotoxy(2,12);
               }
               if(set_count==1)
               {
                lcd_gotoxy(2,15);
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
                     
                     if(min<60 && min>=0)
                     {
                       min++;                      
                       hex_bcd(min);
                       lcd_gotoxy(2,14);
                       lcd_putc(num[a]);
                       lcd_putc(num[b]);
                       write_eeprom (0,min);
                     }
                    }
                  }
          //=========================        
                  if(set_count==0 && input(sw_up) && !input(sw_down))
                  {
                    delay_ms(101);
                    if(set_count==0 && input(sw_up) && !input(sw_down))
                    {
                     if(min>=1)
                     {
                      min--;
                     
                      hex_bcd(min);
                      lcd_gotoxy(2,14);
                      lcd_putc(num[a]);
                      lcd_putc(num[b]);
                      write_eeprom (0,min);
                     } 
                    
                    }
                  } 
          
          //========================
                  if(set_count==1 && !input(sw_up) && input(sw_down))
                  {
                    delay_ms(101);
                    if(set_count==1 && !input(sw_up) && input(sw_down))
                    {
                     if(hr<24 && hr >= 0)
                     {
                       hr++;
                       
                       hex_bcd(hr);
                       lcd_gotoxy(2,11);
                       lcd_putc(num[a]);
                       lcd_putc(num[b]);
                       write_eeprom (1,hr);
                     }
                    
                    }
                  }
          //=========================
                  if(set_count==1 && input(sw_up) && !input(sw_down))
                  {
                    delay_ms(101);
                    if(set_count==1 && input(sw_up) && !input(sw_down))
                    {
                      if(hr>=1)
                      {
                      hr--;
                    
                      hex_bcd(hr);
                      lcd_gotoxy(2,11);
                      lcd_putc(num[a]);
                      lcd_putc(num[b]);
                      write_eeprom (1,hr);
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
                     flg_set_time=0;
                    } 

                  
                 }


                        

        }
//=======SEC/MIN/HOURS/DATE/MONT/YEAR        
set_time(00,min,hr,28,12,04);write_eeprom (2,0);
   lcd_gotoxy(2,1);
   lcd_putc("                    "); 
}

VOID SELECT_MODE(VOID)
{
   lcd_gotoxy(1,0);
   lcd_putc("SELECT MODE PRESS F1"); 
   
   WHILE(FLG_SET_PRG)
   {
      
      IF(!INPUT(SW_SET))
      {
          while(!input(sw_SET)){restart_wdt();}
                   
          //-------------- MODE AUTO--------
          IF(N_MODE==0)
          {
              lcd_gotoxy(2,1);
              lcd_putc(" RUN ");lcd_gotoxy(2,7);
              lcd_putc(" AUTO MODE ");//N_MODE=0;
          }
         //-------------- MODE MANUAL-------- 
          IF(N_MODE==1)
          {
              lcd_gotoxy(2,1);
              lcd_putc(" RUN ");lcd_gotoxy(2,7);
              lcd_putc(" MAN MODE ");//N_MODE+=1;
          }          
          
          N_MODE++;
          IF(N_MODE>=2) N_MODE=0;
           
      }
      //---------------- ENTER MODE -------
        IF(!INPUT(SW_PRG))
        {
          while(!input(sw_PRG)){restart_wdt();}

          IF(N_MODE==0)
          {
           FLG_MODE=1;           
           MANUAL(); 
          }
          IF(N_MODE==1)
          {
            FLG_MODE=1;
            AUTO_();
          }
          
        }
      
   }
   
}
