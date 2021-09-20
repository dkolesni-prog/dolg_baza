//
// Created by Daniil Kolesnik on 14/09/2021.
//

#ifndef MY_DB_DATABASE_H
#define MY_DB_DATABASE_H


#include "faculty.h"
#include <algorithm>

class Database
        {
        public:
            Database(const std::string name)
            : _name(name)
            {
            }

            ~Database()
            {
                _clear();
            }

            void load();
            void save();

            void printAll()
            {
                std::cout << "DB: " << _name << std::endl;
                std::cout << "records :" << std::endl;
                if (_records.empty())
                {
                    std::cout << "\t<empty>" << std::endl;
                }
                else
                {
                    for (auto& r : _records)
                    {
                        if (r)
                        {
                            std::cout << "{" << std::endl;
                            r->print("\t");
                            std::cout << "}" << std::endl;
                        }
                    }
                }
            }

            void deleteRecord(const std::string& name)
            {
                for (auto it = _records.begin(); it!=_records.end(); ++it)
                {
                    if (*it && (*it)->getName() == name)
                    {
                        delete *it;
                        _records.erase(it);
                        break;
                    }
                }
            }

            bool add(Faculty* faculty)
            {
                if (faculty && !getRecord(faculty->getName()))
                {
                    _records.push_back(faculty);
                    return true;
                }
                return false;
            }

            bool isLoaded()
            {
                return _loaded;
            }

            Faculty* getRecord(const std::string& faculty)
            {
                for (auto* i : _records)
                {
                    if (i->getName() == faculty)
                    {
                        return i;
                    }
                }
                return nullptr;
            }

            template<typename T>
            void sort(T t)
            {
                std::sort(_records.begin(), _records.end(), t);
            }

            template<typename T>
            std::vector<Faculty*> select(T t)
            {
                std::vector<Faculty*> ret;
                for(auto* i: _records)
                {
                    if (t(i))
                    {
                        ret.push_back(i);
                    }
                }
                return ret;
            }
        private:
            void _clear()
            {
                for (auto& i : _records)
                {
                    if (i)
                    {
                        delete i;
                    }
                }
            }
            std::string _name;
            std::vector<Faculty*> _records;
            bool _loaded;
        };



#endif //MY_DB_DATABASE_H
