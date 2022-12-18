#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
//using namespace std;

enum kind { ADD, SUB, Mod, DIV, MUL, T, F, LB, RB, LessThan, Equals, ID, NUM, ERROR };

struct Token {
kind TokenType;
std::string lexeme;
int line;
};

Token TokenAssign(kind type, std::string s, int ln ){
	Token temp;
	temp.TokenType=type;
	temp.lexeme=s;
	temp.line=ln;
	return temp;
}

Token checkBoolean(FILE *fp,int ln){
	char c = fgetc(fp);  
	Token temp;
	if(c==EOF){
		return TokenAssign(ERROR,"#",ln);
	}else if(c=='t' || c=='f'){
		if(c=='t'){
			return TokenAssign(T,"T",ln);
		}
		else{
			return TokenAssign(F,"F",ln);
		}
	}else{
		fseek(fp,-1,SEEK_CUR);
		return TokenAssign(ERROR,"#",ln);
	}
}

Token checkNum(FILE *fp,int ln){
	std::string num="";
	char c = fgetc(fp);
	num+=c;

	c=fgetc(fp);

	while(c!=-1){
		
		if(c>=48 && c<=57){
			num+=c;
		}else{
			fseek(fp,-1,SEEK_CUR);
			break;
		}
		c = fgetc(fp);
	}
	return TokenAssign(NUM,num,ln);
}
bool isvalidchar(char c){
	if( (c>=97 && c<=122) || c=='?' || (c>=48 && c<=57) || c=='_' ){
		return true;
	}else{
		return false;
	}
}

Token checkWord(FILE *fp,int ln){
	std::string word="";
	char c = fgetc(fp);
	word+=c;
	c = fgetc(fp);
	while(c!=-1){
		if(isvalidchar(c)){
			word+=c;
		}else{
			fseek(fp,-1,SEEK_CUR);
			break;
		}
		c = fgetc(fp);
	}
		return TokenAssign(ID,word,ln);
}


std::vector<Token> solve(FILE * fp){
	std::vector<Token> ans;
	int line_count=1;
	Token temp;
	char c = fgetc(fp);
	while(c!=-1){	
		bool whitespace=false;
		switch(c){
			case ' ': 
			case '\t' : whitespace=true;break;
			case '\n': whitespace=true;line_count++;break;  
			case '+' : 
				temp=TokenAssign(ADD,"+",line_count);
				break;
			case '/' :
				temp=TokenAssign(DIV,"/",line_count);
				break;
			case '*' :
				temp=TokenAssign(MUL,"*",line_count);
				break;
			case '%' :
				temp=TokenAssign(Mod,"%",line_count);
				break;
			case '=' :
				temp=TokenAssign(Equals,"=",line_count);
				break;
			case '<' :
				temp=TokenAssign(LessThan,"<",line_count);
				break;
			case '(' :
				temp=TokenAssign(LB,"(",line_count);
				break;
			case ')' :
				temp=TokenAssign(RB,")",line_count);
				break;
			case '#' :
				temp=checkBoolean(fp,line_count);
				break;
			case '-' :
				c = fgetc(fp);
				fseek(fp,-1,SEEK_CUR);
				if(c>=48 && c<=57){
					temp=checkNum(fp,line_count);
					temp.lexeme="-"+temp.lexeme;			
				}	
				else{
					temp=TokenAssign(SUB,"",line_count);
				}
				break;
			default :
				if(c>=48 && c<=57){
					fseek(fp,-1,SEEK_CUR);
					temp=checkNum(fp,line_count);

					break;
				}else if(c>=97 && c<=122){
					fseek(fp,-1,SEEK_CUR);
					temp=checkWord(fp,line_count);
					break;
				}else{
					std::string s = "";
					s+=c;
					temp=TokenAssign(ERROR,s,line_count);
					break;
				}
		}
		c=fgetc(fp);
		if(!whitespace)
			ans.push_back(temp);
		//std::cout<<temp.TokenType<<" "<<temp.lexeme<<" "<<temp.line<<'\n';
	}
	return ans;
}


int main(){
	FILE * fp = fopen("/Users/kumarakrishnavaleti/Desktop/sublime_code/inputf.in","r");
	std::vector<Token> ans = solve(fp);
	fclose(fp);
	for(int i=0;i<ans.size();i++){
		std::cout<<ans[i].TokenType<<" "<<ans[i].lexeme<<" "<<ans[i].line<<'\n';
	}
}