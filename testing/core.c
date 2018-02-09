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
#include "core.h"

bulbType BULB_NIL = bulbPrintNil,  BULB_BOOL = bulbPrintBool, 
            BULB_INT = bulbPrintInt, BULB_FLOAT = bulbPrintFloat, 
            BULB_SYMBOL = bulbPrintSymbol, BULB_STRING = bulbPrintString, 
            BULB_PROCEDURE = bulbPrintProcedure, 
            BULB_PRIMITIVE = bulbPrintPrimitive;
bulbType BULB_CONS = bulbPrintCons;

bulbObj bulbNilObj = {&bulbPrintNil, NULL};
bulbObj *bulbNil = &bulbNilObj;

bulbObj bulbTrueObj = {&bulbPrintBool, &bulbNilObj};
bulbObj *bulbTrue = &bulbTrueObj;

bulbObj bulbFalseObj = {&bulbPrintBool, &bulbNilObj};
bulbObj *bulbFalse = &bulbFalseObj;

void bulbPrintNil(bulbObj *o) 
{
}

void bulbPrintBool(bulbObj *o) 
{
    printf((o == bulbTrue) ? "#t" : "#f");
} 

void bulbPrintInt(bulbObj *o) 
{
    printf("%d", *((long *) o->data));
} 

void bulbPrintFloat(bulbObj *o) 
{
    printf("%f", *((double *) o->data));
} 

void bulbPrintSymbol(bulbObj *o) 
{
    printf((*((bulbSymbol *) o->data)).data);
} 

void bulbPrintString(bulbObj *o) 
{
    printf((*((bulbString *) o->data)).data);
} 

void bulbPrintProcedure(bulbObj *o) 
{
    printf("#<PROCEDURE>");
}

void bulbPrintPrimitive(bulbObj *o) 
{
    printf("#<PRIMITIVE>");
}

void bulbPrintCons(bulbObj *o) 
{
    printf("#<PAIR>");
}

bool bulbIsAtom(bulbObj *obj)
{
    return obj->type != BULB_CONS;
}

void bulbPrintAtom(bulbObj *obj)
{
    bulbPrintAtomDisplay(obj, true);
}

void bulbPrintAtomDisplay(bulbObj *obj, bool display)
{
    if (obj->type == BULB_CONS) bulb_err_not_a_pair(obj);
    if ((obj->type) == NULL) {
        printf("#<UNKNOWN>");
        return;
    }
    if (obj->type == BULB_STRING && display) {
        printf("\"");
        obj->type(obj);
        printf("\"");
    } else obj->type(obj);
}


void bulbPrintAst(bulbObj *tree)
{
    bulbPrintAstDisplay(tree, true);
}

void bulbPrintAstDisplay(bulbObj *tree, bool display)
{
    if (bulbIsAtom(tree)) {
        bulbPrintAtomDisplay(tree, display);
    } else if (bulbGetCar(tree) != bulbNil) {
        if (bulbGetCar(tree)->type == BULB_CONS) { 
            printf("(");
            bulbPrintAstDisplay(bulbGetCar(tree), display);
            printf(")");
            if (bulbGetCdr(tree) != bulbNil) printf(" ");
        } else {
            bulbPrintAtomDisplay(bulbGetCar(tree), display);
            if (!bulbIsAtom(bulbGetCdr(tree))) printf(" ");
        }
        bulbPrintAstDisplay(bulbGetCdr(tree), display);
    }
}

bulbCons *bulbMakeCons(bulbObj *obj)
{
    if (obj == NULL || bulbIsAtom(obj)) bulb_err_not_a_pair(obj);
    return (bulbCons*) obj->data;
}

bulbObj *bulbNewConsObj(bulbObj *car, bulbObj *cdr)
{
    bulbObj *cons = (bulbObj*) malloc(sizeof(bulbObj));
    cons->type = BULB_CONS;
    cons->data = malloc(sizeof(bulbCons));
    bulbSetCar(cons, car);
    bulbSetCdr(cons, cdr);
    return cons;
}

bulbObj *bulbNewStringObj(char *text, unsigned len)
{
    bulbObj *string = (bulbObj*) malloc(sizeof(bulbObj));
    string->type = BULB_STRING;
    string->data = (bulbString*) malloc(sizeof(bulbString*));
    ((bulbString*) string->data)->data = text;
    ((bulbString*) string->data)->len = len;
    return string;
}

char *bulbGetStringText(bulbObj *string)
{
    return ((bulbString*) string->data)->data;
}

bulbObj *bulbGetCar(bulbObj *list)
{
    return bulbMakeCons(list)->car;
}

void bulbSetCar(bulbObj *list, bulbObj *val)
{
    bulbMakeCons(list)->car = val;
}

bulbObj *bulbGetCdr(bulbObj *list)
{
    return bulbMakeCons(list)->cdr;
}

void bulbSetCdr(bulbObj *list, bulbObj *val)
{
    bulbMakeCons(list)->cdr = val;
}

bulbObj *bulbGetCaar(bulbObj *list)
{
    return bulbGetCar(bulbGetCar(list));
}

void bulbSetCaar(bulbObj *list, bulbObj *val)
{
    bulbSetCar(bulbGetCar(list), val);
}

bulbObj *bulbGetCddr(bulbObj *list)
{
    return bulbGetCdr(bulbGetCdr(list));
}

void bulbSetCddr(bulbObj *list, bulbObj *val)
{
    bulbSetCdr(bulbGetCdr(list), val);
}

bulbObj *bulbGetCadr(bulbObj *list)
{
    return bulbGetCar(bulbGetCdr(list));
}

void bulbSetCadr(bulbObj *list, bulbObj *val)
{
    bulbSetCar(bulbGetCdr(list), val);
}

bulbObj *bulbGetCdar(bulbObj *list)
{
    return bulbGetCdr(bulbGetCar(list));
}

void bulbSetCdar(bulbObj *list, bulbObj *val)
{
    bulbSetCdr(bulbGetCar(list), val);
}

unsigned bulbListLen(bulbObj *list)
{
    unsigned len = 0;
    while (bulbGetCar(list)->type != BULB_NIL) {
        len++;
        list = bulbGetCdr(list);
    }
    return len;
}

bulbObj *bulbListGet(bulbObj *list, unsigned index)
{
    unsigned i = 0, len = bulbListLen(list);
    if (index > len) bulb_err_out_of_bounds(index, len);
    while (bulbGetCar(list)->type != BULB_NIL && i < index) {
        i++;
        list = bulbGetCdr(list);
    }
    return bulbGetCar(list);
}

void bulb_err_not_a_pair(bulbObj *obj)
{
    printf("Exception: ");
    bulbPrintAst(obj);
    printf(" is not a pair.\n");
    exit(1);
}

void bulb_err_out_of_bounds(unsigned index, unsigned len)
{
    printf("Exception: index (%d) out of bounds (len: %d).\n", index, len);
    exit(1);
}