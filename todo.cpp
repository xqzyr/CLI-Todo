#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stack>
#include <Windows.h>
#include <ctime>


void add(std::string s){

    //Adding todo to todo.txt
    std::ofstream fout;
    fout.open("todo.txt", std::ios_base::app); //seeking to last line
    fout <<  s << "\n";
    fout.close();

    std::cout << "Added todo: \"" << s << "\"" << std::endl; //confirmation

}

void ls(){

    int count = 0;
    std::string line;
    std::ifstream file;

    //Calculating the total number of pending todos
    //and initializing stack
    file.open("todo.txt");
    std::stack <std::string> filestack;
    while(getline(file, line)) {
        count++;
        filestack.push(line);
    }
    file.close();

    if (count == 0) 
        std::cout << "There are no pending todos!" << std::endl;
    
    //Printing pending todos with numbering
    file.open("todo.txt");
    std::string temp;
    for (int i = count; i >= 1; i--) {
        std::cout << "[" << i << "] " << filestack.top() << std::endl;
        filestack.pop();
    }
    file.close();

}

void del(std::string s){

    int tasknum = std::stoi(s);
    int count = 0;

    std::string line;
    std::ifstream file;

    //Counting the number of todos pending
    file.open("todo.txt");
    while(getline(file, line))
        count++;
    file.close();

    //Handling wrong todo list number
    if (tasknum > count || tasknum <= 0) {
        std::cout << "Error: todo #" << tasknum << " does not exist. Nothing deleted.";
        return;
    }

    //Storing the todo to be deleted in line_to_erase
    std::string line_to_erase;
    file.open("todo.txt");
    for (int i = 1; i <= tasknum; i++)
        getline(file, line_to_erase);
    file.close();

    //Removing the todo from todo.txt
    std::ofstream temp;
    file.open("todo.txt");
    temp.open("temp.txt");
    while (getline(file, line)) {
        if (line != line_to_erase)
            temp << line << std::endl;
    }
    temp.close();
    file.close();

    std::string filename = "todo.txt";
    const char * p = filename.c_str();
    remove(p);
    rename("temp.txt", p);

    std::cout << "Deleted todo #" << tasknum << std::endl;

}

void done(std::string s){

    int tasknum = std::stoi(s); //Task number to be marked as done

    std::string line;
    std::ifstream file;

    //Fetching the number of todos pending
    int todo_count = 0;
    file.open("todo.txt");
    while(getline(file, line))
        todo_count++;
    file.close();

    //Handling wrong todo list number
    if (tasknum > todo_count || tasknum <= 0) {
        std::cout << "Error: todo #" << tasknum << " does not exist.";
        return;
    }

    //Storing todo to be marked as done in "line_to_erase"
    std::string line_to_erase;
    file.open("todo.txt");
    for (int i = 1; i <= tasknum; i++)
        getline(file, line_to_erase);
    file.close();

    //Removing the completed todo from todo.txt
    std::ofstream temp;
    file.open("todo.txt");
    temp.open("temp.txt");
    while (getline(file, line)) {
        if (line != line_to_erase)
            temp << line << std::endl;
    }
    temp.close();
    file.close();

    std::string filename = "todo.txt";
    const char * p = filename.c_str();
    remove(p);
    rename("temp.txt", p);

    //Adding completed todo to done.txt
    std::ofstream fout;
    fout.open("done.txt", std::ios_base::app); //seeking to last line
    fout <<  line_to_erase << "\n";
    std::cout << "Marked todo #" << tasknum << " as done" << std::endl; //confirmation

}

void help(){

    std::cout   << "Usage :-" << std::endl
                << "./todo add \"todo item\" \t # Add a new todo" << std::endl
                << "./todo ls \t \t # Show remaining todos" << std::endl
                << "./todo del NUMBER \t # Delete a todo" << std::endl
                << "./todo done NUMBER \t # Complete a todo" << std::endl
                << "./todo help \t \t # Show usage" << std::endl
                << "./todo report \t \t # Statistics" << std::endl;

}

void report(){
    
    std::ifstream file;
    std::string num;

    //Counting number of pending todos
    int todo_count = 0;
    file.open("todo.txt");
    while(getline(file, num))
        todo_count++;
    file.close();

    //Counting number of completed todos
    int done_count = 0;
    file.open("done.txt");
    while(getline(file, num))
        done_count++;
    file.close();
    
    //Date
    std::time_t rawtime;
    std::tm* timeinfo;
    char buffer [80];

    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);

    std::strftime(buffer,80,"%Y-%m-%d",timeinfo);
    std::cout << (buffer) << " ";


    std::cout << "Pending : " << todo_count << " Completed : " << done_count << std::endl;

}

void invalid(){

    std::cout   << "INVALID ARGUMENT(S)" << std::endl
                << "Use \"./todo help\" for list of available commands" << std::endl;

}

int commandValue(std::string s) {

    if (s == "add") 
        return 1;
    else if (s == "ls") 
        return 2;
    else if (s == "del") 
        return 3;
    else if (s == "done") 
        return 4;
    else if (s == "help") 
        return 5;
    else if (s == "report") 
        return 6;
    else
        return 0;
    
}

int main(int argc, char* argv[]) {

    if (argc > 3) {
        invalid();
        return 0;
    }
    if (argc == 1) {
        help();
        return 0;
    }
    
    switch (commandValue(argv[1])) {
        case 1:
            if (argc < 3) 
                std::cout << "Error: Missing todo string. Nothing added!";
            else 
                add(argv[2]);
            break;
        case 2:
            ls();
            break;
        case 3:
            if (argc < 3)
                std::cout << "Error: Missing NUMBER for deleting todo.";
            else
                del(argv[2]);
            break;
        case 4:
            if (argc < 3)
                std::cout << "Error: Missing NUMBER for marking todo as done.";
            else
                done(argv[2]);
            break;
        case 5:
            help();
            break;
        case 6:
            report();
            break;
        case 0:
            invalid();
            break;
        default:
            break;
    }

}
