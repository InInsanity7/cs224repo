#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

const int MAX_MEM_SIZE  = (1 << 13);

void fetchStage(int *icode, int *ifun, int *rA, int *rB, wordType *valC, wordType *valP) {

    byteType b = getByteFromMemory(getPC());
    *icode = b >> 4;
    *ifun = b & 0xf;

    if (*icode == HALT) {
        *valP = getPC() + 1;
        setStatus(STAT_HLT);
    }
    else if (*icode == NOP) {
        *valP = getPC() + 1;
    }
    else if (*icode == RRMOVQ) {
        b = getByteFromMemory(getPC() + 1);
        *rA = b >> 4;
        *rB = b & 0xf;
        *valP = getPC() + 2;
    }
    else if (*icode == CMOVXX) {
        b = getByteFromMemory(getPC() + 1);
        *rA = b >> 4;
        *rB = b & 0xf;
        *valP = getPC() + 2;
    }
    else if (*icode == IRMOVQ) {
        b = getByteFromMemory(getPC() + 1);
        *rA = b >> 4;
        *rB = b & 0xf;
        *valC = getWordFromMemory(getPC() + 2);
        *valP = getPC() + 10;
    }
    else if (*icode == RMMOVQ) {
        b = getByteFromMemory(getPC() + 1);
        *rA = b >> 4;
        *rB = b & 0xf;
        *valC = getWordFromMemory(getPC() + 2);
        *valP = getPC() + 10;
    }
    else if (*icode == MRMOVQ) {
        b = getByteFromMemory(getPC() + 1);
        *rA = b >> 4;
        *rB = b & 0xf;
        *valC = getWordFromMemory(getPC() + 2);
        *valP = getPC() + 10;
    }
    else if (*icode == OPQ) {
        b = getByteFromMemory(getPC() + 1);
        *rA = b >> 4;
        *rB = b & 0xf;
        *valP = getPC() + 2;
    }
    else if (*icode == JXX) {
        b = getByteFromMemory(getPC() + 1);
        *valC = getWordFromMemory(getPC() + 1);
        *valP = getPC() + 9;
    }
    else if (*icode == CALL) {
        b = getByteFromMemory(getPC() + 1);
        *valC = getWordFromMemory(getPC() + 1);
        *valP = getPC() + 9;
    }
    else if (*icode == RET) {
        b = getByteFromMemory(getPC() + 1);
        *valP = getPC() + 2;
    }
    else if (*icode == PUSHQ) {
        b = getByteFromMemory(getPC() + 1);
        *rA = b >> 4;
        *rB = b & 0xf;
        *valP = getPC() + 2;
    }
    else if (*icode == POPQ) {
        b = getByteFromMemory(getPC() + 1);
        *rA = b >> 4;
        *rB = b & 0xf;
        *valP = getPC() + 2;
    }
}

void decodeStage(int icode, int rA, int rB, wordType *valA, wordType *valB) {
 
    /* else if (icode == HALT) { */
    /* } */
    /* else if (icode == NOP) { */
    /* } */
    if (icode == RRMOVQ) {
        *valA = getRegister(rA);
    }
    else if (icode == CMOVXX) {
        *valA = getRegister(rA);
    }
    /* else if (icode == IRMOVQ) { */
    /* } */
    else if (icode == RMMOVQ) {
        *valA = getRegister(rA);
    }
    else if (icode == MRMOVQ) {
        *valB = getRegister(rB);
    }
    else if (icode == OPQ) {
        *valA = getRegister(rA);
        *valB = getRegister(rB);
    }
    /* else if (icode == JXX) { */
    /* } */
    else if (icode == CALL) {
        *valB = getRegister(RSP);
    }
    else if (icode == RET) {
        *valA = getRegister(RSP);
        *valB = getRegister(RSP);
    }
    else if (icode == PUSHQ) {
        *valA = getRegister(rA);
        *valB = getRegister(RSP);
    }
    else if (icode == POPQ) {
        *valA = getRegister(RSP);
        *valB = getRegister(RSP);
    }
}

void executeStage(int icode, int ifun, wordType valA, wordType valB, wordType valC, wordType *valE, bool *Cnd) {
    /* else if (icode == HALT) { */
    /* } */
    /* else if (icode == NOP) { */
    /* } */
    if (icode == RRMOVQ) {
        *valE = 0 + valA;
    }
    else if (icode == CMOVXX) {
        *valE = 0 + valA;
    }
    else if (icode == IRMOVQ) {
        *valE = 0 + valC;
    }
    else if (icode == RMMOVQ) {
        *valE = valB + valC;
    }
    else if (icode == MRMOVQ) {
        *valE = valB + valC;
    }
    else if (icode == OPQ) {
        if (ifun == ADD) {
            *valE = valB + valA;
            setFlags((*valE < 0), (*valE == 0), 
                    (((valB < 0) == (valA < 0)) && ((*valE < 0) != (valB < 0))));
        }
        else if (ifun == SUB) {
            *valE = valB - valA;
            setFlags((*valE < 0), (*valE == 0),
                    (!((valB < 0) == (valA < 0)) && ((*valE < 0) != (valB < 0)))); 
        }
        else if (ifun == AND) {
            setFlags((*valB < 0), (*valB == 0), FALSE);
        }
        else if (ifun == XOR) {
            *valE = valB ^ valA;
            setFlags((*valB < 0), (*valB == 0), FALSE);
        }
    }
    else if (icode == JXX) {
        *Cnd = Cond(ifun);
    }
    else if (icode == CALL) {
        *valE = valB + (-8);
    }
    else if (icode == RET) {
        *valE = valB + 8;
    }
    else if (icode == PUSHQ) {
        *valE = valB + (-8);
    }
    else if (icode == POPQ) {
        *valE = valB + 8;
    }
}

void memoryStage(int icode, wordType valA, wordType valP, wordType valE, wordType *valM) {
 
 
    /* else if (icode == HALT) { */
    /* } */
    /* else if (icode == NOP) { */
    /* } */
    /* else if (icode == RRMOVQ) { */
    /* } */
    /* else if (icode == CMOVXX) { */
    /* } */
    /* else if (icode == IRMOVQ) { */
    /* } */
    if (icode == RMMOVQ) {
        setWordInMemory(valE, valA);
    }
    else if (icode == MRMOVQ) {
        *valM = getWordFromMemory(valE);
    }
    /* else if (icode == OPQ) { */
    /* } */
    /* else if (icode == JXX) { */
    /* } */
    else if (icode == CALL) {
        setWordInMemory(valE, valP);
    }
    else if (icode == RET) {
        *valM = getWordFromMemory(valA);
    }
    else if (icode == PUSHQ) {
        setWordInMemory(valE, valA);
    }
    else if (icode == POPQ) {
        *valM = getWordFromMemory(valA);
    }
}

void writebackStage(int icode, wordType rA, wordType rB, wordType valE, wordType valM) {
 
 
    /* else if (icode == HALT) { */
    /* } */
    /* else if (icode == NOP) { */
    /* } */
    if (icode == RRMOVQ) {
        setRegister(rB, valE);
    }
    else if (icode == CMOVXX) {
        setRegister(rB, valE);
    }
    else if (icode == IRMOVQ) {
        setRegister(rB, valE);
    }
    /* else if (icode == RMMOVQ) { */
    /* } */
    else if (icode == MRMOVQ) {
        setRegister(rA, valM);
    }
    else if (icode == OPQ) {
        setRegister(rB, valE);
    }
    /* else if (icode == JXX) { */
    /* } */
    else if (icode == CALL) {
        setRegister(RSP, valE);
    }
    else if (icode == RET) {
        setRegister(RSP, valE);
    }
    else if (icode == PUSHQ) {
        setRegister(RSP, valE);
    }
    else if (icode == POPQ) {
        setRegister(RSP, valE);
        setRegister(rA, valM);
    }
}

void pcUpdateStage(int icode, wordType valC, wordType valP, bool Cnd, wordType valM) {

    if (icode == JXX) {
        (Cnd == TRUE) ? setPC(valC) : setPC(valP);
    }
    else if (icode == CALL) {
        setPC(valC);
    }
    else if (icode == RET) {
        setPC(valM);
    }
    else {
        setPC(valP);
    }
}

void stepMachine(int stepMode) {
  /* FETCH STAGE */
  int icode = 0, ifun = 0;
  int rA = 0, rB = 0;
  wordType valC = 0;
  wordType valP = 0;
 
  /* DECODE STAGE */
  wordType valA = 0;
  wordType valB = 0;

  /* EXECUTE STAGE */
  wordType valE = 0;
  bool Cnd = 0;

  /* MEMORY STAGE */
  wordType valM = 0;

  fetchStage(&icode, &ifun, &rA, &rB, &valC, &valP);
  applyStageStepMode(stepMode, "Fetch", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);

  decodeStage(icode, rA, rB, &valA, &valB);
  applyStageStepMode(stepMode, "Decode", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);
  
  executeStage(icode, ifun, valA, valB, valC, &valE, &Cnd);
  applyStageStepMode(stepMode, "Execute", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);
  
  memoryStage(icode, valA, valP, valE, &valM);
  applyStageStepMode(stepMode, "Memory", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);
  
  writebackStage(icode, rA, rB, valE, valM);
  applyStageStepMode(stepMode, "Writeback", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);
  
  pcUpdateStage(icode, valC, valP, Cnd, valM);
  applyStageStepMode(stepMode, "PC update", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);

  incrementCycleCounter();
}

/** 
 * main
 * */
int main(int argc, char **argv) {
  int stepMode = 0;
  FILE *input = parseCommandLine(argc, argv, &stepMode);

  initializeMemory(MAX_MEM_SIZE);
  initializeRegisters();
  loadMemory(input);

  applyStepMode(stepMode);
  while (getStatus() != STAT_HLT) {
    stepMachine(stepMode);
    applyStepMode(stepMode);
#ifdef DEBUG
    printMachineState();
    printf("\n");
#endif
  }
  printMachineState();
  return 0;
}
