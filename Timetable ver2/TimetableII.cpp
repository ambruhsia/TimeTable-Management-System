#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <algorithm>
#include <random>
#include <string>

using namespace std;

map<string, vector<string>> Subject_teacher;
map<string, int> no_of_class;
vector<string> rooms;
map<string, vector<vector<string>>> class_per_day;
vector<vector<string>> tt2;

void lecture() {
    for (const auto& day_classes : class_per_day) {
        const string& day = day_classes.first;
        vector<string> tt;

        for (const auto& subject_teachers : Subject_teacher) {
            const string& subject = subject_teachers.first;
            const vector<string>& teachers = subject_teachers.second;

            for (int sub = 0; sub < day_classes.second.size(); sub++) {
                for (int classes = 0; classes < day_classes.second[sub].size(); classes += 2) {
                    const string& teacher = day_classes.second[sub][classes];

                    if (find(teachers.begin(), teachers.end(), teacher) != teachers.end()) {
                        int num_lectures = stoi(day_classes.second[sub][classes + 1]);
                        for (int lec = 0; lec < num_lectures; lec++) {
                            tt.push_back(subject);
                        }
                    }
                }
            }
        }

        tt2.push_back(tt);
    }
}

void timetable() {
    int start_hr = 8;
    int start_min = 30;
    int end_hr = 9;
    int end_min = 15;
    int a6 = 0;

    for (auto it = class_per_day.begin(); it != class_per_day.end(); ++it) {
        string day = it->first;
        vector<string> subjects_for_day = tt2[distance(class_per_day.begin(), it)];
        cout << day << "\n";

        random_shuffle(subjects_for_day.begin(), subjects_for_day.end());

        for (const string& subject : subjects_for_day) {
            cout << start_hr << ":" << start_min << " to " << end_hr << ":" << end_min << "   " << subject
                << "    Room: " << rooms[a6] << "\n";

            start_min += 55;
            end_min += 55;

            if (start_min >= 60) {
                start_min -= 60;
                start_hr += 1;
            }

            if (end_min >= 60) {
                end_min -= 60;
                end_hr += 1;
            }

            a6 += 1;
            if (a6 >= rooms.size()) {
                a6 = 0;
            }
        }
    }
}

int main() {
    int no_of_subject;
    cout << "Enter the total number of subjects: ";
    cin >> no_of_subject;

    for (int i = 0; i < no_of_subject; i++) {
        string key;
        cout << "Enter subject name: ";
        cin >> key;

        int no_of_teacher_subject;
        cout << "Enter the number of teachers in " << key << " subject: ";
        cin >> no_of_teacher_subject;

        vector<string> teachers;

        for (int j = 0; j < no_of_teacher_subject; j++) {
            string teacher;
            cout << "Enter teacher name " << j + 1 << ": ";
            cin >> teacher;
            teachers.push_back(teacher);
        }

        Subject_teacher[key] = teachers;

        int value_2;
        cout << "Enter the number of classes for " << key << " subject in a week: ";
        cin >> value_2;
        no_of_class[key] = value_2;
    }

    int no_of_groups;
    cout << "Enter the number of groups of students: ";
    cin >> no_of_groups;

    int total_teachers = 0;

    map<string, int> teacher_groups;

    for (const auto& subject_teachers : Subject_teacher) {
        const string& subject = subject_teachers.first;
        const vector<string>& teachers = subject_teachers.second;

        if (!teachers.empty()) {
            int batch_sum = 0;
            int last_teacher = 0;
            for (int j = 0; j < teachers.size() - 1; j++) {
                const string& key_3 = teachers[j];
                int value_3 = no_of_groups / teachers.size();
                teacher_groups[key_3] = value_3;
                batch_sum += value_3;
                last_teacher++;
            }
            int value_4 = no_of_groups - batch_sum;
            teacher_groups[teachers[last_teacher]] = value_4;
        }
        else {
            const string& key_3 = teachers[0];
            int value_3 = no_of_groups / 1;
            teacher_groups[key_3] = value_3;
        }
    }

    for (const auto& teacher_group : teacher_groups) {
        total_teachers += teacher_group.second;
    }

    cout << "Batches per subject teacher:\n";
    for (const auto& item : teacher_groups) {
        cout << item.first << ": " << item.second << "\n";
    }

    map<string, int> teacher_classes;

    for (const auto& subject_teachers : Subject_teacher) {
        const string& subject = subject_teachers.first;
        const vector<string>& teachers = subject_teachers.second;

        if (!teachers.empty()) {
            for (const string& teacher : teachers) {
                int value_5 = teacher_groups[teacher] * no_of_class[subject];
                teacher_classes[teacher] = value_5;
            }
        }
    }

    cout << "Classes per teacher per week:\n";
    for (const auto& item : teacher_classes) {
        cout << item.first << ": " << item.second << "\n";
    }

    int Total_no_rooms;
    cout << "Enter total number of rooms for teaching: ";
    cin >> Total_no_rooms;

    for (int i = 0; i < Total_no_rooms; i++) {
        string room;
        cout << "Enter room number " << i + 1 << ": ";
        cin >> room;
        rooms.push_back(room);
    }

    vector<string> weekdays = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" };

    map<string, int> classes_from_mon_to_thu;

    for (const string& day : weekdays) {
        vector<vector<string>> cls;

        if (day != "Friday") {
            for (const auto& subject_teachers : Subject_teacher) {
                const string& subject = subject_teachers.first;
                const vector<string>& teachers = subject_teachers.second;
                vector<string> cls_2;

                for (const string& teacher : teachers) {
                    cls_2.push_back(teacher);
                    int x1 = round(static_cast<double>(teacher_classes[teacher]) / 4.0);
                    cls_2.push_back(to_string(x1));
                    classes_from_mon_to_thu[teacher] += x1;
                }

                cls.push_back(cls_2);
            }

            class_per_day[day] = cls;
        }
        else {
            vector<vector<string>> cls_3;
            for (const auto& subject_teachers : Subject_teacher) {
                const string& subject = subject_teachers.first;
                const vector<string>& teachers = subject_teachers.second;

                for (const string& teacher : teachers) {
                    vector<string> cls_4;
                    cls_4.push_back(teacher);
                    int x2 = abs(teacher_classes[teacher] - classes_from_mon_to_thu[teacher]);
                    cls_4.push_back(to_string(x2));
                    cls_3.push_back(cls_4);
                }
            }

            class_per_day[day] = cls_3;
        }
    }

    cout << "Subject_teacher:\n";
    for (const auto& item : Subject_teacher) {
        cout << item.first << ": ";
        for (const string& teacher : item.second) {
            cout << teacher << " ";
        }
        cout << "\n";
    }

    cout << "Classes:\n";
    for (const auto& item : no_of_class) {
        cout << item.first << ": " << item.second << "\n";
    }


    lecture();
    timetable();

    return 0;
}
