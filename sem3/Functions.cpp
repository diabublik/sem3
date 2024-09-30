#include <iostream>
#include <fstream>
#include <sstream>
#include "Header.h"

static void PrintFile(std::fstream& file) {
	file.clear();
	file.seekg(0);
	std::cout << "================ Contents of the file: =============" << std::endl;
	std::string fileline;
	while (std::getline(file, fileline)) {
		std::cout << fileline << std::endl;
	}
	std::cout << "====================================================" << std::endl;
}
// function 1
static std::fstream CreateFile(std::string filename) {
	std::fstream file(filename, std::ios::in | std::ios::out | std::ios::app);
	if (!file) {
		std::cerr << "File opening error :(" << std::endl;
		exit(1);
	}
	return file;
}

void Console() {
	std::string filename;
	std::cout << "Enter the name of your file: ";
	std::getline(std::cin, filename);
	std::fstream file(filename, std::ios::out | std::ios::in | std::ios::app);
	if (!file) {
		std::cerr << "File opening error :(" << std::endl;
		exit(1);
	}
	PrintFile(file);

	unsigned choice = -1;
	while (choice != 0) {
		std::cout << "Select the action:" << std::endl;
		std::cout << "1 - Open/create a file" << std::endl;
		std::cout << "2 - Add the new task" << std::endl;
		std::cout << "3 - Delete the task" << std::endl;
		std::cout << "4 - Find the task" << std::endl;
		std::cout << "5 - Sort the task list" << std::endl;
		std::cout << "6 - Delete the task file" << std::endl;
		std::cout << "7 - Change the task" << std::endl;
		std::cout << "0 - Exit" << std::endl;
		std::cout << "====================================================" << std::endl;
		std::cout << "Your choice: ";
		std::cin >> choice;
		std::cout << "====================================================" << std::endl;

		std::string phrase;
		std::string tag;
		unsigned priority;
		std::string data;
		FunTask taskvector;

		taskvector.LoadFromFile(file);

		switch (choice) {
		case 1:
			std::cout << "Enter the name of your file: ";
			std::cin.ignore();
			std::getline(std::cin, filename);
			file = CreateFile(filename);
			PrintFile(file);
			break;
		case 2:
			std::cout << "Enter priority of new task: ";
			std::cin >> priority;
			std::cout << "Enter the phrase of new task: ";
			std::cin.ignore();
			std::getline(std::cin, phrase);
			std::cout << "Enter the tag of new task: ";
			std::getline(std::cin, tag);
			std::cout << "Enter the data of completion of new task (DD.MM.YY): ";
			std::getline(std::cin, data);

			taskvector.AddTask(Task(phrase, tag, priority, data));
			taskvector.WriteToFile(Task(phrase, tag, priority, data), file);

			PrintFile(file);
			break;
		case 3:
			taskvector.DeleteTask();
			file = taskvector.WriteAllToFile(file, filename);
			PrintFile(file);
			break;
		case 4:
			taskvector.FindTask();
			PrintFile(file);
			break;
		case 5:
			taskvector.SortList();
			file = taskvector.WriteAllToFile(file, filename);
			PrintFile(file);
			break;
		case 6:
			file.close();
			if (std::remove(filename.c_str()) != 0) {
				std::cerr << "Error deleting file :(" << std::endl;
				exit(1);
			}
			else std::cout << "The file was deleted :)" << std::endl;
			std::cout << "====================================================" << std::endl;
			break;
		case 7:
			taskvector.ChangeTask();
			file = taskvector.WriteAllToFile(file, filename);
			PrintFile(file);
			break;
		case 0:
			break;
		default: std::cout << "Wrong command :(" << std::endl;
		}
	}
}