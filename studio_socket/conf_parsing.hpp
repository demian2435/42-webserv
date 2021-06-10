/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_parsing.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: forsili <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 09:43:52 by forsili           #+#    #+#             */
/*   Updated: 2021/06/10 11:22:11 by forsili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <string.h>

#define MAX_SIZET 18446744073709599
#define MIN_INT -2147483648

static char		*ft_next(char **a, int *size, char c, int i)
{
	char	*start;

	*size = 0;
	start = NULL;
	while (a[0][i])
	{
		if (a[0][i] == c && start)
		{
			a[0] = start + *size;
			return (start);
		}
		else if (a[0][i] != c && !start)
			start = &a[0][i];
		if (a[0][i] != c)
			*size += 1;
		i++;
	}
	a[0] = start + *size;
	if (!(*size))
		return (0);
	return (start);
}

char		*ft_strncpy(char *dest, char *src, size_t n, size_t i)
{
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char			**ft_split(const char *s, char c)
{
	int		size;
	char	**m;
	char	*n;
	char	*a;
	int		i;

	if (!s)
		return (NULL);
	a = (char *)s;
	i = 0;
	while (ft_next(&a, &size, c, 0))
		i++;
	if (!(m = (char **)malloc((i + 1) * sizeof(char *))))
		return (NULL);
	i = 0;
	a = (char *)s;
	while ((n = ft_next(&a, &size, c, 0)))
	{
		if (!(m[i] = (char *)malloc((size + 1) * sizeof(char))))
			return (NULL);
		ft_strncpy(m[i++], n, size, 0);
	}
	m[i] = 0;
	return (m);
}

int		ft_iscontain(char c, char const *set)
{
	while (*set)
		if (c == *set++)
			return (1);
	return (0);
}

char			*ft_strtrim(char **str, char const *set, int mod)
{
	int			len;
	char		*p;
	char		*s1;

	s1 = *str;
	if (!s1 || !set)
		return (NULL);
	while (*s1 && ft_iscontain(*s1, set))
		s1++;
	len = strlen(s1);
	if (!len)
		return (NULL);
	while (ft_iscontain(s1[len - 1], set))
		len--;
	if ((p = strndup(s1, len)) == NULL)
		return (NULL);
	p[len] = 0;
	if (mod)
		free(*str);
	return (p);
}

int 		matrix_len(char **matrix)
{
	int i;

	i = 0;
	if (!matrix)
		return (0);
	while (matrix[i])
		i++;
	return (i);
}

void    free_matrix(char **matrix)
{
    int i = 0;
    while (matrix[i])
    {
        free(matrix[i]);
        i++;
    }
    free(matrix[i]);
    free(matrix);
}

char **check_command(char *cmd)
{
    char **tmp = NULL;
    char **semi = NULL;
    char **space = NULL;
    tmp = ft_split(cmd, '#');
    if (tmp && tmp[0])
    {
        semi = ft_split(tmp[0], ';');
        if (semi && semi[0])
        {
            space = ft_split(semi[0], ' ');
            if (space && space[0])
            {
                free_matrix(tmp);
                free_matrix(semi);
                return (space);
            }
        }
    }
    if (tmp)
        free_matrix(tmp);
    if (semi)
        free_matrix(semi);
    return NULL;
}

class   location{
    public:
        std::string                     path;
        std::string                     root;
        std::list<std::string>          index;
        std::list<std::string>          method;
        std::string                     cgi_path;
        std::string                     cgi_extention;
        bool                            autoindex;
        size_t                          client_max_body_size;
    private:


        int ft_isdigit(char *s)
        {
            int i = 0;
            while(s[i])
            {
                if (!isdigit(s[i]))
                    return(0);
                i++;
            }
            return (1);
        }

    public:
        
        location(void)
        {
            this->path = "";
            this->root = "";
            this->cgi_path = "";
            this->cgi_extention = "";
            this->autoindex = false;
            this->client_max_body_size = MAX_SIZET;
        }

        location(std::string path, std::ifstream &myfile, int &line)
        {
            std::string buff;
            std::list<std::string> lst;
            char **argv;
            char *tmp = NULL;
            this->path = "";
            this->root = "";
            this->cgi_path = "";
            this->cgi_extention = "";
            this->index = lst;
            this->autoindex = false;
            this->client_max_body_size = MAX_SIZET;
            this->path = path;
            while (getline(myfile,buff))
            {
                tmp = (char *)(buff.c_str());
                tmp = ft_strtrim(&tmp, " ", 0);
                argv = check_command(tmp);
                if (matrix_len(argv) == 0 || (!strcmp(argv[0], "#")))
                {
                    line++;
                    continue;
                }
                if (!strcmp(argv[0], "}"))
                {
                    line++;
                    free(tmp);
                    free_matrix(argv);
                    break;
                }
                if (!strcmp(argv[0], "root") && strlen(argv[0]) == strlen("root"))
                {
                    if (matrix_len(argv) != 2)
                        std::cout << RED << "Error: invalid root line: " << line << "\n" << RESET;
                    else
                    {
                        std::string str(argv[1]);
                        this->root = str;
                    }
                    line++;
                }
                else if (!strcmp(argv[0], "index") && strlen(argv[0]) == strlen("index"))
                {
                    for (size_t k = 1; k < (size_t)(matrix_len(argv)); k++)
                        this->index.push_back(std::string(argv[k]));
                    line++;
                }
                //else if (!strcmp(argv[0], "method") && strlen(argv[0]) == strlen("method"))
                //{
                //    for (size_t k = 1; k < matrix_len(argv); k++)
                //    {
                //        this->method.push_back(std::string(argv[k]));
                //    }
                //}
                else if (!strcmp(argv[0], "cgi_path") && strlen(argv[0]) == strlen("cgi_path"))
                {
                    if (matrix_len(argv) != 2)
                        std::cout << RED <<"Error: invalid CGI path at line: " << line << "\n" << RESET;
                    else
                        this->cgi_path = std::string(argv[1]);
                    line++;
                }
                else if (!strcmp(argv[0], "cgi_extension") && strlen(argv[0]) == strlen("cgi_extension"))
                {
                    if (matrix_len(argv) != 2)
                        std::cout << RED << "Error: invalid CGI extension at line: " << line << "\n" <<RESET;
                    else
                        this->cgi_extention = std::string(argv[1]);
                    line++;
                }
                else if (!strcmp(argv[0], "autoindex") && strlen(argv[0]) == strlen("autoindex"))
                {
                    if (matrix_len(argv) != 2)
                        std::cout << YELLOW << "Warning: autoindex mode on false by default at line: " << line << "\n" << RESET;
                    else
                    {
                        if (!strcmp(argv[1], "true") && strlen(argv[1]) == strlen("true"))
                            this->autoindex = false;
                        else
                            this->autoindex = true;

                    }
                    line++;
                }
                else if (!strcmp(argv[0], "client_max_body_size") && strlen(argv[0]) == strlen("client_max_body_size"))
                {
                    if (matrix_len(argv) != 2 || !ft_isdigit(argv[1]))
                        std::cout << YELLOW <<"Warning: invalid max body size, setted as default, at line: " << line << "\n" << RESET;
                    else
                    {
                        this->client_max_body_size = size_t(atoi(argv[1]));
                    }
                    line++;
                }
                else if (argv)
                {
                    argv[0] = ft_strtrim(&argv[0], " ", 1);
                    std::cout << YELLOW <<"Warning: cannot read line " << line << " in location path " << this->path;
                    std::cout << " command " << argv[0] << " not supported\n" <<RESET;           
                    line++;
                }
                if (tmp)
                    free(tmp);
                if (argv)
                    free_matrix(argv);
            }
        }

        ~location(){}

        void    setRoot(std::string input)
        {
            this->root = input;
        }

        void    setPath(std::string input)
        {
            this->path = input;
        }

        void    setIndex(std::string input)
        {
            this->index.push_back(input);
        }

        void    setIndex(std::string *input, int size)
        {
            for (size_t i = 0; i < (size_t)(size); i++)
                this->index.push_back(input[i]);
        }

        void    setMethod(std::string input)
        {
            this->method.push_back(input);
        }

        void    setMethod(std::string *input, int size)
        {
            for (size_t i = 0; i < (size_t)(size); i++)
                this->method.push_back(input[i]);
        }

        void    setCgiExtension(std::string input)
        {
            this->cgi_extention = input;
        }

        void    setCgiPath(std::string input)
        {
            this->cgi_path = input;
        }

        void    setAutoindex(bool input)
        {
            this->autoindex = input;
        }

        void    setMaxBodySize(size_t input)
        {
            this->client_max_body_size = input;
        }

        std::string     getRoot()
        {
            return this->root;
        }

        std::string     getPath()
        {
            return this->path;
        }

        std::list<std::string>  getIndex()
        {
            return this->index;
        }

        std::list<std::string>  getMethod()
        {
            return this->method;
        }

        std::string     getCgiPath()
        {
            return  this->cgi_path;
        }

        std::string getCgiExtension()
        {
            return this->cgi_extention;
        }

        bool        getAutoindex()
        {
            return this->autoindex;
        }

        size_t      getMaxBodySize()
        {
            return this->client_max_body_size;
        }

};

class errorPages
{
    private:
        int         key[700];
        std::string path[700];
    public:
        errorPages()
        {
            for (size_t i = 0; i < 700; i++)
            {
                key[i] = i;
                path[i] = "";
            }
        }

        ~errorPages(){}
        
        void    insertPath(int code, std::string path)
        {
            this->path[code] = path;
        }

        std::string getPath(int code)
        {
            return this->path[code];
        }
    };


class   config{
    
    public:
        std::string             host;
        std::string             port;
        std::string             name;
        errorPages              error_pages;
        std::list<location>     locations;

    private:

        int ft_isdigit(char *s)
        {
            int i = 0;
            while(s[i])
            {
                if (!isdigit(s[i]))
                    return(0);
                i++;
            }
            return (1);
        }

        void    parse(std::ifstream &myfile, int &line)
        {
            std::string buff;
            char **argv;
            char *tmp = NULL;
            int i = 0;
            errorPages  errorP;
            while (getline(myfile,buff))
            {
                tmp = (char *)(buff.c_str());
                tmp = ft_strtrim(&tmp, " ", 0);
                argv = check_command(tmp);
                if (matrix_len(argv) == 0)
                {
                    line++;
                    continue;
                }
                if (!strcmp(argv[0], "}"))
                {
                    free(tmp);
                    free_matrix(argv);
                    line++;
                    break;
                }
                if (!strcmp(argv[0], "listen") && strlen(argv[0]) == strlen("listen"))
                {
                    if (matrix_len(argv) != 3)
                        std::cout << RED << "Error: no port or address at line: " << line << "\n" <<RESET;
                    else
                    {
                        i = 1;
                        while(i < 3)
                        {
                            if (ft_isdigit(argv[i]))
                                this->host = std::string(argv[i]);
                            else
                                this->port = std::string(argv[i]);
                            i++;
                        }
                    }
                    line++;
                }
                else if (!strcmp(argv[0], "server_name") && strlen(argv[0]) == strlen("server_name"))
                {
                    if (matrix_len(argv) != 2)
                        std::cout << RED <<"Error: too or no name for server in line: " << line << "\n" <<RESET;
                    else
                        this->name = std::string(argv[1]);
                    line++;
                }                
                else if (!strcmp(argv[0], "error_page") && strlen(argv[0]) == strlen("error_page"))                
                {
                    if (matrix_len(argv) != 3)
                        std::cout << RED << "Error: too argument for error_page ex.(error_page 404 /path/path;) in line: " << line << "\n" <<RESET;
                    else{
                        if (ft_isdigit(argv[1]) && atoi(argv[1]) < 700)
                            errorP.insertPath(atoi(argv[1]), argv[2]);
                        else
                            std::cout <<YELLOW << "Warning: invalid not numbered error, ignoring at line: " << line << "\n" <<RESET;
                    }
                    line++;
                }
                else if (!strcmp(argv[0], "location") && strlen(argv[0]) == strlen("location"))                
                {
                    if (matrix_len(argv) != 3)
                        std::cout << RED <<"Error: syntax not valid for location in line: " << line << "\n" <<RESET;
                    else {
                        std::string path_location(argv[1]);
                        line++;
                        location    loc(path_location, myfile, line);
                        this->locations.push_back(loc);
                    }
                }
                else if (argv)
                {
                    std::cout << YELLOW<< "Warning: cannot read line " << line;
                    std::cout << " command " << argv[0] << " not supported\n" <<RESET;           
                    line++;
                }
                //std::cout << line << std::endl;
                if (tmp)
                    free(tmp);
                if (argv)
                    free_matrix(argv);
            }
            this->error_pages = errorP;
        }        
    public:
        config()
        {
            std::string path = "../config/webserv.conf";
            std::string buffer;
            std::ifstream myfile(path);
            char    *tmp;
            char    **argv;
            int     line = 1;
            if (myfile.is_open())
            {
                while (getline(myfile,buffer))
                {
                    tmp = (char *)(buffer.c_str());
                    tmp = ft_strtrim(&tmp, " ", 0);
                    argv = check_command(tmp);
                    if (matrix_len(argv) == 0)
                    {
                        line++;
                        continue;
                    }
                    if (!strcmp(argv[0], "}") && strlen(argv[0]) == strlen("}"))
                    {
                        free(tmp);
                        free_matrix(argv);
                        line++;
                        break;
                    }
                    if (!strcmp(argv[0], "server") && strlen(argv[0]) == strlen("server"))
                    {
                        line++;
                        parse(myfile, line);
                    } 
                    if (tmp)
                        free(tmp);
                    if (argv)
                        free_matrix(argv);           
                }
            }
            else
                std::cerr << "Error: can't open config file\n";
        }   
};
