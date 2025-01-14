class MdLexer
{

    char                 tokenChar = 0;
    std::size_t          charCnt   = 0;

    static bool isSpecialChar(char ch)
    {
        return ch=='\'' || ch=='*' || ch=='_' || ch=='`' || ch=='\"' || ch=='#'; // Можно отдельным параметром шаблона сделать, как Traits
    }

public:

    template<typename TokenHandler>
    void operator()(char ch, const TokenHandler &tokenHandler)
    {
        // по нулевому символу - финализируем, сбрасываем кешированное, если было
        // Финализировать можно без проблем много раз, финализация просто сбрасывает текущее состояние в парсер
        // Если входной поток символов какой-то из говна, и там могут быть нулевые символы, и это не было проверено на входе
        // то ничего тут не сломается
        if (ch==0) 
        {
            if (tokenChar!=0)
                tokenHandler(tokenChar, charCnt, true);

            // Нулевой символ не прокидываем, зачем парсеру геммор перекидывать?
            return;
        }

        if (tokenChar==ch)
        {
            // повторение символа
            ++charCnt;
            return;
        }

        // Какой-то другой символ пришел, не тот, что мы коллекционируем.

        // Сбросили в парсер то, что было накоплено, если было
        if (tokenChar!=0)
            tokenHandler(tokenChar, charCnt, true);

        // Обнуляем накопления
        tokenChar = 0;
        charCnt   = 0;

        if (!isSpecialChar(ch))
        {
            tokenHandler(ch, 1, false); // прокинули просто символ
            return;
        }

        // Стартуем накопление спец символов
        tokenChar = ch;
        charCnt   =  1; // Один символ таки накопили сразу при его поступлении

    }

}; // class MdLexer


template<typename TokenHandler, typename MarkdownCharsContainer>
void parseMarkDown(const MarkdownCharsContainer &mdc, const TokenHandler &tokenHandler)
{
    MdLexer lexer;
    for(auto ch: mdc)
        lexer(ch, tokenHandler);
    lexer(0, tokenHandler); // Финализируем
}


