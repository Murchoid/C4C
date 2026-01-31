#ifndef C4C_TOML_PARSER_HPP
#define C4C_TOML_PARSER_HPP

#include <string>
#include <vector>
#include "toml_token.hpp"

class TomlLexer
{
    int col;
    bool has_errors;
    int line;
    int pos;
    int end;
    int token_start;
    std::string src;
    int length;
    int token_capacity;
    int token_count;
    std::vector<TomlTokens> tokens;

private:
    void make_decimal()
    {
        std::string buff;

        while (is_integer())
        {
            buff += consume();
        }

        if (match_token('.') && is_integer(1))
        {
            buff += consume();

            while (is_integer())
                buff += consume();

            add_token(buff, TomlTokenType::TOK_FLOAT);
        }
        else
        {
            add_token(buff, TomlTokenType::TOK_INTEGER);
        }

        update_col(buff.length());
    }

    inline void make_number()
    {
        make_decimal();
    }

    inline void add_keyword(std::string buff)
    {
        if (buff == "true" or buff == "false")
        {
            add_token(buff, TomlTokenType::TOK_BOOLEAN);
        }
        else
        {
            add_token(buff, TomlTokenType::TOK_IDENTIFIER);
        }
    }

    inline void make_identifier()
    {
        std::string buff;
        while (is_alphanumeric() || match_token('_'))
        {
            buff += consume();
        }

        update_col(buff.length());
        add_keyword(buff);
    }

    inline void make_string()
    {
        bool error = false;
        std::string buff;
        consume();

        while (not match_token('\"'))
        {
            if (is_end())
            {
                error = true;
                break;
            }

            buff += consume();
        }

        if (error == true)
        {
            add_token(buff, TomlTokenType::TOK_ERROR);
            this->has_errors = true;
        }
        else
        {
            consume();
            add_token(buff, TomlTokenType::TOK_STRING);
        }

        update_col(buff.length() + 2);
    }

    inline bool is_end()
    {
        int curr = this->pos;
        return curr >= this->length;
    }

    inline void update_row(size_t row_dx)
    {
        this->line += row_dx;
        this->col = 0;
    }

    inline void update_col(size_t len)
    {
        this->col += len;
        this->token_start = this->pos;
        this->end = this->pos;
    }

    inline char consume()
    {
        return this->src[this->pos++];
    }

    inline char peek(size_t lookahead = 0)
    {
        if (this->pos + (int)lookahead >= this->length)
        {
            return '\0';
        }

        return this->src[this->pos + lookahead];
    }

    inline bool is_integer(size_t lookahead = 0)
    {
        int curr_pos = this->pos;
        int idx = curr_pos + lookahead;

        if (idx >= this->length) return false;
        return (this->src[idx] >= '0' && this->src[idx] <= '9');
    }

    inline bool is_aslphabet(size_t lookahead = 0)
    {
        int curr_pos = this->pos;
        int idx = curr_pos + lookahead;
        if (idx >= this->length) return false;

        return (this->src[idx] >= 'a' && this->src[idx] <= 'z') || (this->src[idx] >= 'A' && this->src[idx] <= 'Z');
    }

    inline bool is_alphanumeric(size_t lookahead = 0)
    {
        return is_integer(lookahead) || is_aslphabet(lookahead);
    }

    inline bool match_token(char tok, size_t lookahead = 0)
    {
        int curr_pos = this->pos;
        int idx = curr_pos + lookahead;

        return this->src[idx] == tok;
    }

    inline void add_token(const std::string lexem, TomlTokenType type)
    {
        TomlTokens token(type, lexem, this->token_start, this->col, this->col, this->line);
        this->tokens.push_back(token);
    }

    inline void add_token_single(TomlTokenType type, std::string string = "")
    {
        consume();
        add_token(string, type);
        update_col(1);
    }

    inline void add_token_double(TomlTokenType type, std::string string = "")
    {
        consume();
        consume();
        add_token(string, type);
        update_col(2);
    }

    inline void skip_comment()
    {
        size_t skipped = 0;

        while (not is_end() && peek() != '\n')
        {
            consume();
            skipped++;
        }

        update_col(skipped);
    }

public:
    inline std::vector<TomlTokens> scan_tokens()
    {
        while (not is_end())
        {
            this->token_start = this->pos;
            scan_token();
        }

        add_token("eof", TomlTokenType::TOK_EOF);
        return this->tokens;
    }

    TomlLexer(std::string file_source)
    {
        this->col = 0;
        this->has_errors = false;
        this->line = 0;
        this->pos = 0;
        this->token_start = 0;
        this->end = 0;
        this->src = file_source;
        this->length = file_source.length();
        this->token_capacity = 0;
        this->token_count = 0;
    }

    inline void scan_token()
    {
        char token = peek();

        printf("Cursor pos: %d, char is : %c\n", this->pos, this->src[this->pos]);
        switch (token)
        {
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
            make_identifier();
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            make_number();
            break;
        case '\"':
            make_string();
            break;
        case '[':
            if (match_token('[', 1))
            {
                add_token_double(TomlTokenType::TOK_LDBRACKET, "[[");
            }
            else
            {
                add_token_single(TomlTokenType::TOK_LBRACKET);
            }
            break;
        case ']':
            if (match_token(']', 1))
            {
                add_token_double(TomlTokenType::TOK_RDBRACKET, "]]");
            }
            else
            {
                add_token_single(TomlTokenType::TOK_RBRACKET);
            }
            break;
        case ',':
            add_token_single(TomlTokenType::TOK_COMMA);
            break;
        case '\t':
            update_col(4);
            consume();
            break;
        case ' ':
            update_col(1);
            consume();
            break;
        case '\r':
            update_col(1);
            consume();
            break;
        case '\n':
            add_token_single(TomlTokenType::TOK_NEWLINE, "\\n");
            update_row(1);
            update_col(1);
            break;
        case '.':
            add_token_single(TomlTokenType::TOK_DOT, ".");
            break;
        case '=':
            add_token_single(TomlTokenType::TOK_EQUAL, "=");
            break;
        case '#':
            skip_comment();
            break;
        default:
            consume();
            this->has_errors = true;
            break;
        }
    }
};

#endif // C4C_TOML_PARSER_HPP
