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
        ins_arr = new instruction[inst_num];
        for (int i = 0; i < REG_NUM; i++)
        {
            reg_false_dep[i] = -1;
            reg_last_write[i] = -1;
        }
    }
};

int maxFunc(int first, int second){
    return (first > second) ? first : second;
}

ProgCtx analyzeProg(const unsigned int opsLatency[], const InstInfo progTrace[], unsigned int numOfInsts) {
    //
    //  for all instructions
    //  get dependencies
    //  update reg_last_write
    //  update depth by max of dependencies
    //  false dependecies++

    prog_data PD = new prog_data(numOfInsts);
    for (int i = 0; i < numOfInsts; i++){
        //initializes instruction array
        PD.ins_arr[i] = new instruction;
        //starts with no depth
        PD.ins_arr[i].depth = 0;
        //starts with opcode cycle
        PD.ins_arr[i].exe_cycles = opsLatency[progTrace[i].opcode];
        int tempDepth = 0;
        int tempCycles = 0
        //found dependency with previous instruction and save dep1 and dep2
        for(int j = i; j > 0; j--){
            if(PD.reg_last_write[progTrace[i].src1Idx] != -1){
                PD.ins_arr[i].dep1 = PD.reg_last_write[progTrace[i].src1Idx];
                //finds depth of previous instruction
                tempDepth = PD.ins_arr[PD.ins_arr[i].dep1].depth;
                //finds cycles needed to complete dependency
                tempCycles = PD.ins_arr[PD.ins_arr[i].dep1].exe_cycles;
            }
            if(PD.reg_last_write[progTrace[i].src2Idx] != -1){
                PD.ins_arr[i].dep2 = PD.reg_last_write[progTrace[i].src2Idx];
                //compares two depths from registers used and selects deeper one
                tempDepth = maxFunc(tempDepth, PD.ins_arr[PD.ins_arr[i].dep2].depth);
                //compares two cycles from registers used and selects longer one
                tempCycles = maxFunc(tempCycles, PD.ins_arr[PD.ins_arr[i].dep2].depth);
                break;
            }
        }
        PD.ins_arr[i].exe_cycles += tempCycles;
        PD.ins_arr[i].depth = tempDepth;

        //initializes register last written to array
        PD.reg_last_write[progTrace[i].dstIdx] = i;

        //increases number of registers written to, finding false dependencies
        PD.reg_false_dep[progTrace[i].dstIdx]++;
    }


    return PD;


}
/*
ProgCtx analyzeProg(const unsigned int opsLatency[], const InstInfo progTrace[], unsigned int numOfInsts)
{
    prog_data* prog = new prog_data(numOfInsts);
    prog->instruction_num = numOfInsts;
    int tmp_depth1, tmp_depth2, tmp, reg;
    for (int i = 0; i < numOfInsts; i++)
    {
        prog->ins_arr[i].dep1 = prog->reg_last_write[progTrace[i].src1Idx];
        prog->ins_arr[i].dep2 = prog->reg_last_write[progTrace[i].src2Idx];
        prog->ins_arr[i].exe_cycles = opsLatency[progTrace[i].opcode];
        if(prog->ins_arr[i].dep1 != -1)
        {
            tmp = prog->ins_arr[i].dep1;
            tmp_depth1 = prog->ins_arr[tmp].depth + prog->ins_arr[tmp].exe_cycles;
        }
        else
            tmp_depth1 = 0;
        if(prog->ins_arr[i].dep2 != -1)
        {
            tmp = prog->ins_arr[i].dep2;
            tmp_depth2 = prog->ins_arr[tmp].depth + prog->ins_arr[tmp].exe_cycles;
        }
        else
            tmp_depth2 = 0;
        prog->ins_arr[i].depth = (tmp_depth1 >= tmp_depth2) ? tmp_depth1 : tmp_depth2;
        reg = progTrace[i].dstIdx;
        prog->reg_false_dep[reg]++;
        prog->reg_last_write[reg] = i;
    }
    return prog;
}*/

void freeProgCtx(ProgCtx ctx)
{
    if (ctx == PROG_CTX_NULL)
        return;
    prog_data *prog = (prog_data *) ctx;
    free(prog->ins_arr);
    free(prog);
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


