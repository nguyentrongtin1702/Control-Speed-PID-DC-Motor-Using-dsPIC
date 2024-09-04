#include <30F4011.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#fuses nowdt,put4,XT,noprotect
#use delay(clock=7.3728M)
//#use rs232 (baud=9600,xmit=pin_f5,rcv=pin_f4,parity = n,bits = 8, stop = 1, timeout = 1)// Khoi tao uart
#use rs232 (UART2,baud = 9600)
#define HLIM 91
#define LLIM 0
unsigned int32 rma;
unsigned int8 bdn = 0;
int1 tt_on=0;
unsigned int8 value = 0;
int1 refresh = 0;
int1 tt_nhan = 0;
int1 tt_dem = 0;
int1 tt_on_td = 0;
int1 tt_tn = 1;
int1 tt_pid = 1;
unsigned int16 speed;
unsigned int16 tocdodat;
unsigned int8 bdn1;
unsigned int16 round = 0;
unsigned int16 xung1;
unsigned int16 xung2;
unsigned int16 xung4;

#int_timer1 level = 4
void interrupts_timer1()
{
   set_timer1(42496);
   bdn++;
}

#int_rda2 level = 5
void interrupts_rda2()
{
   if(kbhit() == 1)
   {
      rma = getc();
      tt_nhan = 1;
   }
}

#int_qei level = 7
void interrupt_qei()
{
   round++;
}

#int_timer2 level = 6
void interrupts_timer2()
{
   set_timer2(4294967066);
   bdn1++;
   tt_pid = 1;
   
}

void bao_trang_thai_hoat_dong()
{
   int1 tt_led =0;
   if(bdn == 10)
   {
      tt_led = ~ tt_led;
   }
   if(tt_led == 0) output_high(pin_f6);
   else output_low(pin_f6);
}

void dieu_khien_toc_do_pwm_chay_thuan(unsigned int8 duty)
{
   delay_ms(200);
   output_high(pin_f1);
   set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
   set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,10,30);
   set_motor_pwm_duty(1,1,duty);
   delay_us(6);
   set_motor_pwm_duty(1,2,0x00);
}

void dieu_khien_toc_do_pwm_chay_nghich(unsigned int8 duty)
{
   delay_ms(200);
   output_high(pin_f1);
   set_motor_unit(1,1,MPWM_ENABLE|MPWM_INDEPENDENT,30,30);
   set_motor_unit(1,2,MPWM_ENABLE|MPWM_INDEPENDENT,10,30);
   set_motor_pwm_duty(1,2,duty);
   delay_us(6);
   set_motor_pwm_duty(1,1,0x00);
}

void nhan_du_lieu_dieu_khien()
{
   if(tt_nhan == 1)
   {
      if(rma == 0x30)
      {
         tocdodat = 300;
      }
      tt_nhan = 0;
   }
}

void che_do_xung_x1(unsigned int16 sovong)
{
   setup_qei(QEI_RESET_WHEN_MAXCOUNT|QEI_TIMER_EXTERNAL,QEI_FILTER_DIV_1,1000);
   xung1 = sovong/1000;
   printf("so vong doc duoc o che do X1 : %d\n",xung1);
}

void che_do_xung_x2(unsigned int16 sovong)
{
   setup_qei(QEI_MODE_X2|QEI_RESET_WHEN_MAXCOUNT|QEI_TIMER_EXTERNAL,QEI_FILTER_DIV_1,2000);
   xung2 = sovong/2000;
   printf("so vong doc duoc o che do X1 : %d\n",xung2);
}

void che_do_xung_x4(unsigned int16 sovong)
{
   setup_qei(QEI_MODE_X4|QEI_RESET_WHEN_MAXCOUNT|QEI_TIMER_EXTERNAL,QEI_FILTER_DIV_1,4000);
   xung4 = sovong/4000;
   printf("so vong doc duoc o che do X1 : %d\n",xung4);
}

void canh_bao_dong_dien()
{
   if(bdn1 == 5)
   {
      output_high(pin_d2);
   }
   else if(bdn1 == 10)
   {
      output_low(pin_d2);
      bdn1=0;
   }
}

void canh_bao_ngan_mach_led()
{
   if(bdn == 10)
   {
      output_high(pin_d2);
   }
   else if(bdn == 20)
   {
      output_low(pin_d2);
      bdn = 0;
   }
}

void doc_dong_dien()
{
   unsigned int16 Vin;
   float Amp;
   set_adc_channel(0);
   for(unsigned int8 i =0; i<10;i++)
   {
      Vin = read_adc();
   }
   Vin = Vin/100;
   Vin = Vin/0.01;
   Amp = (Vin*(5000.0f/1024))/23.5;
   printf("dong do duoc la %f :",Amp);
   if(Amp >= 21.0)
   {
      output_low(pin_f1);
      canh_bao_dong_dien();   
      
   }
   else output_high(pin_d2);
}

void doc_dien_ap()
{
   unsigned int16 Vin;
   float Vol;
   set_adc_channel(1);
   for(unsigned int8 i = 0; i<10; i++)
   {
      Vin = read_adc();
   }
   Vin = Vin/100;
   Vol = (Vin*(5000.0f/1024))*31;
   printf("dong do duoc la %f: ", Vol);
}

void canh_bao_ngan_mach()
{
   if(!input(pin_f0))
   {
      output_low(pin_f1);
      canh_bao_ngan_mach_led();
   }
   else 
   {
      output_low(pin_d2);
   }
}

void tinh_pid(unsigned int16 *tocdo ,unsigned int8 sample_time, int1 tt_chay)
{
   unsigned int8 ti ,tp, td;
   unsigned int8 e, e_cu;
   unsigned int16 soxung;
   unsigned int16 speed_new;
   speed_new = (*tocdo*60)/0.001;
   e = tocdodat - speed_new;
   tp = e;
   ti = (e-e_cu)*sample_time;
   td = td + e/sample_time;
   unsigned int8 pwm_duty;
   pwm_duty = tp + ti + td;
   pwm_duty = (unsigned int8)pwm_duty;
   if(pwm_duty > HLIM) pwm_duty = 91;
   else if (pwm_duty < LLIM) pwm_duty = 0;
   if(tt_chay==1)
   {
      dieu_khien_toc_do_pwm_chay_thuan(pwm_duty);
   }
   else dieu_khien_toc_do_pwm_chay_nghich(pwm_duty);
   
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
   enable_interrupts(int_timer2);
   enable_interrupts(int_rda2);
   enable_interrupts(int_qei);
   
   setup_timer1(TMR_INTERNAL|TMR_DIV_BY_8);
   set_timer1(42496);
   bdn = 0;
   
   setup_timer2(TMR_INTERNAL|TMR_DIV_BY_8|TMR_32_BIT);
   set_timer2(4294967066);
   
   
   
   while(true)
   {  
      bao_trang_thai_hoat_dong();
      nhan_du_lieu_dieu_khien();
      che_do_xung_x2(round);
      if(tt_pid == 1)
      {
         tinh_pid(&xung2,1000,1);
         tt_pid = 0;
      }
      doc_dong_dien();
      doc_dien_ap();
      canh_bao_ngan_mach();
      
   }
}
