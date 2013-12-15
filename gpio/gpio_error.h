#pragma once

#include <stdexcept>
#include <string>

class gpio_error : public std::runtime_error {
public:
    gpio_error(const std::string& message) : std::runtime_error(message) { };
};