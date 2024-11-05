#include "../headers/basic.hpp"

int total_tests = 0;
int tests_passed = 0;
int tests_failed = 0;
std::string path = "./config.conf";

std::vector<TestResult> test_results;

void printSummary() {
    std::cout << YELLOW << "==================== RESUMO ====================" << RESET << std::endl;
    std::cout << "Total de testes: " << total_tests << std::endl;
    std::cout << GREEN << "Passaram: " << tests_passed << RESET << std::endl;
    std::cout << RED << "Falharam: " << tests_failed << RESET << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    for (size_t i = 0; i < test_results.size(); ++i) {
        const TestResult& result = test_results[i];
        if (result.passed) {
            std::cout << GREEN << "[PASSOU] " << result.test_name << RESET << std::endl;
        } else {
            std::cout << RED << "[FALHOU] " << result.test_name << " - " << result.message << RESET << std::endl;
        }
    }
    std::cout << YELLOW << "===============================================" << RESET << std::endl;
}
