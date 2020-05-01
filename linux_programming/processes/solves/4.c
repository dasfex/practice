#include <stdio.h>
#include <pthread.h>

struct TContext {
  const char symbol;
};

void* f(void* arg) {
  struct TContext* c = arg;
  for (int i = 0; i < 10000; ++i) {
    printf("%c", c->symbol);
  }
  pthread_exit(0);
}

int main() {
  pthread_t t1;
  pthread_t t2;
  pthread_t t3;

  struct TContext c1 = {'a'};
  struct TContext c2 = {'b'};
  struct TContext c3 = {'c'};

  pthread_create(&t1, 0, f, &c1);
  pthread_create(&t2, 0, f, &c2);
  pthread_create(&t3, 0, f, &c3);

  pthread_join(t1, 0);
  pthread_join(t2, 0);
  pthread_join(t3, 0);

  return 0;
}
