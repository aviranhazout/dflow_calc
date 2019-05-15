/* 046267 Computer Architecture - Spring 2019 - HW #3 */
/* Implementation (skeleton)  for the dataflow statistics calculator */

#include "dflow_calc.h"

#define REG_NUM 32
class insruction
{
public:
    int depth;
    int dep1;
    int dep2;
};
class prog_data
{
public:
    insruction* ins_arr;
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

int getInstDepth(ProgCtx ctx, unsigned int theInst) {
    return -1;
}

int getInstDeps(ProgCtx ctx, unsigned int theInst, int *src1DepInst, int *src2DepInst) {
    return -1;
}

int getRegfalseDeps(ProgCtx ctx, unsigned int reg){
	return -1;
}

int getProgDepth(ProgCtx ctx) {
    return 0;
}


