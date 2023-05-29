#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cassert>

/*
@brief Функція для обчислення суми певної частини масиву
*
*@param numbers Вектор чисел
* @param start Початковий індекс для обчислення суми
* @param end Кінцевий індекс(не включно) для обчислення суми
* @param result Результат обчислення суми
*/
void calculateSum(const std::vector<int>&numbers, int start, int end, int& result) {
    for (int i = start; i < end; ++i) {
        result += numbers[i];
    }
}

/*
@brief Функція для виконання юніт - тестів
*/
void runUnitTests() {
    std::vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int totalSum = 55;

    // Тестовий випадок 1: Обчислити суму за допомогою одного потоку
    {
        int sum = 0;
        calculateSum(numbers, 0, numbers.size(), sum);
        assert(sum == totalSum);
    }

    // Тестовий випадок 2: Обчислити суму за допомогою кількох потоків
    {
        int numThreads = 4;
        int numElementsPerThread = numbers.size() / numThreads;
        std::vector<std::thread> threads;
        std::vector<int> partialSums(numThreads);

        for (int i = 0; i < numThreads; ++i) {
            int start = i * numElementsPerThread;
            int end = (i == numThreads - 1) ? numbers.size() : start + numElementsPerThread;

            threads.emplace_back(calculateSum, std::ref(numbers), start, end, std::ref(partialSums[i]));
        }

        for (auto& thread : threads) {
            thread.join();
        }

        int sum = 0;
        for (int partialSum : partialSums) {
            sum += partialSum;
        }

        assert(sum == totalSum);
    }

    // Виведення результату тестів
    std::cout << "Юніт-тести " << ((totalSum == 55) ? "пройшли успішно" : "не пройшли") << std::endl;
}

/*
@brief Функція для вимірювання часу виконання
*
*@tparam Func Тип функції, яку потрібно виміряти
* @param func Функція, яку потрібно виміряти
* @return Час виконання у секундах
*/
template<typename Func>
double measureExecutionTime(Func && func) {
    auto startTime = std::chrono::steady_clock::now();
    func();
    auto endTime = std::chrono::steady_clock::now();

    return std::chrono::duration<double>(endTime - startTime).count();
}

/*
@brief Головна функція
*
*@return Код повернення
*/
int main() {
    system("chcp 1251");
    // Виконати юніт-тести
    runUnitTests();

    // Виміряти час виконання основного коду
    double executionTime = measureExecutionTime([] {
        // Створити масив чисел
        std::vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        // Визначити кількість потоків, що будуть використовуватися
        int numThreads = 4;

        // Обчислити кількість елементів, яку буде обробляти кожен потік
        int numElementsPerThread = numbers.size() / numThreads;

        // Створити вектор для збереження об'єктів потоків
        std::vector<std::thread> threads;

        // Створити вектор для збереження часткових сум
        std::vector<int> partialSums(numThreads);

        // Запустити потоки
        for (int i = 0; i < numThreads; ++i) {
            int start = i * numElementsPerThread;
            int end = (i == numThreads - 1) ? numbers.size() : start + numElementsPerThread;

            threads.emplace_back(calculateSum, std::ref(numbers), start, end, std::ref(partialSums[i]));
        }

        // Очікувати завершення всіх потоків
        for (auto& thread : threads) {
            thread.join();
        }

        // Обчислити загальну суму
        int totalSum = 0;
        for (int sum : partialSums) {
            totalSum += sum;
        }

        // Вивести результат
        std::cout << "Сума масиву: " << totalSum << std::endl;
        });

    std::cout << "Час виконання: " << executionTime << " секунд" << std::endl;

    return 0;
}