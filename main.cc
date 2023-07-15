#include <iostream>

#include "./src/decs.h"
#include "./src/qres.h"

using namespace Query;

int main(){
    // string query = "insert in tablename id=10,name="ayush",score=69;";
    // string query1 = "get from tablename where id=10;";
    // string query2 = "update in tablename set score=20 where id=10;";
    // string query3 = "delete from tablename where id=10;";
    std::cout << "\\\\        //  ||[=======]  ||\\\\    ||   [======] \n";
    std::cout << " \\\\      //   ||           || \\\\   ||  ||      ||\n";
    std::cout << "  \\\\    //    ||[=======]  ||  \\\\  ||  ||      ||\n";
    std::cout << "   \\\\  //     ||           ||   \\\\ ||  ||      ||\n";
    std::cout << "    \\\\//      ||[=======]  ||    \\\\||   [======] \n";
    std::string statement;
    char delimiter = ';';
    char c;
    while(true){
        statement = "";
        std::cout << std::endl << std::endl << ">";
        while (std::cin.get(c)) {
            if (c == delimiter) {
                statement += ' ';
                break;
            } else if (c == '\n') {
                statement += ' ';
                std::cout<<"..";
            } else {
                statement += c;
            }
        }
        statement += ';';
        Statement* st = resolveStatement(statement);
    }
    return 0;
}