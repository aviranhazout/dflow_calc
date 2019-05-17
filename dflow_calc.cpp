/* 046267 Computer Architecture - Spring 2019 - HW #3 */
/* Implementation (skeleton)  for the dataflow statistics calculator */

#include "dflow_calc.h"

#define REG_NUM 32

class instruction
{
public:
    int depth;
    int dep1;
    int dep2;
    int exe_cycles;
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
        ins_arr = new insruction[inst_num];
        for (int i = 0; i < REG_NUM; i++)
        {
            reg_false_dep[i] = -1;
            reg_last_write[i] = -1;
        }
    }
};

ProgCtx analyzeProg(const unsigned int opsLatency[], const InstInfo progTrace[], unsigned int numOfInsts) {
    /*
     * for all instructions
     *      get dependencies
     *      update reg_last_write
     *      update depth by max of dependencies
     *      false dependecies++
     */
    return PROG_CTX_NULL;
}

void freeProgCtx(ProgCtx ctx) {
}

int getInstDepth(ProgCtx ctx, unsigned int theInst)
{
    if (ctx == PROG_CTX_NULL)
        return -1;
    if (theInst >= prog->instruction_num)
        return -1;
    prog_data* prog = (prog_data*)ctx;
    return prog->ins_arr[theInst].depth;
}

int getInstDeps(ProgCtx ctx, unsigned int theInst, int *src1DepInst, int *src2DepInst)
{
    if (ctx == PROG_CTX_NULL)
        return -1;
    if (theInst >= prog->instruction_num)
        return -1;
    prog_data* prog = (prog_data*)ctx;
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


