#include <iostream>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define RESET   "\033[0m"

#define ASSERT_TRUE(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr <<RED<< "[FALHOU] " << message <<RESET<< std::endl; \
            return false; \
        } else { \
            std::cout <<GREEN<< "[PASSOU] " << message <<RESET<< std::endl; \
        } \
    } while (0)

#define RUN_TEST(test) \
    do { \
        std::cout << "Executando " << #test << "..." << std::endl; \
        if (test()) { \
            std::cout <<GREEN<< #test << " concluído com sucesso." <<RESET<< std::endl; \
        } else { \
            std::cout <<RED<< #test << " falhou." <<RESET<< std::endl; \
        } \
    } while (0)
