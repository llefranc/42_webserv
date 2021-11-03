# webserv (@42Paris)

> This project was code for MACOS. Here is the [subject][1].
>

*"This is when you finally understand why a URL starts with HTTP"*

![Alt text](https://github.com/llefranc/42_webserv/blob/main/webserv_example.png)

## About

Webserv is a project of the mandatory part of the cursus that I made with my friend [Helene Herin][2]. 

<br/>It's only made in C++ and was about creating a web server able to handle one of the most famous web protocol, HTTP 1.1.
Our program is working with a configuration file, so you can setup the port, some routes, the allowed methods and some other basic paramaters for a webserver.
<br/>It's build based on UNIX sockets, and since it's an HTTP server you can access web pages using your favorite browser.

Our server handles the following HTTP features :
- The basics of HTTP protocol (status line / request line, a few headers, and payload).
- GET, POST and DELETE methods.
- HTTP response status codes (200, 404, ...).
- Error pages for the appropriate status code.
- HTTP redirection (status code 301).
- CGI, like rendering a .php file thanks to php-cgi for example.

## Understanding the configuration file

</br>It's only made in C and was about creating two programs : one sorting two "special" stacks, and another one checking if the stacks are sorted or not. 
</br>We were only allowed to modify the two "stacks", called **stack a** and **stack b**, using a set of instruction : 
- `sa` *(swap a)* -- swap the first 2 elements at the top of stack a. Do nothing if there is only one or no elements).
- `sb` *(swap b)* -- swap the first 2 elements at the top of stack b. Do nothing if there is only one or no elements).
- `ss` sa and sb at the same time.
- `pa` *(push a)* -- take the first element at the top of b and put it at the top of a. Do nothing if b is empty.
- `pb` *(push b)* -- take the first element at the top of a and put it at the top of b. Do nothing if a is empty.
- `ra` *(rotate a)* -- shift up all elements of stack a by 1. The first element becomes the last one.
- `rb` *(rotate b)* -- shift up all elements of stack b by 1. The first element becomes the last one.
- `rr` -- ra and rb at the same time.
- `rra` *(reverse rotate a)* -- shift down all elements of stack a by 1. The flast element becomes the first one.
- `rrb` *(reverse rotate b)* -- shift down all elements of stack b by 1. The flast element becomes the first one.
- `rrr` -- rra and rrb at the same time.


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