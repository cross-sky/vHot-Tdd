/***
 * Excerpted from "Test-Driven Development for Embedded C",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material, 
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose. 
 * Visit http://www.pragmaticprogrammer.com/titles/jgade for more book information.
***/
/*- ------------------------------------------------------------------ -*/
/*-    Copyright (c) James W. Grenning -- All Rights Reserved          -*/
/*-    For use by owners of Test-Driven Development for Embedded C,    -*/
/*-    and attendees of Renaissance Software Consulting, Co. training  -*/
/*-    classes.                                                        -*/
/*-                                                                    -*/
/*-    Available at http://pragprog.com/titles/jgade/                  -*/
/*-        ISBN 1-934356-62-X, ISBN13 978-1-934356-62-3                -*/
/*-                                                                    -*/
/*-    Authorized users may use this source code in your own           -*/
/*-    projects, however the source code may not be used to            -*/
/*-    create training material, courses, books, articles, and         -*/
/*-    the like. We make no guarantees that this source code is        -*/
/*-    fit for any purpose.                                            -*/
/*-                                                                    -*/
/*-    www.renaissancesoftware.net james@renaissancesoftware.net       -*/
/*- ------------------------------------------------------------------ -*/

#ifndef D_CircularBuffer_H
#define D_CircularBuffer_H
#include <stdint.h>

typedef struct CircularBufferStruct *CircularBuffer;

CircularBuffer CircularBuffer_Create(int16_t capacity);
void CircularBuffer_Destroy(CircularBuffer);
int16_t CircularBuffer_IsEmpty(CircularBuffer);
int16_t CircularBuffer_IsFull(CircularBuffer);
int16_t CircularBuffer_Put(CircularBuffer, int16_t);
int16_t CircularBuffer_Get(CircularBuffer);
int16_t CircularBuffer_Capacity(CircularBuffer);
void CircularBuffer_Print(CircularBuffer);
int16_t CircularBuffer_VerifyIntegrity(CircularBuffer);
#endif  /* D_CircularBuffer_H */
