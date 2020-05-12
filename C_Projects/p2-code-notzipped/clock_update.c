#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "clock.h"

int set_tod_from_secs(int TIME_OF_DAY_SEC, tod_t *tod)
{
  if (TIME_OF_DAY_SEC < 0 || TIME_OF_DAY_SEC > 86400)
  {
    return 1; // error has occurred, return 1 for failure
  }
  tod->ispm = 0;
  int hours = TIME_OF_DAY_SEC / 3600; // integer division to find how many hours
  if (hours > 12)
  {
    hours = hours % 12;
    tod->ispm = 1;
  }
  if (hours == 12){
    tod->ispm = 1;
  }
  tod->hours = hours;
  tod-> minutes = (TIME_OF_DAY_SEC % 3600) / 60; //take the remainder of the hours and divide by 60 to get minutes
  tod->seconds = ((TIME_OF_DAY_SEC % 3600) % 60); //take remainder of minutes and hours is seconds
  if (tod->hours ==0){
    tod->hours = 12;
  }
  return 0; // all fields of tod have been set, return 0 for success
}

int set_display_bits_from_tod(tod_t tod, int *display){
  if (tod.hours > 12 || tod.hours < 0){  // i want to do a huge logical or statement, should I
    return 1;
  }
  if (tod.minutes > 59 || tod.minutes < 0){
    return 1;
  }
  if (tod.seconds > 59 || tod.seconds < 0){
    return 1;
  }
  // is this how you set a int list?//
  int mask[11] = {0b0111111, 0b0000110, 0b1011011, 0b1001111, 0b1100110, 0b1101101, 0b1111101, 0b0000111, 0b1111111, 0b1101111, 0b0000000};
  // list of mask values where index matches the displayed number
  // except for the 10th index being blank
  int min_tens = (tod.minutes) / 10;
  int min_ones = (tod.minutes) % 10;
  int hour_tens = (tod.hours) / 10;
  int hour_ones = (tod.hours) % 10;
  *display = 0b00000000000000000000000000000000; // should this be 32 or 30? rn its 32
  if (tod.ispm==1){
    *display = *display | (0b1<<29); //setting pm
  }
  else{
    *display = *display | (0b1<<28); //setting am
  }
  if (hour_tens == 0){
    hour_tens = 10; // setting hours_tens to index into the blank mask
  }
  *display = *display | (mask[hour_tens] << 21);
  *display = *display | (mask[hour_ones] << 14);
  *display = *display | (mask[min_tens] << 7);
  *display = *display | (mask[min_ones]);
  return 0;
}

int clock_update(){
  tod_t tod;
  if (set_tod_from_secs (TIME_OF_DAY_SEC, &tod) == 1){
    return 1;   //reports error back from set_tod
  }
  if (set_display_bits_from_tod(tod, &CLOCK_DISPLAY_PORT) == 1){
    return 1;   //reports error back from set_display
  }
  return 0;     //reports success
}
