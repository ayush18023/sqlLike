# sqlLike
A sql like query language interpreter written in c++.<br>
This includes a lexer which tokenizes the input query and a parser to parse the query in appropriate format and rules.
Instead of using AST in parser it uses a linear vector due to nature of query being linera and single liner.<br>
**Example**:
```
get id, name from tablename where id = 10, name="ayush des";
```
![image](https://github.com/ayush18023/sqlLike/assets/80062397/0329ca07-23db-498c-88f4-07b2e0136471)

**Other query examples:**
Insert query in a table
```
insert in tablename id=10, name="ayush", score=6;
```
Update query in a table given a condition
```
update in tablename set score=20 where id=10;
```
Delete query given a condition
```
delete from tablename where id=10;
```
query to create other database related constructs
```
create instance instance_name;
create index instance.attr_name;
create cluster instance.attr_name
```


Precompiled executable -> main.exe or compile with
```
g++ main.cc ./impl/qres.cc -o main -lstdc++ -std=c++17; ./main
```
