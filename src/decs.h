#pragma once

#include <string>
#include <vector>
#include <variant>
#include <unordered_map>

using Flex = std::variant<int, double, std::string, bool, std::vector<double>>;
using IFT =  std::unordered_map<std::string,Flex>;

class Token{
    public:
    enum class Kind{
        KEYWORD_FROM,
        KEYWORD_IN,
        KEYWORD_WHERE,
        KEYWORD_SET,
        KEYWORD_GET,
        KEYWORD_INSERT,
        KEYWORD_UPDATE,
        KEYWORD_DELETE,
        KEYWORD_NEAREST,
        KEYWORD_TO,
        Number,
        Identifier,
        LeftParen,
        RightParen,
        LeftSquare,
        RightSquare,
        LeftCurly,
        RightCurly,
        LessThan,
        GreaterThan,
        Equal,
        Plus,
        Minus,
        Asterisk,
        Comma,
        Semicolon,
        SingleQuote,
        DoubleQuote,
        Comment,
        Unexpected,
    };
    Kind kind;
    std::string val;
    Token(Kind _kind,std::string word){
        kind = _kind;
        val = word;
    }
};

typedef enum{
    TYPE_IFT,
    TYPE_SUCCESS,
    TYPE_FALIURE,
    TYPE_ERROR
} Types;

enum class StatementType{
    STATEMENT_CREATE,
    STATEMENT_GET,
    STATEMENT_INSERT,
    STATEMENT_UPDATE,
    STATEMENT_DELETE,
} ;

typedef struct {
    StatementType type;
    std::string tablename;
    IFT body; 
    IFT condition;
    Types returnType;
} Statement;