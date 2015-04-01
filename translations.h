using namespace std;

string translate(string message, string language, ...);
string PL(string message);
string EN(string message);
string to_en(string message);
string form(string message, ...);
string loc(string message, string language, ...);

string loc(string message, string language, ...)
{
    return translate(message, language);
}

string translate(string message, string language, ...)
{
    if(language == "PL")
        return form(PL(message));
    if(language == "EN")
        return form(EN(message));
    return message;
}

string PL(string message)
{
    if(message == "Here we go! [press any button]")
        return "Zaczynajmy! [nacisnij dowolny przycisk]";
    if(message == "Flags left: ")
        return "Pozostale flagi: ";
    if(message == "Tell me what to do! (action[show/mark], row[1-9] , col[1-9]):\n>")
        return "Powiedz mi co mam zrobic! (akcja[pokaz/oznacz], rzad[1-9], kolumna[1-9])\n>";
    if(message == "\n\nWhat a pity! You stepped on a bomb!\n")
        return "\n\nNiestety! Wszedles na bombe!\n";
    if(message == "\n\nCongratulations! You marked all bombs correctly and saved many human beings!\n")
        return "\n\nGratulacje! Poprawnie zaznaczyles wszystkie bomby i uratowales wielu ludzi!\n";
    if(message == "Would you like to play once again? [yes/no]\n")
        return "Czy chcesz zagrac jeszcze raz? [tak/nie]\n";
    if(message == "Ok, goodbye! :) [press any button]")
        return "Do nastepnego razu! :) [nacisnij dowolny przycisk]";
    // default
    return message;
}

string EN(string message)
{
    return message;
}

string form(string message, ...)
{
    return message;
}

string to_en(string message)
{
    if(message == "tak")
        return "yes";
    if(message == "nie")
        return "no";
    if(message == "pokaz")
        return "show";
    if(message == "oznacz")
        return "mark";
    // default
    return message;
}
