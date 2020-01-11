#ifndef _FLAGS_STATIC_H_
#define _FLAGS_STATIC_H_

/*
Toma el mutex de las flags
@Params:
@Return: true si tomó el mutex, false de lo contrario
*/
static bool _flags_take_mutex();

/*
Devuelve mutex
@Params:
@Return:
*/
static void _flags_give_mutex();

#endif