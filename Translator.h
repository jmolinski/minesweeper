#ifndef TRANSLATIONS_H
#define TRANSLATIONS_H

#include <string>

class Translator
{
private:
    std::string language;
    std::string PL(std::string message);
    std::string EN(std::string message);
public:
    std::string get_language();
    void set_language(std::string lang);
    std::string loc(std::string message);
    std::string to_en(std::string message);
    std::string translate(std::string message);
};

#endif // TRANSLATIONS
