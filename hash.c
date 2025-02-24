#include "interface/interface.h"
#include "student/student.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

student *id_hash_table[TABLE_SIZE];
student *name_hash_table[TABLE_SIZE];

// 哈希函数
unsigned int hash(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str++;
    }
    return hash % TABLE_SIZE;
}

// 添加学生
void add_student(const char *id, const char *name, const char *major, int grade, int classId, int subjectNum, score *scores) {
    unsigned int id_index = hash(id);
    unsigned int name_index = hash(name);

    student *new_student = (student *)malloc(sizeof(student));
    strcpy(new_student->id, id);
    strcpy(new_student->name, name);
    strcpy(new_student->major, major);
    new_student->grade = grade;
    new_student->classId = classId;
    new_student->subjectNum = subjectNum;
    memcpy(new_student->scores, scores, subjectNum * sizeof(score));
    new_student->next = id_hash_table[id_index];
    id_hash_table[id_index] = new_student;

    new_student->name_next = name_hash_table[name_index];
    name_hash_table[name_index] = new_student;
}

// 通过学号查找学生
student *find_student_by_id(const char *id) {
    unsigned int index = hash(id);
    student *s = id_hash_table[index];
    while (s != NULL && strcmp(s->id, id) != 0) {
        s = s->next;
    }
    return s;
}

// 通过姓名查找学生（处理重名情况）
student *find_students_by_name(const char *name) {
    unsigned int index = hash(name);
    student *result = NULL;
    student *last = NULL;
    student *s = name_hash_table[index];
    while (s != NULL) {
        if (strcmp(s->name, name) == 0) {
            student *new_student = (student *)malloc(sizeof(student));
            memcpy(new_student, s, sizeof(student));
            new_student->name_next = NULL;
            if (result == NULL) {
                result = new_student;
            } else {
                last->name_next = new_student;
            }
            last = new_student;
        }
        s = s->name_next;
    }
    return result;
}

// 打印学生信息
void print_student(student *s) {
    if (s == NULL) {
        printf("Student not found.\n");
        return;
    }
    printf("ID: %s\n", s->id);
    printf("Name: %s\n", s->name);
    printf("Major: %s\n", s->major);
    printf("Grade: %d\n", s->grade);
    printf("Class: %d\n", s->classId);
    printf("Number of subjects: %d\n", s->subjectNum);
    for (int i = 0; i < s->subjectNum; i++) {
        printf("Subject %d: Score = %.2f\n", i + 1, s->scores[i].score);
    }
}

// 打印学生链表信息
void print_students(student *s) {
    while (s != NULL) {
        print_student(s);
        s = s->name_next;
    }
}

int main() {
    // 示例数据
    score scores1[] = {{90.5}, {85.0}};
    add_student("1", "Alice", "Computer Science", 2, 1, 2, scores1);

    score scores2[] = {{88.0}, {92.5}};
    add_student("2", "Bob", "Mathematics", 3, 2, 2, scores2);

    score scores3[] = {{78.0}, {82.5}};
    add_student("3", "Alice", "Physics", 1, 1, 2, scores3);

    // 通过学号查找并打印学生信息
    student *s = find_student_by_id("1");
    print_student(s);

    s = find_student_by_id("2");
    print_student(s);

    // 通过姓名查找并打印学生信息（处理重名情况）
    student *students = find_students_by_name("Alice");
    print_students(students);

    return 0;
}