/*
|			Sistemas Distribuídos 2016.1			|
|		TP2 Parte 1 - Serialização por spinlocks	|
|		Alunos:	Gabriel Rebello						|
|				Eduardo Castanho					|
*/


#include <iostream>
#include <string.h>
#include <pthread.h>
#include <cstdlib>
#include <time.h>
#include <atomic>
#include <chrono>

using namespace std;
using namespace chrono;

struct lock {

	long double sum = 0;
	atomic_flag m_flag = ATOMIC_FLAG_INIT; // Inicializa com m_flag = 0

	void acquire() {
		while(m_flag.test_and_set());
	}
	void release() {
		m_flag.clear();
	}
};

struct t_data{
	int t_id;
	int sumArrayLen;
	char* sumArray;
	struct lock* spinlock;
};

void* sum(void* thread_data) {

	struct t_data *my_data;
	long double m_sum=0;
	my_data = (struct t_data *) thread_data; 

	for (int i=0;i<my_data->sumArrayLen;i++) {
		m_sum += my_data->sumArray[i];
	}

	my_data->spinlock->acquire();
	cout << "THREAD ID: " << my_data->t_id << endl;
	cout << "Acquired lock." << endl;
	my_data->spinlock->sum += m_sum;
	cout << "Releasing lock..." << endl << endl;
	my_data->spinlock->release();

}

int main(int argc, char** argv) {

	// Inicializar cronômetro geral, arrays, spinlock,etc.																																																																																																																																																							, spinlock, etc. 
	system_clock::time_point starting_time = system_clock::now();
	int K = atoi(argv[1]);
	int N = atoi(argv[2]);
	char temp;
	double expected_sum = 0;
	pthread_t threads[K];
	struct t_data threads_data[K];
	struct lock spinlock;
	const long int numElements = (long int) N/K;  //Divisão inteira de N/K para dividir array em parcelas

	// Criar array[K] de arrays para delegar cada sub-lista a uma thread
	char** numArrays = new char*[K]; 
	for(int i=0;i<K;i++)
		numArrays[i] = new char[numElements];  //Char utilizado para que seja alocado apenas 1 byte por número

	// Inicializar seed aleatória para preenchimento de arrays
	srand(time(NULL)); 

	//Preencher arrays que serão passados às threads criadas
	for (int i=0;i<K;i++) {
		for(int j=0;j<numElements;j++) {
			temp = rand() % 201 - 100;
			numArrays[i][j] = temp;
			expected_sum+=temp;
		}
		//Criar threads e passando argumentos(id,spinlock,array)
		threads_data[i].t_id = i;
		threads_data[i].sumArrayLen = numElements;
		threads_data[i].spinlock = &spinlock;
		threads_data[i].sumArray = numArrays[i];
		pthread_create(&threads[i],NULL,sum,(void*)&threads_data[i]);
	}

	system_clock::time_point calc_time = system_clock::now();

	// Parar o programa até que todas as threads terminem para imprimir soma correta
	for (int i=0;i<K;i++){
		if(pthread_join(threads[i],NULL)) cout << "Error waiting for threads." << endl;
	}

	// Somando últimos valores restantes no caso de N%K != 0 (esta parcela torna-se irrelevante à medida que N >> K)
	for(int i=0;i<(int)N%K;i++) {
		temp = rand() % 201 - 100;
		spinlock.sum+=temp;
		expected_sum+=temp;
	}

	// Printar resultado esperado, o calculado e tempo de execução
    cout << "----------------------------" << endl;
	cout << "EXPECTED SUM = " << expected_sum << endl;
	cout << "CALCULATED SUM = " << spinlock.sum << endl;

	// Liberar memória alocada
	for(int i=0;i<K;i++)
		delete[] numArrays[i];

	delete[] numArrays;

    auto start_ms = time_point_cast<milliseconds>(starting_time);
    auto calc_ms = time_point_cast<milliseconds>(calc_time);
    auto now = system_clock::now();
    auto now_ms = time_point_cast<milliseconds>(now);
    auto total_value = now_ms - start_ms;
    auto calc_value = now_ms - calc_ms;
	long t_total = total_value.count();
	long t_calc = calc_value.count();
    cout << "----------------------------" << endl;
    cout << "Total execution time: " << t_total	<< "ms" << endl;
    cout << "Calculation execution time: " << t_calc << "ms" << endl;
	
	return 0;
}

