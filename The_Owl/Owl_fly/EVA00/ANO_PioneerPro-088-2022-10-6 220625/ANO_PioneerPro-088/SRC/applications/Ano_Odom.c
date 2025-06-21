/*==========================================================================
 * 描述    ：通过对速度积分生成里程计数据供给SLAM使用
 * 更新时间：2025-06-21 
 * 作者		 ：RZR
============================================================================*/


#include "Ano_Odom.h"
#include "Ano_LocCtrl.h"

#define _VEL_X      (loc_ctrl_1.fb[0])
#define _VEL_Y      (loc_ctrl_1.fb[1])

Position2D total_displacement;

// 位移计算函数
void CalculateDisplacement(float delta_time_sec) 
{
    // 计算微小位移增量 (Δd = v * Δt)
    float delta_x = _VEL_X * delta_time_sec;
    float delta_y = _VEL_Y * delta_time_sec;
    
    // 累加到总位移
    total_displacement.X += delta_x;
    total_displacement.Y += delta_y;

}

void PosCalculation_Task(u8 dT_ms) 
{
    static u16 timtmp = 0;
    timtmp += dT_ms;
    
    // 步骤1：将毫秒时间转换为秒（用于速度积分）
    float dT_sec = (float)dT_ms / 1000.0f;
    // 步骤2：执行位移计算
    CalculateDisplacement(dT_sec);

}
