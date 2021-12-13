#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

typedef union {
  struct
  {
    uint8_t a,b,c,d;
  };
  uint32_t val;
}ISA;

#define CADDR 45 // console address
ISA Mem[46] = {0};
/*
d=0;
Instruction subleq a, b, c, d
    Mem[Mem[a]] = Mem[Mem[a]] - Mem[b]
    if (Mem[Mem[a]] <= 0)
        goto Mem[c] + PC

b = 0,d = 0; mem[0] is PC
Instruction subleq2 a, b, c, d
    Mem[Mem[a]] = Mem[Mem[a]] - PC
    if (Mem[Mem[a]] <= 0)
        goto Mem[c] + PC

b=0; mem[0] is PC
Instruction subleq3 a, b, c, d
    Mem[Mem[a]] = Mem[d] - PC
    if (Mem[Mem[a]] <= 0)
        goto Mem[c] + PC

Instruction subleq4 a, b, c, d
    Mem[Mem[a]] = Mem[d] - Mem[b]
    if (Mem[Mem[a]] <= 0)
        goto Mem[c] + PC

*/
int32_t subleq(ISA ins){
    Mem[Mem[ins.a].val].val = ((ins.d)?Mem[ins.d].val:Mem[Mem[ins.a].val].val) - Mem[ins.b].val;
    if (Mem[Mem[ins.a].val].val <= 0)
      return (int32_t)Mem[ins.c].val;
    return 0;
}

void readf(char* file,ISA* arr,size_t max){
  char * line = NULL;
  size_t len = 0;
  FILE* fp = stdin;
  if(file)
    fp  = fopen(file, "r");
  if(!fp){
    puts("cant open file");
    exit(-1);
  }
  size_t ptr = 4;
  while(getline(&line, &len, fp)) {
    if(strlen(line) == 1) break;
    size_t index = 0;
    size_t last = 0;
    bool go = true;
    {
      size_t i = 0;
      while(line[i]){
            if(line[i] == '#'){
              line[i--] = 0;
              break;
            }
            if(line[i] == '\n'){
              line[i--] = 0;
              break;
            }
            i++;
      }
      while(line[i] == ' '){
          line[i--] = 0;
      }
    }
    while(go) {
      if(!line[index]) go = false;
      if(line[index] == ',' || line[index] == ' ' || !line[index]){
        if(!go && last == 0){
          arr[ptr/4].val = (int32_t)atoi(line);
          ptr++;
        } else{
          line[index] = 0;
          ((uint8_t*)arr)[ptr++] = atoi(line+last);
          last = index+1;
        }
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

  for(size_t i = 0; i < sizeof(Mem)/sizeof(ISA); i++){
    printf("%u: x%02x, x%02x, x%02x, x%02x, %d\n",i,Mem[i].a,Mem[i].b,Mem[i].c,Mem[i].d,Mem[i].val);
  }

  Mem[0].val = 1;
  while(Mem[0].val < sizeof(Mem)/sizeof(ISA) && Mem[Mem[0].val].val){
    Mem[0].val += subleq(Mem[Mem[0].val])+1;
    if(Mem[CADDR].val){
      printf("%c",Mem[CADDR].val,Mem[CADDR].val);
      Mem[CADDR].val = 0;
    }
  }

  printf("\n%u\n",Mem[0].val);
  /*for(size_t i = 0; i < sizeof(Mem)/sizeof(ISA); i++){
    printf("%u: x%02x, x%02x, x%02x, x%02x, %d\n",i,Mem[i].a,Mem[i].b,Mem[i].c,Mem[i].d,Mem[i].val);
  }*/
  return 0;
}
