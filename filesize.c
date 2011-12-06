#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

void __assert_io(const char *func, const unsigned int line, int32_t test)
{
  if(!test) {
      char *msg = NULL;
      msg = malloc(sizeof(char) * (1 + strlen("Func: ") + strlen(func) + strlen(" Line: ") + 10 + strlen(" Error")));
      assert(NULL != msg);
      assert(0 < sprintf(msg, "Func: %s Line: %u Error", func, line)); 
      perror(msg);
      free(msg);
      exit(1);
  }
}

#define assert_io(__test) (__assert_io(__func__, __LINE__, __test));

void test(size_t size)
{
  FILE *fp = NULL;
  size_t i, n;
  size_t buf_size = 268435456;// 2^28
  uint32_t *p = NULL;
  char name[L_tmpnam];
  uint32_t val = 13;

  fprintf(stderr, "************************************************************\n");
  fprintf(stderr, "Testing size: %llu\n", (long long unsigned int)size);

  // tmp name
  assert(NULL != tmpnam(name));
  
  // init
  n = size / sizeof(uint32_t);
  p = malloc(n*sizeof(uint32_t));
  assert(p != NULL);
  for(i=0;i<n;i++) p[i] = val;

  // write
  fp = fopen(name, "wb");
  assert_io(NULL != fp);
  assert_io(n == fwrite(p, sizeof(uint32_t), n, fp)); 
  fclose(fp);
  
  // read - buffered
  for(i=0;i<n;i++) p[i] = 0; // reset
  fp = fopen(name, "rb");
  assert_io(NULL != fp);
  for(i=0;i<n;i+=buf_size/sizeof(uint32_t)) { 
     size_t m;
     m = (n - i < buf_size/sizeof(uint32_t)) ? (n - i) : buf_size/sizeof(uint32_t);
     assert_io(m == fread(p + i, sizeof(uint32_t), m, fp));
  }
  fclose(fp);
  for(i=0;i<n;i++) {
      if(val != p[i]) {
          fprintf(stderr, "Buffered did not match\n");
          break;
      }
  }
  if(i==n) {
      fprintf(stderr, "Buffered matched\n");
  }

  // read - all at once
  for(i=0;i<n;i++) p[i] = 0; // reset
  fp = fopen(name, "rb");
  assert_io(NULL != fp);
  assert_io(n == fread(p, sizeof(uint32_t), n, fp));
  fclose(fp);
  for(i=0;i<n;i++) {
      if(val != p[i]) {
          fprintf(stderr, "All-at-once did not match\n");
          break;
      }
  }
  if(i==n) {
      fprintf(stderr, "All-at-once matched\n");
  }
  
  fprintf(stderr, "************************************************************\n");

  // unlink
  assert(0 == unlink(name));
}

int main(int argc, char *argv[])
{
  size_t size, max;
  fprintf(stderr, "sizeof(size_t)=%lu\n", (long unsigned int)sizeof(size_t));

  size = 1073741824;
  max = 8589934592; // 2^33
  while(size <= max) {
      test(size);
      size <<= 1;
  }
  return 0;
}
