#include <cstring>
#include <iostream>
#include <cstdio>

struct Applicant {
    char firstName[64];
    char lastName[64];
    char middleName[64];
    int birthYear = 0;
    int examScores[3];
    int averageGrade = 0;
};

void check_cin_int() {
    if (std::cin.good())
        return;
    std::cin.clear();
    std::string ignore;
    std::cin >> ignore;
}

int readInt(const char *prompt) {
    int result = 0;
    while (true) {
        printf(prompt);
        std::cin >> result;
        check_cin_int();
        if (result >= 0)
            break;
    }

    return result;
}

void write_struct(FILE *fp, Applicant str) {
    std::string birthYear = std::to_string(str.birthYear);
    std::string avgGrade = std::to_string(str.averageGrade);
    auto *scores = new std::string[]{
        std::to_string(str.examScores[0]),
        std::to_string(str.examScores[1]),
        std::to_string(str.examScores[2]),
    };

    fwrite(str.firstName, strlen(str.firstName), 1, fp);
    fwrite(";", 1, 1, fp);
    fwrite(str.lastName, strlen(str.lastName), 1, fp);
    fwrite(";", 1, 1, fp);
    fwrite(str.middleName, strlen(str.middleName), 1, fp);
    fwrite(";", 1, 1, fp);
    fwrite(birthYear.c_str(), birthYear.length(), 1, fp);
    fwrite(";", 1, 1, fp);

    fwrite(scores[0].c_str(), scores[0].length(), 1, fp);
    fwrite(";", 1, 1, fp);
    fwrite(scores[1].c_str(), scores[1].length(), 1, fp);
    fwrite(";", 1, 1, fp);
    fwrite(scores[2].c_str(), scores[2].length(), 1, fp);
    fwrite(";", 1, 1, fp);

    fwrite(avgGrade.c_str(), avgGrade.length(), 1, fp);
    fwrite("\n", 1, 1, fp);

    //fwrite(&str, sizeof(Applicant), 1, fp);

    delete[] scores;
}

int limit_strlen(std::string &str, int limit) {
    int len = str.length();
    return limit > len ? len : limit;
}

bool read_struct(FILE *fp, Applicant &app) {
    int idx = 0;
    auto *fields = new std::string[8];
    bool valid = true;
    char ch;

    while (fread(&ch, sizeof(char), 1, fp)) {
        if (ch == '\n')
            break;
        if (ch == ';') {
            idx++;
            if (idx >= 8)
                valid = false;
            continue;
        }
        if (!valid)
            continue;

        fields[idx] += ch;
    }
    valid = valid && idx == 7;

    if (valid) {
        std::fill(app.firstName, app.firstName + 64, 0);
        std::fill(app.lastName, app.lastName + 64, 0);
        std::fill(app.middleName, app.middleName + 64, 0);

        strncpy(app.firstName, fields[0].c_str(), limit_strlen(fields[0], 63));
        strncpy(app.lastName, fields[1].c_str(), limit_strlen(fields[1], 63));
        strncpy(app.middleName, fields[2].c_str(), limit_strlen(fields[2], 63));
        app.birthYear = std::stoi(fields[3]);
        app.examScores[0] = std::stoi(fields[4]);
        app.examScores[1] = std::stoi(fields[5]);
        app.examScores[2] = std::stoi(fields[6]);
        app.averageGrade = std::stoi(fields[7]);
    }

    delete[] fields;

    return valid;
}

Applicant make_applicant() {
    Applicant a;

    printf("Enter last name: ");
    std::cin >> a.lastName;

    printf("Enter first name: ");
    std::cin >> a.firstName;

    printf("Enter middle name: ");
    std::cin >> a.middleName;

    a.birthYear = readInt("Enter birth year: ");
    for (int j = 0; j < 3; j++)
        a.examScores[j] = readInt(" Enter exam score: ");

    a.averageGrade = readInt(" Enter average grade: ");

    return a;
}

void write_structs() {
    FILE *fp = fopen("structs.txt", "a");
    int n = readInt("1. Enter structs count: ");

    for (int i = 0; i < n; i++)
        write_struct(fp, make_applicant());

    fclose(fp);
}

void remove_struct() {
    int n = readInt("3. Enter struct index to remove: ");

    FILE *fp = fopen("structs.txt", "rb");
    FILE *fp_t = fopen("structs_temp.txt", "wb");

    Applicant a;
    for (int i = 0; read_struct(fp, a); i++) {
        if (i == n)
            continue;
        write_struct(fp_t, a);
    }

    fclose(fp);
    fclose(fp_t);
    remove("structs.txt");
    rename("structs_temp.txt", "structs.txt");
}

void add_structs() {
    int k = readInt("4. Enter number of structs to add: ");

    FILE *fp = fopen("structs.txt", "rb");
    FILE *fp_t = fopen("structs_temp.txt", "wb");

    for (int i = 0; i < k; i++)
        write_struct(fp_t, make_applicant());

    Applicant a;
    while (read_struct(fp, a))
        write_struct(fp_t, a);

    fclose(fp);
    fclose(fp_t);
    remove("structs.txt");
    rename("structs_temp.txt", "structs.txt");
}

void print_structs() {
    FILE *fp = fopen("structs.txt", "r");

    Applicant a;
    while (read_struct(fp, a))
        printf(
            "First name: %s, Last name: %s, Middle name: %s, Birth year: %d, Exam scores: %d %d %d, Average grade: %d\n",
            a.firstName, a.lastName, a.middleName, a.birthYear, a.examScores[0], a.examScores[1], a.examScores[2],
            a.averageGrade
        );

    fclose(fp);
}

int main() {
    write_structs();
    remove_struct();
    add_structs();
    print_structs();

    return 0;
}
