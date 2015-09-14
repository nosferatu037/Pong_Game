/* 
 * File:   fitrange.h
 * Author: nosferatu
 *
 * Created on July 7, 2015, 2:32 PM
 */

#ifndef FITRANGE_H
#define	FITRANGE_H

float fitrange(float cur, float newMin, float newMax, float oldMin, float oldMax)
{
    if(cur > oldMax)
    {
        cur = oldMax;
    }
    if(cur < oldMin)
    {
        cur = oldMin;
    }
    
    return ((cur - oldMin) / (oldMax - oldMin)) * (newMax - newMin) + newMin;
}


#endif	/* FITRANGE_H */

