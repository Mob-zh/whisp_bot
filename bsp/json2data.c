#include "cJSON.h"
#include <stdlib.h>
#include "json2data.h"
#include "string.h"
#include <ctype.h>

//找到命令源（未测试）
ParsedData Commandname_parse_json(ParsedData last_sta,const char *input_str) {
    ParsedData result = last_sta; 

		const char *text_str = "\"command_name\":\"";
    char *text_pos = strstr(input_str, text_str);
		char command_name[100];
    if (text_pos != NULL) {
        text_pos += strlen(text_str); 

				int i = 0;
        while (*text_pos && (*text_pos) != '\"'&& i<100) {
                command_name[i++] = *text_pos;
                text_pos++;
        }

        if (command_name[0]) {
						strcpy(result.command_name,command_name);
            result.valid = 1;
        }
				
    }
    return result;
}

//找到move操作指令
ParsedData move_parse_json(ParsedData last_sta,const char *input_str) {
    ParsedData result = last_sta;

		const char *move_str = "\"move\":";
    const char *move_pos = strstr(input_str, move_str);
    if (move_pos != NULL) {
        move_pos += strlen(move_str); // 跳过 "move": 部分

        // 查找数字部分
        while (*move_pos && !isdigit(*move_pos)) {
            move_pos++;
        }

        if (isdigit(*move_pos)) {
            result.move = atoi(move_pos);
            result.valid = 1;
        }
				
    }
		
    return result;
}


//找到医嘱文字（未测试）
ParsedData text_parse_json(ParsedData last_sta,const char *input_str) {
	  strcpy(last_sta.yizhu,"");
    ParsedData result = last_sta; 
		const char *text_str = "\"text\":\"";
    const char *text_pos = strstr(input_str, text_str);
		char yizhu[100];
    if (text_pos != NULL) {
        text_pos += strlen(text_str); 

				int i = 0;
        while (*text_pos && (*text_pos) != '\"'&& i<100) {
						yizhu[i++] = *text_pos;
            text_pos++;
        }

        if (yizhu[0]) {
						strcpy(result.yizhu,yizhu);
            result.valid = 1;
        }
				
    }
		
    return result;
}

//找到开关药盒指令（未测试）
ParsedData Box_parse_json(ParsedData last_sta,const char *input_str) {
    ParsedData result = last_sta; // 初始化结构体，默认无效

		const char *Boxid_str = "\"Boxid\":";
    const char *Boxid_pos = strstr(input_str, Boxid_str);
    if (Boxid_pos != NULL) {
        Boxid_pos += strlen(Boxid_str); // 跳过 "move": 部分
				
        // 查找数字部分
        while (*Boxid_pos && !isdigit(*Boxid_pos)) {
            Boxid_pos++;
        }

        if (isdigit(*Boxid_pos)) {
            int Boxid = atoi(Boxid_pos);
					
						const char *switch_str = "\"switch\":";
						const char *switch_pos = strstr(input_str, switch_str);
						if (switch_pos != NULL) {
								switch_pos += strlen(switch_str); // 跳过 "switch_pos": 部分
								// 查找数字部分
								while (*switch_pos && !isdigit(*switch_pos)) {
										switch_pos++;
								}
								if(isdigit(*switch_pos))
								{
									if(Boxid == 1||Boxid == '1'){
										result.Box1 = atoi(switch_pos);
									}
									else if(Boxid == 2||Boxid == '2'){
										result.Box2 = atoi(switch_pos);
									}
								}
						}
        }
				
    }
		
    return result;
}


