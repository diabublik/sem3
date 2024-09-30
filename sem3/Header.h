#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class Task {
private:
	std::string phrase;
	std::string tag;
	unsigned priority = 0;
	std::string data;
public:
	Task(std::string tphrase, std::string ttag, unsigned tpriority, std::string tdata) :
		phrase(tphrase), tag(ttag), priority(tpriority), data(tdata) {}

	std::string Getphrase() const { return phrase; }
	std::string GetTag() const { return tag; }
	unsigned GetPriority() const { return priority; }
	std::string GetData() const { return data; }

	void SetPhrase(const std::string newphrase) { phrase = newphrase; }
	void SetTag(const std::string newtag) { tag = newtag; }
	void SetPriority(const unsigned newpriority) { priority = newpriority; }
	void SetData(const std::string newdata) { data = newdata; }

	friend std::ostream& operator<<(std::ostream& os, const Task& task) {
		os << task.priority << " | " << task.phrase << " | #" << task.tag << " | " << task.data << std::endl;
		return os;
	}

	bool operator==(const Task& task) const {
		return phrase == task.phrase;
	}
};

class FunTask {
private:
	std::vector <Task> tasks;
public:
	void AddTask(const Task& task) {
		tasks.push_back(task);
	}

	void WriteToFile(const Task& task, std::fstream& file) {
		file.clear();
		file << task.GetPriority() << " | " << task.Getphrase() << " | #" <<
			task.GetTag() << " | " << task.GetData() << std::endl;
	}

	std::fstream WriteAllToFile(std::fstream& file, std::string filename) {
		std::fstream file2("temp.txt", std::ios::in | std::ios::out | std::ios::app);
		for (auto& task : tasks) {
			file2 << task.GetPriority() << " | " << task.Getphrase() << " | #" <<
				task.GetTag() << " | " << task.GetData() << std::endl;
		}

		file.close();
		if (std::remove(filename.c_str()) != 0) {
			std::cerr << "Error deleting file: " << filename << std::endl;
			exit(1);
		}
		file2.close();
		if (std::rename("temp.txt", filename.c_str()) != 0) {
			perror("Error renaming file");
			exit(1);
		}

		file2.open(filename, std::ios::out | std::ios::in | std::ios::app);
		return file2;
	}

	void FindTask() {
		std::cout << "1) Priority | 2) phrase | 3) Tag | 4) Data of completion" << std::endl;
		std::cout << "Select the known information (enter the digit): ";
		unsigned choice;
		std::cin >> choice;

		bool flag = 0;
		unsigned priority;
		std::string phrase;
		std::string tag;
		std::string data;

		std::cout << "====================================================" << std::endl;
		switch (choice) {
		case 1:
			std::cout << "Enter the priority of necessary task: ";
			std::cin >> priority;
			for (const auto& iter : tasks) {
				if (iter.GetPriority() == priority) {
					std::cout << "Necessary task:" << std::endl;
					std::cout << iter;
					flag = 1;
				}
			}
			if (flag == 0) std::cout << "There isn`t task with such information :(" << std::endl;
			break;
		case 2:
			std::cout << "Enter the phrase of necessary task: ";
			std::cin.ignore();
			std::getline(std::cin, phrase);
			for (const auto& iter : tasks) {
				if (iter.Getphrase() == phrase) {
					std::cout << "Necessary task:" << std::endl;
					std::cout << iter;
					flag = 1;
				}
			}
			if (flag == 0) std::cout << "There isn`t task with such information :(" << std::endl;
			break;
		case 3:
			std::cout << "Enter the tag of necessary task: ";
			std::cin.ignore();
			std::getline(std::cin, tag);
			for (const auto& iter : tasks) {
				if (iter.GetTag() == tag) {
					std::cout << "Necessary task:" << std::endl;
					std::cout << iter;
					flag = 1;
				}
			}
			if (flag == 0) std::cout << "There isn`t task with such information :(" << std::endl;
			break;
		case 4:
			std::cout << "Enter the data of completion of necessary task: ";
			std::cin.ignore();
			std::getline(std::cin, data);
			for (const auto& iter : tasks) {
				if (iter.GetData() == data) {
					std::cout << "Necessary task:" << std::endl;
					std::cout << iter;
					flag = 1;
				}
			}
			if (flag == 0) std::cout << "There isn`t task with such information :(" << std::endl;
			break;
		default: std::cout << "Wrong command :(" << std::endl; break;
		}
	}

	void LoadFromFile(std::fstream& file) {
		file.clear();
		file.seekg(0, std::ios::beg);

		std::string fileline;
		while (std::getline(file, fileline)) {
			std::vector<std::string> tokens;
			size_t pos = 0;
			while ((pos = fileline.find('|')) != std::string::npos) {
				std::string token = fileline.substr(0, pos);
				token.erase(0, token.find_first_not_of(' '));
				token.erase(token.find_last_not_of(' ') + 1);
				tokens.push_back(token);
				fileline.erase(0, pos + 1);
			}

			fileline.erase(0, fileline.find_first_not_of(' '));
			fileline.erase(fileline.find_last_not_of(' ') + 1);
			tokens.push_back(fileline);

			if (tokens.size() == 4) {
				AddTask(Task(tokens[1], tokens[2].substr(1, tokens[2].length()), std::stoi(tokens[0]), tokens[3]));
			}
			else {
				std::cerr << "Loading info from file error :(" << std::endl;
				exit(1);
			}
		}
	}

	void DeleteTask() {
		std::cout << "Enter the phrase of necessary task(s): ";
		std::string delline;
		std::cin.ignore();
		std::getline(std::cin, delline);

		// Удаляем все задачи с нужной фразой
		tasks.erase(std::remove_if(tasks.begin(), tasks.end(),
			[&delline](const Task& task) {
				return task.Getphrase() == delline;
			}), tasks.end());
	}

	void SortList() {
		for (int i = 0; i < tasks.size(); i++) {
			for (int j = 0; j < tasks.size() - i - 1; j++) {
				if (tasks[j].GetPriority() > tasks[j + 1].GetPriority()) {
					std::swap(tasks[j], tasks[j + 1]);
				}
			}
		}
		std::cout << "The list is sorted :)" << std::endl;
	}

	void ChangeTask() {
		std::cout << "Enter the phrase of necessary task: ";
		std::string phrase;
		std::cin.ignore();
		std::getline(std::cin, phrase);

		int ind;
		for (int i = 0; i < tasks.size(); i++) {
			if (tasks[i].Getphrase() == phrase) ind = i;
		}

		std::cout << "1) Priority | 2) phrase | 3) Tag | 4) Data of completion" << std::endl;
		std::cout << "Select what you want to change in this task (enter the digit): ";
		unsigned choice;
		std::cin >> choice;

		unsigned newpriority;
		std::string newphrase;
		std::string newtag;
		std::string newdata;

		switch (choice) {
		case 1:
			std::cout << "Enter a new priority for this task: ";
			std::cin >> newpriority;
			for (auto& task : tasks) {
				if (task == tasks[ind]) {
					tasks[ind].SetPriority(newpriority);
				}
			}
			break;
		case 2:
			std::cout << "Enter a new phrase for this task: ";
			std::cin.ignore();
			std::getline(std::cin, newphrase);
			for (auto& task : tasks) {
				if (task == tasks[ind]) {
					tasks[ind].SetPhrase(newphrase);
				}
			}
			break;
		case 3:
			std::cout << "Enter a new tag for this task: ";
			std::cin.ignore();
			std::getline(std::cin, newtag);
			for (auto& task : tasks) {
				if (task == tasks[ind]) {
					tasks[ind].SetTag(newtag);
				}
			}
			break;
		case 4:
			std::cout << "Enter a new data for this task: ";
			std::cin.ignore();
			std::getline(std::cin, newdata);
			for (auto& task : tasks) {
				if (task == tasks[ind]) {
					tasks[ind].SetData(newdata);
				}
			}
			break;
		default: std::cout << "Wrong command :(" << std::endl;
		}
	}
};

void Console();