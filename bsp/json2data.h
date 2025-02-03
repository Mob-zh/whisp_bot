#ifndef _JSON2DATA_H_
#define _JSON2DATA_H_

#include "main.h"
typedef struct {
    int move;
		int Box1;
		int Box2;
		char yizhu[100];
    char service_id[50];
    char command_name[50];
    char request_id[100];
    int valid;
} ParsedData;
ParsedData Commandname_parse_json(ParsedData last_sta,const char *input_str) ;//查命令名
ParsedData move_parse_json(ParsedData last_sta,const char *json_str);//查运动指令
ParsedData text_parse_json(ParsedData last_sta,const char *input_str);//查医嘱文本
ParsedData Box_parse_json(ParsedData last_sta,const char *input_str) ;//查箱子开关
#endif

