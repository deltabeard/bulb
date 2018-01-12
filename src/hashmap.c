/*
 *  Bulb - the Lisp Interpreter
 *  Copyright (C) 2018-2019 bnzis (bonzisoft@protonmail.com)
 *  Copyright (C) 2012-2016, Yann Collet (xxhash)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "hashmap.h"
#include <stdio.h>
unsigned long long hash(const void* buffer, int length)
{
    unsigned long long const seed = 0;  
    unsigned long long const hash = XXH64(buffer, length, seed);
    return hash;
}

void put(hashmap_t *map, char *key, obj_t *obj)
{
    unsigned len = strlen(key), index = hash(key, len);
    index %= HMAP_ROWS;
    obj_t *pair = malloc(sizeof(obj_t));
    pair->type = CONS;
    pair->data.cons.car = malloc(sizeof(obj_t));
    pair->data.cons.car->type = CONS;
    pair->data.cons.car->data.cons.car = malloc(sizeof(obj_t));
    pair->data.cons.car->data.cons.car->type = SYMBOL;
    pair->data.cons.car->data.cons.car->data.symbol.buff = key;
    pair->data.cons.car->data.cons.car->data.symbol.len = len;
    pair->data.cons.car->data.cons.cdr = obj;
    if (map->data[index] == NULL)
        map->data[index] = pair;
    else {
        obj_t **ptr = &map->data[index]->data.cons.cdr;
        while ((*ptr) != NULL)
            ptr = &((*ptr)->data.cons.cdr);
        (*ptr) = pair;
    }
}

obj_t *get(hashmap_t *map, char *key)
{
    unsigned index = hash(key, strlen(key));
    index %= HMAP_ROWS;
    char *t = map->data[index]->data.cons.car->data.cons.car->data.symbol.buff;
    if (strcmp(t, key) == 0)
        return map->data[index]->data.cons.car->data.cons.cdr;
    else {
        obj_t *ptr = map->data[index]->data.cons.cdr;
        while (strcmp(t, key) != 0) {    
            t = ptr->data.cons.car->data.cons.car->data.symbol.buff;
            ptr = ptr->data.cons.cdr;
        }
        return ptr->data.cons.car->data.cons.cdr;
    }
}

void set(hashmap_t *map, char *key, obj_t *obj)
{
    unsigned index = hash(key, strlen(key));
    index %= HMAP_ROWS;
    char *t = map->data[index]->data.cons.car->data.cons.car->data.symbol.buff;
    if (strcmp(t, key) == 0)
        map->data[index]->data.cons.car->data.cons.cdr = obj;
    else {
        obj_t **ptr = &map->data[index];
        ptr = &((*ptr)->data.cons.cdr);
        t = (*ptr)->data.cons.car->data.cons.car->data.symbol.buff;
        while (strcmp(t, key) != 0) {
            t = (*ptr)->data.cons.car->data.cons.car->data.symbol.buff;
            ptr = &((*ptr)->data.cons.cdr);
        }
        (*ptr)->data.cons.car->data.cons.cdr = obj;
    }
}

void delete(hashmap_t *map, char *key)
{
    unsigned index = hash(key, strlen(key));
    index %= HMAP_ROWS;
    obj_t *tmp;
    char *t = map->data[index]->data.cons.car->data.cons.car->data.symbol.buff;
    if (strcmp(t, key) == 0) {
        tmp = map->data[index]->data.cons.car;
        map->data[index] = map->data[index]->data.cons.cdr;
        free(tmp);
    } else {
        obj_t **ptr = &map->data[index];
        do {
            ptr = &((*ptr)->data.cons.cdr);
            t = (*ptr)->data.cons.car->data.cons.car->data.symbol.buff;
        } while (strcmp(t, key) != 0);
        (*ptr) = (*ptr)->data.cons.cdr;
    }
}
