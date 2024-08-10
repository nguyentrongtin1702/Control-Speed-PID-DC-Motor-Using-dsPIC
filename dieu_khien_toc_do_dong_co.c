#include <30F4011.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#fuses nowdt,put4,XT,noprotect
#use delay(clock=7.3728M)
//#use rs232 (baud=9600,xmit=pin_f5,rcv=pin_f4,parity = n,bits = 8, stop = 1, timeout = 1)// Khoi tao uart
#use rs232 (UART2,baud = 9600)

unsigned int32 rma;
unsigned int8 bdn = 0;
int1 tt_on=0;
unsigned int8 value = 0, value_tam = 10;
int1 refresh = 0;
int1 tt_nhan = 0;
unsigned int8 value_new = 0;
int1 tt_dem = 0;
int1 tt_on_td = 0;
int1 tt_tn = 1;
unsigned int16 speed;

void nhan_du_lieu_dieu_khien();
void doc_encoder_va_gui_du_lieu();
void dieu_khien_toc_do();

#int_timer1
void interrupt_timer1()
{
   set_timer1(42496);
   bdn++;
   if(bdn < 10 )
   {
      tt_on = 1;
   }
   else if(bdn >= 10 && bdn <20)
   {
      tt_on = 0;
   }
   else if(bdn == 20)
   {
      bdn = 0;
   }
   else if(bdn == 2)
   {
      tt_dem = 1;
   }
   set_timer4(42496);

}


#int_rda2
void interrupt_rda2()
{
   if(kbhit() == 1)
   {
      rma = getch();
      tt_nhan = 1;
   }
}

#int_timer4
void interrupt_timer4()
{
   set_timer4(42496);
   refresh = 1;
   
}


void doc_encoder_va_gui_du_lieu()
{
   value =  qei_get_count();
   speed = (value*300)/1000;
   putc(speed);  
   
}

void nhan_du_lieu_dieu_khien()
{
   if(tt_nhan == 1)
   {
      
      if(rma == 0x30 )
      {
         delay_ms(1000);
         output_high(pin_f1); // cho phep chan SD o muc cao
         output_low(pin_d2);
         set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
         set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,10,30);
         set_motor_pwm_duty(1,1,0x0B);
         delay_us(6);
         set_motor_pwm_duty(1,2,0x00);
         tt_on_td = 1;
         tt_tn = 0; // chay thuan
         
         
      }
      else if(rma == 0x31)
      {
         delay_ms(1000);
         output_high(pin_f1); // cho phep chan SD o muc cao
         output_low(pin_d2);
         set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
         set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
         set_motor_pwm_duty(1,2,0x0B);
         delay_us(6);
         set_motor_pwm_duty(1,1,0x00);
         tt_on_td = 1;
         tt_tn = 1;// chay nghich
         
      }
      else if(rma == 0x32)
      {
         output_low(pin_f1); // cho phep chan SD o muc thap
         output_high(pin_d2);
         set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
         set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
         set_motor_pwm_duty(1,2,0x00);
         delay_us(6);
         set_motor_pwm_duty(1,1,0x00); 
         tt_on_td = 0;
         
      }
      
      dieu_khien_toc_do();
   }
  
   tt_nhan = 0;
   
}

void dieu_khien_toc_do()
{
   if(tt_nhan == 1 && tt_on_td == 1 && tt_tn == 0 ) {
      switch(rma)
      {
         case 0x33 :delay_ms(200); // chay 10%
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,10,30);
                    set_motor_pwm_duty(1,1,0x0B); // 10%
                    delay_us(6);
                    set_motor_pwm_duty(1,2,0x00);
                    break; 
                    
         case 0x34 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,10,30);
                    set_motor_pwm_duty(1,1,0x12); // 20%
                    delay_us(6);
                    set_motor_pwm_duty(1,2,0x00);
                    break; // chay 20%
                    
         case 0x35 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,10,30);
                    set_motor_pwm_duty(1,1,0x1B); // 30%
                    delay_us(6);
                    set_motor_pwm_duty(1,2,0x00);
                    break; // chay 30%
                    
          case 0x36 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,10,30);
                    set_motor_pwm_duty(1,1,0x24); // 40%
                    delay_us(6);
                    set_motor_pwm_duty(1,2,0x00);
                    break; // chay 40%
           
          case 0x37 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,10,30);
                    set_motor_pwm_duty(1,1,0x2d); // 50%
                    delay_us(6);
                    set_motor_pwm_duty(1,2,0x00);
                    break; // chay 50%
                    
          case 0x38 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,10,30);
                    set_motor_pwm_duty(1,1,0x36); // 60%
                    delay_us(6);
                    set_motor_pwm_duty(1,2,0x00);
                    break; // chay 60%
          
          case 0x39 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,10,30);
                    set_motor_pwm_duty(1,1,0x3f); // 70%
                    delay_us(6);
                    set_motor_pwm_duty(1,2,0x00);
                    break; // chay 70%
          
          case 0x41 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,10,30);
                    set_motor_pwm_duty(1,1,0x48); // 80%
                    delay_us(6);
                    set_motor_pwm_duty(1,2,0x00);
                    break; // chay 80%
                    
          case 0x42 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,10,30);
                    set_motor_pwm_duty(1,1,0x51); // 90%
                    delay_us(6);
                    set_motor_pwm_duty(1,2,0x00);
                    break; // chay 90%
                    
          case 0x43 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,10,30);
                    set_motor_pwm_duty(1,1,0x5b); // 100%
                    delay_us(6);
                    set_motor_pwm_duty(1,2,0x00);
                    break; // chay 100%
          
      }
   }
   else if (tt_nhan == 1 && tt_on_td == 1 && tt_tn == 1 ) // chay nghich
   {
      switch(rma)
      {
         case 0x33 :delay_ms(200); // chay 10%
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_high(pin_d2);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_pwm_duty(1,2,0x0B); // 10%
                    delay_us(6);
                    set_motor_pwm_duty(1,1,0x00);
                    break; 
                    
         case 0x34 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_pwm_duty(1,2,0x12); // 20%
                    delay_us(6);
                    set_motor_pwm_duty(1,1,0x00);
                    break; // chay 20%
                    
         case 0x35 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_pwm_duty(1,2,0x1B); // 30%
                    delay_us(6);
                    set_motor_pwm_duty(1,1,0x00);
                    break; // chay 30%
                    
          case 0x36 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_pwm_duty(1,2,0x24); // 40%
                    delay_us(6);
                    set_motor_pwm_duty(1,1,0x00);
                    break; // chay 40%
           
          case 0x37 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_pwm_duty(1,2,0x2d); // 50%
                    delay_us(6);
                    set_motor_pwm_duty(1,1,0x00);
                    break; // chay 50%
                    
          case 0x38 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_pwm_duty(1,2,0x36); // 60%
                    delay_us(6);
                    set_motor_pwm_duty(1,1,0x00);
                    break; // chay 60%
          
          case 0x39 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_pwm_duty(1,2,0x3f); // 70%
                    delay_us(6);
                    set_motor_pwm_duty(1,1,0x00);
                    break; // chay 70%
          
          case 0x41 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_pwm_duty(1,2,0x48); // 80%
                    delay_us(6);
                    set_motor_pwm_duty(1,1,0x00);
                    break; // chay 80%
                    
          case 0x42 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_pwm_duty(1,2,0x51); // 90%
                    delay_us(6);
                    set_motor_pwm_duty(1,1,0x00);
                    break; // chay 90%
                    
          case 0x43 :delay_ms(200); 
                    output_high(pin_f1); // cho phep chan SD o muc cao
                    output_low(pin_d2);
                    set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
                    set_motor_pwm_duty(1,2,0x5b); // 100%
                    delay_us(6);
                    set_motor_pwm_duty(1,1,0x00);
                    break; // chay 100%      
      }
   }  
}


void main()
{

   set_tris_f(0x00); // thiet lap port f la ngo ra
   set_tris_d(0x00);
   set_tris_b(0xff);
   
   output_high(pin_d2);
   output_low(pin_f1);
   setup_motor_pwm(1,MPWM_FREE_RUN,4,1,0x5b);

   enable_interrupts(global);
   enable_interrupts(int_timer1);
   enable_interrupts(int_timer4);
   enable_interrupts(int_rda2);
   
   setup_timer1(TMR_INTERNAL|TMR_DIV_BY_8);
   set_timer1(42496);
   bdn = 0;
   
   setup_timer4(TMR_INTERNAL|TMR_DIV_BY_8);
   
   
   setup_qei(QEI_MODE_X2|QEI_RESET_WHEN_MAXCOUNT,QEI_FILTER_DIV_1,0x7d0);

   

   while(true)
   {  
      if(tt_on == 1)
      {
         if(tt_dem == 1)
         {
            doc_encoder_va_gui_du_lieu();
            tt_dem = 0;
         }
         output_high(pin_f6);
         nhan_du_lieu_dieu_khien();

      } 
      else 
      {
         doc_encoder_va_gui_du_lieu();
         output_low(pin_f6);
         nhan_du_lieu_dieu_khien();
         
      }

   }
}