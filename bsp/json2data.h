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
ParsedData Commandname_parse_json(ParsedData last_sta,const char *input_str) ;//��������
ParsedData move_parse_json(ParsedData last_sta,const char *json_str);//���˶�ָ��
ParsedData text_parse_json(ParsedData last_sta,const char *input_str);//��ҽ���ı�
ParsedData Box_parse_json(ParsedData last_sta,const char *input_str) ;//�����ӿ���
#endif

