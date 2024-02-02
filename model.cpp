#include "openai.hpp"

#include <iostream>

int main() {
    openai::start();

    // api çagrıma
    auto models = openai::model().list();
    std::cout << models["data"][0] << std::endl;

    
    auto& openai = openai::instance();
    auto models_other_approach = openai.model.list();
    std::cout << models_other_approach["data"][1] << std::endl;

    // Hazir kalip func. .
    
    auto model = openai::model().retrieve("text-davinci-003");
    std::cout << model << std::endl;
}
