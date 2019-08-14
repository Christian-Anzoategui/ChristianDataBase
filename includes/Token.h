#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>

using namespace std;

class Token
{

public:
    Token();
    Token(string str, int type);

    friend ostream& operator <<(ostream& outs, const Token& t)
    {
	outs << "|" << t._token << "|";	
	return outs;
    }

    int type();
    string type_string();
    string token_str();

private:
    string _token;
    int _type;
};

Token::Token()
{
	_token = string();
	_type = -1;	
	
}

Token::Token(string str, int type)
{
	_token = str;
	_type = type;
}

int Token::type()
{
	return _type;
}

string Token::token_str()
{
	return _token;
}

string Token::type_string()
{
	string temp_str = string();
	
	if(_type == NUM_STATE_START)
	{
		temp_str = "NUMERIC";
	}

	else if(_type == ALPHA_STATE_START)
	{
		temp_str = "ALPHA";
	}

	else if(_type == SPACE_STATE_START)
	{
		temp_str = "SPACE";
	}

	else if(_type == PUN_STATE_START)
	{
		temp_str = "PUNCTUATION";
	}

	else {

		temp_str = "UNKNOWN";

	}
	
	return temp_str;	

}
#endif
