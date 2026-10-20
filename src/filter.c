/* Creative Mic, MIT license. RNNoise remains BSD licensed. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "rnnoise.h"
static int number(const char *s, float *v, float max) {
 char *end; *v=strtof(s,&end); return *s && !*end && isfinite(*v) && *v>=0 && *v<=max;
}
int main(int argc,char **argv) {
 float mix=1,gain=1,in[480],wet[480],dry[480]={0}; unsigned char bytes[960];
 for(int a=1;a<argc;a+=2) {
  if(a+1>=argc) goto usage;
  if(!strcmp(argv[a],"--mix")) { if(!number(argv[a+1],&mix,1)) goto usage; }
  else if(!strcmp(argv[a],"--gain")) { if(!number(argv[a+1],&gain,4)) goto usage; }
  else goto usage;
 }
 DenoiseState *st=rnnoise_create(); if(!st) return 3;
 for(;;) {
  size_t n=0,k; while(n<960 && (k=fread(bytes+n,1,960-n,stdin))) n+=k;
  if(!n) { rnnoise_destroy(st); return ferror(stdin)?3:0; }
  if(n!=960) { fprintf(stderr,"Truncated PCM frame\n"); rnnoise_destroy(st); return 2; }
  for(int i=0;i<480;i++) in[i]=(int16_t)((unsigned)bytes[2*i]|((unsigned)bytes[2*i+1]<<8));
  rnnoise_process_frame(st,wet,in);
  for(int i=0;i<480;i++) {
   /* RNNoise v0.1 overlap-add emits the previous 480-sample window. */
   float v=gain*((1-mix)*dry[i]+mix*wet[i]);
   v=fmaxf(-32768,fminf(32767,v));
   int16_t s=(int16_t)lrintf(v); bytes[2*i]=(uint16_t)s&255; bytes[2*i+1]=(uint16_t)s>>8;
   dry[i]=in[i];
  }
  if(fwrite(bytes,1,960,stdout)!=960 || fflush(stdout)) {rnnoise_destroy(st); return 3;}
 }
usage: fprintf(stderr,"Usage: creative-mic-filter [--mix 0..1] [--gain 0..4]\n"); return 2;
}
