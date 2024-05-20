#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <atomic>
#include <ctime>

// Función que ejecutarán los hilos
void monte_carlo_pi_part(int samples, std::atomic<int>& count, unsigned int seed) {
    std::mt19937 generator(seed);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    int local_count = 0;
    for (int i = 0; i < samples; i++) {
        double x = distribution(generator);
        double y = distribution(generator);
        if (x * x + y * y <= 1.0) {
            local_count++;
        }
    }
    count += local_count;
}

int main() {
    int num_samples = 10000000; // Número total de muestras
    int num_threads = std::thread::hardware_concurrency(); // Número de hilos (puedes ajustarlo)
    std::atomic<int> total_count(0);
    std::vector<std::thread> threads;

    // Crear y lanzar hilos
    for (int i = 0; i < num_threads; i++) {
        unsigned int seed = time(nullptr) + i; // Semilla única para cada hilo
        threads.emplace_back(monte_carlo_pi_part, num_samples / num_threads, std::ref(total_count), seed);
    }

    // Esperar a que todos los hilos terminen
    for (auto& t : threads) {
        t.join();
    }

    // Calcular el valor de PI
    double pi_estimate = (4.0 * total_count) / num_samples;
    std::cout << "Estimated value of PI: " << pi_estimate << std::endl;

    return 0;
}
