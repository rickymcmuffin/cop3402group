#include "lexer.h"

FILE *fp;
token *ret;
// -1 = ignore
// 0 = nothing
// 1 = end of file
int state = 0;
int column = 0;
int line = 1;

// Requires: fname != NULL
// Requires: fname is the name of a readable file
// Initialize the lexer and start it reading
// from the given file name
extern void lexer_open(const char *fname)
{

	fp = fopen(fname, "r");
	if (fp == NULL)
	{
		bail_with_error("Error");
	}
	ret = malloc(sizeof(token));
	ret->line = 1;
	ret->column = 1;
	ret->filename = calloc(strlen(fname) + 1, sizeof(char));
	ret->text = calloc(MAX_IDENT_LENGTH + 1, sizeof(char));
	strcpy(ret->filename, fname);
}

// Close the file the lexer is working on
// and make this lexer be done
extern void lexer_close()
{
	free(ret->filename);
	free(ret);
	fclose(fp);
}

// Is the lexer's token stream finished
// (either at EOF or not open)?
extern bool lexer_done()
{
	if (fp == NULL)
	{
		return 1;
	}
	int c = fgetc(fp);
	if (c == EOF)
	{
		lexer_close();
		return 1;
	}
	ungetc(c, fp);
	return 0;
}

// Requires: !lexer_done()
// Return the next token in the input file,
// advancing in the input
extern token lexer_next()
{	
	if (lexer_done())
	{
		return *ret;
	}
	int tokenFinished = 0;

	while (!tokenFinished)
	{

		int c = fgetc(fp);
		column++;
		// what caden said last time
		if (state == 0)
		{
			if (baseState(c))
			{
				tokenFinished = 1;
			}
		}
		else if (state == -1)
		{
			ignoreState(c);
		}

		if (state == 1)
		{
			break;
		}
	}
	ret->line = line;
	if (ret->text != NULL)
	{
		ret->column = column - (strlen(ret->text) - 1);
	}
	else
	{
		ret->column = column;
	}
	return *ret;
}

void ignoreState(int c)
{
	while (state == -1)
	{
		c = fgetc(fp);
		column++;
		if (c == EOF)
		{
			lexical_error(ret->filename, line, column + 1, "File ended while reading comment!");
		}
		if (c == '\n')
		{
			state = 0;
			column = 0;
			line++;
		}
	}
}

// return 1 if token is finsihed
// return 0 otherwise
int baseState(int c)
{
	if (c == EOF)
	{
		ret->typ = eofsym;
		free(ret->text);
		ret->text = NULL;
		state = 1;
		return 1;
	}
	else if (c == '#')
	{
		state = -1;
		return 0;
	}
	else if (c == '\n')
	{
		line++;
		column = 0;
		return 0;
	}
	else if (isspace(c))
	{
		return 0;
	}
	else if (c == ';')
	{
		ret->typ = semisym;
		strcpy(ret->text, ";");
		
		return 1;
	}
	else if (isalpha(c))
	{
		return readWord(c);
	}
	else if (c == '<')
	{
		c = fgetc(fp);
		column++;
		if (c == '=')
		{
			ret->typ = leqsym;
			strcpy(ret->text, "<=");
		}
		else if (c == '>')
		{
			ret->typ = neqsym;
			strcpy(ret->text, "<>");
		}
		else
		{
			ungetc(c, fp);
			column--;
			ret->typ = lessym;
			strcpy(ret->text, "<");
		}

		return 1;
	}
	else if (c == '>')
	{
		c = fgetc(fp);
		column++;
		if (c == '=')
		{
			ret->typ = geqsym;
			strcpy(ret->text, ">=");
		}
		else
		{
			ungetc(c, fp);
			column--;
			ret->typ = gtrsym;
			strcpy(ret->text, ">");
		}
		return 1;
	}
	else if (c == '=')
	{
		ret->typ = eqsym;
		strcpy(ret->text, "=");
		return 1;
	}
	else if (c == '+')
	{
		ret->typ = plussym;
		strcpy(ret->text, "+");
		return 1;
	}
	else if (c == '-')
	{
		ret->typ = minussym;
		strcpy(ret->text, "-");
		return 1;
	}
	else if (c == '*')
	{
		ret->typ = multsym;
		strcpy(ret->text, "*");
		return 1;
	}
	else if (c == '/')
	{
		ret->typ = divsym;
		strcpy(ret->text, "/");
		return 1;
	}
	else if (c == '.')
	{
		ret->typ = periodsym;
		strcpy(ret->text, ".");
		return 1;
	}
	else if (isdigit(c))
	{
		return readNumber(c);
	}
	else if (c == '(')
	{
		ret->typ = lparensym;
		strcpy(ret->text, "(");
		return 1;
	}
	else if (c == ')')
	{
		ret->typ = rparensym;
		strcpy(ret->text, ")");
		return 1;
	}
	else if (c == ',')
	{
		ret->typ = commasym;
		strcpy(ret->text, ",");
		return 1;
	}
	else if (c == ':')
	{
		c = fgetc(fp);
		column++;
		if (c == '=')
		{
			ret->typ = becomessym;
			strcpy(ret->text, ":=");
			return 1;
		}
		else
		{
			lexical_error(ret->filename, line, column + 1, "Expecting '=' after a colon, not ' '");
		}
	}
	lexical_error(ret->filename, line, column + 1, "Illegal character '%c' (%.3o)", c, c);
	return 0;
}

int readNumber(int c)
{
	int len = 1;
	ret->text[len - 1] = c;
	ret->text[len] = '\0';

	do
	{
		if (!isdigit(c))
		{
			lexical_error(ret->filename, line, column, "invalid character in number");
		}
		if (len > MAX_IDENT_LENGTH)
		{
			lexical_error(ret->filename, line, column, "number greater than max length");
		}
		ret->text[len - 1] = c;
		ret->text[len] = '\0';
		c = fgetc(fp);
		column++;
		len++;
	} while (!isspace(c) && c != '.' && c != ',' && c != ';');
	ungetc(c, fp);
	column--;
	len--;
	ret->text[len] = '\0';
	int x = atoi(ret->text);
	if (x > __SHRT_MAX__)
	{
		lexical_error(ret->filename, line, column + 1, "The value of %d is too large for a short!", x);
	}
	ret->value = x;
	ret->typ = numbersym;

	return 1;
}

// reads keyword with input of first character
// returns 1 if ready to return token
// returns 0 otherwise
int readWord(int c)
{
	int len = 1;

	do
	{

		/*if (!isalnum(c))
		{

			lexical_error(ret->filename, line, column, "invalid character in identifier");
		}*/
		if (len > MAX_IDENT_LENGTH)
		{
			lexical_error(ret->filename, line, column + 1, "Identifier starting \"%s\" is too long!", ret->text);
		}
		ret->text[len - 1] = c;
		ret->text[len] = '\0';
		c = fgetc(fp);
		column++;
		len++;
	} while (isalnum(c));
	ungetc(c, fp);
	column--;
	len--;
	ret->text[len] = '\0';
	ret->typ = stringToToken(ret->text);

	return 1;
}

token_type stringToToken(char *w)
{

	if (strcmp(w, "const") == 0)
	{
		return constsym;
	}
	if (strcmp(w, "var") == 0)
	{
		return varsym;
	}
	if (strcmp(w, "procedure") == 0)
	{
		return procsym;
	}
	if (strcmp(w, "begin") == 0)
	{
		return beginsym;
	}
	if (strcmp(w, "end") == 0)
	{
		return endsym;
	}
	if (strcmp(w, "while") == 0)
	{
		return whilesym;
	}
	if (strcmp(w, "do") == 0)
	{
		return dosym;
	}
	if (strcmp(w, "if") == 0)
	{
		return ifsym;
	}
	if (strcmp(w, "then") == 0)
	{
		return thensym;
	}
	if (strcmp(w, "else") == 0)
	{
		return elsesym;
	}
	if (strcmp(w, "call") == 0)
	{
		return callsym;
	}
	if (strcmp(w, "write") == 0)
	{
		return writesym;
	}
	if (strcmp(w, "read") == 0)
	{
		return readsym;
	}
	if (strcmp(w, "odd") == 0)
	{
		return oddsym;
	}
	if (strcmp(w, "numbers") == 0)
	{
		return numbersym;
	}
	if (strcmp(w, "skip") == 0)
	{
		return skipsym;
	}
	else
	{
		return identsym;
	}
}

/*
    commasym,

*/
/*
Token Definitions:
constsym 1 const
varsym 2 var
procsym 3 procedure
beginsym 4 begin
endsym 5 end
whilesym 6 while
dosym 7 do
ifsym 8 if
thensym 9 then
elsesym 10 else
callsym 11 call
writesym 12 write
readsym 13 read
idensym 14 identifiers
numbersym 15 numbers
assignsym 16 :=
addsym 17 +
subsym 18 -
multsym 19 *
divsym 20 /
modsym 21 %
eqlsym 22 ==
neqsym 23 !=
lsssym 24 <
leqsym 25 <=
gtrsym 26 >
geqsym 27 >=
oddsym 28 odd
lparensym 29 (
rparensym 30 )
commasym 31 ,
periodsym 32 .
semicolonsym 33 ;
*/

// Requires: !lexer_done()
// Return the name of the current file
extern const char *lexer_filename()
{
	if (lexer_done())
		return NULL;
	return ret->filename;
}

// Requires: !lexer_done()
// Return the line number of the next token
extern unsigned int lexer_line()
{
	if (lexer_done())
		return 0;
	return ret->line;
}

// Requires: !lexer_done()
// Return the column number of the next token
extern unsigned int lexer_column()
{
	if (lexer_done())
		return 0;
	return ret->column;
}