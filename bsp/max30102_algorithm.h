#ifndef _MAX30102_ALGORITHM_H_
#define _MAX30102_ALGORITHM_H_

#include "main.h"
#include "max30102.h"

#define true 1
#define false 0
#define FS 100
#define BUFFER_SIZE  (FS* 5) 
#define HR_FIFO_SIZE 7
#define MA4_SIZE  4 // DO NOT CHANGE
#define HAMMING_SIZE  5// DO NOT CHANGE
#define min(x,y) ((x) < (y) ? (x) : (y))
 
extern uint8_t TempData[6];
extern uint32_t red_buffer[500];  //红光数据red，用于计算心率
extern uint32_t ir_buffer[500];  //红外数据 ir，用于计算血氧
extern int32_t ir_buffer_length;   //计算前500个样本得到的数据
extern int32_t pn_SpO2_value;   //血氧实际值
extern int8_t SpO2_valid;        //血氧值有效标志
extern int32_t pn_hr_value;    //心率实际值
extern int8_t hr_valid;         //心率有效标志
extern uint32_t red_max,red_min;  //红光取值范围
extern uint32_t prev_data;      //前一次的值
extern float f_temp;           //临时变量
extern int32_t n_brightness;    //明确变量 
 
void maxim_heart_rate_and_oxygen_saturation(uint32_t *pun_ir_buffer ,  int32_t n_ir_buffer_length, uint32_t *pun_red_buffer ,   int32_t *pn_spo2, int8_t *pch_spo2_valid ,  int32_t *pn_heart_rate , int8_t  *pch_hr_valid);
void maxim_find_peaks( int32_t *pn_locs, int32_t *pn_npks,  int32_t *pn_x, int32_t n_size, int32_t n_min_height, int32_t n_min_distance, int32_t n_max_num );
void maxim_peaks_above_min_height( int32_t *pn_locs, int32_t *pn_npks,  int32_t *pn_x, int32_t n_size, int32_t n_min_height );
void maxim_remove_close_peaks( int32_t *pn_locs, int32_t *pn_npks,   int32_t  *pn_x, int32_t n_min_distance );
void maxim_sort_ascend( int32_t *pn_x, int32_t n_size );
void maxim_sort_indices_descend(  int32_t  *pn_x, int32_t *pn_indx, int32_t n_size);



#endif
