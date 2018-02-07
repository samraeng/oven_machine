void set_time(char sec,char min,char hour,char dat1,char month1,char year1)
{
  char i;
  ds1307_regs[seconds_reg]=sec;//seconds
  ds1307_regs[minutes_reg]=min;//minutes
  ds1307_regs[hours_reg]=hour;//hour
  ds1307_regs[day_of_week_reg]=0;//not use
  ds1307_regs[date_reg]=dat1;//date
  ds1307_regs[month_reg]=month1;//mont
  ds1307_regs[year_reg]=year1;//year
  for(i=0;i<7;i++)
  {
   ds1307_regs[i]=tobcd(ds1307_regs[i]);
  }
  ds1307_regs[seconds_reg]&=0x7f;
  ds1307_regs[hours_reg]&=0x3f;
  i2c_start();
  i2c_write(write_cntrl);
  i2c_write(seconds_reg);
  for(i=0;i<7;i++)
  {
   i2c_write(ds1307_regs[i]);
  }
  i2c_write(control_init_val);
  i2c_stop();

}
void read_time(void)
{
 i2c_start();
 i2c_write(write_cntrl);
 i2c_write(seconds_reg);
 i2c_start();
 i2c_write(read_cntrl);
  ds1307_regs[seconds_reg]=i2c_read() & 0x7f;//seconds
  ds1307_regs[minutes_reg]=i2c_read() & 0x7f;//minutes
  ds1307_regs[hours_reg]=i2c_read() & 0x3f;//hour
  ds1307_regs[day_of_week_reg]=i2c_read() & 0x07;//not use
  ds1307_regs[date_reg]=i2c_read() & 0x3f;//date
  ds1307_regs[month_reg]=i2c_read() & 0x1f;//mont
  ds1307_regs[year_reg]=i2c_read(0);//year
  i2c_stop();
}
char tobcd(bin_val)
{
  char tem_p;// edit 16_11_2560
  char retval;
  tem_p=bin_val;
  retval=0;
  while(tem_p>=10)
  {
  restart_wdt();
   retval+=0x10;
   tem_p-=10;
  }
  retval+=tem_p;
 return retval;
}
void get_sec(char sec)
{ int secon;
  secon=sec;
  sec00=secon;
  sec00=sec00&0x0f;
  sec10=secon;
  sec10=sec10>>4;
  sec10=sec10&0x0f;
}
void get_min(char min)
{ int8 minut;

  minut=min;
  min00=minut;
  min00=min00&0x0f;
  min10=minut;
  min10=min10>>4;
  min10=min10&0x0f;
  
  cmd_min=(min10*10) + min00;
       // hex_bcd(cmd_min);lcd_gotoxy(2,15);
     // lcd_putc(num[a]);lcd_putc(num[b]); 
      

   
}
void get_hr(char hr)
{
 int8 temphr;
 temphr=hr; 
 hr00=temphr;
 hr00=hr00 & 0x0f;
 hr10=temphr;
 hr10=hr10>>4;
 hr10=hr10 & 0x0f;
 cmd_hr=(hr10*10)+hr00; 
     //  hex_bcd(cmd_hr);lcd_gotoxy(2,12);
     // lcd_putc(num[a]);lcd_putc(num[b]);
}
void get_date(char dat)
{
 int tempdat;
 tempdat=dat;
 dat00=tempdat;
 dat00=dat00 & 0x0f;
 dat10=tempdat;
 dat10=dat10>>4;
 dat10=dat10 & 0x0f;
}
void get_month(char month)
{
  int tempmonth;
  tempmonth=month;
  month00=tempmonth;
  month00=month00 & 0x0f;
  month10=tempmonth;
  month10=month10>>4;
  month10=month10 & 0x0f;
}
void get_year(char year)
{ int tempyear;
  tempyear=year;
  year00=tempyear;
  year00=year00 & 0x0f;
  year10=tempyear;
  year10=year10>>4;
  year10=year10 & 0x0f;

}
void ds1307_init(void)
{
   BYTE seconds = 0;

   i2c_start();
   i2c_write(0xD0);      // WR to RTC
   i2c_write(0x00);      // REG 0
   i2c_start();
   i2c_write(0xD1);      // RD from RTC
   seconds = tobcd(i2c_read(0)); // Read current "seconds" in DS1307
   i2c_stop();
   seconds &= 0x7F;

   delay_us(3);

   i2c_start();
   i2c_write(0xD0);      // WR to RTC
   i2c_write(0x00);      // REG 0
   i2c_write(tobcd(seconds));     // Start oscillator with current "seconds value
   i2c_start();
   i2c_write(0xD0);      // WR to RTC
   i2c_write(0x07);      // Control Register
   i2c_write(0xd0);     // Disable squarewave output pin
   i2c_stop();

}
