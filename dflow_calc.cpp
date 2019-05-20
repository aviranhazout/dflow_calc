/* 046267 Computer Architecture - Spring 2019 - HW #3 */
/* Implementation (skeleton)  for the dataflow statistics calculator */

#include "dflow_calc.h"
#include <stdio.h>

#define REG_NUM 32

class instruction
{
public:
    int depth;
    int dep1;
    int dep2;
    int exe_cycles;
    int opcode_cyc;
    instruction* next_ins;
};
class prog_data
{
public:
    instruction* ins_arr;
    int instruction_num;
    int reg_false_dep[REG_NUM];
    int reg_last_write[REG_NUM];

    prog_data(int inst_num)
    {
        instruction_num = inst_num;
        ins_arr = new instruction[inst_num];
        for (int j = 0; j < inst_num; ++j) {
            ins_arr[j].dep1 = -1;
            ins_arr[j].dep2 = -1;
            ins_arr[j].exe_cycles = 0;
            //starts with no depth
            ins_arr[j].depth = 0;
            //for testing - REMOVE BEFORE SUBMISSION
            ins_arr[j].next_ins = nullptr;
            if(j < inst_num -1)
                ins_arr[j].next_ins = &ins_arr[j+1];
        }
        for (int i = 0; i < REG_NUM; i++)
        {
            reg_false_dep[i] = 0;
            reg_last_write[i] = -1;
        }
    }
};

int maxFunc(int first, int second){
    return (first > second) ? first : second;
}
//ProgCtx analyzeProg(const unsigned int opsLatency[], const InstInfo progTrace[], unsigned int numOfInsts);
//ProgCtx analyzeProg(const unsigned int opsLatency[], const InstInfo progTrace[], unsigned int numOfInsts);
//
//  for all instructions
//  get dependencies
//  update reg_last_write
//  update depth by max of dependencies
//  false dependencies++
ProgCtx analyzeProg(const unsigned int opsLatency[],  InstInfo progTrace[], unsigned int numOfInsts)
{
    prog_data* PD = new prog_data(numOfInsts);
    PD->instruction_num = numOfInsts;
    unsigned int tmp_depth1, tmp_depth2, tmp;
    for (int i = 0; i < numOfInsts; i++){
        //starts with opcode cycle
        PD->ins_arr[i].opcode_cyc = opsLatency[progTrace[i].opcode];
        int tempDepth = 0;
        int tempCycles = 0;

        PD->ins_arr[i].dep1 = PD->reg_last_write[progTrace[i].src1Idx];
        PD->ins_arr[i].dep2 = PD->reg_last_write[progTrace[i].src2Idx];
        PD->ins_arr[i].exe_cycles = opsLatency[progTrace[i].opcode];
        if(PD->ins_arr[i].dep1 != -1)
        {
            tmp = PD->ins_arr[i].dep1;
            tmp_depth1 = PD->ins_arr[tmp].depth + PD->ins_arr[tmp].exe_cycles;
        }
        else
            tmp_depth1 = 0;
        if(PD->ins_arr[i].dep2 != -1)
        {
            tmp = PD->ins_arr[i].dep2;
            tmp_depth2 = PD->ins_arr[tmp].depth + PD->ins_arr[tmp].exe_cycles;
        }
        else
            tmp_depth2 = 0;
        PD->ins_arr[i].depth = maxFunc(tmp_depth1, tmp_depth2);
        //initializes register last written to array
        PD->reg_last_write[progTrace[i].dstIdx] = i;

        //increases number of registers written to, finding false dependencies
        int reg = progTrace[i].dstIdx;
        if (i > 0 &&
          (reg == progTrace[i - 1].dstIdx || reg == progTrace[i - 1].src1Idx || reg == progTrace[i - 1].src2Idx))
        {
            PD->reg_false_dep[progTrace[i].dstIdx]++;
        }
    }


    return PD;


}

void freeProgCtx(ProgCtx ctx)
{
    if (ctx == PROG_CTX_NULL)
        return;
    prog_data *prog = (prog_data *) ctx;
    delete prog->ins_arr;
    delete prog;
}

int getInstDepth(ProgCtx ctx, unsigned int theInst)
{
    if (ctx == PROG_CTX_NULL)
        return -1;
    prog_data* prog = (prog_data*)ctx;
    if (theInst >= prog->instruction_num)
        return -1;
    return prog->ins_arr[theInst].depth;
}

int getInstDeps(ProgCtx ctx, unsigned int theInst, int *src1DepInst, int *src2DepInst)
{
    if (ctx == PROG_CTX_NULL)
        return -1;
    prog_data* prog = (prog_data*)ctx;
    if (theInst >= prog->instruction_num)
        return -1;
    *src1DepInst = prog->ins_arr[theInst].dep1;
    *src2DepInst = prog->ins_arr[theInst].dep2;
    return 0;
}

int getRegfalseDeps(ProgCtx ctx, unsigned int reg)
{
    if (ctx == PROG_CTX_NULL)
        return -1;
    if (reg > REG_NUM)
	    return -1;
    prog_data* prog = (prog_data*)ctx;
    if (prog->reg_false_dep[reg] == -1)
        return 0;
    return prog->reg_false_dep[reg];
}

int getProgDepth(ProgCtx ctx)
{
    if (ctx == PROG_CTX_NULL)
        return -1;
    prog_data* prog = (prog_data*)ctx;
    int max_depth = 0;
    int ins_depth;
    for (int i = 0; i < prog->instruction_num; i++)
    {
        ins_depth = prog->ins_arr[i].depth + prog->ins_arr[i].exe_cycles;
        if (ins_depth > max_depth)
            max_depth = ins_depth;
    }
    return max_depth;
}


