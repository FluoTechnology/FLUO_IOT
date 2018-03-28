/*
*  Print last reset reason of ESP32
*  =================================
*
*  Use either of the methods print_reset_reason
*  or verbose_print_reset_reason to display the
*  cause for the last reset of this device.
*
*  Public Domain License.
*
*  Author:
*  Evandro Luis Copercini - 2017
*/

#include <rom/rtc.h>
#include <Fluotube.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

void print_reset_reason(RESET_REASON reason)
{
  switch ( reason)
  {
    case 1 : FluoTube.debugln ("POWERON_RESET");break;          /**<1,  Vbat power on reset*/
    case 3 : FluoTube.debugln ("SW_RESET");break;               /**<3,  Software reset digital core*/
    case 4 : FluoTube.debugln ("OWDT_RESET");break;             /**<4,  Legacy watch dog reset digital core*/
    case 5 : FluoTube.debugln ("DEEPSLEEP_RESET");break;        /**<5,  Deep Sleep reset digital core*/
    case 6 : FluoTube.debugln ("SDIO_RESET");break;             /**<6,  Reset by SLC module, reset digital core*/
    case 7 : FluoTube.debugln ("TG0WDT_SYS_RESET");break;       /**<7,  Timer Group0 Watch dog reset digital core*/
    case 8 : FluoTube.debugln ("TG1WDT_SYS_RESET");break;       /**<8,  Timer Group1 Watch dog reset digital core*/
    case 9 : FluoTube.debugln ("RTCWDT_SYS_RESET");break;       /**<9,  RTC Watch dog Reset digital core*/
    case 10 : FluoTube.debugln ("INTRUSION_RESET");break;       /**<10, Instrusion tested to reset CPU*/
    case 11 : FluoTube.debugln ("TGWDT_CPU_RESET");break;       /**<11, Time Group reset CPU*/
    case 12 : FluoTube.debugln ("SW_CPU_RESET");break;          /**<12, Software reset CPU*/
    case 13 : FluoTube.debugln ("RTCWDT_CPU_RESET");break;      /**<13, RTC Watch dog Reset CPU*/
    case 14 : FluoTube.debugln ("EXT_CPU_RESET");break;         /**<14, for APP CPU, reseted by PRO CPU*/
    case 15 : FluoTube.debugln ("RTCWDT_BROWN_OUT_RESET");break;/**<15, Reset when the vdd voltage is not stable*/
    case 16 : FluoTube.debugln ("RTCWDT_RTC_RESET");break;      /**<16, RTC Watch dog reset digital core and rtc module*/
    default : FluoTube.debugln ("NO_MEAN");
  }
}

void verbose_print_reset_reason(RESET_REASON reason)
{
  switch ( reason)
  {
    case 1  : FluoTube.debugln ("Vbat power on reset");break;
    case 3  : FluoTube.debugln ("Software reset digital core");break;
    case 4  : FluoTube.debugln ("Legacy watch dog reset digital core");break;
    case 5  : FluoTube.debugln ("Deep Sleep reset digital core");break;
    case 6  : FluoTube.debugln ("Reset by SLC module, reset digital core");break;
    case 7  : FluoTube.debugln ("Timer Group0 Watch dog reset digital core");break;
    case 8  : FluoTube.debugln ("Timer Group1 Watch dog reset digital core");break;
    case 9  : FluoTube.debugln ("RTC Watch dog Reset digital core");break;
    case 10 : FluoTube.debugln ("Instrusion tested to reset CPU");break;
    case 11 : FluoTube.debugln ("Time Group reset CPU");break;
    case 12 : FluoTube.debugln ("Software reset CPU");break;
    case 13 : FluoTube.debugln ("RTC Watch dog Reset CPU");break;
    case 14 : FluoTube.debugln ("for APP CPU, reseted by PRO CPU");break;
    case 15 : FluoTube.debugln ("Reset when the vdd voltage is not stable");break;
    case 16 : FluoTube.debugln ("RTC Watch dog reset digital core and rtc module");break;
    default : FluoTube.debugln ("NO_MEAN");
  }
}

void setup() {
  // put your setup code here, to run once:
  FluoTube.setup();

  delay(2000);

  FluoTube.debugln("CPU0 reset reason:");
  print_reset_reason(rtc_get_reset_reason(0));
  verbose_print_reset_reason(rtc_get_reset_reason(0));

  FluoTube.debugln("CPU1 reset reason:");
  print_reset_reason(rtc_get_reset_reason(1));
  verbose_print_reset_reason(rtc_get_reset_reason(1));

  // Set ESP32 to go to deep sleep to see a variation
  // in the reset reason. Device will sleep for 5 seconds.
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  FluoTube.debugln("Going to sleep");
  esp_deep_sleep(5 * uS_TO_S_FACTOR);
}

void loop() {
  // put your main code here, to run repeatedly:

}

/*
  Example Serial Log:
  ====================

rst:0x10 (RTCWDT_RTC_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0x00
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:1
load:0x3fff0008,len:8
load:0x3fff0010,len:160
load:0x40078000,len:10632
load:0x40080000,len:252
entry 0x40080034
CPU0 reset reason:
RTCWDT_RTC_RESET
RTC Watch dog reset digital core and rtc module
CPU1 reset reason:
EXT_CPU_RESET
for APP CPU, reseted by PRO CPU
Going to sleep
ets Jun  8 2016 00:22:57

rst:0x5 (DEEPSLEEP_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0x00
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:1
load:0x3fff0008,len:8
load:0x3fff0010,len:160
load:0x40078000,len:10632
load:0x40080000,len:252
entry 0x40080034
CPU0 reset reason:
DEEPSLEEP_RESET
Deep Sleep reset digital core
CPU1 reset reason:
EXT_CPU_RESET
for APP CPU, reseted by PRO CPU
Going to sleep
ets Jun  8 2016 00:22:57

rst:0x5 (DEEPSLEEP_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0x00
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:1
load:0x3fff0008,len:8
load:0x3fff0010,len:160
load:0x40078000,len:10632
load:0x40080000,len:252
entry 0x40080034
CPU0 reset reason:
DEEPSLEEP_RESET
Deep Sleep reset digital core
CPU1 reset reason:
EXT_CPU_RESET
for APP CPU, reseted by PRO CPU
Going to sleep

*/
