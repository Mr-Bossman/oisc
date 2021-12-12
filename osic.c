#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/*
d=0;
Instruction subleq a, b, c, d
    Mem[a] = Mem[a] - Mem[b]
    if (Mem[a] <= 0)
        goto c + PC

b = 0,d = 0; mem[0] is acc
Instruction subleq2 a, b, c, d
    Mem[a] = Mem[a] - ACCUM
    ACCUM = Mem[a]
    if (Mem[a] <= 0)
        goto c + PC

b=0; mem[0] is acc
Instruction subleq3 a, b, c, d
    Mem[a] = Mem[d] - ACCUM
    ACCUM = Mem[a]
    if (Mem[a] <= 0)
        goto c + PC

Instruction subleq4 a, b, c, d
    Mem[a] = Mem[d] - Mem[b]
    if (Mem[a] <= 0)
        goto c + PC

*/
typedef union {
  struct
  {
    uint8_t a,b,c,d;
  };
  uint32_t val;
}ISA;
ISA Mem[15] = {0};
int subleq(ISA ins){
    Mem[ins.a].val = ((ins.d)?Mem[ins.d].val:Mem[ins.a].val) - Mem[ins.b].val;
    if(!ins.b)
      Mem[ins.b].val = Mem[ins.a].val;
    if (Mem[ins.a].val <= 0)
      return (int8_t)ins.c;
    return 0;
}

void readf(char* file,ISA* arr,size_t max){
  char * line = NULL;
  size_t len = 0;
  FILE* fp = stdin;
  if(file)
    fp  = fopen(file, "r");
  size_t ptr = 4;
  while(getline(&line, &len, fp)) {
    if(strlen(line) == 1) break;
    size_t index = 0;
    size_t last = 0;
    bool go = true;
    while(go) {
      if(!line[index]) go = false;
      if(line[index] == ',' || line[index] == ' ' || !line[index]){
        line[index] = 0;
        ((uint8_t*)arr)[ptr++] = atoi(line+last);
        last = index+1;
      }
      index++;
    }
    if(ptr%4)
      ptr += 4-(ptr%4);
  }
  fclose(fp);
  if (line)
      free(line);
}
int main(int argc, char *argv[]){
  if(argc == 2)
    readf(argv[1],Mem,sizeof(Mem)/sizeof(ISA));
  else
    readf(NULL,Mem,sizeof(Mem)/sizeof(ISA));
  for(size_t i = 0; i < sizeof(Mem); i++)
    printf("x%02x, ",((uint8_t*)Mem)[i]);
  puts("\n");
  size_t PC=1;
  while(PC < sizeof(Mem)/sizeof(ISA) && Mem[PC].val) PC += subleq(Mem[PC])+1;
  printf("%u\n",Mem[0].d);
  for(size_t i = 0; i < sizeof(Mem); i++){
    if(!(i%4))  puts("\n");
    printf("x%02x, ",((uint8_t*)Mem)[i]);
  }
   puts("\n");
  return 0;
}
