#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <math.h>
#include <chrono>
#include <ctime>

using namespace std;
using namespace chrono;

const int MAX_PRIMES = 10000;

int *shared;
int primes_counter = 0;

system_clock::time_point starting_time;

sem_t mutex;
sem_t empty;
sem_t full;
sem_t mutex_counter;

bool prime(int number){
  bool is_prime = true;
  for (int i=2; i<=sqrt(number); i++){
    if (number%i == 0){
      is_prime = false;
      break;
    }
  }
  return is_prime;
}

void *produce(void *pthreadid){
  srand (time(NULL));
  while (true){
    int num = rand()%10000000 + 1;
    int index = 0;
    bool reading = true;

    sem_wait(&empty);
    sem_wait(&mutex);
    while (shared[index] != 0)
      index ++;
    shared[index] = num;
    sem_post(&mutex);
    sem_post(&full);
  }
}

void *consume(void *pthreadid){
  while (true){
    int index = 0;
    bool is_prime;

    sem_wait(&full);
    sem_wait(&mutex);
    while (shared[index] == 0)
      index ++;
    const int number = shared[index];
    shared[index] = 0;
    sem_post(&mutex);
    sem_post(&empty);

    is_prime = prime(number);
    if (is_prime){
      sem_wait(&mutex_counter);
      primes_counter++;
      if (primes_counter == MAX_PRIMES){
        //cout << system_clock::to_time_t(system_clock::now() - starting_time);
        auto start_ms = time_point_cast<milliseconds>(starting_time);
        auto now = system_clock::now();
        auto now_ms = time_point_cast<milliseconds>(now);
        auto value = now_ms - start_ms;
        long execution_time = value.count();
        cout << "-----------------------" << endl;
        cout << "Execution time: " << execution_time << "ms" << endl;
        exit(1);
      }
      sem_post(&mutex_counter);
      cout << "Found a prime - " << number << endl;
    }
  }
}

int main (int argc, char** argv) {

  starting_time = system_clock::now();

  long n_producers = atoi(argv[1]);
  long n_consumers = atoi(argv[2]);
  long n_threads = n_producers + n_consumers;

  int shared_size = atoi(argv[3]);
  shared = (int *) malloc (shared_size * sizeof (shared[0]));

  sem_init(&mutex, 0, 1);
  sem_init(&empty, 0, shared_size);
  sem_init(&full, 0, 0);
  sem_init(&mutex_counter, 0, 1);

  pthread_t threads[n_threads];

  for (long i=0; i<n_threads; i++){
    if (i < n_producers)
      pthread_create(&threads[i], NULL, produce, (void *)i);
    else
      pthread_create(&threads[i], NULL, consume, (void *)i);
  }

  pthread_exit(NULL);
  return 0;
}
