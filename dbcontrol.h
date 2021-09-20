//
// Created by Daniil Kolesnik on 14/09/2021.
//

#ifndef MY_DB_DBCONTROL_H
#define MY_DB_DBCONTROL_H


#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>
#include "Database.h"


class DBcontrol
        {
        public:
            DBcontrol()
            {
                _commands.emplace_back("help", "Print command list", "", &DBcontrol::printHelp);
                _commands.emplace_back("create", "Create new database", "[name]", &DBcontrol::createDB);
                _commands.emplace_back("printdb", "Print database list", "", &DBcontrol::printDBs);
                _commands.emplace_back("load", "Save current database", "[name]", &DBcontrol::loadDB);
                _commands.emplace_back("save", "Save current database", "", &DBcontrol::saveDB);
                _commands.emplace_back("deletedb", "Delete database", "[name]", &DBcontrol::deleteDB);
                _commands.emplace_back("rename", "Rename database", "[oldname] [newname]", &DBcontrol::renameDB);
                _commands.emplace_back("dump", "Print all records from current DB", "", &DBcontrol::dumpRecords);
                _commands.emplace_back("delete", "Delete faculty record from current DB", "[faculty_name]", &DBcontrol::deleteRecord);
                _commands.emplace_back("sort", "Sort records by faculty name (f) or by number of departments (d)", "[f | d]", &DBcontrol::sortRecords);
                _commands.emplace_back("add", "Add record to current DB", "", &DBcontrol::addRecord);
                _commands.emplace_back("select", "Select records from current db using NUK (nuk) or discipline (disc)", "[nuk | disc] [name]", &DBcontrol::selectRecords);
                _commands.emplace_back("savesel", "Save selection as new DB", "[db_name]", &DBcontrol::saveSelection);
                _commands.emplace_back("edit", "Edit record from current db", "[faculty_name]", &DBcontrol::editRecord);
                _commands.emplace_back("calcdisc", "Calculate total number of disciplines from faculty", "[faculty_name]", &DBcontrol::calculateDisciplines);
                _commands.emplace_back("find", "Search faculty by name", "[faculty_name]", &DBcontrol::findFaculty);
            }

            void printHelp(std::string params = "")
            {
                std::ignore = params;
                for(auto& command: _commands)
                {
                    std::cout << command.command << " " << command.params << "\t - " << command.help << std::endl;
                }
            }

            // 5.1
            void createDB(std::string params)
            {
                auto name = extractParam(params);
                if (name.empty())
                {
                    std::cout << "error: enter proper name" << std::endl;
                }
                std::__fs::filesystem::path path = std::__fs::filesystem::current_path() / "dbs";

                if (std::__fs::filesystem::exists(path))
                {
                    std::ofstream f(path.string() + "/" + name + ".txt");
                    if (f)
                    {
                        std::cout << "db " << name << " created" << std::endl;
                    }
                    else
                    {
                        std::cout << "error: dbs not created" << std::endl;
                    }
                }
                else
                {
                    std::cout << "error: dbs folder does not exist in " << std::__fs::filesystem::current_path() << std::endl;;
                }
            }

            // 5.2
            void printDBs(std::string params)
            {
                std::ignore = params;
                std::__fs::filesystem::path path = std::__fs::filesystem::current_path() / "dbs";
                if (std::__fs::filesystem::exists(path))
                {
                    bool found = false;
                    for (const auto& entry : std::__fs::filesystem::directory_iterator(path))
                    {
                        if (entry.is_regular_file() && entry.path().extension() == ".txt")
                        {
                            std::cout << entry.path().filename() << std::endl;
                            found = true;
                        }
                    }
                    if (!found)
                    {
                        std::cout << "<empty>" << std::endl;
                    }
                }
                else
                {
                    std::cout << "error: dbs folder does not exist in " << std::__fs::filesystem::current_path() << std::endl;;
                }
            }

            // 5.3
            void deleteDB(std::string params)
            {
                auto name = extractParam(params);
                if (name.empty())
                {
                    std::cout << "error: enter proper name" << std::endl;
                }
                else
                {
                    std::__fs::filesystem::path path = std::__fs::filesystem::current_path() / "dbs" / (name + ".txt");
                    if (std::__fs::filesystem::exists(path))
                    {
                        try
                        {
                            if (std::__fs::filesystem::remove(path))
                            {
                                std::cout << "db " << name << " successfully deleted" << std::endl;
                            }
                            else
                            {
                                std::cout << "error: delete db failed" << std::endl;
                            }
                        }
                        catch (const std::__fs::filesystem::filesystem_error&)
                        {
                            std::cout << "error: delete db failed" << std::endl;
                        }
                    }
                    else
                    {
                        std::cout << "error: db does not exist" << std::endl;
                    }
                }
            }

            // 5.4
            void renameDB(std::string params)
            {
                auto nameold = extractParam(params);
                auto namenew = extractParam(params);
                if (nameold.empty() || namenew.empty() || nameold == namenew)
                {
                    std::cout << "error: enter proper names" << std::endl;
                }
                else
                {
                    std::__fs::filesystem::path path = std::__fs::filesystem::current_path() / "dbs" / (nameold + ".txt");
                    std::__fs::filesystem::path pathnew = std::__fs::filesystem::current_path() / "dbs" / (namenew + ".txt");
                    if (std::__fs::filesystem::exists(path))
                    {
                        try
                        {
                            std::__fs::filesystem::rename(path, pathnew);
                        }
                        catch (const std::__fs::filesystem::filesystem_error&)
                        {
                            std::cout << "error: delete db failed" << std::endl;
                        }
                    }
                    else
                    {
                        std::cout << "error: db does not exist" << std::endl;
                    }
                }
            }

            // 5.5
            void loadDB(std::string params)
            {
                if (currentDB)
                {
                    delete currentDB;
                }
                auto name = extractParam(params);
                if (name.empty())
                {
                    std::cout << "error: enter proper name" << std::endl;
                }
                else
                {
                    currentDB = new Database(name);
                    currentDB->load();
                }
                _currentSelection.clear();
            }

            // 5.6
            void saveDB(std::string params)
            {
                std::ignore = params;
                if (currentDB)
                {
                    currentDB->save();
                }
                else
                {
                    std::cout << "error: no db loaded" << std::endl;
                }
            }

            // 5.7
            void addRecord(std::string params)
            {
                std::ignore = params;
                if (currentDB)
                {
                    std::cout << "create record > Press '" << BaseFaculty::id() << "' to create base faculty or '" << BranchFaculty::id() << "' to create branch faculty" << std::endl;
                    char c;
                    std::cin >> c;
                    std::cin.get(); // '\n'
                    auto* f = Faculty::create(c);
                    if (!f)
                    {
                        std::cout << "error: invalid choice" << std::endl;
                    }
                    else
                    {
                        if (f->interactiveCreate())
                        {
                            while (currentDB->getRecord(f->getName()))
                            {
                                std::cout << "faculty with name " << f->getName() << "already exists in db. Press 1 to replace or press 2 to edit name" << std::endl;
                                int i;
                                std::cin >> i;
                                if (i == 1)
                                {
                                    currentDB->deleteRecord(f->getName());
                                    break;
                                }
                                else if (i == 2)
                                {
                                    f->interactiveEdit();
                                }
                            }
                            if (!currentDB->add(f))
                            {
                                std::cout << "error: adding record to db failed" << std::endl;
                            }
                        }
                        else
                        {
                            delete f;
                        }
                    }
                }
                else
                {
                    std::cout << "error: no db loaded" << std::endl;
                }
            }

            // 5.8
            void deleteRecord(std::string params)
            {
                if (currentDB)
                {
                    auto name = extractParam(params);
                    if (name.empty())
                    {
                        std::cout << "error: enter proper name" << std::endl;
                    }
                    else
                    {
                        currentDB->deleteRecord(name);
                    }
                }
                else
                {
                    std::cout << "error: no db loaded" << std::endl;
                }
            }

            // 5.9
            void dumpRecords(std::string params)
            {
                std::ignore = params;
                if (currentDB)
                {
                    currentDB->printAll();
                }
                else
                {
                    std::cout << "error: no db loaded" << std::endl;
                }
            }

            // 5.10
            void sortRecords(std::string params)
            {
                if (currentDB)
                {
                    auto param = extractParam(params);
                    if(param == "f")
                    {
                        currentDB->sort([](const Faculty* f1, const Faculty* f2) {return f1->getName() < f2->getName(); });
                    }
                    else if (param == "d")
                    {
                        currentDB->sort([](const Faculty* f1, const Faculty* f2) {return f1->getDeptCount() > f2->getDeptCount(); });
                    }
                    else
                    {
                        std::cout << "error: enter proper param" << std::endl;
                    }
                }
            }

            // 5.11
            void selectRecords(std::string params)
            {
                if (currentDB)
                {
                    auto type = extractParam(params);
                    auto name = extractParam(params);
                    _currentSelection.clear();
                    if (type == "nuk" && !name.empty())
                    {
                        _currentSelection = currentDB->select([name](const Faculty* f1) {return f1->getNuk() == name; });
                    }
                    else if(type == "disc" && !name.empty())
                    {
                        _currentSelection = currentDB->select([name](const Faculty* f1) {auto a = f1->getDisciplines(); return a.find(name) != a.end(); });
                    }
                    else
                    {
                        std::cout << "error: enter proper params" << std::endl;
                        return;
                    }
                    if (_currentSelection.empty())
                    {
                        std::cout << "<empty>" << std::endl;
                    }
                    else
                    {
                        for (auto* a : _currentSelection)
                        {
                            std::cout << "{" << std::endl;
                            a->print("\t");
                            std::cout << "}" << std::endl;
                        }
                    }
                }
                else
                {
                    std::cout << "error: no db loaded" << std::endl;
                }
            }

            // 5.12
            void saveSelection(std::string params)
            {
                if (currentDB)
                {
                    auto name = extractParam(params);
                    if (name.empty())
                    {
                        std::cout << "error: enter proper name" << std::endl;
                    }
                    else
                    {
                        auto db = new Database(name);
                        for (auto* i : _currentSelection)
                        {
                            db->add(i);
                        }
                        db->save();
                        delete db;
                    }
                }
                else
                {
                    std::cout << "error: no db loaded" << std::endl;
                }
            }

            // 5.13
            void editRecord(std::string params)
            {
                if (currentDB)
                {
                    auto name = extractParam(params);
                    if (name.empty())
                    {
                        std::cout << "error: enter proper name" << std::endl;
                    }
                    else
                    {
                        auto* f = currentDB->getRecord(name);
                        if (f)
                        {
                            f->interactiveEdit();
                        }
                        else
                        {
                            std::cout << "faculty " << name << " not found" << std::endl;
                        }
                    }
                }
                else
                {
                    std::cout << "error: no db loaded" << std::endl;
                }
            }

            // functions
            void calculateDisciplines(std::string params)
            {
                if (currentDB)
                {
                    auto name = extractParam(params);
                    if (name.empty())
                    {
                        std::cout << "error: enter proper name" << std::endl;
                    }
                    else
                    {
                        auto* f = currentDB->getRecord(name);
                        if (f)
                        {
                            std::cout << "Faculty " << name << " has " << f->getDisciplines().size() << " unique disciplines" << std::endl;
                        }
                        else
                        {
                            std::cout << "faculty " << name << " not found" << std::endl;
                        }
                    }
                }
                else
                {
                    std::cout << "error: no db loaded" << std::endl;
                }
            }
            void findFaculty(std::string params)
            {
                if (currentDB)
                {
                    auto name = extractParam(params);
                    if (name.empty())
                    {
                        std::cout << "error: enter proper name" << std::endl;
                    }
                    else
                    {
                        auto* f= currentDB->getRecord(name);
                        if (f)
                        {
                            std::cout << "faculty " << name << " found:" << std::endl;
                            f->print("");
                        }
                        else
                        {
                            std::cout << "faculty " << name << " not found" << std::endl;
                        }
                    }
                }
                else
                {
                    std::cout << "error: no db loaded" << std::endl;
                }
            }

            // command line processing
            void execute(std::string commandLine)
            {
                auto command = extractParam(commandLine);
                for (auto& c : _commands)
                {
                    if (c.command == command)
                    {
                        (this->*c.func)(commandLine);
                        return;
                    }
                }
                std::cout << "error: unknown command" << std::endl;
            }

            std::string extractParam(std::string& params)
            {
                auto pos = params.find_first_of(' ');
                auto command = params.substr(0, pos);
                if (pos != std::string::npos)
                {
                    params = params.substr(pos + 1);
                }
                else
                {
                    params.clear();
                }
                return command;
            }
        private:
            struct Command
                    {
                using Func = void (DBcontrol::*) (std::string params);
                Command(const std::string& command, const std::string& help, const std::string& params, Func func)
                : command(command),help(help), params(params), func(func)
                {}
                std::string command;
                std::string help;
                std::string params;
                Func func;
                    };
            std::vector<Command> _commands;
            Database* currentDB = nullptr;
            std::vector<Faculty*> _currentSelection;
        };



#endif //MY_DB_DBCONTROL_H
