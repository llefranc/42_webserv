# webserv (@42Paris)

> This project was code for MACOS. Here is the [subject][1].
>

*"This is when you finally understand why a URL starts with HTTP"*

![Alt text](https://github.com/llefranc/42_webserv/blob/main/webserv_example.png)

## About

Webserv is a project of the mandatory part of the cursus that I made with my friend [Helene Herin][2].
<br/><br/>It's only made in C++ and was about creating a web server able to handle one of the most famous web protocol, HTTP 1.1.
Our program is working with a configuration file, so you can setup the port, some routes, the allowed methods and some other basic paramaters for a webserver.
<br/>It's build based on UNIX sockets, and since it's an HTTP server you can access web pages using your favorite browser.

Our server handles the following HTTP features :
- The basics of HTTP protocol (status line / request line, a few headers, and payload).
- GET, POST and DELETE methods.
- HTTP response status codes (200, 404, ...).
- Error pages for the appropriate status code.
- CGI, like rendering a .php file in your browser thanks to php-cgi for example.

## Understanding the configuration file

Webserv need a configuration file that will be loaded at the beginning of the program, setting up all the necessary parameters. It works in a similar way than Nginx configuration file.
</br>It will allow you to setup the following parameters :
- `server` -- to define several virtual servers (with multiple server blocks).
- `server_name` -- to identify a specific server block thanks to host header.
- `listen` -- to configure on which port the server will be listening (only one `listen` per `server` block).
- `error` -- to set a path for custom error pages.
- `location` -- to set parameters for a specific route.
- `index` -- to serve a default file if none match the requested URI.
- `root` -- to route the requested URI to another directory.
- `upload_path` -- to set a specific directory for upload requests.
- `autoindex` -- to list all files present in the targeted directory.
- `allow_method` -- to define the allowed methods for HTTP requests.
- `cgi` -- to configure a cgi to execute for a specific file extenstion (php-cgi for .php files for example).

![Alt text](https://github.com/llefranc/42_webserv/blob/main/config_file_example.png)
https://github.com/llefranc/42_webserv/blob/main/config_file_example.png

Here is a short example of a configuration file. 

You can fing some examples in `conf` directory. If testing with your broser, use the browser*.conf files.

## How I solved the problem

For small list of integers (< 5 numbers) I used custom sortings to optimize the number of instructions. 
</br>For bigger list, I implemented a variant of quicksort algorithm with several optimizations.
</br></br>Average nb of insctructions for 100 tests :
- :white_check_mark: : 3 numbers in `1` instruction *(grade 5/5)*
- :white_check_mark: : 5 numbers in `9` instructions *(grade 5/5)*
- :white_check_mark: : 100 numbers in `772` instructions *(grade 4/5)*
- :white_check_mark: : 500 numbers in `5357` instructions *(grade 5/5)*

## Building and running the project

1. Download/Clone this repo

        git clone https://github.com/llefranc/42_push_swap.git

2. `cd` into the root directory and run `make`

        cd 42_push_swap
        make

3.  Run `push_swap` with a list of integers in order to print the list of instructions on stdout.
	
		./push_swap 2 1 5 4

4.	Run `checker` with a list of integers to solve manually the stack. It will wait for instruction on stdin. Ctrl+D to stop typing instructions
	
		./checker 2 1 5 4


5.	Pipe `push_swap` into `checker` with the same list of integers as argument to see if the output of `push_swap` correctly solved the problem
	
		./push_swap 2 1 5 4 | ./checker 2 1 5 4

6. Use `checker` with `-v` option to visualize how push_swap solved the problem
	
		./push_swap 2 1 5 4 | ./checker -v 2 1 5 4

http://localhost:8080/www/tests/test_cgi/index.php (browser.conf)
http://localhost:8080/index
http://localhost:8080/file_doesnt_exist

curl -H "host:index_test" http://localhost:8080/index
curl -H "host:localhost" http://localhost:8080/www/tests/test_cgi/hello_world.php


## Sources

- [Quicksort algorithm explained step by step][7]

[1]: https://github.com/llefranc/42_webserv/blob/main/webserv.en.subject.pdf
[2]: https://github.com/hherin