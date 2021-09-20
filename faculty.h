//
// Created by Daniil Kolesnik on 14/09/2021.
//

#ifndef MY_DB_FACULTY_H
#define MY_DB_FACULTY_H


#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <map>
#include <set>

class Faculty
        {
        public:
            virtual ~Faculty() {}
            virtual void print(const std::string& indent) const = 0;
            virtual void serialize(std::ostream& stream) const = 0;
            virtual void deserialize(std::istream& stream) = 0;
            virtual std::string getName() const = 0;
            virtual std::string getNuk() const = 0;
            virtual int getDeptCount() const = 0;
            virtual char getId() const = 0;
            virtual std::set<std::string> getDisciplines() const = 0;
            virtual bool interactiveCreate() = 0;
            virtual bool interactiveEdit() = 0;
        public:
            static Faculty* create(int c);
        };

std::string _getStr(std::istream& stream, char delimiter = '\n');

class BaseFaculty : public Faculty
        {
        public:
            virtual void print(const std::string& indent) const override
            {
                std::cout << indent << "Name = " << _name << std::endl;
                std::cout << indent << "NUK = " << _NUK << std::endl;
                std::cout << indent << "Departments = " << std::endl;
                std::cout << indent << "{" << std::endl;
                bool _1st = true;
                for (auto& dept : _departments)
                {
                    if (!_1st)
                    {
                        std::cout << "," << std::endl;
                    }
                    _1st = false;
                    std::cout << indent << "\tname = " << dept._name << std::endl;
                    std::cout << indent << "\tDisciplines = " << std::endl;
                    std::cout << indent << "\t{" << std::endl;
                    bool _1st = true;
                    for (auto& disc : dept.disciplines)
                    {
                        if (!_1st)
                        {
                            std::cout << "," << std::endl;
                        }
                        _1st = false;
                        std::cout << indent << "\t\t" << disc.first << " : " << disc.second;
                    }
                    std::cout << std::endl << indent << "\t}";
                }
                std::cout << std::endl << indent << "}" << std::endl;
            }
            virtual void serialize(std::ostream& stream) const override
            {
                stream << _name << ',' << _NUK << ',';
                for (auto& dept : _departments)
                {
                    stream << ",";
                    stream << "{";
                    stream << dept._name << ',';
                    for (auto& disc : dept.disciplines)
                    {
                        stream << ",";
                        stream << "{";
                        stream << disc.first << "," << disc.second;
                        stream << "}";
                    }
                    stream << ';';
                    stream << "}";
                }
                stream << ';';
                stream << std::endl;
            }
            virtual void deserialize(std::istream& stream) override
            {
                _name = _getStr(stream, ',');
                _NUK = _getStr(stream, ',');

                while (true)
                {
                    auto c = stream.get(); // , or ;
                    if (c == ';')
                    {
                        break;
                    }
                    else
                    {
                        stream.get(); // {
                        Department dept;
                        dept._name = _getStr(stream, ',');
                        while (true)
                        {
                            auto c = stream.get(); // , or ;
                            if (c == ';')
                            {
                                break;
                            }
                            else
                            {
                                stream.get(); // {
                                auto discipline = _getStr(stream, ',');
                                auto teachers = atoi(_getStr(stream, '}').c_str());
                                dept.disciplines[discipline] = teachers;
                            }
                        }
                        stream.get(); // }
                        _departments.push_back(dept);
                    }
                }
            }
            virtual std::string getName() const override
            {
                return _name;
            }
            virtual std::string getNuk() const override
            {
                return _NUK;
            }
            virtual char getId() const override
            {
                return id();
            }
            virtual std::set<std::string> getDisciplines() const override
            {
                std::set<std::string> ret;
                for (auto& i : _departments)
                {
                    for(auto& j : i.disciplines)
                    {
                        ret.emplace(j.first);
                    }
                }
                return ret;
            }
            virtual int getDeptCount() const override
            {
                return _departments.size();
            }
            virtual bool interactiveCreate() override
            {
                std::cout << "create record > Enter faculty name:" << std::endl;
                _name = _getStr(std::cin);

                std::cout << "create record > Enter NUK:" << std::endl;
                _NUK = _getStr(std::cin);

                while (true)
                {
                    std::cout << "create record > Do you want to create department [y/n]?" << std::endl;
                    char c;
                    std::cin >> c;
                    std::cin.get(); // '\n'
                    if (c == 'y')
                    {
                        std::cout << "create record > create department > Enter department name:" << std::endl;
                        Department dept;
                        dept._name = _getStr(std::cin);
                        while (true)
                        {
                            std::cout << "create record > create department > Do you want to create discipline [y/n]?" << std::endl;
                            char c;
                            std::cin >> c;
                            std::cin.get(); // '\n'
                            if (c == 'y')
                            {
                                std::cout << "create record > create department > create discipline > Enter name:" << std::endl;
                                auto discipline = _getStr(std::cin);
                                std::cout << "create record > create department > create discipline > Enter number of teachers : " << std::endl;
                                auto teachers = atoi(_getStr(std::cin).c_str());
                                dept.disciplines[discipline] = teachers;
                            }
                            else if (c == 'n')
                            {
                                break;
                            }
                            else
                            {
                                std::cout << "create record > create department > invalid choice" << std::endl;
                            }
                        }
                        _departments.push_back(dept);
                    }
                    else if (c == 'n')
                    {
                        break;
                    }
                    else
                    {
                        std::cout << "create record > invalid choice" << std::endl;
                    }
                }
                return true;
            }
            virtual bool interactiveEdit() override
            {
                std::cout << "edit record > Current faculty name = "<<_name<<". Do you want to change it [y/n]?" << std::endl;
                char c;
                std::cin >> c;
                std::cin.get(); // '\n'
                if (c == 'y')
                {
                    std::cout << "edit record > Enter faculty name:" << std::endl;
                    _name = _getStr(std::cin);
                }

                std::cout << "edit record > Current NUK = " << _NUK << ". Do you want to change it [y/n]?" << std::endl;
                std::cin >> c;
                std::cin.get(); // '\n'
                if (c == 'y')
                {
                    std::cout << "edit record > Enter NUK:" << std::endl;
                    _NUK = _getStr(std::cin);
                }

                for (auto it = _departments.begin(); it != _departments.end();)
                {
                    std::cout << "edit record > Do you want to delete department " << it->_name << "[y/n]?" << std::endl;
                    std::cin >> c;
                    std::cin.get(); // '\n'
                    if (c == 'y')
                    {
                        it = _departments.erase(it);
                        continue;
                    }
                    std::cout << "edit record > Do you want to edit department " << it->_name << "[y/n]?" << std::endl;
                    std::cin >> c;
                    std::cin.get(); // '\n'
                    if (c == 'y')
                    {
                        std::cout << "edit record > edit department > Current department name = " << it->_name << ". Do you want to change it [y/n]?" << std::endl;
                        std::cin >> c;
                        std::cin.get(); // '\n'
                        if (c == 'y')
                        {
                            std::cout << "edit > edit department record > Enter department name:" << std::endl;
                            it->_name = _getStr(std::cin);
                        }

                        for (auto it2 = it->disciplines.begin(); it2 != it->disciplines.end();)
                        {
                            std::cout << "edit record > edit department > Do you want to delete discipline " << it2->first << "[y/n]?" << std::endl;
                            std::cin >> c;
                            std::cin.get(); // '\n'
                            if (c == 'y')
                            {
                                it2 = it->disciplines.erase(it2);
                            }
                            std::cout << "edit record > edit department > edit discipline > Current number of teachers = " << it2->second << ". Do you want to change it [y/n]?" << std::endl;
                            std::cin >> c;
                            std::cin.get(); // '\n'
                            if (c == 'y')
                            {
                                std::cout << "edit > edit department record > Enter number of teachers:" << std::endl;
                                it2->second = atoi(_getStr(std::cin).c_str());
                            }
                        }

                        std::cout << "edit record > edit department > Do you want to add discipline " << it->_name << "[y/n]?" << std::endl;
                        std::cin >> c;
                        std::cin.get(); // '\n'
                        if (c == 'y')
                        {
                            while (true)
                            {
                                std::cout << "edit record > create department > Do you want to create discipline [y/n]?" << std::endl;
                                char c;
                                std::cin >> c;
                                std::cin.get(); // '\n'
                                if (c == 'y')
                                {
                                    std::cout << "edit record > create department > create discipline > Enter name:" << std::endl;
                                    auto discipline = _getStr(std::cin);
                                    std::cout << "edit record > create department > create discipline > Enter number of teachers : " << std::endl;
                                    auto teachers = atoi(_getStr(std::cin).c_str());
                                    it->disciplines[discipline] = teachers;
                                }
                                else if (c == 'n')
                                {
                                    break;
                                }
                                else
                                {
                                    std::cout << "edit record > create department > invalid choice" << std::endl;
                                }
                            }
                        }
                        else if (c == 'n')
                        {
                            break;
                        }
                        else
                        {
                            std::cout << "edit record > invalid choice" << std::endl;
                        }
                    }
                    ++it;
                }


                while (true)
                {
                    std::cout << "edit record > Do you want to create department [y/n]?" << std::endl;
                    char c;
                    std::cin >> c;
                    std::cin.get(); // '\n'
                    if (c == 'y')
                    {
                        std::cout << "edit record > create department > Enter department name:" << std::endl;
                        Department dept;
                        dept._name = _getStr(std::cin);
                        while (true)
                        {
                            std::cout << "edit record > create department > Do you want to create discipline [y/n]?" << std::endl;
                            char c;
                            std::cin >> c;
                            std::cin.get(); // '\n'
                            if (c == 'y')
                            {
                                std::cout << "edit record > create department > create discipline > Enter name:" << std::endl;
                                auto discipline = _getStr(std::cin);
                                std::cout << "edit record > create department > create discipline > Enter number of teachers : " << std::endl;
                                auto teachers = atoi(_getStr(std::cin).c_str());
                                dept.disciplines[discipline] = teachers;
                            }
                            else if (c == 'n')
                            {
                                break;
                            }
                            else
                            {
                                std::cout << "edit record > create department > invalid choice" << std::endl;
                            }
                        }
                        _departments.push_back(dept);
                    }
                    else if (c == 'n')
                    {
                        break;
                    }
                    else
                    {
                        std::cout << "edit record > invalid choice" << std::endl;
                    }
                }
                return true;
            }
        public:
            static char id()
            {
                return 'b';
            }
        private:
            std::string _name;
            std::string _NUK;
            struct Department
                    {
                std::string _name;
                std::map<std::string, int> disciplines;
                    };
            std::vector<Department> _departments;
        };

class BranchFaculty : public BaseFaculty
        {
        public:
            virtual void print(const std::string& indent) const override
            {
                BaseFaculty::print(indent);
                std::cout << indent << "BaseOrganisations = " << std::endl;
                std::cout << indent << "{" << std::endl;
                bool _1st = true;
                for (auto& org : _organisations)
                {
                    if (!_1st)
                    {
                        std::cout << "," << std::endl;
                    }
                    _1st = false;
                    std::cout << indent << "\tname = " << org._name << std::endl;
                    std::cout << indent << "\tDisciplines = " << std::endl;
                    std::cout << indent << "\t{" << std::endl;
                    bool _1st = true;
                    for (auto& disc : org.disciplines)
                    {
                        if (!_1st)
                        {
                            std::cout << "," << std::endl;
                        }
                        _1st = false;
                        std::cout << indent << "\t\t" << disc.first << " : " << disc.second;
                    }
                    std::cout << std::endl << indent << "\t}";
                }
                std::cout << std::endl << indent << "}" << std::endl;
            }
            virtual void serialize(std::ostream& stream) const override
            {
                BaseFaculty::serialize(stream);
                for (auto& dept : _organisations)
                {
                    stream << ",";
                    stream << "{";
                    stream << dept._name << ',';
                    for (auto& disc : dept.disciplines)
                    {
                        stream << ",";
                        stream << "{";
                        stream << disc.first << "," << disc.second;
                        stream << "}";
                    }
                    stream << ';';
                    stream << "}";
                }
                stream << ';';
                stream << std::endl;
            }
            virtual void deserialize(std::istream& stream) override
            {
                BaseFaculty::deserialize(stream);
                std::stringbuf buf;
                stream.get(); // \n ,
                while (true)
                {
                    auto c = stream.get(); // , or ;
                    if (c == ';')
                    {
                        break;
                    }
                    else
                    {
                        stream.get(); // {
                        stream.get(buf, ',');
                        BaseOrganisation org;
                        org._name = buf.str();
                        buf.str({});
                        stream.get(); // ,
                        while (true)
                        {
                            auto c = stream.get(); // , or ;
                            if (c == ';')
                            {
                                break;
                            }
                            else
                            {
                                stream.get(); // {
                                std::string discipline;
                                int teachers = 0;
                                stream.get(buf, ',');
                                discipline = buf.str();
                                stream.get(); // ,
                                buf.str({});
                                stream.get(buf, '}');
                                teachers = atoi(buf.str().c_str());
                                stream.get(); // }
                                buf.str({});
                                org.disciplines[discipline] = teachers;
                            }
                        }
                        stream.get(); // }
                        _organisations.push_back(org);
                    }
                }
            }
            virtual char getId() const override
            {
                return id();
            }
            static char id()
            {
                return 'o';
            }
            virtual std::set<std::string> getDisciplines() const override
            {
                auto ret = BaseFaculty::getDisciplines();
                for (auto& i : _organisations)
                {
                    for (auto& j : i.disciplines)
                    {
                        ret.emplace(j.first);
                    }
                }
                return ret;
            }
            virtual bool interactiveCreate() override
            {
                if (BaseFaculty::interactiveCreate())
                {
                    while (true)
                    {
                        std::cout << "create record > Do you want to create organisation [y/n]?" << std::endl;
                        char c;
                        std::cin >> c;
                        std::cin.get(); // '\n'
                        if (c == 'y')
                        {
                            std::cout << "create record > create organisation > Enter organisation name:" << std::endl;
                            BaseOrganisation dept;
                            dept._name = _getStr(std::cin);
                            while (true)
                            {
                                std::cout << "create record > create organisation > Do you want to create discipline [y/n]?" << std::endl;
                                char c;
                                std::cin >> c;
                                std::cin.get(); // '\n'
                                if (c == 'y')
                                {
                                    std::cout << "create record > create organisation > create discipline > Enter name:" << std::endl;
                                    auto discipline = _getStr(std::cin);
                                    std::cout << "create record > create organisation > create discipline > Enter number of teachers : " << std::endl;
                                    auto teachers = atoi(_getStr(std::cin).c_str());
                                    dept.disciplines[discipline] = teachers;
                                }
                                else if (c == 'n')
                                {
                                    break;
                                }
                                else
                                {
                                    std::cout << "create record > create organisation > invalid choice" << std::endl;
                                }
                            }
                            _organisations.push_back(dept);
                        }
                        else if (c == 'n')
                        {
                            break;
                        }
                        else
                        {
                            std::cout << "create record > invalid choice" << std::endl;
                        }
                    }
                    return true;
                }
                else
                {
                    return false;
                }
            }
            virtual bool interactiveEdit() override
            {
                char c;
                if (BaseFaculty::interactiveEdit())
                {
                    for (auto it = _organisations.begin(); it != _organisations.end();)
                    {
                        std::cout << "edit record > Do you want to delete organisation " << it->_name << "[y/n]?" << std::endl;
                        std::cin >> c;
                        std::cin.get(); // '\n'
                        if (c == 'y')
                        {
                            it = _organisations.erase(it);
                            continue;
                        }
                        std::cout << "edit record > Do you want to edit organisation " << it->_name << "[y/n]?" << std::endl;
                        std::cin >> c;
                        std::cin.get(); // '\n'
                        if (c == 'y')
                        {
                            std::cout << "edit record > edit organisation > Current organisation name = " << it->_name << ". Do you want to change it [y/n]?" << std::endl;

                            std::cin >> c;
                            std::cin.get(); // '\n'
                            if (c == 'y')
                            {
                                std::cout << "edit record > edit organisation > Enter organisation name:" << std::endl;
                                it->_name = _getStr(std::cin);
                            }

                            for (auto it2 = it->disciplines.begin(); it2 != it->disciplines.end();)
                            {
                                std::cout << "edit record > edit organisation > Do you want to delete discipline " << it2->first << "[y/n]?" << std::endl;
                                std::cin >> c;
                                std::cin.get(); // '\n'
                                if (c == 'y')
                                {
                                    it2 = it->disciplines.erase(it2);
                                }
                                std::cout << "edit record > edit organisation > edit discipline > Current number of teachers = " << it2->second << ". Do you want to change it [y/n]?" << std::endl;
                                std::cin >> c;
                                std::cin.get(); // '\n'
                                if (c == 'y')
                                {
                                    std::cout << "edit record > edit organisation > edit discipline > Enter number of teachers:" << std::endl;
                                    it2->second = atoi(_getStr(std::cin).c_str());
                                }
                            }

                            std::cout << "edit record > edit organisation > Do you want to add discipline " << it->_name << "[y/n]?" << std::endl;
                            std::cin >> c;
                            std::cin.get(); // '\n'
                            if (c == 'y')
                            {
                                while (true)
                                {
                                    std::cout << "edit record > create organisation > Do you want to create discipline [y/n]?" << std::endl;
                                    std::cin >> c;
                                    std::cin.get(); // '\n'
                                    if (c == 'y')
                                    {
                                        std::cout << "edit record > create organisation > create discipline > Enter name:" << std::endl;
                                        auto discipline = _getStr(std::cin);
                                        std::cout << "edit record > create organisation > create discipline > Enter number of teachers : " << std::endl;
                                        auto teachers = atoi(_getStr(std::cin).c_str());
                                        it->disciplines[discipline] = teachers;
                                    }
                                    else if (c == 'n')
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        std::cout << "edit record > create organisation > invalid choice" << std::endl;
                                    }
                                }
                            }
                            else if (c == 'n')
                            {
                                break;
                            }
                            else
                            {
                                std::cout << "edit record > invalid choice" << std::endl;
                            }
                        }
                        ++it;
                    }

                    while (true)
                    {
                        std::cout << "edit record > Do you want to create organisation [y/n]?" << std::endl;
                        std::cin >> c;
                        std::cin.get(); // '\n'
                        if (c == 'y')
                        {
                            std::cout << "edit record > create organisation > Enter organisation name:" << std::endl;
                            BaseOrganisation org;
                            org._name = _getStr(std::cin);
                            while (true)
                            {
                                std::cout << "edit record > create organisation > Do you want to create discipline [y/n]?" << std::endl;
                                std::cin >> c;
                                std::cin.get(); // '\n'
                                if (c == 'y')
                                {
                                    std::cout << "edit record > create organisation > create discipline > Enter name:" << std::endl;
                                    auto discipline = _getStr(std::cin);
                                    std::cout << "edit record > create organisation > create discipline > Enter number of teachers : " << std::endl;
                                    auto teachers = atoi(_getStr(std::cin).c_str());
                                    org.disciplines[discipline] = teachers;
                                }
                                else if (c == 'n')
                                {
                                    break;
                                }
                                else
                                {
                                    std::cout << "edit record > create organisation > invalid choice" << std::endl;
                                }
                            }
                            _organisations.push_back(org);
                        }
                        else if (c == 'n')
                        {
                            break;
                        }
                        else
                        {
                            std::cout << "edit record > invalid choice" << std::endl;
                        }
                    }
                    return true;
                }
                else
                {
                    return false;
                }
            }
        private:
            struct BaseOrganisation
                    {
                std::string _name;
                std::map<std::string, int> disciplines;
                    };
            std::vector<BaseOrganisation> _organisations;
        };



#endif //MY_DB_FACULTY_H
