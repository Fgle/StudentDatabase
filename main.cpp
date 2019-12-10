#include <cstdio>
#include <cstring>
#include <stdlib.h>

#define MAX 1024
#define ALL 12333
#define ID 12334
#define NAME 12335
#define S1 12336
#define S2 12337
#define S3 12338
#define CURRENT 12339

struct Student {
    /* Student */
    char* id;
    char* name;
    float s1;
    float s2;
    float s3;
    float average;
    Student *next;
};

Student *current = NULL;   //  当前记录
Student *head = (Student*)malloc(sizeof(Student));    //  数据记录

const char *fileName = "database.txt";
//文件操作
void outFile();

void inputData();
void clear(int tag);
//命令选择
void cmd(char* command, char *parameter);

int main(int argc, char const *argv[]) {
    /* code */
    char command[MAX];
    memset(command, '\0', sizeof(command));
    inputData();
    while(true) {
        printf("->");
        if (fgets(command, MAX, stdin) == NULL) {
            printf("读入命令失败，请重新输入！！！");
        }
        if (command[strlen(command) - 1] == '\n')
            command[strlen(command) - 1] = '\0';
        char *parameter;
        strtok(command, " ");
        parameter = strtok(NULL, " ");
        if (strcmp(command, "exit") == 0)
            break;
        cmd(command, parameter);
    }
    outFile();
    clear(ALL);
    return 0;
}
//转换学生信息到字符串
void structToString(const Student *student, char *info) {
    /* code */
    if (student == NULL)
        return;
    if (strlen(info) != 0)
        memset(info, '\0', sizeof(char) * strlen(info));
    sprintf(info, "%s    %s    %.3f    %.3f    %.3f    %.3f",
            student->id, student->name, student->s1,
            student->s2, student->s3, student->average);
}
//转换字符串到学生信息结构体
Student* stringToStruct(const char *str) {
    if (str == NULL)
        return NULL;

    char *temp[5];
    char info[MAX];
    strcpy(info, str);
    char *p;
    int i = 0;
    Student *student = (Student*)malloc(sizeof(Student));
    p = strtok(info, " ");
    while(p != NULL) {
        temp[i] = p;
        ++i;
        p = strtok(NULL, " ");
    }
    student->id = (char*)malloc(sizeof(char) * strlen(temp[0]));
    student->name = (char*)malloc(sizeof(char) * strlen(temp[1]));
    strcpy(student->id, temp[0]);
    strcpy(student->name, temp[1]);
    student->s1 = atof(temp[2]);
    student->s2 = atof((temp[3]));
    student->s3 = atof(temp[4]);
    student->average = (student->s1 + student->s2 + student->s3) / 3;
    return student;
}
//打开文件
FILE *openFile(){
    FILE *fp = NULL;
    char fmt[3];
    for (int i = 0; i < 3; ++i)
        fmt[i] = *(fileName + 9 + i);
    if (strcmp(fmt, "bin") == 0)
        fp = fopen(fileName, "br+");
    else if (strcmp(fmt, "txt") == 0)
        fp = fopen(fileName, "r+");
    if(!fp) {
        printf("打开%s失败\n", fileName);
        return NULL;
    }
    return fp;
}
//输出数据信息
void outFile(){
    FILE *fp = fopen("database.txt", "w+");
    if (!fp) {
        printf("Failed to open file.\n");
        return;
    }
    Student *student = head->next;
    char info[MAX];
    memset(info, '\0', sizeof(info));

    fputs("id        name        s1        s2         s3        average\n", fp);
    while (student != NULL) {
        structToString(student, info);
        fputs(info, fp);
    }
    fclose(fp);

    fp = fopen("database.bin", "r+b");
    if (!fp) {
        printf("Failed to open file.\n");
        return;
    }
    memset(info, '\0', sizeof(info));
    fputs("id        name        s1        s2         s3        average\n", fp);
    student = head->next;
    while (student != NULL) {
        structToString(student, info);
        fwrite(info, sizeof(char), strlen(info), fp);
    }
    fclose(fp);
}
//输入数据信息
void inputData() {
    FILE *fp = openFile();
    char info[MAX];
    memset(info, '\0', sizeof(info));
    Student *student = NULL;
    Student *pointer = head;
    if (fgets(info, MAX, fp) == NULL) {
        printf("fgets error\n");
        ferror(fp);
    }
    while (fgets(info, MAX, fp) != NULL) {
        student = stringToStruct(info);
        pointer->next = student;
        pointer = student;
    }
    fclose(fp);
}
//判断是否以存在相同ID
bool assertId(const char *id) {
    if (strlen(id) == 0)
        return false;
    Student *student = head->next;
    bool flag = false;
    while (student != NULL) {
        if (strcmp(student->id, id) == 0)
            flag = true;
        student = student->next;
    }
    return flag;
}
void freeStruct(Student *student) {
    free(student->id);
    free(student->name);
    student->next = NULL;
    free(student);
    student = NULL;
}
//list命令
void showInfo(int tag, char *str) {
    char info[MAX];
    memset(info, '\0', sizeof(info));

    Student *student = head->next;

    if (tag == CURRENT) {
        if (current == NULL) {
            printf("Please set current record.\n");
            return;
        }
        printf("id    name    s1    s2    s3    average\n");
        structToString(current, info);
        printf("%s\n", info);
    }

    switch (tag) {
        case ALL:
            printf("id    name    s1    s2    s3    average\n");
            while (student != NULL) {
                structToString(student, info);
                printf("%s\n", info);
                student = student->next;
            }
            break;
        case ID:
            printf("id    name    s1    s2    s3    average\n");
            while (student != NULL) {
                if(strcmp(str, student->id) == 0) {
                    structToString(student, info);
                    printf("%s\n", info);
                }
                student = student->next;
            }
            if (strlen(info) == 0)
                printf("Failed to find id!!!\n");
            break;
        case NAME:
            printf("id    name    s1    s2    s3    average\n");
            while (student != NULL) {
                if(strcmp(str, student->name) == 0) {
                    structToString(student, info);
                    printf("%s\n", info);
                }
                student = student->next;
            }
            if (strlen(info) == 0)
                printf("Failed to find name!!!\n");
            break;
        default:
            break;
    }
}
void list(char *parameter) {
    char info[MAX];
    memset(info, '\0', sizeof(info));

    if (parameter == NULL)
        showInfo(CURRENT, NULL);
    else if (strcmp(parameter, "a") == 0) {
        showInfo(ALL, NULL);
    }else if (strcmp(parameter, "x") == 0) {
        printf("Please input student id: ");
        scanf("%s", info);
        showInfo(ID, info);
    }else if (strcmp(parameter, "n") == 0) {
        printf("Please input student name: ");
        scanf("%s", info);
        showInfo(NAME, info);
    }
}
//add命令
void inputId(Student *student) {
    char temp[MAX];
    printf("student Id: ");
    scanf("%s", temp);

    while (assertId(temp)) {
        printf("Student id already existed, please input again: ");
        scanf("%s", temp);
    }
    if (strlen(temp) != 0) {
        if (current->id != NULL) {
            free(current->id);
            current->id = NULL;
        }
        student->id = (char*)malloc(sizeof(char) * strlen(temp));
        strcpy(student->id, temp);
    }
}
void inputName(Student *student) {
    char temp[MAX];

    printf("student name: ");
    scanf("%s", temp);
    if (strlen(temp) != 0) {
        if (current->name != NULL) {
            free(current->name);
            current->name = NULL;
        }
        student->name = (char*)malloc(sizeof(char) * strlen(temp));
        strcpy(student->name, temp);
    }
}
void inputScore(Student *student) {
    printf("s1: ");
    scanf("%.3f", &student->s1);

    printf("s2: ");
    scanf("%.3f", &student->s2);

    printf("s3: ");
    scanf("%.3f", &student->s3);
    student->average = (student->s1 + student->s2 + student->s3) / 3;
}
Student* inputInfoByPrompt(){
    Student *student = (Student*)malloc(sizeof(Student));
    student->id = NULL;
    student->name = NULL;
    student->next = NULL;

    /***输入学生ID***/
    inputId(student);
    while (student->id == NULL) {
        printf("Please input again.\n");
        inputName(student);
    }
    /***输入学生姓名***/
    inputName(student);
    while (student->name == NULL) {
        printf("Please input again.\n");
        inputName(student);
    }
    /***输入学生分数***/
    inputScore(student);

    return student;
}
void add(char *parameter) {
    Student *student = head;

    while (student->next != NULL)
        student = student->next;
    if (parameter == NULL)  //add
        student->next = inputInfoByPrompt();
    else                    //add id name s1 s2 s3 average
        student->next = stringToStruct(parameter);
}
//rem命令
void clear(int tag) {
    Student *next = head->next;
    Student *p = head;
    char id[MAX];
    memset(id, '\0', sizeof(id));
    switch (tag) {
        case ALL:                   //循环清除所有学生信息
            while (next != NULL) {
                p = next->next;
                freeStruct(next);
                next = p;
            }
            head->next = NULL;
            break;
        case ID:                    //rem x
            printf("Please input id: ");
            scanf("%s", id);
            while (next != NULL) {          //根据学号删除查找并学生信息
                if (strcmp(next->id, id) == 0) {
                    p->next = next->next;
                    freeStruct(next);
                    break;
                }
                p = next;
                next = p->next;
            }
        default:
            break;
    }
}
void rem(char *parameter) {
    if (parameter == NULL) {        //rem
        if (current == NULL) {
            printf("Current record not set,\n");
            return;
        }
        current = NULL;
    } else if (strcmp(parameter, "a") == 0) //rem a
        clear(ALL);
    else if (strcmp(parameter, "x") == 0)   //rem x
        clear(ID);
}
//edit命令
void editInfo(int flag){
    switch (flag) {
        case ID:
            inputId(current);
            break;
        case NAME:
            inputName(current);
            break;
        case S1:
            printf("Please input s1: ");
            scanf("%.3f", &current->s1);
            current->average = (current->s1 + current->s2 + current->s3) / 3;
            break;
        case S2:
            printf("Please input s2: ");
            scanf("%.3f", &current->s2);
            current->average = (current->s1 + current->s2 + current->s3) / 3;
            break;
        case S3:
            printf("Please input s3: ");
            scanf("%.3f", &current->s3);
            current->average = (current->s1 + current->s2 + current->s3) / 3;
            break;
        default:
            break;
    }
}

void edit (char *parameter) {
    if (current == NULL) {
        printf("Current record not set,\n");
        return;
    }
    if (parameter == NULL || strcmp(parameter, "a") == 0)
        current = inputInfoByPrompt();
    else if (strcmp(parameter, "x") == 0)
        editInfo(ID);
    else if (strcmp(parameter, "n") == 0)
        editInfo(NAME);
    else if (strcmp(parameter, "s1") == 0)
        editInfo(S1);
    else if (strcmp(parameter, "s2") == 0)
        editInfo(S2);
    else if (strcmp(parameter, "s3") == 0)
        editInfo(S3);
}
//set命令
void set(char *parameter) {
    char id[MAX];
    Student *student = head->next;
    printf("Please input id: ");
    scanf("%s", id);
    while (student != NULL) {
        if (strcmp(student->id, id) == 0)
            break;
        student = student->next;
    }
    if (student == NULL) {
        printf("Failed to find id!!!\n");
        return;
    }
    current = student;
}
//sort命令
int compare(int tags, const Student *s1, const Student *s2) {
    switch (tags) {
        case ID:
            return strcmp(s1->id, s2->id);
        case NAME:
            return strcmp(s1->name, s2->name);
        case S1:
            return (s1->s1 - s2->s1) > 0 ? 1 :
                   ( (s1->s1 - s2->s1) < 0 ? -1 : 0);
        case S2:
            return (s1->s2 - s2->s2) > 0 ? 1 :
                   ( (s1->s2 - s2->s2) < 0 ? -1 : 0);
        case S3:
            return (s1->s3 - s2->s3) > 0 ? 1 :
                   ( (s1->s3 - s2->s3) < 0 ? -1 : 0);
        default:
            return (s1->average - s2->average) > 0 ? 1 :
                   ( (s1->average - s2->average) < 0 ? -1 : 0);
    }
}
void insertSort(int tags) {

    Student *pre = NULL;
    Student *student = NULL;
    Student *p = head->next;
    Student *next = NULL;
    head->next = NULL;
    while (p != NULL) {
        pre = head;
        student = head->next;
        next = p->next;
        while (student != NULL) {
            if (compare(tags, p, student) <= 0) {
                pre->next = p;
                p->next = student;
                break;
            }
            pre = student;
            student = student->next;
        }
        if (pre->next == NULL) {
            pre->next = p;
            p->next = NULL;
        }
        p = next;
    }
}
void sort(char *parameter) {
    if (parameter == NULL)
        insertSort(0);
    else if (strcmp(parameter, "x") == 0)
        insertSort(ID);
    else if (strcmp(parameter, "n") == 0)
        insertSort(NAME);
    else if (strcmp(parameter, "s1") == 0)
        insertSort(S1);
    else if (strcmp(parameter, "s2") == 0)
        insertSort(S2);
    else if (strcmp(parameter, "s3") == 0)
        insertSort(S3);
}
//help命令
void helpList() {
    printf("list:\n"
           "       default: show current record.\n"
           "       a: show all record.\n"
           "       x: show student info by inputting student id.\n"
           "       n: show student info by inputting student name.\n");
}
void helpAdd() {
    printf( "add:\n"
            "       default: add student record by inputting all info.\n"
            "       <id> <name> <s1> <s2> <s3>: add student record.\n");
}
void helpRem() {
    printf("rem:\n"
           "       default: delete current record.\n"
           "       a: delete all record.\n"
           "       x: delete student record by inputting student id.\n");
}
void helpSet() {
    printf("set:\n"
           "       default: set current record.\n");
}
void helpEdit() {
    printf("edit:\n"
           "       default: edit current record, must input all info.\n"
           "       a: edit current record.\n"
           "       x: edit current id.\n"
           "       n: edit current name.\n"
           "       s1: edit current s1.\n"
           "       s2: edit current s2.\n"
           "       s3: edit current s3.\n");
}
void helpSort() {
    printf("sort:\n"
           "       default: sort by average.\n"
           "       x: sort by id.\n"
           "       n: sort by name.\n"
           "       s1: sort by s1.\n"
           "       s2: sort by s2.\n"
           "       s3: sort by s3.\n");
}
void help(char *parameter) {
    if (parameter == NULL) {
        printf("<cmd>  <parameter>:\n");
        helpList();
        helpAdd();
        helpRem();
        helpSet();
        helpEdit();
        helpSort();
    } else if (strcmp(parameter, "list") == 0) {
        printf("<cmd>  <parameter>:\n");
        helpList();
    } else if (strcmp(parameter, "add") == 0) {
        printf("<cmd>  <parameter>:\n");
        helpAdd();
    } else if (strcmp(parameter, "rem")== 0) {
        printf("<cmd>  <parameter>:\n");
        helpRem();
    } else if (strcmp(parameter, "set") == 0) {
        printf("<cmd>  <parameter>:\n");
        helpSet();
    } else if (strcmp(parameter, "edit") == 0) {
        printf("<cmd>  <parameter>:\n");
        helpEdit();
    } else if (strcmp(parameter, "sort") == 0) {
        printf("<cmd>  <parameter>:\n");
        helpSort();
    }

}

void cmd(char *command, char *parameter) {
    if (strcmp(command, "list") == 0)
        list(parameter);
    else if (strcmp(command, "add") == 0)
        add(parameter);
    else if (strcmp(command, "rem") == 0)
        rem(parameter);
    else if (strcmp(command, "edit") == 0)
        edit(parameter);
    else if (strcmp(command, "set") == 0)
        set(parameter);
    else if (strcmp(command, "sort") == 0)
        sort(parameter);
    else if (strcmp(command, "help") == 0)
        help(parameter);
    else
        printf("command error!!!\n");
}
