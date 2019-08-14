Christian DataBase (cdb) Version 1.01 8/14/2019

A simple SQL like database made in C++. Originally made in my data structures class, using VIM, now can be ran in QT creator, or using the Makefile($make in \cdb\).

Guide (commands)

create table [name] [field1], [field2], [field3]
//create table teachers fname, lname, age

load [name]
//load teachers

insert into [name] value [value1], [value2], [value3]
//insert into teachers value katie, money, 28
//insert into teachers values katie, money, 28

select [field1] from [name] where [field1] = '[value]'
//select fname from teachers where fname = 'katie'
//select fname, lname from teachers where fname = 'katie'
//select * from teachers
//select * from teachers where lname = 'money'

print [name]
//print teachers

exit


