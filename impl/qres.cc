#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include "../src/decs.h"

namespace Query{
    class Lexer{
        
        public:
        std::vector<Token> tokens;

        bool is_space(char word){
            switch (word) {
                case ' ':
                case '\t':
                case '\r':
                case '\n':
                    return true;
                default:
                    return false;
            }
        }

        bool is_number(char word){
            switch(word){
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '0':
                    return true;
                default:
                    return false;
            }
        }

        bool is_special(char word){
            switch(word){
                case '(':
                case ')':
                case '[':
                case ']':
                case '{':
                case '}':
                case '<':
                case '>':
                case '=':
                case '+':
                case '-':
                case '*':
                case ',':
                case ';':
                case '\'':
                case '\"':
                    return true;
                default:
                    return false;
            }   
        }

        bool is_char(char word){
            switch(word){
                case 'a':
                case 'b':
                case 'c':
                case 'd':
                case 'e':
                case 'f':
                case 'g':
                case 'h':
                case 'i':
                case 'j':
                case 'k':
                case 'l':
                case 'm':
                case 'n':
                case 'o':
                case 'p':
                case 'q':
                case 'r':
                case 's':
                case 't':
                case 'u':
                case 'v':
                case 'w':
                case 'x':
                case 'y':
                case 'z':
                case 'A':
                case 'B':
                case 'C':
                case 'D':
                case 'E':
                case 'F':
                case 'G':
                case 'H':
                case 'I':
                case 'J':
                case 'K':
                case 'L':
                case 'M':
                case 'N':
                case 'O':
                case 'P':
                case 'Q':
                case 'R':
                case 'S':
                case 'T':
                case 'U':
                case 'V':
                case 'W':
                case 'X':
                case 'Y':
                case 'Z':
                case '_':
                    return true;
                default:
                    return false;
            }
        }

        Token::Kind getSpecial( char word ){
            switch(word){
                case '(':
                    return Token::Kind::LeftParen;
                case ')':
                    return Token::Kind::RightParen;
                case '[':
                    return Token::Kind::LeftSquare;
                case ']':
                    return Token::Kind::RightParen;
                case '{':
                    return Token::Kind::LeftCurly;
                case '}':
                    return Token::Kind::RightCurly;
                case '<':
                    return Token::Kind::LessThan;
                case '>':
                    return Token::Kind::GreaterThan;
                case '=':
                    return Token::Kind::Equal;
                case '+':
                    return Token::Kind::Plus;
                case '-':
                    return Token::Kind::Minus;
                case '*':
                    return Token::Kind::Asterisk;
                case ',':
                    return Token::Kind::Comma;
                case ';':
                    return Token::Kind::Semicolon;
                case '\'':
                    return Token::Kind::SingleQuote;
                case '\"':
                    return Token::Kind::DoubleQuote;
                default:
                    return Token::Kind::Unexpected;
            }
        }

        Token::Kind getKeyword( std::string word ){
            if(word == "get" || word == "GET"){
                return Token::Kind::KEYWORD_GET;
            }
            else if(word == "insert" || word == "INSERT"){
                return Token::Kind::KEYWORD_INSERT;
            }
            else if(word == "update" || word == "UPDATE"){
                return Token::Kind::KEYWORD_UPDATE;
            }
            else if(word == "delete" || word == "DELETE"){
                return Token::Kind::KEYWORD_DELETE;
            }
            else if(word == "from" || word == "FROM"){
                return Token::Kind::KEYWORD_FROM;
            }
            else if(word == "in" || word == "IN"){
                return Token::Kind::KEYWORD_IN;
            }
            else if(word == "where" || word == "WHERE"){
                return Token::Kind::KEYWORD_WHERE;
            }
            else if(word == "nearest" || word == "NEAREST"){
                return Token::Kind::KEYWORD_NEAREST;
            }
            else if(word == "to" || word == "TO"){
                return Token::Kind::KEYWORD_TO;
            }
            else if(word == "set" || word == "SET"){
                return Token::Kind::KEYWORD_SET;
            }
            else{
                return Token::Kind::Identifier;
            }
        }

        void lexme(std::string query){
            char curr;
            std::string word;

            int i =0;

            while(query[i]!=';'){

                if(is_space(query[i])){
                    i+=1;
                }

                else if(is_special(query[i])){
                    word += query[i];
                    tokens.push_back(
                        Token(getSpecial(query[i]),word)
                    );
                    i+=1;
                    word="";
                }

                else if(is_char(query[i])){
                    word += query[i];
                    i+=1;
                    while(is_char(query[i]) || is_number(query[i])){
                        word+=query[i];
                        i+=1;
                    }
                    tokens.push_back(
                        Token(getKeyword(word),word)
                    );
                    word = "";
                }

                else if(is_number(query[i])){
                    word += query[i];
                    i+=1;
                    while(is_number(query[i])){
                        word+=query[i];
                        i+=1;
                    }
                    tokens.push_back(
                        Token(Token::Kind::Number,word)
                    );
                    word = "";
                }
            }
            tokens.push_back(
                Token(Token::Kind::Semicolon,";")
            );
        }

        void print(){
            for(auto x:tokens){
                // if(x.kind==Token::Kind::KE){
                std::cout << x.val << std::endl;
                
                // }
            }
        }

    };

    class Parser{
        public:
        std::vector<Token> tokens;
        int index = 0;
        Parser(std::vector<Token> _tokens){
            tokens = _tokens;
        }

        Token peek(){
            return tokens[index+1];
        }

        void next(){
            index += 1;
        }

        void printIFT(IFT data){
            for(const auto attr:data){
                std::string key = attr.first;
                auto & val = attr.second;
                if(const int* temp = std::get_if<int>(&val)){
                    std::cout << key << " " << *temp;
                }
                else if(const bool* temp = std::get_if<bool>(&val)){
                    std::cout << key << " " << std::boolalpha <<*temp;
                }
                else if(const std::string* temp = std::get_if<std::string>(&val)){
                    std::cout << key << " " << *temp;
                }
                else if(const std::vector<double>* temp = std::get_if<std::vector<double>>(&val)){
                    std::cout << key << " " << "{";
                    for(double ab:*temp){
                        std::cout<< ab << "," ;
                    }
                    std::cout<<"}";
                }
                std::cout<<std::endl;
            }
        }

        void printQuery(Statement* statement){
            std::cout<<std::endl<<"=======================\n";
            std::cout<<"Query Type: " << statement->type <<std::endl<<std::endl;
            std::cout<<"Table     :" << statement->tablename <<std::endl<<std::endl;
            std::cout<<"Body      :\n";
            printIFT(statement->body);
            std::cout<<std::endl;
            std::cout<<"Conditions: \n";
            printIFT(statement->condition);
            std::cout<<"=======================";
            std::cout<<std::endl;
        }

        void parseConditions(IFT & conditions){
            assert(tokens[index].kind == Token::Kind::Identifier);
            index += 1;
            assert(tokens[index].kind == Token::Kind::Equal);
            if(tokens[index+1].kind == Token::Kind::DoubleQuote){
                std::string key = tokens[index-1].val;
                index +=2;
                std::string value = ""; 
                while(tokens[index].kind != Token::Kind::DoubleQuote){
                    value += tokens[index].val + " ";
                    index += 1;
                }
                conditions[key] = value;
                index -= 1;
            }
            else if(tokens[index+1].kind == Token::Kind::Number){
                conditions[tokens[index-1].val] = stoi(tokens[index+1].val);
            }
            else if(tokens[index+1].kind == Token::Kind::LeftCurly
            ){
                std::string key = tokens[index-1].val;
                std::vector<double> veka;
                index+=2;
                assert(tokens[index].kind == Token::Kind::Number);
                veka.push_back(stod(tokens[index].val));
                index+=1;
                while(tokens[index].kind == Token::Kind::Comma){
                    index+=1;
                    assert(tokens[index].kind == Token::Kind::Number);
                    veka.push_back(stod(tokens[index].val));
                    index+=1;
                }
                conditions[key] = std::vector<double>(veka);
            }
            index += 2;
            if(tokens[index].kind == Token::Kind::Comma){
                index+=1;
                parseConditions(conditions);
            }
        }

        void GET(Statement* statement){
            if(tokens[index].kind == Token::Kind::Identifier){
                IFT body;
                body[tokens[index].val] = true;
                index += 1;
                while(tokens[index].kind == Token::Kind::Comma){
                    index += 1;
                    assert(tokens[index].kind == Token::Kind::Identifier);
                    body[tokens[index].val] = true;
                    index+=1;
                }
                statement->body = body;
            }
            assert(tokens[index].kind == Token::Kind::KEYWORD_FROM);
            index += 1;
            assert(tokens[index].kind == Token::Kind::Identifier);
            statement->tablename = tokens[index].val;
            index += 1;
            if(tokens[index].kind == Token::Kind::KEYWORD_WHERE){
                index += 1;
                if(tokens[index].kind == Token::Kind::KEYWORD_NEAREST){
                    index += 1;
                    std::string nn = "";
                    if(tokens[index].kind == Token::Kind::Number){
                        nn = tokens[index].val;   
                        index +=1;     
                    }
                    else{
                        nn = "1";
                    }
                    assert(tokens[index].kind == Token::Kind::KEYWORD_TO);
                    index += 1;
                    IFT condition;
                    condition[tokens[index].val] = "$"+nn;
                    statement->condition = condition;
                }
                else{
                    IFT conditions;
                    parseConditions(conditions);
                    statement->condition = conditions;
                }
            }
        }

        void UPDATE(Statement* statement){
            assert(tokens[index].kind == Token::Kind::KEYWORD_IN);
            index +=1;
            assert(tokens[index].kind == Token::Kind::Identifier);
            statement->tablename = tokens[index].val;
            index+=1;
            assert(tokens[index].kind == Token::Kind::KEYWORD_SET);
            index+=1;
            IFT body;
            parseConditions(body);
            statement->body = body;
            assert(tokens[index].kind == Token::Kind::KEYWORD_WHERE);
            index+=1;
            IFT condition;
            parseConditions(condition);
            statement->condition = condition;
        }

        void INSERT(Statement* statement){
            assert(tokens[index].kind == Token::Kind::KEYWORD_IN);
            index +=1;
            assert(tokens[index].kind == Token::Kind::Identifier);
            statement->tablename = tokens[index].val;
            index+=1;
            IFT body;
            parseConditions(body);
            statement->body = body;
        }

        void DELETE(Statement* statement){
            if(tokens[index].kind == Token::Kind::KEYWORD_FROM){
                index += 1;
                assert(tokens[index].kind == Token::Kind::Identifier);
                statement->tablename = tokens[index].val;
                index += 1;
                if(tokens[index].kind == Token::Kind::KEYWORD_WHERE){
                    index += 1;
                    IFT conditions;
                    parseConditions(conditions);
                    statement->condition = conditions;
                }
            }
            else if(tokens[index].kind == Token::Kind::Identifier){
                IFT body;
                body[tokens[index].val] = true;
                index += 1;
                while(tokens[index].kind == Token::Kind::Comma){
                    index += 1;
                    assert(tokens[index].kind == Token::Kind::Identifier);
                    body[tokens[index].val] = true;
                    index+=1;
                }

                statement->body = body;

                assert(tokens[index].kind == Token::Kind::KEYWORD_FROM);
                index += 1;
                assert(tokens[index].kind == Token::Kind::Identifier);
                statement->tablename = tokens[index].val;
                index += 1;
                if(tokens[index].kind == Token::Kind::KEYWORD_WHERE){
                    index += 1;
                    IFT conditions;
                    parseConditions(conditions);
                    statement->condition = conditions;
                }
            }
        }

        Statement* parse(){
            Statement* statement = new Statement();
            if(tokens[index].kind == Token::Kind::KEYWORD_GET){
                statement->type = STATEMENT_GET;
                index++;
                GET(statement);
            }
            else if(tokens[index].kind == Token::Kind::KEYWORD_INSERT){
                statement->type = STATEMENT_INSERT;
                index++;
                INSERT(statement);
            }
            else if(tokens[index].kind == Token::Kind::KEYWORD_UPDATE){
                statement->type = STATEMENT_UPDATE;
                index++;
                UPDATE(statement);
            }
            else if(tokens[index].kind == Token::Kind::KEYWORD_DELETE){
                statement->type = STATEMENT_DELETE;
                index++;
                DELETE(statement);
            }
            printQuery(statement);
            return statement;
        }
    };

    Statement* resolveStatement(std::string statement){
        Lexer lexer;
        lexer.lexme(statement);
        Parser parser(lexer.tokens);
        return parser.parse();
    }
}