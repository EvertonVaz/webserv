#ifndef BASIC_HPP
#define BASIC_HPP

#include <iostream>
#include <string>
#include <vector>
#include <exception>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RESET   "\033[0m"

extern int total_tests;
extern int tests_passed;
extern int tests_failed;

struct TestResult {
    std::string test_name;
    bool passed;
    std::string message;

    TestResult(const std::string& name, bool p, const std::string& msg)
        : test_name(name), passed(p), message(msg) {}
};

// Declare o vetor de resultados como extern
extern std::vector<TestResult> test_results;

#define ASSERT_TRUE(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << RED << "[FALHOU] " << message << " (" << __FILE__ << ":" << __LINE__ << ") " \
                      << "Condição: " << #condition << RESET << std::endl; \
            return false; \
        } else { \
            std::cout << GREEN << "[PASSOU] " << message << RESET << std::endl; \
        } \
    } while (0)

#define RUN_TEST(test) \
    do { \
        total_tests++; \
        std::cout << YELLOW << "Executando " << #test << "..." << RESET << std::endl; \
        try { \
            if (test()) { \
                std::cout << GREEN << #test << " concluído com sucesso." << RESET << std::endl; \
                tests_passed++; \
                test_results.push_back(TestResult(#test, true, "")); \
            } else { \
                std::cout << RED << #test << " falhou." << RESET << std::endl; \
                tests_failed++; \
                test_results.push_back(TestResult(#test, false, "Falha desconhecida")); \
            } \
        } catch (const std::exception& e) { \
            std::cout << RED << #test << " lançou uma exceção: " << e.what() << RESET << std::endl; \
            tests_failed++; \
            test_results.push_back(TestResult(#test, false, e.what())); \
        } catch (...) { \
            std::cout << RED << #test << " lançou uma exceção desconhecida." << RESET << std::endl; \
            tests_failed++; \
            test_results.push_back(TestResult(#test, false, "Exceção desconhecida")); \
        } \
        std::cout << std::endl; \
    } while (0)

// Declare a função printSummary
void printSummary();

#endif // BASIC_HPP
