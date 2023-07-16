#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include "../src/decs.h"

namespace Query{
    class Lexer{
        public:
        std::vector<Token> tokens;

        bool is_space(char word);

        bool is_number(char word);

        bool is_special(char word);

        bool is_char(char word);

        Token::Kind getSpecial( char word );

        Token::Kind getKeyword( std::string word );

        void lexme(std::string query);

        void print();
    };

    class Parser{
        public:
        std::vector<Token> tokens;
        int index = 0;
        Parser(std::vector<Token> _tokens);

        Token peek();

        void next();

        void printIFT(IFT data);

        void printQuery(Statement* statement);

        void parseConditions(IFT & conditions);

        void GET(Statement* statement);

        void UPDATE(Statement* statement);

        void INSERT(Statement* statement);

        void DELETE(Statement* statement);

        Statement* parse();
    };

    Statement* resolveStatement(std::string statement);
}