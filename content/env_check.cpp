#include <iostream>

int main(int ac, char **av, char **env)
{
    std::cout << "CGI Environment Variables:" << std::endl << "__________________________" << std::endl;
    for (int i = 0; env[i] != NULL; i++)
    {
        std::cout << env[i] << std::endl;
    }
    std::cout << "__________________________" << std::endl;
    return 0;
}
